#include <stdio.h>
#include <iostream>

#include <string>
#include <vector>

#include <sstream>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#elif MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

#include <boost/thread.hpp>

#include "functions.h"

using namespace std;

// ---------------------------------------------------------------------

int substr_count(string source, string substring)
{
	int count = 0;
	
	for (size_t pos = 0; pos < source.size(); pos += substring.size())
	{
		pos = source.find(substring, pos);
		if (pos != string::npos)
		{
			++count;
		}
		else
		{
			break;
		}
	}
	
	return count;
}

// ---------------------------------------------------------------------

string trim(string& str)
{
	str.erase(0, str.find_first_not_of(' '));	//prefixing spaces
	str.erase(str.find_last_not_of(' ') + 1);	//surfixing spaces
	return str;
}

// ---------------------------------------------------------------------

void fast_exec(string command)
{
	boost::thread bthrd(boost::bind(exec, command));
}

// ---------------------------------------------------------------------

string exec(string command)
{
	string excmd;
	
#ifdef _WIN32
	FILE * f = _popen( &command[0], "r" );
#else
	FILE * f = popen( &command[0], "r" );
#endif
	
    if ( f == 0 ) {
        return "";
    }
    
    const int BUFSIZE = 1000;
    char buf[ BUFSIZE ];
    
    while( fgets( buf, BUFSIZE,  f ) ) {
        excmd = excmd + buf;
    }
    
#ifdef _WIN32
	_pclose( f );
#else
	pclose( f );
#endif

    return excmd;
}

// ---------------------------------------------------------------------

vector<string> explode(string delimiter, string inputstring){
    vector<string> explodes;
    
    inputstring.append(delimiter);
    
    while(inputstring.find(delimiter)!=string::npos){
        explodes.push_back(inputstring.substr(0, inputstring.find(delimiter)));
        inputstring.erase(inputstring.begin(), inputstring.begin()+inputstring.find(delimiter)+delimiter.size());
    }
    
    return explodes;
}

// ---------------------------------------------------------------------

string implode(string delimiter, vector<string> & elements)
{
    string full;
    
    for (vector<string>::iterator it = elements.begin(); it != elements.end(); ++it)
    {
        full += (*it);
        if (it != elements.end()-1)
            full += delimiter;
 
    }
    return full;
}

// ---------------------------------------------------------------------

bool file_exists(string file_name) 
{
    std::ifstream f(file_name.c_str());
    
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}


// ---------------------------------------------------------------------

/**
* �������� ���������� ����
*/
int get_cores_count() {
#ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif MACOS
    int nm[2];
    size_t len = 4;
    uint32_t count;
 
    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);
 
    if(count < 1) {
    nm[1] = HW_NCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);
    if(count < 1) { count = 1; }
    }
    return count;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}
