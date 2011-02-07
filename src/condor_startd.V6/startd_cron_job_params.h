/***************************************************************
 *
 * Copyright (C) 1990-2010, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
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

#ifndef _STARTD_CRON_JOB_PARAMS_H
#define _STARTD_CRON_JOB_PARAMS_H

#include "classad_cron_job.h"
#include <list>
using namespace std;

// Define a "ClassAd" cron job parameter object
class StartdCronJobParams : public ClassAdCronJobParams
{
  public:
	StartdCronJobParams( const char			*job_name,
						 const CronJobMgr	&mgr );
	~StartdCronJobParams( void ) { };

	// Finish initialization
	bool Initialize( void );
	bool InSlotList( unsigned slot ) const;

  private:
	list<unsigned>	m_slots;
};

#endif /* _STARTD_CRON_JOB_PARAMS_H */
