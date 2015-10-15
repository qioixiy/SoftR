#pragma once
#pragma warning(disable : 4005)
#include "GlobalEnv.h"
#include "Shader.h"
#include "D3D11Buffer.h"
#include <d3d11.h>
#include "Effect.h"
#include "Object.h"
#include "2DRendering.h"

class RBD3D11Renderer
{
public:
	static void render(const RBEffect* effect,const RBObject* obj)
	{
		if (obj->debug_draw)
			effect->active_normal();
		effect->apply();
		UINT stride = obj->get_stride();
		UINT offset = 0;
		g_env->d3d_context->IASetVertexBuffers(0,1,obj->get_buffer()->get_buffer_ptr(),&stride,&offset);
		g_env->d3d_context->IASetIndexBuffer(obj->get_buffer(RBObject::e_index)->get_buffer(),DXGI_FORMAT_R32_UINT,0);
		g_env->d3d_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_env->d3d_context->DrawIndexed(obj->get_index_count(),0,0);
		if (obj->debug_draw)
			effect->inactive_normal();
	}

	static void render(RBCamera* cam, EffectObjects& things,RBEffect* effect)
	{

		RBEffect* ef;
		for (EffectObjects::_EffectObjects::iterator it1 = things.pairs.begin(); it1 != things.pairs.end(); ++it1)
		{
			ef = it1->first;
			//ef->apply();
			EffectObjects::_SingleList p = *(it1->second);
			ef->render(cam,p);
		}
	}

	static void render(RBCamera* cam, std::vector<RBD3D11Texture2D* >& things, RBEffect* effect)
	{
		RB2DRenderingEffect* ef = (RB2DRenderingEffect*)effect;
		ef->render(cam, things);
	}
};