#pragma once
#include <vector>
#include "D3D11Texture.h"
#include "d3dUtil.h"
#include "GlobalEnv.h"
#include  "D3D11TextureArray.h"

class RBD3D11TextureArray
{
public:
	static RBD3D11TextureArray* create(std::vector<RBD3D11Texture*>& in)
	{
		RBD3D11TextureArray* ret = new RBD3D11TextureArray(in);
		return ret;

	}
protected:
	RBD3D11TextureArray(std::vector<RBD3D11Texture*>& in)
	{
		D3D11_TEXTURE2D_DESC texElementDesc;
		in[0]->tex->GetDesc(&texElementDesc);
		D3D11_TEXTURE2D_DESC texArrayDesc;
		texArrayDesc.Width = texElementDesc.Width;
		texArrayDesc.Height = texElementDesc.Height;
		texArrayDesc.MipLevels = texElementDesc.MipLevels;
		texArrayDesc.ArraySize = in.size();
		texArrayDesc.Format = texElementDesc.Format;
		texArrayDesc.SampleDesc.Count = 1;
		texArrayDesc.SampleDesc.Quality = 0;
		texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
		texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texArrayDesc.CPUAccessFlags = 0;
		texArrayDesc.MiscFlags = 0;

		HR(g_env->d3d_device->CreateTexture2D(&texArrayDesc, 0, &_tex_array));

		int i = 0;
		std::vector<RBD3D11Texture*>::iterator it = in.begin();
		for (; it != in.end(); ++it)
		{	
			ID3D11Texture2D* v = (*it)->tex;
			for (UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
			{
				/*
				D3D11_MAPPED_SUBRESOURCE texsrc;
				HR(g_env->d3d_context->Map(v, mipLevel, D3D11_MAP_READ, 0, &texsrc));
				g_env->d3d_context->UpdateSubresource(_tex_array, 
					D3D11CalcSubresource(mipLevel, i, texElementDesc.MipLevels), 
					0, texsrc.pData, texsrc.RowPitch, texsrc.DepthPitch);
				g_env->d3d_context->Unmap(v, mipLevel);
				*/
				g_env->d3d_context->CopySubresourceRegion(_tex_array,
					D3D11CalcSubresource(mipLevel, i, texElementDesc.MipLevels),
					NULL,
					NULL,
					NULL,
					v,
					mipLevel,
					nullptr);
			}

			i++;
		}



		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		viewDesc.Format = texArrayDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		viewDesc.Texture2DArray.MostDetailedMip = 0;
		viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
		viewDesc.Texture2DArray.FirstArraySlice = 0;
		viewDesc.Texture2DArray.ArraySize = in.size();
		
		HR(g_env->d3d_device->CreateShaderResourceView(_tex_array, &viewDesc, &_tex_array_view));

		len = in.size();
	}
	~RBD3D11TextureArray()
	{
		if (_tex_array_view)
		{
			_tex_array_view->Release();
			_tex_array_view = NULL;
		}

		if (_tex_array)
		{
			_tex_array->Release();
			_tex_array = NULL;
		}
	}
public:
	unsigned int get_size()
	{
		return len;
	}

	ID3D11ShaderResourceView* get_view()
	{
		return _tex_array_view;
	}
private:
	ID3D11ShaderResourceView* _tex_array_view;
	ID3D11Texture2D* _tex_array;
	unsigned int len;
};