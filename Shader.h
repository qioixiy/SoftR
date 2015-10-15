#pragma once
#pragma warning(disable : 4005)
#include "d3d11.h"
#include "d3dx11.h"
#include <string>
#include "RBMath/Inc/Matrix.h"
#include "RBMath/Inc/Vector4.h"


class RBShaderBase
{
public:
	bool load(std::string file_name,std::string entry_name,std::string profile_name);
	void release_shader_buffer();
	void* get_buffer_pointer();
	//return -1 if nullptr
	int get_buffer_size();
	//virtual auto get_dx_shader() = 0;
	void show_error_message(ID3D10Blob* error,const char* filename);
	//虚析构，确保基类析构
	virtual ~RBShaderBase();
protected:
	ID3D10Blob *_shader_buffer;
};



class RBVertexShader : public RBShaderBase
{
public:
	bool load(std::string file_name,std::string entry_name,std::string profile_name,ID3D11Device* dev);
	inline ID3D11VertexShader* get_dx_shader(){return _vs_ptr;}
	bool set_matrix(ID3D11DeviceContext* dev_cont,const RBMatrix& model,const RBMatrix& view,const RBMatrix& projection);
	~RBVertexShader()
	{
	//	_vs_ptr->Release();
	}

	struct MatrixBuffer
	{
		RBMatrix m;
		RBMatrix v;
		RBMatrix p;
	};
private:
	ID3D11VertexShader *_vs_ptr;
	ID3D11Buffer* _matrix_buffer;
};



class RBPixelShader : public RBShaderBase
{
public:
	bool load(std::string file_name,std::string entry_name,std::string profile_name,ID3D11Device* dev);
	inline ID3D11PixelShader* get_dx_shader(){return _ps_ptr;}
	bool set_color(ID3D11DeviceContext* dev_cont,RBVector4 v);
	void update_const_buffer(ID3D11DeviceContext*dc,RBVector4 v);
	~RBPixelShader()
	{
	//	_ps_ptr->Release();
	}
private:
	ID3D11PixelShader *_ps_ptr;
	//ID3D11Buffer* _color_buffer;
};

class RBGeometryShader : public RBShaderBase
{
public:
	bool load(std::string file_name,std::string entry_name,std::string profile_name,ID3D11Device* dev);
	inline ID3D11GeometryShader* get_dx_shader(){return _gs_ptr;}
	bool set_matrix(ID3D11DeviceContext* dev_cont,const RBMatrix& model,const RBMatrix& view,const RBMatrix& projection);
	~RBGeometryShader()
	{

	}
protected:
	struct MatrixBuffer
	{
		RBMatrix m;
		RBMatrix v;
		RBMatrix p;
	};
private:
	ID3D11GeometryShader* _gs_ptr;
	ID3D11Buffer* _matrix_buffer;
};