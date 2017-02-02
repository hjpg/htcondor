/***************************************************************
 * 
 * Copyright 2012 Red Hat, Inc. 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you 
 * may not use this file except in compliance with the License.  You may 
 * obtain a copy of the License at 
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and 
 * limitations under the License. 
 * 
 ***************************************************************/

#include "classad/common.h"
#include "classad/classadCache.h"
#include "classad/sink.h"
#include <assert.h>
#include <stdio.h>
#include <list>

using namespace classad;
using namespace std;

/**
 * ClassAdCache - is meant to be the storage container which is used to cache classads,
 * I've tried some fancy tricks but they don't actually yield much better performance 
 * characteristics so I've decided to keep it simple stupid (KISS)
 * 
 * @author Timothy St. Clair
 */
class ClassAdCache
{
protected:

	typedef classad_unordered<std::string, pCacheEntry> AttrValues;
	typedef classad_unordered<std::string, pCacheEntry>::iterator value_iterator;

	typedef classad_unordered<std::string, AttrValues, ClassadAttrNameHash, CaseIgnEqStr> AttrCache;
	typedef classad_unordered<std::string, AttrValues, ClassadAttrNameHash, CaseIgnEqStr>::iterator cache_iterator;

	AttrCache m_Cache;		///< Data Store
	unsigned long m_HitCount;	///< Hit Counter
	unsigned long m_MissCount;	///< Miss Counter
	unsigned long m_QueryCount;	///< Checks that don't offer an expr-tree
	unsigned long m_HitDelete;	///< Hits that freed the incoming expr tree
	unsigned long m_RemovalCount;	///< Useful to see churn
	unsigned long m_UnparseCount; ///< number of times we had to unparse a tree to populate the cache.
	bool          m_destroyed;
	
public:
	ClassAdCache()
	: m_HitCount(0)
	, m_MissCount(0)
	, m_QueryCount(0)
	, m_HitDelete(0)
	, m_RemovalCount(0)
	, m_UnparseCount(0)
	, m_destroyed(false)
	{ 
	};
	
	virtual ~ClassAdCache(){ m_destroyed = true; };

#ifndef WIN32
	pCacheData cache( std::string & szName, ExprTree * pVal)
#else
	pCacheData cache(const std::string & szName, ExprTree * pVal)
#endif
	{
		std::string szValue;
		if (pVal) {
			m_UnparseCount++;
			ClassAdUnParser unparser;
			//PRAGMA_REMIND("this should probably unparse in old classad form")
			unparser.Unparse(szValue, pVal);
		}
		return cache(szName, szValue, pVal);
	}

	///< cache's a local attribute->ExpTree
#ifndef WIN32
	pCacheData cache( std::string & szName, const std::string & szValue , ExprTree * pVal)
#else
	pCacheData cache(const std::string & szName, const std::string & szValue , ExprTree * pVal)
#endif
	{
		pCacheData pRet;

		cache_iterator itr = m_Cache.find(szName);
		bool bValidName=false;

		if (itr != m_Cache.end()) {
			bValidName = true;
			value_iterator vtr = itr->second.find(szValue);
#ifndef WIN32 // this just wastes time on windows
			szName = itr->first;
#endif

			// check the value cache
			if (vtr != itr->second.end()) {
				pRet = vtr->second.lock();

				m_HitCount++;
				if (pVal) {
					delete pVal;
					m_HitDelete++;
				} else {
					m_QueryCount++;
				}

				// don't to any more checks just return.
				return pRet;
			}
		}

		// if we got here we missed 
		if (pVal) {
			pRet.reset( new CacheEntry(szName,szValue,pVal) );

			if (bValidName) {
				itr->second[szValue] = pRet;
			} else {
				(m_Cache[szName])[szValue] = pRet;
			}

			m_MissCount++;
		} else {
			m_QueryCount++;
		}

		return pRet;
	}

	///< clears a cache key
	bool flush(const std::string & szName, const std::string & szValue)
	{
		// this can get called after the cache has been destroyed, and that will cause an abort in MSVC11
		// and possibly other places as well.
		if (m_destroyed) return false;

		cache_iterator itr = m_Cache.find(szName);

		if (itr != m_Cache.end()) {
			if (itr->second.size() == 1) {
				m_Cache.erase(itr);
			} else {
				value_iterator vtr = itr->second.find(szValue);
				itr->second.erase(vtr);
			}

			m_RemovalCount++;
			return (true);
		}

		return false;
	} 
	
