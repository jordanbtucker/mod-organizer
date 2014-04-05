/*	BOSS
	
	A "one-click" program for users that quickly optimises and avoids 
	detrimental conflicts in their TES IV: Oblivion, Nehrim - At Fate's Edge, 
	TES V: Skyrim, Fallout 3 and Fallout: New Vegas mod load orders.

    Copyright (C) 2009-2012    BOSS Development Team.

	This file is part of BOSS.

    BOSS is free software: you can redistribute 
	it and/or modify it under the terms of the GNU General Public License 
	as published by the Free Software Foundation, either version 3 of 
	the License, or (at your option) any later version.

    BOSS is distributed in the hope that it will 
	be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with BOSS.  If not, see 
	<http://www.gnu.org/licenses/>.

	$Revision: 3184 $, $Date: 2011-08-26 20:52:13 +0100 (Fri, 26 Aug 2011) $
*/


#include "Support/Types.h"
#include "Support/Helpers.h"
#include "Support/ModFormat.h"
#include "Support/Logger.h"
#include "Common/Globals.h"

#include <boost/spirit/include/karma.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/crc.hpp>
#include <boost/regex.hpp>

#include "alphanum.hpp"
#include "source/utf8.h"

#include <cstring>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sstream>

#if _WIN32 || _WIN64
#	include "Windows.h"
#	include "Shlobj.h"
#endif

namespace boss {
	using namespace std;
	using namespace boost;
	using boost::algorithm::replace_all;
	using boost::algorithm::replace_first;
	namespace karma = boost::spirit::karma;

	//Calculate the CRC of the given file for comparison purposes.
	uint32_t GetCrc32(const fs::path& filename) {
		uint32_t chksum = 0;
		static const size_t buffer_size = 8192;
		char buffer[buffer_size];
		ifstream ifile(filename.c_str(), ios::binary);
		LOG_TRACE("calculating CRC for: '%s'", filename.string().c_str());
		boost::crc_32_type result;
		if (ifile) {
			do {
				ifile.read(buffer, buffer_size);
				result.process_bytes(buffer, ifile.gcount());
			} while (ifile);
			chksum = result.checksum();
		} else {
			LOG_WARN("unable to open file for CRC calculation: '%s'", filename.string().c_str());
		}
		LOG_DEBUG("CRC32('%s'): 0x%x", filename.string().c_str(), chksum);
        return chksum;
	}

	//Reads an entire file into a string buffer.
	void fileToBuffer(const fs::path file, string& buffer) {
		ifstream ifile(file.c_str());
		if (ifile.fail())
			return;
		ifile.unsetf(ios::skipws); // No white space skipping!
		copy(
			istream_iterator<char>(ifile),
			istream_iterator<char>(),
			back_inserter(buffer)
		);
	}

	//UTF-8 file validator.
	bool ValidateUTF8File(const fs::path file) {
		ifstream ifs(file.c_str());

		istreambuf_iterator<char> it(ifs.rdbuf());
		istreambuf_iterator<char> eos;

		if (!utf8::is_valid(it, eos))
			return false;
		else
			return true;
	}

	//Converts an integer to a string using BOOST's Spirit.Karma, which is apparently a lot faster than a stringstream conversion...
	BOSS_COMMON string IntToString(const uint32_t n) {
		string out;
		back_insert_iterator<string> sink(out);
		karma::generate(sink,karma::upper[karma::uint_],n);
		return out;
	}

	//Converts an integer to a hex string using BOOST's Spirit.Karma, which is apparently a lot faster than a stringstream conversion...
	string IntToHexString(const uint32_t n) {
		string out;
		back_insert_iterator<string> sink(out);
		karma::generate(sink,karma::upper[karma::hex],n);
		return out;
	}

	//Converts a boolean to a string representation (true/false)
	string BoolToString(bool b) {
		if (b)
			return "true";
		else
			return "false";
	}

	//Turns "true", "false", "1", "0" into booleans.
	bool StringToBool(string str) {
		return (str == "true" || str == "1");
	}

	//Check if registry subkey exists.
	BOSS_COMMON bool RegKeyExists(string keyStr, string subkey, string value) {
		if (RegKeyStringValue(keyStr, subkey, value).empty())
			return false;
		else
			return true;
	}

	//Get registry subkey value string.
	string RegKeyStringValue(string keyStr, string subkey, string value) {
#if _WIN32 || _WIN64
		HKEY hKey, key;
		DWORD BufferSize = 4096;
		wchar_t val[4096];

		if (keyStr == "HKEY_CLASSES_ROOT")
			key = HKEY_CLASSES_ROOT;
		else if (keyStr == "HKEY_CURRENT_CONFIG")
			key = HKEY_CURRENT_CONFIG;
		else if (keyStr == "HKEY_CURRENT_USER")
			key = HKEY_CURRENT_USER;
		else if (keyStr == "HKEY_LOCAL_MACHINE")
			key = HKEY_LOCAL_MACHINE;
		else if (keyStr == "HKEY_USERS")
			key = HKEY_USERS;

		LONG ret = RegOpenKeyEx(key, fs::path(subkey).wstring().c_str(), 0, KEY_READ|KEY_WOW64_32KEY, &hKey);

		if (ret == ERROR_SUCCESS) {
			ret = RegQueryValueEx(hKey, fs::path(value).wstring().c_str(), NULL, NULL, (LPBYTE)&val, &BufferSize);
			RegCloseKey(hKey);

			if (ret == ERROR_SUCCESS)
				return fs::path(val).string();  //Easiest way to convert from wide to narrow character strings.
			else
				return "";
		} else
			return "";
#else
		return "";
#endif
	}

	
	//////////////////////////////
	// Version Class Functions
	//////////////////////////////

