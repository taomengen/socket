#include "seg.h"

Segment::Segment()
{
	GetDict();
}

Segment::~Segment()
{
	//no
}

//读入词典
void Segment::GetDict(void)
{
	string strtmp; 
	int num;
	string word;  
	
	ifstream dictfile("dict.txt.utf8");
	if (!dictfile.is_open())
	{
		cerr << "Unable to open dictput file: " << endl;
		exit(-1);
	}
	while (getline(dictfile, strtmp)) 
	{
		istringstream istr(strtmp);
		istr >> num;  
		istr >> word;  
		wordmap[word]++;
	}
	dictfile.close();
}

//用词典做正向最大匹配法分词
string Segment::ForwardDictSegment(string str1)
{
	string str2 = ""; 
	while (!str1.empty()) 
	{
		int len = (int) str1.length(); 
		if (len > MaxWordLength)    
		{
			len = MaxWordLength;     
		}

		string w = str1.substr(0, len); 
		int n = (wordmap.find(w) != wordmap.end()); 
		while (len > UTF8_CN_LEN && n == 0) 
		{
			len -= UTF8_CN_LEN; 
			
			w = w.substr(0, len); 
		
			n = (wordmap.find(w) != wordmap.end());
		}
		
		str2 += w + Separator;

		str1 = str1.substr(w.length(), str1.length()); 	
	}

	str2 = str2.substr(0, str2.length()-1);
	return str2;
}

//用词典做逆向最大匹配法分词
string Segment::ReverseDictSegment(string str1)
{
	string str2 = ""; 
	while (!str1.empty()) 
	{
		int len = (int) str1.length(); 
		if (len > MaxWordLength)     
		{
			len = MaxWordLength;     
		}
	
		string w = str1.substr(str1.length() - len, len); 	
		int n = (wordmap.find(w) != wordmap.end());
		while (len > UTF8_CN_LEN && n == 0) 
		{
			len -= UTF8_CN_LEN; 
			w = str1.substr(str1.length()-len, len); 	
			n = (wordmap.find(w) != wordmap.end());
		}
		w = w + Separator; 
		str2 = w + str2; 
		str1 = str1.substr(0, str1.length()-len); 
	}

	str2 = str2.substr(0, str2.length()-1);
	return str2;
}

//双向最大匹配法
string Segment::DictSegment(string str)
{
	string str1 = ForwardDictSegment(str);
	string str2 = ReverseDictSegment(str);
	int numspace1 = 0;
	int numspace2 = 0;
	for (int i = 0; i < str1.size(); i++)
	{
		if (str1[i] == ' ')
		{
			numspace1++;
		}
	} 
	for (int i = 0; i < str2.size(); i++)
	{
		if (str2[i] == ' ')
		{
			numspace2++;
		}
	}
	return numspace1 > numspace2 ? str1 : str2; 
}
/*
int main(int argc, char* argv[])
{
	Segment s;
	cout << s.DictSegment("我们是相亲相爱的一家人") << endl;

	return 0;
}
*/