	///< dumps the contents of the cache to the file
	bool dump_keys(const std::string & szFile)
	{
	  FILE * fp = fopen ( szFile.c_str(), "a+" );
	  bool bRet = false;

	  if (fp)
	  {
	    double dHitRatio = (m_HitCount/ ((double)m_HitCount+m_MissCount))*100;
	    double dMissRatio = (m_MissCount/ ((double)m_HitCount+m_MissCount))*100;
	    unsigned long lTotalUseCount=0;
	    unsigned long lTotalPruned=0;
	    unsigned long lEntries=0;

	    cache_iterator itr = m_Cache.begin();

	    while ( itr != m_Cache.end() )
	    {
              value_iterator vtr = itr->second.begin();
              
              while (vtr != itr->second.end())
              {
                if (vtr->second.expired())
                {
                    // this should never happen.
                    fprintf( fp, "EXPIRED ** %s = %s\n", itr->first.c_str(), vtr->first.c_str() );
                    vtr = itr->second.erase(vtr);
                    lTotalPruned++;
                }
                else
                {
                    lTotalUseCount += vtr->second.use_count();
                    lEntries++;
                    
                    // it's written directly to a file b/c it has the potential to be very large 
                    fprintf( fp, "[%s = %s] - %lu\n", itr->first.c_str(), vtr->first.c_str(), vtr->second.use_count() );
                    vtr++;
                }
              }
              
              itr++;
	    }

	    // written at the end so you can tail the file.
	    fprintf( fp, "------------------------------------------------\n");
	    fprintf( fp, "ClassAdCache data for PID(%d)\n",
#ifdef WIN32
	    GetCurrentProcessId()
#else
	    getpid()
#endif
	    );
	    fprintf( fp, "Hits [%lu - %f] Misses[%lu - %f] Querys[%lu]\n", m_HitCount,dHitRatio,m_MissCount,dMissRatio,m_QueryCount ); 
	    fprintf( fp, "Entries[%lu] UseCount[%lu] FlushedCount[%lu]\n", lEntries,lTotalUseCount,m_RemovalCount );
	    fprintf( fp, "Pruned[%lu] - SHOULD BE 0\n",lTotalPruned);
	    fprintf( fp, "------------------------------------------------\n");
	    fclose(fp);

	    bRet = true;

	  }

	  return (bRet);
	}

	///< dumps the contents of the cache to the file
	void print_stats(FILE* fp)
	{
		double dHitRatio = 0.0;
		double dMissRatio = 0.0;
		unsigned long cTotalUseCount = 0;
		unsigned long cMaxUseCount = 0;
		unsigned long cTotalPruned = 0;
		unsigned long cTotalValues = 0;
		unsigned long cAttribs = 0;
		unsigned long cSingletonValues = 0;
		unsigned long cAttribsWithOnlySingletonValues = 0;
		unsigned long cSingletonAttribs = 0;

		if (m_HitCount+m_MissCount) {
			double dTot = m_HitCount + m_MissCount;
			dHitRatio = (100.0 * m_HitCount) / dTot;
			dMissRatio = (100.0 * m_MissCount) / dTot;
		}

		cache_iterator itr = m_Cache.begin();
		while (itr != m_Cache.end())
		{
			value_iterator vtr = itr->second.begin();

			unsigned long cValues = 0;
			unsigned long cMaxUse = 0;
			while (vtr != itr->second.end())
			{
				unsigned long cUseCount = vtr->second.use_count();
				if (cUseCount == 1) { ++cSingletonValues; }
				cTotalUseCount += cUseCount;
				if (cMaxUse < cUseCount) { cMaxUse = cUseCount; }

				++cTotalValues;
				++cValues;
				vtr++;
			}

			if (cMaxUseCount < cMaxUse) { cMaxUseCount = cMaxUse; }
			if (cMaxUse <= 1) { ++cAttribsWithOnlySingletonValues; }
			if (cValues <= 1) { ++cSingletonAttribs; }

			++cAttribs;
			itr++;
		}

		fprintf( fp, "Attribs: %lu SingleUseAttribs: %lu AttribsWithOnlySingletons: %lu\n",  cAttribs, cSingletonAttribs, cAttribsWithOnlySingletonValues);
		fprintf( fp, "Values: %lu SingleUseValues: %lu UseCountTot:%lu UseCountMax: %lu\n", cTotalValues, cSingletonValues, cTotalUseCount, cMaxUseCount);
		fprintf( fp, "Hits:%lu (%.2f%%) Misses: %lu (%.2f%%) Querys: %lu\n", m_HitCount,dHitRatio,m_MissCount,dMissRatio,m_QueryCount ); 
	};

	void get_counts(unsigned long &hits, unsigned long &misses, unsigned long &querys, unsigned long & hitdels, unsigned long &removals, unsigned long &unparse) {
		hits = m_HitCount;
		misses = m_MissCount;
		querys = m_QueryCount;
		hitdels = m_HitDelete;
		removals = m_RemovalCount;
		unparse = m_UnparseCount;
	}
};


static classad_shared_ptr<ClassAdCache> _cache;
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

CacheEntry::CacheEntry()
{
	pData=0;
}

CacheEntry::CacheEntry(const std::string & szNameIn, const std::string & szValueIn, ExprTree * pDataIn)
{
    szName = szNameIn;
    szValue = szValueIn;
    pData = pDataIn;
}

CacheEntry::~CacheEntry()
{
    if (pData && _cache && _cache.use_count())
    {
        _cache->flush(szName, szValue);
		delete pData;
        pData=0;
    }
}

CachedExprEnvelope::~CachedExprEnvelope()
{
  // nothing to do shifted to the cache entry. 
}

