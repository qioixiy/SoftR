#pragma once
#include <stdlib.h>
#include "stdio.h"
#include "RBBasedata.h"


<<<<<<< HEAD
//�ڴ���룺byte����
#define BYTEALIGN (size_t)8
//ǿ�ƶ���
=======
//内存对齐：byte对齐
#define BYTEALIGN (size_t)8
//强制对齐
>>>>>>> e23b7ce55eedc98e00e9c0ddc27461be9ae87376
#define ALIGNUP(naddress,nbytes) ((((size_t)naddress)+\
	(nbytes)-1 ) & (~((nbytes)-1)))

typedef bool MPOS;
/*
class IRBBaseAlloctor abstract
{
public:
	virtual INI_RESULT init(size_t tsize) = 0;
	//返回空则返回失败，信息被写到日志里面
	virtual void* alloc(size_t tsize) = 0;
	//virtual void* alloc(size_t tsize,MPOS tpos) = 0;
	virtual INI_RESULT shutdown() = 0;

private:
	//IRBBaseAlloctor(IRBBaseAlloctor&);

};

class  IRBSystemAlloctor abstract
{
public:

	void *operator new(size_t);
	void operator delete(void*){printf("现在在删除，但是依然没有分配内存！");}

	void *operator new[](size_t){void* p;return p;}
	void operator delete[](void*){}

};
*/
