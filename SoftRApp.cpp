//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//***************************************************************************************
#include "GlobalEnv.h"
#include "SoftRApp.h"
#include "Uitilities.h"
#include "Input.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "D3D11Renderer.h"
#include "RBMath\\Inc\\Color32.h"
#include "VertexFormats.h"
#include "SoftR\\Texture2D.h"


SoftRApp::SoftRApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}
#include "Logger.h"
SoftRApp::~SoftRApp()
{
	g_logger->shutdown();
	RBclose_log();
	g_input_manager->shutdown();
	g_res_manager->shutdown();
}

#include "BaseGeometryGenerator.h"

bool SoftRApp::Init()
{
	g_res_manager->startup();
	g_input_manager->startup("");
	g_logger->startup("log/");
	g_logger->debug_log(WIP_WARNING, "现在运行的是SoftRApp!!!");
	RBopen_log();
	if (!D3DApp::Init())
		return false;

	RBlog("Console Test~~\n");

	cam = new RBCamera();
	cam->set_position(0, 0, -15);
	RBVector4 position(0, 0, 0);
	cam->set_target(position);
	cam->set_fov_y(60);
	cam->set_ratio(16.f / 9.f);
	cam->set_near_panel(1.f);
	cam->set_far_panel(2000.f);

	cam_sys = new RBCamera();
	cam_sys->set_position(0,0,-15);
	cam_sys->set_target(RBVector4(0,0,0));
	cam_sys->set_fov_y(45);
	cam_sys->set_ratio(16.f/9.f);
	cam_sys->set_near_panel(0.1f);
	cam_sys->set_far_panel(100.f);

	set_cam_move_speed(5, 5);
	set_cam_rotate_speed(1, 1);

	ef = RB2DRenderingEffect::create();

	out_tex = RBD3D11Texture2D::create(35, 20, RBD3D11Texture2D::E_DYNAMIC);
	//out_tex = RBD3D11Texture2D::create(2, 2, RBD3D11Texture2D::E_TEXTURE, "Res\\right.jpg");
	out_tex->obj->_node->set_position(0,0,0);
	//out_tex->obj->_node->rotate(0,180,0);

	_sprites.push_back(out_tex);

	pip = new SrPipeline();

	obj = RBObject::create_object();
	obj->load_mesh("objs/tri.obj");
	obj->generate_softr_buffer<VertexFormats::Vertex_PCNT>();
	obj->_node->set_position(0, -1,0);
	obj->_node->rotate(-30, 195, 0);

	ps = new BaseShaderPS();
	vs = new BaseShaderVS();

	mb = new ShaderMatrixBuffer();

	obj->_node->get_mat(mb->m);
	cam->get_view_matrix(mb->v);
	cam->get_perspective_matrix(mb->p);

	
	bf = new SrBufferConstant();
	bf->init(mb, sizeof(ShaderMatrixBuffer));

	tf = SrTexture2D::creat("Res/pp.jpg");
	

	vs->set_constant_buffer("matrix", bf);
	ps->set_texture("texture", tf);
	pip->set_ps(ps);
	pip->set_vs(vs);
	pip->set_out_tex(out_tex);

	pip->draw(*obj->get_softr_vertex_buffer(), *obj->get_softr_index_buffer(), obj->get_index_count()/3);
	
	//pip->draw(*obj->get_softr_vertex_buffer(), *obj->get_softr_index_buffer(), obj->get_index_count() / 3);
	return true;
}

void SoftRApp::OnResize()
{
	D3DApp::OnResize();
}

