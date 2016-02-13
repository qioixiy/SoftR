#include "Uitilities.h"
#include "Windows.h"
#include "Logger.h"

void RBopen_log()
{
	AllocConsole();
}


void RBlog(const char* strbuf)
{	
	if(g_logger->isInitialized())
	{
		g_logger->debug_print(strbuf);
	}
}

void RBclose_log()
{
	FreeConsole();
}