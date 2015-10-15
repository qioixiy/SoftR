#pragma once
#pragma warning(disable : 4005)
#include "D3D11.h"
#include "ResourceManager.h"
#include "GlobalEnv.h"
#include "D3DUtil.h"
#include "Uitilities.h"

class RBD3D11Texture
{
public:

	static RBD3D11Texture* create(float w,float h,bool b_dynamic,bool b_gen_mip,const char* filename=NULL)
	{
		RBD3D11Texture* ret = new RBD3D11Texture();
		if (!filename)
		{
			ret->generate_texture(w, h, true, false, NULL);
			//ret->generate_sampler();
		}
		else
		{
			ResHandler* handler = ret->load_resource(filename);
			ret->generate_texture(w, h, b_dynamic, b_gen_mip, handler);
			//ret->generate_sampler();
			g_res_manager->free(handler, handler->size);
		}
		return ret;
	}

	static RBD3D11Texture* create(const char* filename,bool b_dynamic, bool b_gen_mip)
	{
		RBD3D11Texture* ret = new RBD3D11Texture();
		ResHandler* handler = ret->load_resource(filename);
		if (!handler)
			RBlog("No this file!\n");
		TextureData *hd = (TextureData *)handler->extra;
		ret->generate_texture(hd->width, hd->height, b_dynamic, b_dynamic ?false:b_gen_mip, handler);
		
		g_res_manager->free(handler, handler->size);

		return ret;
	}
private:
	ResHandler * load_resource(const char* filename)
	{
		ResHandler* h = g_res_manager->load_resource(filename);
		if (h)
		{
			return h;
		}
		else
		{
			RBlog("load picture resource filed!");
			return NULL;
		}
	}
public:
	void generate_texture(float w,float h,bool b_dynamic,bool b_gen_mip,ResHandler* hander)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = w;
		desc.Height = h;
		desc.MipLevels = b_gen_mip ? 0:1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = !b_dynamic?D3D11_USAGE_DEFAULT:D3D11_USAGE_DYNAMIC;
		desc.BindFlags = b_dynamic ? D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = b_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.MiscFlags = b_gen_mip ? D3D11_RESOURCE_MISC_GENERATE_MIPS:0;

		if (hander)
		{
			TextureData *hd = (TextureData *)hander->extra;
			//这里无论填写什么都正确，有疑问！！
			D3D11_SUBRESOURCE_DATA initData;
			//val(x,y) = (const char*)pSysMem + SysMemPitch*y + (x * BytesPerPixel)
			initData.pSysMem = hander->ptr;
			UINT rowPitch = (hd->width * 32 + 7) / 8;
			UINT Size = rowPitch*hd->height;
			initData.SysMemPitch = static_cast<UINT>(rowPitch);
			initData.SysMemSlicePitch = static_cast<UINT>(Size);

			if (b_gen_mip)
			{
				//如果要求自动生成的话，这儿不应该填充初始化数据而应该写NULL
				HR(g_env->d3d_device->CreateTexture2D(&desc, NULL, &tex));
			}
			else
			{
				//如果要求自动生成的话，这儿不应该填充初始化数据而应该写NULL
				HR(g_env->d3d_device->CreateTexture2D(&desc, &initData, &tex));
			}


		}
		else
		{
			HR(g_env->d3d_device->CreateTexture2D(&desc, NULL, &tex));
		}

		if (tex)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srdes;
			memset(&srdes, 0, sizeof(srdes));
			srdes.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srdes.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srdes.Texture2D.MipLevels = b_gen_mip? -1:1;
			HR(g_env->d3d_device->CreateShaderResourceView(tex, &srdes, &textureView));
		}

		if (b_gen_mip&&hander)
		{
			TextureData *hd = (TextureData *)hander->extra;
			g_env->d3d_context->UpdateSubresource(tex, 0, NULL, hander->ptr, static_cast<UINT>(hd->width * 4), static_cast<UINT>(hd->size));
			g_env->d3d_context->GenerateMips(textureView);
		}

		/*
		if (hander)
			g_res_manager->free(hander, hander->size);
			*/
	}

	int get_size()
	{
		return _size;
	}

	void init()
	{
		
		ResHandler* h = g_res_manager->load_resource("pp.jpg");
		TextureData *hd = (TextureData *)h->extra;

		D3D11_TEXTURE2D_DESC desc;
		desc.Width =  hd->width;
		desc.Height =  hd->height;
		desc.MipLevels = 0;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		//这里无论填写什么都正确，有疑问！！
		D3D11_SUBRESOURCE_DATA initData;
		//val(x,y) = (const char*)pSysMem + SysMemPitch*y + (x * BytesPerPixel)
		initData.pSysMem = h->ptr;
		UINT rowPitch = (hd->width * 32 + 7) / 8;
		UINT Size = rowPitch*hd->height;
		initData.SysMemPitch = static_cast<UINT>(rowPitch);
		initData.SysMemSlicePitch =  static_cast<UINT>(Size);

		_size = Size;

		//如果要求自动生成的话，这儿不应该填充初始化数据而应该写NULL
		HR(g_env->d3d_device->CreateTexture2D(&desc, NULL, &tex));

		if (tex)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srdes;
			memset(&srdes,0,sizeof(srdes));
			srdes.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srdes.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srdes.Texture2D.MipLevels =  -1;
			HR(g_env->d3d_device->CreateShaderResourceView(tex, &srdes, &textureView));
		}


		g_env->d3d_context->UpdateSubresource(tex, 0, NULL, h->ptr, static_cast<UINT>(hd->width * 4), static_cast<UINT>(hd->size));
		g_env->d3d_context->GenerateMips(textureView);

		//HR(D3DX11CreateShaderResourceViewFromFile(g_env->d3d_device,L"ju.jpg",NULL,NULL,&textureView,NULL));

		/*
		D3D11_SAMPLER_DESC spdes;
		spdes.Filter = D3D11_FILTER_ANISOTROPIC;// D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		spdes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		spdes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		spdes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		spdes.MipLODBias = 0.0f;
		spdes.MaxAnisotropy = 16;
		spdes.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		spdes.BorderColor[0] = 0;
		spdes.BorderColor[1] = 0;
		spdes.BorderColor[2] = 0;
		spdes.BorderColor[3] = 0;
		spdes.MinLOD = 0;
		spdes.MaxLOD = D3D11_FLOAT32_MAX;

		HR(g_env->d3d_device->CreateSamplerState(&spdes, &m_sampleState));
		*/

	}
	RBD3D11Texture() :tex(NULL), textureView(NULL), _gen_mip(false), _size(0)
	{}
protected:
	
public:
	bool _gen_mip;

	ID3D11Texture2D* tex;
	ID3D11ShaderResourceView* textureView;

	int _size;
	//ID3D11SamplerState* m_sampleState;
};