#include "D3D11Texture2D.h"
#include "2DRendering.h"

RBD3D11Texture2D* RBD3D11Texture2D::create(float w, float h, Texture2DTypes type, const char* filename)
{
	RBD3D11Texture2D* ret = new RBD3D11Texture2D();
	//创建纹理
	switch (type)
	{
	case RBD3D11Texture2D::E_RENDERTARGET:
		//纹理尺寸！注意
		ret->init_render_texture(1280, 720);
		break;
	case RBD3D11Texture2D::E_TEXTURE:
		ret->init_texture(filename);
		break;
	case RBD3D11Texture2D::E_DYNAMIC:
		ret->init_dynamic(1920,1080);
		break;
	case RBD3D11Texture2D::E_UNKOWN:
	case RBD3D11Texture2D::E_TATOL:
	default:
		delete ret;
		return NULL;
		break;
	}
	//创建mesh
	ret->obj = RBObject::create_object();
	//ret->obj->load_mesh("Res/p.obj");
	ret->obj->load_quad(w, h);
	ret->obj->generate_buffer<RB2DRenderingEffect>();

	return ret;
}

void RBD3D11Texture2D::init_texture(const char* filename)
{
	_texture.tex = RBD3D11Texture::create(filename, false, false);
	_type = E_TEXTURE;
}

void RBD3D11Texture2D::init_render_texture(float w, float h)
{
	_texture.render_tex = RBD3D11RenderTexture::create(w, h, DXGI_FORMAT_R8G8B8A8_UNORM);
	_type = E_RENDERTARGET;
}

void RBD3D11Texture2D::init_dynamic(float w, float h)
{
	_texture.tex = RBD3D11Texture::create(w, h, true, false);
	_type = E_TEXTURE;
}

ID3D11ShaderResourceView* RBD3D11Texture2D::get_texture_view()
{
	switch (_type)
	{
	case RBD3D11Texture2D::E_RENDERTARGET:
		return _texture.render_tex->textureView;
	case RBD3D11Texture2D::E_TEXTURE:
		return _texture.tex->textureView;
	case RBD3D11Texture2D::E_UNKOWN:
	case RBD3D11Texture2D::E_TATOL:
	default:
		return NULL;
	}
}


/*
ID3D11SamplerState* RBD3D11Texture2D::get_sampler()
{
	switch (_type)
	{
	case RBD3D11Texture2D::E_RENDERTARGET:
		return _texture.render_tex->m_sampleState;
	case RBD3D11Texture2D::E_TEXTURE:
		return _texture.tex->m_sampleState;
	case RBD3D11Texture2D::E_UNKOWN:
	case RBD3D11Texture2D::E_TATOL:
	default:
		return NULL;
	}
}
*/

void RBD3D11Texture2D::write_data(RBColor32* data,int w,int h)
{
	/*
	g_env->d3d_context->CopySubresourceRegion(_texture.tex->tex,
		D3D11CalcSubresource(0, 0, 0), NULL, NULL, NULL, static_cast<unsigned char* >(data), 0, NULL);
		*/
	//g_env->d3d_context->UpdateSubresource(_texture.tex->tex, 0, 0, &data, g_env->window_width * 4, 0);

	RBColor32* dao = data;// reinterpret_cast<unsigned char*>(data.data());
	D3D11_MAPPED_SUBRESOURCE resource;
	g_env->d3d_context->Map(_texture.tex->tex, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	RBColor32* da = reinterpret_cast<RBColor32*>(resource.pData);
	//for (int i = 0; i < 2; ++i)
	{
		memcpy(da, dao, 4*w*h);
		//da += 400;
		//dao += 400;
	}
	//resource.pData = (void*)m_renderBuffer.buffer;
	g_env->d3d_context->Unmap(_texture.tex->tex,0);
}


RBD3D11Texture2D::RBD3D11Texture2D()
{
	_type = E_UNKOWN;
	obj = NULL;
}
