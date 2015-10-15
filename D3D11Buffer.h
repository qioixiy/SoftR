#pragma once

#include <D3D11.h>
#pragma warning(disable : 4005)
#define PAD16(x) (((x)+15)/16*16)

class RBD3D11Buffer
{
public:
	RBD3D11Buffer();
	~RBD3D11Buffer();
	void set_property(UINT bytes,UINT binflags,D3D11_USAGE usage = D3D11_USAGE_DEFAULT,UINT cpu_access = 0 ,UINT byte_stride = 0);
	//重装数据，只能重装进初始化时大小的数据
	void put_data(void* data);
	void create_with_data(void* data);
	inline ID3D11Buffer* get_buffer();
	inline ID3D11Buffer** get_buffer_ptr();
	inline int get_buffer_size();
	//set to current shader
	void set_to_gs(unsigned int register_n);
	void set_to_ps(unsigned int register_n);
	void set_to_vs(unsigned int register_n);
protected:

private:
	D3D11_BUFFER_DESC _des;
	ID3D11Buffer* _buffer; 

	int _data_size;
};

ID3D11Buffer* RBD3D11Buffer::get_buffer()
{
	return _buffer;
}

ID3D11Buffer** RBD3D11Buffer::get_buffer_ptr()
{
	return &_buffer;
}

int RBD3D11Buffer::get_buffer_size()
{
	return _data_size;
}