void SoftRApp::handle_input(float dt)
{
	RBVector3 v = cam->get_forward();
	v.normalize();
	v *= dt * _cam_move_speed.y;
	RBVector3 vr = cam->get_right();
	vr.normalize();
	vr *= dt * _cam_move_speed.x;
	if (Input::get_key_pressed(WIP_W))
	{
		cam->translate(v.x, v.y, v.z);
	}
	if (Input::get_key_pressed(WIP_S))
	{
		cam->translate(-v.x, -v.y, -v.z);
	}
	if (Input::get_key_pressed(WIP_A))
	{
		cam->translate(-vr.x, -vr.y, -vr.z);
	}
	if (Input::get_key_pressed(WIP_D))
	{
		cam->translate(vr.x, vr.y, vr.z);
	}



	int x = Input::get_mouse_x();
	int y = Input::get_mouse_y();
	int dx = 0, dy = 0;
	RBVector2 mouse_m;
	if (Input::get_sys_key_pressed(WIP_MOUSE_RBUTTON))
	{
		if (Input::is_move())
		{
			dx = x - old_x;
			dy = y - old_y;
		}
	}
	mouse_m.set(dx, dy);
	old_x = x;
	old_y = y;

	//cam->rotate(-mouse_m.y*dt*_cam_rotate_speed.y, -mouse_m.x*dt*_cam_rotate_speed.x,0);
	float val = mouse_m.y*_cam_rotate_speed.y / (dt*1000.f);


	RBVector3 down = RBVector3(0, -1, 0);


	cam->rotate_by_axis(val, cam->get_right());
	RBVector3 f = cam->get_forward();
	f.normalize();
	if (down.is_parallel(f, down))
	{
		cam->rotate_by_axis(-val, cam->get_right());
	}
	cam->rotate_by_axis(mouse_m.x*_cam_rotate_speed.x / (dt*1000.f), cam->get_up());

}

void SoftRApp::UpdateScene(float dt)
{

	//obj->_node->translate(0,dt,0);
	//obj1->_node->rotate(0,10*dt,0);
	//obj->_node->rotate(0,10*dt,0);
	RBVector3 tar = RBVector3(0, 5, -3);
	//cam->pan(tar,10*dt);
	//cam->rotate(0,0.1,0);

	handle_input(dt);


}


void SoftRApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	g_env->d3d_context->OMSetRenderTargets(1, &g_env->d3d_RenderTargetView, g_env->d3d_DepthStencilView);
	g_env->d3d_context->ClearRenderTargetView(g_env->d3d_RenderTargetView, reinterpret_cast<const float*>(&RBColorf::gray));
	g_env->d3d_context->ClearDepthStencilView(g_env->d3d_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	//std::vector<RBColor32> vd;

	/*
	for (int i = 0; i < 160; ++i)
		for (int j = 0; j < 160; ++j)
		{
			RBColor32 c(255, 0, 0, 255);
				vd.push_back(c);
		}

	out_tex->write_data(reinterpret_cast<RBColor32*>(&vd[0]));
	*/
	obj->_node->get_mat(mb->m);
	cam->get_view_matrix(mb->v);
	cam->get_perspective_matrix(mb->p);
	pip->draw(*obj->get_softr_vertex_buffer(), *obj->get_softr_index_buffer(), obj->get_index_count() / 3);

	RBD3D11Renderer::render(cam_sys,_sprites,ef);

	HR(mSwapChain->Present(0, 0));

}

void SoftRApp::set_depth_state(bool val)
{

	if (val)
	{
		g_env->d3d_context->OMSetDepthStencilState(m_depthStencilState, 1);
	}
	else
	{
		g_env->d3d_context->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	}


}

void SoftRApp::set_viewport(float ltx, float lty, float w, float h, float min_depth, float max_depth)
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = ltx;
	vp.TopLeftY = lty;
	vp.Width = w;
	vp.Height = h;
	vp.MinDepth = min_depth;
	vp.MaxDepth = max_depth;

	g_env->d3d_context->RSSetViewports(1, &vp);
}



void SoftRApp::set_cam_move_speed(float x, float y)
{
	_cam_move_speed.x = x;
	_cam_move_speed.y = y;
}

void SoftRApp::set_cam_rotate_speed(float x, float y)
{
	_cam_rotate_speed.x = x;
	_cam_rotate_speed.y = y;
}
