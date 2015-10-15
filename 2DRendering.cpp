#include "2DRendering.h"
#include "D3D11Texture.h"
#include "GlobalEnv.h"
#include "D3D11InputElementDesc.h"
#include "Camera.h"
#include "D3D11Texture2D.h"

RB2DRenderingEffect::~RB2DRenderingEffect()
{
	if (_vs)
	{
		delete _vs;
		_vs = NULL;
	}
	if (_ps)
	{
		delete _ps;
		_ps = NULL;
	}
}

RB2DRenderingEffect* RB2DRenderingEffect::create()
{
	RB2DRenderingEffect* ret = new RB2DRenderingEffect();
	ret->init(".\\Shader\\2DRendering.hlsl");
	return ret;
}

bool RB2DRenderingEffect::init(const char* shader_file)
{
	bool ret = true;
	_vs = new RBVertexShader();
	_ps = new RBPixelShader();
	ret &= _vs->load(shader_file, "VShader", "vs_5_0", g_env->d3d_device);
	ret &= _ps->load(shader_file, "PShader", "ps_5_0", g_env->d3d_device);
	generate_sampler(D3D11_FILTER_MIN_MAG_MIP_POINT, 1);
	//g_env->d3d_device->CreateInputLayout(RBD3D11InputElementDesc::vertexDescNormalTexture, 4, _vs->get_buffer_pointer(), _vs->get_buffer_size(), &_layout);
	g_env->d3d_device->CreateInputLayout(RBD3D11InputElementDesc::vertexDescPT, 2, _vs->get_buffer_pointer(), _vs->get_buffer_size(), &_layout);
	return true;
}

void RB2DRenderingEffect::set_matrices(const RBMatrix& model, const RBMatrix& view, const RBMatrix& ortho)
{
	_vs->set_matrix(g_env->d3d_context, model, view, ortho);
}

void RB2DRenderingEffect::apply() const
{
	g_env->d3d_context->VSSetShader(_vs->get_dx_shader(), 0, 0);
	g_env->d3d_context->PSSetShader(_ps->get_dx_shader(), 0, 0);

	//g_env->d3d_context->PSSetShaderResources(0, 1, &_tex->textureView);
	//g_env->d3d_context->PSSetSamplers(0, 1, &_tex->m_sampleState);

	
	g_env->d3d_context->IASetInputLayout(_layout);
}

void RB2DRenderingEffect::update(float dt, RBCamera* cam)
{

}

void RB2DRenderingEffect::release_layout()
{
	if (_layout)
	{
		_layout->Release();
		_layout = NULL;
	}
}

void RB2DRenderingEffect::set_texture(RBD3D11Texture* _tex)
{
	this->_tex = _tex;
}

void RB2DRenderingEffect::render(const RBCamera* cam, EffectObjects::_SingleList& obj)
{
	RBlog("Old Render function for 2D Sprites!\n");
}

void RB2DRenderingEffect::render(const RBCamera* cam, std::vector<RBD3D11Texture2D*>& objs)
{
	//g_env->d3d_context->OMSetRenderTargets(1, &g_env->d3d_RenderTargetView, g_env->d3d_DepthStencilView);
	//g_env->d3d_context->ClearRenderTargetView(g_env->d3d_RenderTargetView, reinterpret_cast<const float*>(&RBColorf::gray));
	//g_env->d3d_context->ClearDepthStencilView(g_env->d3d_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	std::vector<RBD3D11Texture2D*>::iterator it;
	RBD3D11Texture2D* tex;
	apply();
	it = objs.begin();
	for (;it!=objs.end();++it)
	{
		tex = *it;
		if (!tex->obj->is_visible()) continue;
		ID3D11ShaderResourceView* view = tex->get_texture_view();
		g_env->d3d_context->PSSetShaderResources(0, 1, &view);
		
		g_env->d3d_context->PSSetSamplers(0, 1, &m_sampleState);
		
		RBMatrix m, v, o;
		tex->obj->_node->get_mat(m);
		cam->get_view_matrix(v);
		//cam->get_ortho(o);
		cam->get_perspective_matrix(o);
		set_matrices(m.get_transposed(), v.get_transposed(), o.get_transposed());
		if (tex->obj->debug_draw)
			active_normal();

		RBEffect::render(tex->obj);


		if (tex->obj->debug_draw)
			inactive_normal();


	}

	clear();
}

void RB2DRenderingEffect::clear()
{
	g_env->d3d_context->VSSetShader(0, 0, 0);
	g_env->d3d_context->PSSetShader(0, 0, 0);
	g_env->d3d_context->IASetInputLayout(0);
}

