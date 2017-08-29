#include "..\\RBMath\\Inc\\RBMathUtilities.h"
#include "MemoryPool.h"
#include <memory>

RBPoolAllctor::RBPoolAllctor()
{
	printf("construct pool\n");
	_bactive = false;
	_alignment = 4;
	_total_alloced = 0;
}

RBPoolAllctor::~RBPoolAllctor()
{

}

INI_RESULT RBPoolAllctor::init()
{
#ifdef REPORT
	printf("init pool\n");
#endif
	_bactive = true;
	return true;
}

INI_RESULT RBPoolAllctor::shutdown()
{
#ifdef REPORT
	printf("shotdown pool\n");
#endif
	_bactive = false;
	return true;
}

void RBPoolAllctor::new_pool(void** header, size_t single_size)
{
#ifdef REPORT
	printf("get a new pool\n");
#endif
	expand_free_list(single_size, header);
}

void* RBPoolAllctor::alloc(void** header, size_t single_size)
{
	if (NULL == NEXT_NODE(*header))
	{
		expand_free_list(single_size, &NEXT_NODE(*header));
		//#ifdef _DEBUG
#ifdef REPORT
		printf("NOTE:Memory pool expanded at %p\n", NEXT_NODE(*header));
#endif
		//#endif
	}
	void *r_header = *header;
	*header = NEXT_NODE(*header);
	return r_header;
}

void RBPoolAllctor::free(void** header, void *p, size_t single_size)
{
	//删掉之后要防止p被非法使用
	NEXT_NODE(p) = *header;
	*header = p;

}

void RBPoolAllctor::free(void** header, void *p)
{

}

//4/single_size/4
void RBPoolAllctor::expand_free_list(int single_size, void** header, int n /* = g_pool_expand_node_number = 32 */)
{

	CHECK(*header == NULL);
	/*     p
	//XXX4/XXXXXXXXXXXXXXXX.../XXXX
	*/
	void* p = NEW_NODE(single_size);
#ifdef REPORT

	printf("return : %02X | \n", p);
#endif
	*header = (void*)p;
	//*(void**)&p[-4] = p;

	for (int i = 0; i<n - 1; ++i)
	{
		NEXT_NODE(p) = NEW_NODE(single_size);
		p = NEXT_NODE(p);
#ifdef REPORT

		printf("return : %02X | \n", p);
#endif

	}

	NEXT_NODE(p) = NULL;
#ifdef REPORT

	printf("\n");
#endif

	report(*header, single_size);

}

void *RBPoolAllctor::allocate_aligned(size_t tsize)
{
	CHECK(_alignment>1);
	CHECK(RBMath::is_pow_2(_alignment));

	size_t size = tsize + _alignment;
	size_t origin_address = (size_t)::malloc(size);
	_total_alloced += size;
#ifdef REPORT
	printf("original block:%02X ~ %02X ", origin_address, origin_address + size);
#endif
	std::memset((void*)origin_address, 0, size);
	if (!origin_address)
		return nullptr;
	size_t tmask = (_alignment - 1);
	size_t misaligment = (origin_address & tmask);
	size_t adjust = _alignment - misaligment;
	CHECK(adjust>0);

	size_t aligned_address = origin_address + adjust;

	//save alignment adjustment 
	u8 *adjust_ptr = (u8*)aligned_address;
	CHECK(adjust<256);
	adjust_ptr[-1] = (u8)adjust;

	//save next address
	//XXX4/XXXXXXXXXXXXXXXX.../XXXX
	/*
	//return value is also aligned after 4 added.
	return (void*)(aligned_address + 4);
	*/
	return (void*)aligned_address;
}

//free aligned_address located on '\'(/XXX4\XXXXXXXXXX.../XXXX)
void RBPoolAllctor::free_aligned(void *p)
{
	size_t aligned_address = (size_t)p;
	u8 *adjust_ptr = (u8*)aligned_address;
	size_t adjust = (size_t)adjust_ptr[-1];

	size_t origin_address = aligned_address - adjust;

	::free((void*)origin_address);

}


void RBPoolAllctor::set_alignment(uint val)
{
	CHECK(RBMath::is_pow_2(val) && val != 1);
	_alignment = val;
}


