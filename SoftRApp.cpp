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
#include "SoftR\\SimGPU.h"

SoftRApp::SoftRApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
	_index = 0;
}
#include "Logger.h"
SoftRApp::~SoftRApp()
{
	Termination();
}

#include "BaseGeometryGenerator.h"

bool SoftRApp::_init_platform()
{
	bool ret = true;

	ret &= g_res_manager->startup();
	ret &= g_input_manager->startup("");
	
	if (!D3DApp::Init())
		return false;

	cam_d3d = new RBCamera();
	if (!cam_d3d) return false;
	cam_d3d->set_position(0, 0, -15);
	cam_d3d->set_target(RBVector4(0, 0, 0));
	cam_d3d->set_fov_y(45);
	cam_d3d->set_ratio(16.f / 9.f);
	cam_d3d->set_near_panel(0.1f);
	cam_d3d->set_far_panel(100.f);

	ef = RB2DRenderingEffect::create();
	if (!ef) return false;

	out_tex = RBD3D11Texture2D::create(35, 20, RBD3D11Texture2D::E_DYNAMIC);
	//out_tex = RBD3D11Texture2D::create(2, 2, RBD3D11Texture2D::E_TEXTURE, "Res\\right.jpg");
	out_tex->obj->_node->set_position(0, 0, 0);
	//out_tex->obj->_node->rotate(0,180,0);

	_sprites.push_back(out_tex);

	return ret;
}

bool SoftRApp::_init_softr()
{
	bool ret = true;

	cam = new RBCamera();
	if (!cam) return false;
	cam->set_position(0, 0, -15);
	RBVector4 position(0, 0, 0);
	cam->set_target(position);
	cam->set_fov_y(60);
	cam->set_ratio(16.f / 9.f);
	cam->set_near_panel(1.f);
	cam->set_far_panel(2000.f);

	set_cam_move_speed(5, 5);
	set_cam_rotate_speed(1, 1);

	pip = new SrPipeline();
	if (!pip) return false;

	obj = RBObject::create_object();
	if (!obj) return false;
	//用于做oit的模型
	obj->load_mesh("Res/cube.obj");
	//obj->load_mesh("objs/tri.obj");
	obj->generate_softr_buffer<VertexFormats::Vertex_PCNT>();
	obj->_node->set_position(0, -1, 120);
	obj->_node->rotate(-30, 195, 0);

	obj1 = RBObject::create_object();
	if (!obj1) return false;
	obj1->load_mesh("Res/0001.obj");
	obj1->generate_softr_buffer<VertexFormats::Vertex_PCNT>();
	obj1->_node->set_position(5, -1, 200);
	obj1->_node->rotate(-30, 195, 0);

	obj2 = RBObject::create_object();
	if (!obj2) return false;
	obj2->load_mesh("Res/man.obj");
	obj2->generate_softr_buffer<VertexFormats::Vertex_PCNT>();
	obj2->_node->set_position(-10, -1, 120);
	obj2->_node->rotate(-10, 230, 0);


	g_logger->debug_print("Total surface: %d", obj2->get_index_count() / 3);
	g_logger->debug_print("Total surface: %d", obj->get_index_count() / 3);
	g_logger->debug_print("Total surface: %d", obj1->get_index_count() / 3);


	ps = new BaseShaderPS();
	vs = new BaseShaderVS();

	mb = new ShaderMatrixBuffer();

	obj->_node->get_mat(mb->m);
	cam->get_view_matrix(mb->v);
	cam->get_perspective_matrix(mb->p);

	bf = new SrBufferConstant();
	bf->init(mb, sizeof(ShaderMatrixBuffer));

	tf = SrTexture2D::creat("Res/man.png");

	/*
	vs->set_constant_buffer("matrix", bf);
	ps->set_texture("texture", tf);
	*/
	vs->set_constant_buffer_index(0,bf);
	ps->set_texture_index(0,tf);
	pip->set_ps(ps);
	pip->set_vs(vs);
	pip->set_out_tex(out_tex);

	//pip->draw(*obj->get_softr_vertex_buffer(), *obj->get_softr_index_buffer(), obj->get_index_count() / 3);

	RBlog("现在运行的是SoftRApp!!!\n");
	g_logger->debug_log(WIP_WARNING, "现在运行的是SoftRApp!!!");

	return ret;
}