#ifndef WIN32
ExprTree * CachedExprEnvelope::cache (std::string & pName, ExprTree * pTree, const std::string & szValue)
#else
ExprTree * CachedExprEnvelope::cache (const std::string & pName, ExprTree * pTree, const std::string & szValue)
#endif
{
	ExprTree * pRet = pTree;
	CachedExprEnvelope * pNewEnv = NULL;
	
	NodeKind nk = pTree->GetKind();
	switch (nk)
	{
	case EXPR_ENVELOPE:
		pRet = pTree;
		break;

	case EXPR_LIST_NODE:
	case CLASSAD_NODE:
#ifndef WIN32 // this just wastes time on windows.
		// for classads the values are already cached but we still should string space the name
		check_hit (pName, szValue);
#endif
		break;

	default:
		if ( ! _cache) { _cache.reset( new ClassAdCache() ); }
		pNewEnv = new CachedExprEnvelope();
		pNewEnv->m_pLetter = _cache->cache(pName, szValue, pTree);
		pRet = pNewEnv;
		break;
	}
	
	return pRet;
}

bool CachedExprEnvelope::_debug_dump_keys(const string & szFile)
{
  if ( ! _cache) return false;
  return _cache->dump_keys(szFile);
}

bool CachedExprEnvelope::_debug_get_counts(unsigned long &hits, unsigned long &misses, unsigned long &querys, unsigned long & hitdels, unsigned long &removals, unsigned long &unparse)
{
	if ( ! _cache) return false;
	_cache->get_counts(hits, misses, querys, hitdels, removals, unparse);
	return true;
}

void CachedExprEnvelope::_debug_print_stats(FILE* fp)
{
  if (_cache) _cache->print_stats(fp);
}

CachedExprEnvelope * CachedExprEnvelope::check_hit (string & szName, const string& szValue)
{
   CachedExprEnvelope * pRet = 0; 

   if (!_cache)
   {
	_cache.reset(new ClassAdCache());
   }

   pCacheData cache_check = _cache->cache( szName, szValue, 0);

   if (cache_check)
   {
     pRet = new CachedExprEnvelope();
     pRet->m_pLetter = cache_check;
   }

   return pRet;
}

void CachedExprEnvelope::getAttributeName(std::string & szFillMe)
{
    if (m_pLetter)
    {
        szFillMe = m_pLetter->szName;
    }
}

ExprTree * CachedExprEnvelope::get()
{
	ExprTree * pRet = 0;
	
	if (m_pLetter)
	{
		pRet = m_pLetter->pData;
	}
	
	return ( pRet );
}

ExprTree * CachedExprEnvelope::Copy( ) const
{
	CachedExprEnvelope * pRet = new CachedExprEnvelope();
	
	// duplicate as little data as possible.
	pRet->m_pLetter = this->m_pLetter;
#if defined(SCOPE_REFACTOR)
#else
	pRet->parentScope = this->parentScope;
#endif
	
	return ( pRet );
}

const ExprTree* CachedExprEnvelope::self() const
{
	return m_pLetter->pData;
}

/* This version is for shared-library compatibility.
 * Remove it the next time we have to bump the ClassAds SO version.
 */
const ExprTree* CachedExprEnvelope::self()
{
	return m_pLetter->pData;
}

#if defined(SCOPE_REFACTOR)
void CachedExprEnvelope::_SetParentScope( const ClassAd* parent)
#else
void CachedExprEnvelope::_SetParentScope( const ClassAd* )
#endif
{
#if defined(SCOPE_REFACTOR)
	parentScope = parent;
#endif
	// nothing to do here already set @ base
}

bool CachedExprEnvelope::SameAs(const ExprTree* tree) const
{
	bool bRet = false;
	
	if (tree && m_pLetter && m_pLetter->pData)
	{
		bRet = m_pLetter->pData->SameAs(((ExprTree*)tree)->self()) ;
	}

	return bRet;
}

bool CachedExprEnvelope::isClassad( ClassAd ** ptr )
{
	bool bRet = false;
	
	if (m_pLetter && m_pLetter->pData && CLASSAD_NODE == m_pLetter->pData->GetKind() )
	{
		if (ptr)
		{
			*ptr = (ClassAd *) m_pLetter->pData;
		}
		bRet = true;
	}
	
	return bRet;
}


bool CachedExprEnvelope::_Evaluate( EvalState& st, Value& v ) const
{
	bool bRet = false;
	
	if (m_pLetter && m_pLetter->pData)
	{
		bRet = m_pLetter->pData->Evaluate(st,v);
	}

	return bRet;
}

bool CachedExprEnvelope::_Evaluate( EvalState& st, Value& v, ExprTree*& t) const
{
	bool bRet = false;
	
	if (m_pLetter && m_pLetter->pData)
	{
		bRet = m_pLetter->pData->Evaluate(st,v,t);
	}

	return bRet;
	
}

bool CachedExprEnvelope::_Flatten( EvalState& st, Value& v, ExprTree*& t, int* i)const
{
	bool bRet = false;
	
	if (m_pLetter && m_pLetter->pData)
	{
		bRet = m_pLetter->pData->Flatten(st,v,t,i);
	}

	return bRet;
	
}





