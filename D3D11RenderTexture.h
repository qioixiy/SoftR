#pragma once
#include "D3D11Texture.h"
#include "GlobalEnv.h"
#include "RBMath/Inc/Colorf.h"

class RBD3D11RenderTexture : public RBD3D11Texture
{
public:
	//尺寸不对的纹理一样渲染不出来，不知道什么原因
	static RBD3D11RenderTexture* create(float w, float h, DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT)
	{
		RBD3D11RenderTexture* ret = new RBD3D11RenderTexture();
		ret->generate_texture(w, h,format);
		//ret->generate_sampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, 1);
		return ret;

	}
	void generate_texture(float w, float h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM/*DXGI_FORMAT_R32G32B32A32_FLOAT*/)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = w;
		desc.Height = h;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;// DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		HR(g_env->d3d_device->CreateTexture2D(&desc, NULL, &tex));
		D3D11_RENDER_TARGET_VIEW_DESC rtdes;
		rtdes.Format = format;
		rtdes.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtdes.Texture2D.MipSlice = 0;
		HR(g_env->d3d_device->CreateRenderTargetView(tex,&rtdes,&_render_target_view));

		D3D11_SHADER_RESOURCE_VIEW_DESC srdes;
		memset(&srdes, 0, sizeof(srdes));
		srdes.Format = format;// DXGI_FORMAT_R8G8B8A8_UNORM;
		srdes.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srdes.Texture2D.MostDetailedMip = 0;
		srdes.Texture2D.MipLevels = 1;
		HR(g_env->d3d_device->CreateShaderResourceView(tex, &srdes, &textureView));
	}
	/*
	virtual void generate_sampler(D3D11_FILTER filter = D3D11_FILTER_ANISOTROPIC, int aniso_n = 16, D3D11_TEXTURE_ADDRESS_MODE address_mode = D3D11_TEXTURE_ADDRESS_WRAP)
	{

	}
	*/
	void set_render_target(ID3D11DepthStencilView* depthStencilView = 0)
	{
		g_env->d3d_context->OMSetRenderTargets(1, &_render_target_view, g_env->d3d_DepthStencilView);
	}
	void clear_render_target(const RBColorf& color)
	{
		g_env->d3d_context->ClearRenderTargetView(_render_target_view, reinterpret_cast<const float*>(&color));
		g_env->d3d_context->ClearDepthStencilView(g_env->d3d_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	void back_render_target()
	{
		//没有权利去清空屏幕buffer
		g_env->d3d_context->OMSetRenderTargets(1, &g_env->d3d_RenderTargetView, g_env->d3d_DepthStencilView);
		g_env->d3d_context->ClearDepthStencilView(g_env->d3d_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
protected:
	RBD3D11RenderTexture(){};
public:
	ID3D11RenderTargetView* _render_target_view;
};