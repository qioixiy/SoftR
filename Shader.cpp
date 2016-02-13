#include "Shader.h"
//#include "dxerr.h"
#include "D3DCompiler.h"
#include <iostream>
#include <fstream>
#include "RBMath/Inc/Matrix.h"

RBShaderBase::~RBShaderBase()
{
	if (_shader_buffer)
	{
		//有可能是因为先销毁了D3D导致所有相关的东西都已经被析构过了
	//_shader_buffer->Release();
	}
}

bool RBShaderBase::load(std::string file_name,std::string entry_name,std::string profile_name)
{
	WCHAR name[1024];  
	memset(name,0,sizeof(name));  
	MultiByteToWideChar(CP_ACP,0,file_name.c_str(),strlen(file_name.c_str())+1,name,sizeof(name)/sizeof(name[0]));  

	DWORD fl = D3DCOMPILE_DEBUG;
	ID3D10Blob* err = 0;	
	HRESULT hr = D3DX11CompileFromFile(name,0,0,entry_name.c_str(),profile_name.c_str(),fl,0,0,&_shader_buffer,&err,0);
	
	if(FAILED(hr))
	{
		if(err)
		{
			show_error_message(err,file_name.c_str());
		}
		if(err)
		{
			//DXTRACE_ERR_MSGBOX(DXGetErrorDescription(hr),hr);
		}
		return false;
	}

	return true;
}

void RBShaderBase::release_shader_buffer()
{
	if (_shader_buffer)
	{
		_shader_buffer->Release();
		_shader_buffer = NULL;
	}
}

void *RBShaderBase::get_buffer_pointer()
{
	return _shader_buffer->GetBufferPointer();
}

int RBShaderBase::get_buffer_size()
{
	return _shader_buffer ? _shader_buffer->GetBufferSize() : -1;
}

void RBShaderBase::show_error_message(ID3D10Blob* error,const char* filename)
{
	char* e = (char*)(error->GetBufferPointer());
	unsigned long bs = error->GetBufferSize();
	std::ofstream fout;

	fout.open("error.txt");


	fout<<filename<<" compile error : "<<std::endl;
	for (unsigned long i =0;i<bs;++i)
	{
		fout<<e[i];
	}

	fout<<std::endl<<"=================="<<std::endl;

	error->Release();
	error = 0;


}

bool RBVertexShader::load(std::string file_name,std::string entry_name,std::string profile_name,ID3D11Device* dev)
{
	RBShaderBase::load(file_name,entry_name,profile_name);
	if(dev&&_shader_buffer)
		dev->CreateVertexShader(_shader_buffer->GetBufferPointer(),_shader_buffer->GetBufferSize(),NULL,&_vs_ptr);
	else
	{
		::MessageBox(0,L"Shader加载失败，即将退出！",L"警告",MB_OK);
		return false;
		//exit(0);
	}

	D3D11_BUFFER_DESC mbd;

	mbd.Usage = D3D11_USAGE_DYNAMIC;
	mbd.ByteWidth = sizeof(RBMatrix)*3;
	mbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mbd.MiscFlags = 0;
	mbd.StructureByteStride = 0;

	HRESULT hr = dev->CreateBuffer(&mbd,0,&_matrix_buffer);
	if(FAILED(hr))
	{
		::MessageBox(0,L"创建Constant Buffer失败！",L"错误",MB_OK);
		return false;
	}


	return true;
}

bool RBVertexShader::set_matrix(ID3D11DeviceContext* dev_cont,const RBMatrix& model,const RBMatrix& view,const RBMatrix& projection)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mpr;

	hr = dev_cont->Map(_matrix_buffer,0,D3D11_MAP_WRITE_DISCARD,0,&mpr);
	if (FAILED(hr))
	{
		::MessageBox(0,L"Map资源失败！",L"警告",MB_OK);
		return false;
	}
	MatrixBuffer* mb;
	mb = (MatrixBuffer*)mpr.pData;
	mb->m = model;
	mb->v = view;
	mb->p = projection;
	dev_cont->Unmap(_matrix_buffer,0);
	dev_cont->VSSetConstantBuffers(1,1,&_matrix_buffer);
	return true;
}

