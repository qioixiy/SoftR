#pragma once
#pragma warning(disable : 4005)
#include <d3d11.h>

struct RBGlobalEnv 
{
	static RBGlobalEnv* get()
	{
		if (!instance)
			instance = new RBGlobalEnv();
		return instance;
	}
	RBGlobalEnv()
	{
		d3d_context = NULL;
		d3d_device = NULL;
	}
	ID3D11Device* d3d_device;
	ID3D11DeviceContext* d3d_context;
	ID3D11RenderTargetView* d3d_RenderTargetView;
	ID3D11DepthStencilView* d3d_DepthStencilView;
	int window_heigt;
	int window_width;
private:
	static RBGlobalEnv* instance;
};

extern RBGlobalEnv* g_env;