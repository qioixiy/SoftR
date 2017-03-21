
#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__
#pragma once

#include "IMemory.h"
#include "Assertion.h"
/*
struct MemoryPoolNode
{
	void* next;
};
*/

//TODO:内存存放位置的调整使得程序不那么别扭
#define REPORT


#define NEXT_NODE(p) *(void**)((size_t)p+single_size)

#define NEW_NODE(single_size) (void*)(((size_t)allocate_aligned(sizeof(void*)+single_size)))
//这个参数控制单次malloc内存的大小
const int g_pool_expand_node_number = 1024;
class RBPoolAllctor
{
public:
	static RBPoolAllctor* instance()
	{
		static RBPoolAllctor* _s = nullptr;
		if (_s)
			return _s;
		else
		{
			printf("get pool singleton.\n");
			_s = new RBPoolAllctor;
			return _s;
		}

	}

	RBPoolAllctor();
	~RBPoolAllctor();

	INI_RESULT init();
	void new_pool(void** header, size_t single_size);
	void * alloc(void** header, size_t single_size);
	void free(void** header, void *p);
	//TODO:需要在删除之后禁止用户再次使用p指针.
	void free(void** header, void *p, size_t single_size);
	INI_RESULT shutdown();

	//default:4 bytes
	void set_alignment(size_t alignment);

	template<class T>
	void print_list(void *header)
	{
		size_t single_size = sizeof(T);
		void* p = header;
		while (NULL != p)
		{
			((T*)p)->print();
			p = NEXT_NODE(p);
		}
	}

	//报告内存使用情况
	void report(void* header, size_t single_size)
	{
#ifdef REPORT
		if (!header)
			return;
		void* p = header;
		size_t ad = (size_t)header;
		size_t adjust = ((u8*)ad)[-1];
		CHECK(adjust > 0 && adjust <= _alignment);
		size_t od = ad - adjust;
		void* nextp = header;
		int total_node = 0;
		size_t tail = 0;
		while (nextp)
		{
			total_node++;
			void* nex = (void*)*(size_t*)((size_t)nextp + single_size);
			printf("links:\n %02X --%d--> %02X ", nextp, single_size + sizeof(size_t), nex);
			if (!nex)
				tail = (size_t)nextp + single_size + sizeof(size_t);


			size_t adj = ((u8*)nextp)[-1];
			printf("%d ", adj);
			printf("| %02X --%d--> %02X \n", (size_t)nextp - adj, single_size + sizeof(size_t), (size_t)nextp + single_size + sizeof(size_t));


			nextp = nex;
		}

		printf("memroy extends:\n %02X ~ %02X\n", od, tail);

		printf("total:%d nodes.\n", total_node);

		size_t a = (_total_alloced >> 20);
		size_t b = (((_total_alloced >> 10) - ((_total_alloced >> 20) << 10)));
		size_t c = (_total_alloced - (b << 10) - (a << 20));
		printf("System memory used:%d M %d K %d B\n", a, b, c);
#endif

	}

private:
	void expand_free_list(int single_size, void** header, int n = g_pool_expand_node_number);
	//return aligned_address located on '\'(/XXX4\XXXXXXXXXX.../XXXX)
	void *allocate_aligned(size_t size);
	//free aligned_address located on '\'(/XXX4\XXXXXXXXXX.../XXXX)
	void free_aligned(void *p);
	bool _bactive;
	size_t _alignment;

	size_t _total_alloced;
};

#endif
