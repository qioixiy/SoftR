//_Pragma("once")
#pragma once
#pragma warning(disable : 4005)
#include <vector>
#include "d3dApp.h"
#include "RBMath\\Inc\\Matrix.h"
#include "RBMath\Inc\Vector2.h"
#include "RBMath\Inc\Vector3.h"
#include "RBMath\Inc\Vector4.h"
#include "RBMath\Inc\Colorf.h"

#include "Camera.h"
#include "Uitilities.h"
#include "D3D11Texture2D.h"
#include "2DRendering.h"
#include "SoftR\\Pipeline.h"
#include "SoftR\\BaseShaderVS.h"
#include "SoftR\\BaseShaderPS.h"
#include "SoftR\\SimGPU.h"

struct ShaderMatrixBuffer;

class SoftRApp : public D3DApp
{
public:
	SoftRApp(HINSTANCE hInstance);
	~SoftRApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
	void Termination();
	void set_viewport(float ltx, float lty, float w, float h, float min_depth = 0.f, float max_depth = 1.f);
	void set_depth_state(bool val);


	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

	virtual void OnKeyDown(WPARAM btnState);
	virtual void OnKeyUp(WPARAM btnState);

	void set_cam_move_speed(float x, float y);
	void set_cam_rotate_speed(float x, float y);
protected:
	//void load_assets();
	//void load_pipeline();
	//void load_scene1();
	//void load_sprites();

	//void update_effects(float dt);
	void handle_input(float dt);
	//void load_texture();

	RBMatrix m, v, p;

	RBCamera* cam;
	RBCamera* cam_d3d;

	int old_x, old_y;

	RBVector2 _cam_move_speed;
	RBVector2 _cam_rotate_speed;


	RBD3D11Texture2D *out_tex;

	RB2DRenderingEffect* ef;
	std::vector<RBD3D11Texture2D* > _sprites;


	RBObject* obj;
	SrPipeline* pip;
	BaseShaderPS* ps;
	BaseShaderVS* vs;
	SrBufferConstant* bf;
	SrTexture2D* tf;
	ShaderMatrixBuffer* mb;

private:
	bool _init_platform();
	bool _init_softr();

	std::thread* t;
};