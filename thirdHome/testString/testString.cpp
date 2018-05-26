#include<iostream>
#include<string>
#include<stdlib.h>
#include<string.h>
using std::cout;
using std::cin;
using std::string;

int main()
{
	char name[] = "1234";
	char * pstring = (char*)malloc(100);
	memcpy(pstring,name,sizeof(name));
	cout << pstring <<std::endl;
	string testString(pstring);
	cout<<testString<<std::endl;

return 0;
}

