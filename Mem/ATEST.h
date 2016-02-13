#pragma once
#include "stdio.h"
class RBFrameAlloctor;
class ATEST
{
public:
	ATEST(int a,int b)
	{
		w=a;
		aa=b;
	}
	int a;
	void init(){a=1;k=1;w=1;}
	void print(){printf("%d  %d  %d\n",a,k,aa);}

	
	static void * operator new(size_t size)
	{
		 return pool->alloc(&nodes,size);
	}

	static void operator delete(void* p)
	{
		pool->free(&nodes,p,sizeof(ATEST));
	}
	
	static void* nodes;
	static RBFrameAlloctor* all;
	static RBPoolAllctor* pool;

	static bool Init()
	{
		pool = RBPoolAllctor::instance();
		nodes = nullptr;
		pool->new_pool(&nodes,sizeof(ATEST));
		return true;
	}

	static void log()
	{
		pool->print_list<ATEST>(nodes);
	}

private:
	int k;
	int w;
	int aa;
};
