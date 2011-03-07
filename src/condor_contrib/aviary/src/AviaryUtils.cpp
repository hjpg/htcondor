/*
 * Copyright 2009-2011 Red Hat, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// c++ includes
#include <map>

// condor includes
#include "condor_common.h"
#include "condor_classad.h"
#include "condor_debug.h"
#include "compat_classad_util.h"
#include "condor_qmgr.h"

// local includes
#include "AviaryUtils.h"

using namespace std;
using namespace compat_classad;
using namespace aviary::util;

string
getPoolName()
{
    string poolName;
    char *tmp;

    tmp = param("COLLECTOR_HOST");
    if (!tmp) {
        tmp = strdup("NO COLLECTOR_HOST, NOT GOOD");
    }
    poolName = tmp;
    free(tmp); tmp = NULL;

    return poolName;
}

// cleans up the quoted values from the job log reader
string trimQuotes(const char* str) {
	string val = str;

	size_t endpos = val.find_last_not_of("\\\"");
	if( string::npos != endpos ) {
		val = val.substr( 0, endpos+1 );
	}
	size_t startpos = val.find_first_not_of("\\\"");
	if( string::npos != startpos ) {
		val = val.substr( startpos );
	}

	return val;
}

// validate that an incoming group/user name is
// alphanumeric, underscores, or a dot separator
bool isValidGroupUserName(const string& _name, string& _text) {
	const char* ptr = _name.c_str();
	while( *ptr ) {
		char c = *ptr++;
		if (	('a' > c || c > 'z') &&
			('A' > c || c > 'Z') &&
			('0' > c || c > '9') &&
			(c != '_' ) &&
			(c != '.' ) ) {
			_text = "Invalid name for group/user - alphanumeric, underscore and dot characters only";
			return false;
		}
	}
	return true;
}

// validate that an incoming attribute name is
// alphanumeric, or underscores
bool isValidAttributeName(const string& _name, string& _text) {
	const char* ptr = _name.c_str();
	while( *ptr ) {
		char c = *ptr++;
		if (	('a' > c || c > 'z') &&
			('A' > c || c > 'Z') &&
			('0' > c || c > '9') &&
			(c != '_' ) ) {
			_text = "Invalid name for attribute - alphanumeric and underscore characters only";
			return false;
		}
	}
	return true;
}

bool checkRequiredAttrs(compat_classad::ClassAd& ad, const char* attrs[], string& missing) {
	bool status = true;
	int i = 0;

        while (NULL != attrs[i]) {
		if (!ad.Lookup(attrs[i])) {
			missing += " "; missing += attrs[i];
			status = false;
		}
		i++;
	}
	return status;
}