bool SoftRApp::Init()
{
	bool ret = true;
	g_logger->startup("log/");
	
	//RBopen_log();
	if (!_init_platform())
	{
		g_logger->debug_log(WIP_ERROR, "系统初始化失败！");
		return false;
	}
	if (!_init_softr())
	{
		g_logger->debug_log(WIP_ERROR, "SoftR初始化失败！");
		return false;
	}
	//pip->draw(*obj->get_softr_vertex_buffer(), *obj->get_softr_index_buffer(), obj->get_index_count() / 3);
	return ret;
}

void SoftRApp::Termination()
{
	RBlog("正在清理PS-OM queue中的垃圾.....");

	//多线程开启
	/*
	g_gpu->terminal();
	if (t->joinable())
		t->join();
	delete g_gpu;
	delete t;
	*/
	
	delete ps;
	delete vs;
	delete mb;
	delete bf;
	delete tf;
	delete pip;
	g_logger->shutdown();
	//RBclose_log();
	g_input_manager->shutdown();
	g_res_manager->shutdown();

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
	if (Input::get_key_down(WIP_V))
	{
		int i = thread_num + 1;
		pip->show_buffer_index((_index++)%i);
		g_logger->debug_print("change to thread %d\n", (_index-1) % i -1);
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
	if (down.is_parallel(f.get_normalized(), down))
	{
		cam->rotate_by_axis(-val, cam->get_right());
	}
	cam->rotate_by_axis(mouse_m.x*_cam_rotate_speed.x / (dt*1000.f), cam->get_up());

}

void SoftRApp::UpdateScene(float dt)
{

	//obj->_node->translate(0,dt,0);
	obj2->_node->rotate(0,10*dt,0);
	obj->_node->rotate(10*dt, 10 * dt, 0);
	obj1->_node->rotate(0, 10 * dt, 5*dt);
	//obj->_node->rotate(0,10*dt,0);
	//RBVector3 tar = RBVector3(0, 0, 0);
	//cam->pan(tar,100*dt);
	//cam->rotate(0,0.1,0);

	handle_input(dt);

	obj1->_node->get_mat(mb->m);
	cam->get_view_matrix(mb->v);
	cam->get_perspective_matrix(mb->p);
	pip->draw(*obj1->get_softr_vertex_buffer(), *obj1->get_softr_index_buffer(), obj1->get_index_count() / 3);

	obj->_node->get_mat(mb->m);
	cam->get_view_matrix(mb->v);
	cam->get_perspective_matrix(mb->p);
	pip->draw(*obj->get_softr_vertex_buffer(), *obj->get_softr_index_buffer(), obj->get_index_count() / 3);

	obj2->_node->get_mat(mb->m);
	cam->get_view_matrix(mb->v);
	cam->get_perspective_matrix(mb->p);
	pip->draw(*obj2->get_softr_vertex_buffer(), *obj2->get_softr_index_buffer(), obj2->get_index_count() / 3);

	pip->clear();
	
}


void SoftRApp::DrawScene()
{

	assert(md3dImmediateContext);
	assert(mSwapChain);

	g_env->d3d_context->OMSetRenderTargets(1, &g_env->d3d_RenderTargetView, g_env->d3d_DepthStencilView);
	g_env->d3d_context->ClearRenderTargetView(g_env->d3d_RenderTargetView, reinterpret_cast<const float*>(&RBColorf::gray));
	g_env->d3d_context->ClearDepthStencilView(g_env->d3d_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	RBD3D11Renderer::render(cam_d3d,_sprites,ef);

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