	Version::Version() {}

	Version::Version(const char * ver) 
		: verString(ver) {}

	Version::Version(const string ver) 
		: verString(ver) {}

	Version::Version(const fs::path file) {
		LOG_TRACE("extracting version from '%s'", file.string().c_str());
#if _WIN32 || _WIN64
		DWORD dummy = 0;
		DWORD size = GetFileVersionInfoSize(file.wstring().c_str(), &dummy);

		if (size > 0) {
			LPBYTE point = new BYTE[size];
			UINT uLen;
			VS_FIXEDFILEINFO *info;
			string ver;

			GetFileVersionInfo(file.wstring().c_str(),0,size,point);

			VerQueryValue(point,L"\\",(LPVOID *)&info,&uLen);

			DWORD dwLeftMost     = HIWORD(info->dwFileVersionMS);
			DWORD dwSecondLeft   = LOWORD(info->dwFileVersionMS);
			DWORD dwSecondRight  = HIWORD(info->dwFileVersionLS);
			DWORD dwRightMost    = LOWORD(info->dwFileVersionLS);
			
			delete [] point;

			verString = IntToString(dwLeftMost) + '.' + IntToString(dwSecondLeft) + '.' + IntToString(dwSecondRight) + '.' + IntToString(dwRightMost);
		}
#else
        // ensure filename has no quote characters in it to avoid command injection attacks
        if (string::npos != file.string().find('"')) {
    	    LOG_WARN("filename has embedded quotes; skipping to avoid command injection: '%s'", file.string().c_str());
        } else {
            // command mostly borrowed from the gnome-exe-thumbnailer.sh script
            // wrestool is part of the icoutils package
            string cmd = "wrestool --extract --raw --type=version \"" + file.string() + "\" | tr '\\0, ' '\\t.\\0' | sed 's/\\t\\t/_/g' | tr -c -d '[:print:]' | sed -r 's/.*Version[^0-9]*([0-9]+(\\.[0-9]+)+).*/\\1/'";

            FILE *fp = popen(cmd.c_str(), "r");

            // read out the version string
            static const uint32_t BUFSIZE = 32;
            char buf[BUFSIZE];
            if (NULL == fgets(buf, BUFSIZE, fp)) {
    	        LOG_DEBUG("failed to extract version from '%s'", file.string().c_str());
            }
            else {
                verString = string(buf);
	   	        LOG_DEBUG("extracted version from '%s': %s", file.string().c_str(), retVal.c_str());
            }
            pclose(fp);
        }
#endif
	}
	
	string Version::AsString() const {
		return verString;
	}

	bool Version::operator < (Version ver) {
		//Version string could have a wide variety of formats. Use regex to choose specific comparison types.

		boost::regex reg1("(\\d+\\.?)+");  //a.b.c.d.e.f.... where the letters are all integers, and 'a' is the shortest possible match.

		//boost::regex reg2("(\\d+\\.?)+([a-zA-Z\\-]+(\\d+\\.?)*)+");  //Matches a mix of letters and numbers - from "0.99.xx", "1.35Alpha2", "0.9.9MB8b1", "10.52EV-D", "1.62EV" to "10.0EV-D1.62EV".

		if (boost::regex_match(verString, reg1) && boost::regex_match(ver.AsString(), reg1)) {
			//First type: numbers separated by periods. If two versions have a different number of numbers, then the shorter should be padded 
			//with zeros. An arbitrary number of numbers should be supported.
			istringstream parser1(verString);
			istringstream parser2(ver.AsString());
			while (parser1.good() || parser2.good()) {
				//Check if each stringstream is OK for i/o before doing anything with it. If not, replace its extracted value with a 0.
				uint32_t n1, n2;
				if (parser1.good()) {
					parser1 >> n1;
					parser1.get();
				} else
					n1 = 0;
				if (parser2.good()) {
					parser2 >> n2;
					parser2.get();
				} else
					n2 = 0;
				if (n1 < n2)
					return true;
				else if (n1 > n2)
					return false;
			}
			return false;
		} else {
			//Wacky format. Use the Alphanum Algorithm. (what a name!)
			return (doj::alphanum_comp(verString, ver.AsString()) < 0);
		}
	}

	bool Version::operator > (Version ver) {
		return (*this != ver && !(*this < ver));
	}

	bool Version::operator >= (Version ver) {
		return (*this == ver || *this > ver);
	}

	bool Version::operator == (Version ver) {
		return (verString == ver.AsString());
	}

	bool Version::operator != (Version ver) {
		return !(*this == ver);
	}
}