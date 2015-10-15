#pragma once
#include "BufferConstant.h"
#include <vector>
#include <map>
#include <string>
#include "Texture2D.h"

enum SrShaderTypes
{
	E_VERTEX,
	E_GEOMETRY,
	E_PIXEL,
	E_UNKOWN,
	E_TOTAL,
};

class SrShaderBase
{
public:
	SrShaderBase();
	~SrShaderBase();

	SrBufferConstant* get_cbuffer(std::string name)
	{
		return _constant_buffers[name];
	}

	void set_constant_buffer(std::string name,SrBufferConstant* cb)
	{
		_constant_buffers.insert(std::make_pair(name, cb));
	}

	SrTexture2D* get_texture2d(std::string name)
	{
		return _tex_2d[name];
	}

	void set_texture(std::string name, SrTexture2D* cb)
	{
		_tex_2d.insert(std::make_pair(name, cb));
	}

	void clear()
	{
		_constant_buffers.clear();
		_tex_2d.clear();
	}

protected:
	//Æ¾Ãû×ÖË÷Òýconstant buffer
	std::map<std::string,SrBufferConstant*> _constant_buffers;
	//ÎÆÀíbuffer
	std::map<std::string, SrTexture2D*> _tex_2d;


	SrShaderTypes _type;
private:

};

