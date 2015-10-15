#pragma once
#include "D3D11Texture.h"
#include "Object.h"
#include "Camera.h"
#include "D3D11RenderTexture.h"
#include "RBMath\\Inc\\Color32.h"

class RBD3D11Texture2D
{
public:
	enum Texture2DTypes
	{
		E_RENDERTARGET = 0,
		E_TEXTURE,
		E_DYNAMIC,
		E_UNKOWN,
		E_TATOL
	};
	static RBD3D11Texture2D* create(float w, float h, Texture2DTypes type, const char* filename = NULL);
	void init_texture(const char* filename);
	void init_render_texture(float w, float h);
	void init_dynamic(float w,float h);
	ID3D11ShaderResourceView* get_texture_view();

	void set_render_target()
	{
		if (E_RENDERTARGET!=_type)
		{
			return;
		}
		_texture.render_tex->set_render_target();
	}

	void clear_render_target(const RBColorf& color)
	{
		if (E_RENDERTARGET != _type)
		{
			return;
		}
		_texture.render_tex->clear_render_target(color);
	}

	void back_render_target()
	{
		if (E_RENDERTARGET != _type)
		{
			return;
		}
		_texture.render_tex->back_render_target();
	}

	void write_data(RBColor32* data,int w,int h);

protected:
	RBD3D11Texture2D();
private:
	union Texture
	{
		RBD3D11Texture* tex;
		RBD3D11RenderTexture* render_tex;
	} _texture;
	Texture2DTypes _type;
public:
	RBObject* obj;
};