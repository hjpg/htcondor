#ifndef __TOTALS_H__
#define __TOTALS_H__

#include "condor_common.h"
#include "condor_classad.h"
#include "HashTable.h"
#include "MyString.h"
#include "status_types.h"

// object keeps track of totals within a single class (same key)
// (i.e., "OpSys/Arch" for a startd, "Name" for schedd, etc.)
class ClassTotal
{
  	public:
		ClassTotal ();
		virtual ~ClassTotal ();

		static ClassTotal *makeTotalObject(ppOption);
		static int makeKey( MyString &, ClassAd *, ppOption);

		virtual int update(ClassAd*) 	= 0;
		virtual void displayHeader(FILE*)= 0;

		// param is zero for non-final totals
		virtual void displayInfo(FILE*, int=0)	= 0;

  	protected:
		ppOption ppo;
};


// object manages totals for different classes within the same ppo
// (e.g., different "Arch/OpSys" in startd normal mode)
class TrackTotals
{
	public:
		TrackTotals (ppOption);
		~TrackTotals();

		int  update(ClassAd *);
		void displayTotals(FILE *, int keyLength);

	private:
		ppOption ppo;
		int malformed;
		HashTable<MyString,ClassTotal*> allTotals;
		ClassTotal*	topLevelTotal;
};



// startd totals
class StartdNormalTotal : public ClassTotal
{
	public:
		StartdNormalTotal();
		virtual int update (ClassAd *);
		virtual void displayHeader(FILE *);
		virtual void displayInfo(FILE *, int);

  	protected:
		int machines;
		int owner;
		int unclaimed;
		int claimed;
		int matched;
		int preempting;
};


class StartdRunTotal : public ClassTotal
{
	public:
		StartdRunTotal();
		virtual int update (ClassAd *);
		virtual void displayHeader(FILE *);
		virtual void displayInfo(FILE *,int);

	protected:
		int machines;
		int condor_mips;
		int kflops;
		float loadavg;
};


class StartdServerTotal : public ClassTotal
{
	public:
		StartdServerTotal();
		virtual int update (ClassAd *);
		virtual void displayHeader(FILE*);
		virtual void displayInfo(FILE*,int);

  	protected:
		int machines;
		int avail;
		int memory;
		int disk;
		int condor_mips;
		int kflops;
};


class StartdStateTotal : public ClassTotal
{
	public:
		StartdStateTotal();
		virtual int update (ClassAd *);
		virtual void displayHeader(FILE*);
		virtual void displayInfo(FILE*,int);

  	protected:
		int machines;
		int owner;
		int unclaimed;
		int claimed;
		int preempt;
		int matched;
};


// schedd totals
class ScheddNormalTotal : public ClassTotal
{
	public:
		ScheddNormalTotal();
		virtual int update (ClassAd *);
		virtual void displayHeader(FILE*);
		virtual void displayInfo(FILE*,int);

	protected:
		int runningJobs;
		int idleJobs;
};


class ScheddSubmittorTotal : public ClassTotal
{
	public:
		ScheddSubmittorTotal();
		virtual int update (ClassAd *);
		virtual void displayHeader(FILE*);
		virtual void displayInfo(FILE*,int);

	protected:
		int runningJobs;
		int idleJobs;
};


// ckptserver totals
class CkptSrvrNormalTotal : public ClassTotal
{
	public:
		CkptSrvrNormalTotal();
		virtual int update (ClassAd *);
		virtual void displayHeader(FILE*);
		virtual void displayInfo(FILE*,int);

	protected:
		int numServers;
};


#endif//__TOTALS_H__
