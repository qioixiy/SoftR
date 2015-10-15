#include "Uitilities.h"
#include "Windows.h"

void RBopen_log()
{
	AllocConsole();
}


void RBlog(const char* strbuf)
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	unsigned long lgsize;
	WriteFile(hOutput, strbuf, strlen(strbuf), &lgsize, 0);
	
}

void RBclose_log()
{
	FreeConsole();
}