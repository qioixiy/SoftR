#pragma once
#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__


//#include "IMemory.h"
#include "Assertion.h"
#include <list>
#include <string>
#include "..\\RBMath\\Inc\\Platform\\RBBasedata.h"

//#define REPORT

//TODO:�ڴ���λ�õĵ���ʹ�ó�����ô��Ť

#define NEXT_NODE(p) *(void**)((size_t)p+single_size)

#define NEW_NODE(single_size) (void*)(((size_t)allocate_aligned(sizeof(void*)+single_size)))
//����������Ƶ���malloc�ڴ�Ĵ�С
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
	void new_pool(void** header, size_t single_size, const char* name = "null", u32 hash = 0);
	void * alloc(void** header, size_t single_size, u32 hash = 0);
	void free(void** header, void *p, size_t single_size, u32 hash = 0);
	//��δ����Ľڵ�ȫ���ͷ�
	void reduce(void** header, size_t single_size);
	//ȫ����տ��к��ѷ���
	void clear();
	INI_RESULT shutdown();

	//default:8 bytes
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

	//����ָ�����ڴ�ʹ�����
	void report(void* header, size_t single_size, u32 hash, bool show_links = false);
	//���������ڴ�
	void report(bool show_links = false);


private:
	void expand_free_list(int single_size, void** header, int n = g_pool_expand_node_number);
	//return aligned_address located on '\'(/XXX4\XXXXXXXXXX.../XXXX)
	void *allocate_aligned(size_t size);
	//free aligned_address located on '\'(/XXX4\XXXXXXXXXX.../XXXX)
	void free_aligned(void *p);
	bool _bactive;
	size_t _alignment;
	size_t _total_alloced;
	struct _LinkHeader
	{
		_LinkHeader(size_t sl, void* hd, u32 has, const char* id) :hash(has), single_size(sl), header(hd), name(id), using_header(NULL), last_using(NULL) {}
		size_t single_size;
		void* using_header;
		void* last_using;
		void* header;
		std::string name;
		u32 hash;
	};
	std::list<_LinkHeader> _links;

	//ͳ���ۼƷ���Object����
	uint mm;
};

#endif