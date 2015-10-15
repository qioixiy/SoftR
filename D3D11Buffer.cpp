#pragma once
#include "GlobalEnv.h"
#include "D3D11Buffer.h"
#include "d3dUtil.h"

void RBD3D11Buffer::set_property(UINT bytes,UINT binflags,D3D11_USAGE usage /*= D3D11_USAGE_DEFAULT*/,UINT cpu_access /*= 0 */,UINT byte_stride /*= 0*/)
{
	_des.Usage = usage;
	_des.BindFlags = binflags;
	_des.ByteWidth = PAD16(bytes);

	_des.MiscFlags = 0;

	_des.StructureByteStride = byte_stride;
	_des.CPUAccessFlags = cpu_access;

}

RBD3D11Buffer::RBD3D11Buffer()
{
	_buffer = NULL;
	_data_size = 0;
	ZeroMemory(&_des, sizeof(_des));
}

void RBD3D11Buffer::create_with_data(void* data)
{
	if (_buffer)
	{
		_buffer->Release();
		_buffer = NULL;
	}
	D3D11_SUBRESOURCE_DATA initdata;
	initdata.pSysMem = data;
	initdata.SysMemPitch = 0;
	initdata.SysMemSlicePitch = 0;

	_data_size = _des.ByteWidth;

	//buffer大小一定是16的倍数
	HR(g_env->d3d_device->CreateBuffer(&_des,&initdata,&_buffer));
}

RBD3D11Buffer::~RBD3D11Buffer()
{
	_data_size = 0;
	_buffer->Release();
}

void RBD3D11Buffer::put_data(void* data)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mpr;

	hr = g_env->d3d_context->Map(_buffer,0,D3D11_MAP_WRITE_DISCARD,0,&mpr);
	if (FAILED(hr))
	{
		::MessageBox(0,L"Map资源失败！",L"警告",MB_OK);
		exit(0);
	}
	memcpy(mpr.pData, data, _data_size);
	g_env->d3d_context->Unmap(_buffer,0);
	
}

	void RBD3D11Buffer::set_to_gs(unsigned int register_n)
	{
		g_env->d3d_context->GSSetConstantBuffers(register_n,1,&_buffer);
	}
	void RBD3D11Buffer::set_to_ps(unsigned int register_n)
	{
		g_env->d3d_context->PSSetConstantBuffers(register_n,1,&_buffer);
	}
	void RBD3D11Buffer::set_to_vs(unsigned int register_n)
	{
		g_env->d3d_context->VSSetConstantBuffers(register_n,1,&_buffer);
	}