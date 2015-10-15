#pragma once
#pragma warning(disable : 4005)
#include "Shader.h"
#include "Uitilities.h"
#include "Object.h"
#include "GlobalEnv.h"
#include "d3dUtil.h"

class RBCamera;


class RBEffect
{
public:
	static RBEffect* create();
	virtual void apply() const = 0;
	virtual void set_matrices(const RBMatrix& model,const RBMatrix& view,const RBMatrix& projection) = 0;
	virtual void update(float dt,RBCamera* cam) = 0;
	virtual void active_normal() const {}
	virtual void inactive_normal() const {}
	virtual void render(const RBObject* obj)
	{
		UINT stride = obj->get_stride();
		UINT offset = 0;
		g_env->d3d_context->IASetVertexBuffers(0, 1, obj->get_buffer()->get_buffer_ptr(), &stride, &offset);
		g_env->d3d_context->IASetIndexBuffer(obj->get_buffer(RBObject::e_index)->get_buffer(), DXGI_FORMAT_R32_UINT, 0);
		g_env->d3d_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_env->d3d_context->DrawIndexed(obj->get_index_count(), 0, 0);
	}
	void generate_sampler(D3D11_FILTER filter = D3D11_FILTER_ANISOTROPIC, int aniso_n = 16, D3D11_TEXTURE_ADDRESS_MODE address_mode = D3D11_TEXTURE_ADDRESS_WRAP)
	{
		D3D11_SAMPLER_DESC spdes;
		spdes.Filter = filter;// D3D11_FILTER_ANISOTROPIC;// D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		spdes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		spdes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		spdes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		spdes.MipLODBias = 0.0f;
		spdes.MaxAnisotropy = aniso_n > 0 ? aniso_n : 1;
		spdes.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		spdes.BorderColor[0] = 0;
		spdes.BorderColor[1] = 0;
		spdes.BorderColor[2] = 0;
		spdes.BorderColor[3] = 0;
		spdes.MinLOD = 0;
		spdes.MaxLOD = D3D11_FLOAT32_MAX;

		HR(g_env->d3d_device->CreateSamplerState(&spdes, &m_sampleState));
	}
	//virtual void begin(){}
	//virtual void end(){}
	virtual void render(const RBCamera* cam,EffectObjects::_SingleList& obj)=0;
	//清理shader，将使用过的shader对象全部重管线上下架
	virtual void clear() = 0{}


protected:
	RBEffect(){}
	virtual ~RBEffect()
	{
		if (m_sampleState)
		{
			m_sampleState->Release();
			m_sampleState = NULL;
		}
		
	}
	ID3D11SamplerState* m_sampleState;
private:

};

