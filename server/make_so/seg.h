#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <map>

using namespace std;

class Segment
{
public:
	// 最大词长字节（即4个汉字）
	static const int MaxWordLength = 21; 
	// 词界标记
	static const char Separator = ' ';   
	// 汉字的UTF-8编码为3字节
	static const int UTF8_CN_LEN = 3;  

public:
	Segment();
	~Segment();

public:

	void   GetDict(void);
	string ForwardDictSegment(string str1);
	string ReverseDictSegment(string str1);
	string DictSegment(string str);

private:
	map<string, int> wordmap; // 词典
};