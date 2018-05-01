#include <string>
#include <iostream>
#include <string.h>
#include <memory.h>
#include "seg.h"
using namespace std;
extern "C"
{
	char* segment(char* ptr);
	char* segment(char* ptr)
	{
		char* buf = new char[1024];
		memset(buf, 0, sizeof(buf));

		Segment s;
		string res;
		res = s.DictSegment(ptr);
		strcpy(buf, res.c_str());
		
		return buf;
	}
}