bool RBPixelShader::load(std::string file_name,std::string entry_name,std::string profile_name,ID3D11Device* dev)
{
	RBShaderBase::load(file_name,entry_name,profile_name);
	if(dev&&_shader_buffer)
		dev->CreatePixelShader(_shader_buffer->GetBufferPointer(),_shader_buffer->GetBufferSize(),NULL,&_ps_ptr);
	else
	{
		::MessageBox(0,L"Shader加载失败，即将退出！",L"警告",MB_OK);
		return false;
		//exit(0);
	}

	/*
	D3D11_BUFFER_DESC mbd;

	mbd.Usage = D3D11_USAGE_DYNAMIC;
	mbd.ByteWidth = sizeof(RBVector4);
	mbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mbd.MiscFlags = 0;
	mbd.StructureByteStride = 0;

	HRESULT hr = dev->CreateBuffer(&mbd,0,&_color_buffer);
	if(FAILED(hr))
	{
		::MessageBox(0,L"创建Constant Buffer失败！",L"错误",MB_OK);
		return false;
	}
	*/

	return true;
}

bool RBPixelShader::set_color(ID3D11DeviceContext* dev_cont,RBVector4 v)
{
	/*
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mpr;

	hr = dev_cont->Map(_color_buffer,0,D3D11_MAP_WRITE_DISCARD,0,&mpr);
	if (FAILED(hr))
	{
		::MessageBox(0,L"Map资源失败！",L"警告",MB_OK);
		return false;
	}
	RBVector4* s = (RBVector4*)mpr.pData;
	*s = v;
	dev_cont->Unmap(_color_buffer,0);
	dev_cont->PSSetConstantBuffers(2,1,&_color_buffer);
	*/

	return true;
}

void RBPixelShader::update_const_buffer(ID3D11DeviceContext*dc,RBVector4 v)
{
	/*
	dc->UpdateSubresource(_color_buffer,0,0,&v,0,0);
	dc->PSSetConstantBuffers(2,1,&_color_buffer);
	*/
}

bool RBGeometryShader::load(std::string file_name,std::string entry_name,std::string profile_name,ID3D11Device* dev)
{
	RBShaderBase::load(file_name,entry_name,profile_name);
	if(dev&&_shader_buffer)
		dev->CreateGeometryShader(_shader_buffer->GetBufferPointer(),_shader_buffer->GetBufferSize(),NULL,&_gs_ptr);
	else
	{
		::MessageBox(0,L"GS Shader加载失败，即将退出！",L"警告",MB_OK);
		return false;
		//exit(0);
	}
	D3D11_BUFFER_DESC mbd;

	mbd.Usage = D3D11_USAGE_DYNAMIC;
	mbd.ByteWidth = sizeof(RBMatrix)*3;
	mbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mbd.MiscFlags = 0;
	mbd.StructureByteStride = 0;

	HRESULT hr = dev->CreateBuffer(&mbd,0,&_matrix_buffer);
	if(FAILED(hr))
	{
		::MessageBox(0,L"GS创建Constant Buffer失败！",L"错误",MB_OK);
		return false;
	}
	return true;
}

bool RBGeometryShader::set_matrix(ID3D11DeviceContext* dev_cont,const RBMatrix& model,const RBMatrix& view,const RBMatrix& projection)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mpr;

	hr = dev_cont->Map(_matrix_buffer,0,D3D11_MAP_WRITE_DISCARD,0,&mpr);
	if (FAILED(hr))
	{
		::MessageBox(0,L"Map资源失败！",L"警告",MB_OK);
		return false;
	}
	MatrixBuffer* mb;
	mb = (MatrixBuffer*)mpr.pData;
	mb->m = model;
	mb->v = view;
	mb->p = projection;
	dev_cont->Unmap(_matrix_buffer,0);
	dev_cont->GSSetConstantBuffers(1,1,&_matrix_buffer);
	return true;
}
