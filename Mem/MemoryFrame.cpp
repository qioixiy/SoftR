#include "Trace.h"
#include "MemoryFrame.h"
#include <fstream>

/*���ĺ�������������ȫ���*/
INI_RESULT RBFrameAlloctor::init(size_t tsize, const char* taga)
{
	RBFN(RBFrameAlloctor::init);

	tag = taga;

#if _DEBUG
	printf("RBFrameAlloctor is starting up...\n");
#endif
	_byte_alignment = BYTEALIGN;
	tsize = ALIGNUP(tsize, _byte_alignment);
	_all_memory = tsize + _byte_alignment;
	//����_byte_alignment��ֹ�ڶ����ڴ�ʱ��ջ��Խ��
	_pmemory = (u8*)malloc(_all_memory);
	if (!_pmemory)
	{
		//log???
#ifdef _DEBUG
		printf("��ʼ���ڴ����ʧ�ܣ�\n");
#endif
		return SYSTEM_MEMORY_SHORT;
	}

	_pbase = (u8*)ALIGNUP(_pmemory, _byte_alignment);
	_pcap = (u8*)ALIGNUP(_pmemory + tsize, _byte_alignment);

	_pframe_base = _pbase;
	_pframe_cap = _pcap;

	return SYSTEM_MEMORY_SUCCESS;

}

INI_RESULT RBFrameAlloctor::shutdown()
{
	RBFN(RBFrameAlloctor::shutdown);
#if _DEBUG
	printf("RBFrameAlloctor is shutting down...\n");
#endif
	free(_pmemory);
	_pmemory = NULL;

	return INI_SUCCESS;
}

/*0 low 1 high*/
void* RBFrameAlloctor::alloc(size_t tsize, MPOS tpos)
{

	//return malloc(tsize);

	RBFN(RBFrameAlloctor::alloc);

	u8* _pret;

	tsize = ALIGNUP(tsize, _byte_alignment);

	if (_pframe_base + tsize>_pframe_cap)
	{
		//log???
		//#ifdef _DEBUG
		printf("��̬�ڴ���䲻�㣡�����˳���\n");
		u64 a = get_allocated_memory();
		u64 b = get_all_memory();
		printf("allocator [%s] \n total:%d,allocated:%d,fraction:%f\n", tag.c_str(), b, a, (f64)a / b);
		getchar();
		//#endif
		//exit(0);
		//��������
		return NULL;
	}

	if (!tpos)
	{
		_pret = _pframe_base;
		_pframe_base += tsize;
	}
	else
	{
		_pframe_cap -= tsize;
		_pret = _pframe_cap;
	}

	return (void*)_pret;

}

size_t RBFrameAlloctor::get_all_memory()
{
	RBFN(RBFrameAlloctor::get_all_memory);
	return _all_memory;
}

//��ȡ��ǰջ��Ϊ֡
void RBFrameAlloctor::getframe(MemoryFrame& mf, MPOS tpos)
{
	RBFN(RBFrameAlloctor::getframe);
	mf.frame_pos = tpos;
	if (!tpos)
	{
		mf.memory_ptr = _pframe_base;
	}
	else
	{
		mf.memory_ptr = _pframe_cap;
	}
}

//�ͷ�֡����������ڴ�
void RBFrameAlloctor::release(MemoryFrame& mf)
{
	RBFN(RBFrameAlloctor::release);
	if (mf.memory_ptr)
	{
		if (!mf.frame_pos)
		{
			_pframe_base = mf.memory_ptr;
		}
		else
		{
			_pframe_cap = mf.memory_ptr;
		}
		//mf.memory_ptr = NULL;
	}
}

size_t RBFrameAlloctor::get_allocated_memory()
{
	return _pframe_base - _pbase + _pcap - _pframe_cap;
}

size_t RBFrameAlloctor::get_remain_memory()
{
	return _pframe_cap - _pframe_base;
}


void RBFrameAlloctor::serialize(const char* filename, MPOS tpos)
{

	std::ofstream fout(filename, std::ios::binary);
	if (tpos)
	{
		fout.write((char*)_pframe_cap, _pcap - _pframe_cap);
	}
	else
	{
		printf("%d\n", _pframe_base - _pbase);
		fout.write((char*)_pbase, _pframe_base - _pbase);
	}

	fout.close();

}

void* RBFrameAlloctor::deserialize(const char* filename, MPOS tpos)
{
	std::ifstream fin(filename, std::ios::binary);
	if (!fin)
	{
		printf("read %s failed!\n", filename);
		return nullptr;
	}
	fin.seekg(0, fin.end);
	int size = fin.tellg();
	printf("deserialize total size%d\n", size);
	void* p = alloc(size, tpos);
	fin.seekg(0, fin.beg);
	fin.read((char*)p, size);
	fin.close();
	return p;
}

