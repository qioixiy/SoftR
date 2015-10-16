Intro
=======

这是一个简单的可编程软件渲染器[SoftR](http://claireswallet.farbox.com/post/graphics/guang-zha-hua-xuan-ran-qi)。

## API

SoftRApp中可以设置渲染的模型（目前只支持obj格式）和纹理。
```cpp
  //创建渲染管线并初始化输出设备
  SrPipeline* pip = new SrPipeline();
  pip->set_out_tex(out_tex);
  //读取obj模型数据并初始化顶点buffer
  RBObject* obj = RBObject::create_object();
  obj->load_mesh("objs/tri.obj");
  obj->generate_softr_buffer<VertexFormats::Vertex_PCNT>();
  obj->_node->set_position(0, -1,0);
  obj->_node->rotate(-30, 195, 0);
  //创建着色器
  BaseShaderPS* ps = new BaseShaderPS();
  BaseShaderPS* vs = new BaseShaderVS();
  //创建常量结构体
  ShaderMatrixBuffer* mb = new ShaderMatrixBuffer();
  //创建常量buffer，并写入数据
  SrBufferConstant* bf = new SrBufferConstant();
  bf->init(mb, sizeof(ShaderMatrixBuffer));

  //创建纹理
  SrTexture2D* tf = SrTexture2D::creat("Res/pp.jpg");

  //...
  //draw call准备
  obj->_node->get_mat(mb->m);
  cam->get_view_matrix(mb->v);
  cam->get_perspective_matrix(mb->p);
  //使用名字索引shader uniform变量
  vs->set_constant_buffer("matrix", bf);
  ps->set_texture("texture", tf);
  pip->set_ps(ps);
  pip->set_vs(vs);
  //draw call
  pip->draw(*obj->get_softr_vertex_buffer(), *obj->get_softr_index_buffer(),obj->get_index_count()/3);

```

## Shader例程

目前shader使用C++写成，只需要继承shader父类即可。

VS例子：
```cpp
#include "ShaderVertex.h"
#include "..\\RBMath\\Inc\\Matrix.h"
#include "BufferConstant.h"
struct ShaderMatrixBuffer
{
	RBMatrix m;
	RBMatrix v;
	RBMatrix p;
};
class BaseShaderVS : public SrShaderVertex
{
public:
	void shade(VertexP3N3T2& v)
	{
		VertexP3N3T2 iv = v;
		SrBufferConstant* mb = get_cbuffer("matrix");
		ShaderMatrixBuffer* cmb = (ShaderMatrixBuffer*)(mb->_data);
		v.position = iv.position * cmb->m;
		v.position = v.position * cmb->v;
		v.position = v.position * cmb->p;
		RBMatrix rot = cmb->m.get_rotation();
		v.normal = RBVector4(v.normal,1) * rot;
	}
};
```
PS例子：
```cpp
#include "ShaderPixel.h"
#include "Texture2D.h"
#include "SamplePoint.h"
class BaseShaderPS : public SrShaderPixel
{
public:
	RBColorf shade(VertexP3N3T2& vert_lerp)
	{
		SrTexture2D* tex = get_texture2d("texture");
		SrSamplerPoint sp;
		RBColorf tc = sp.sample(tex, vert_lerp.text_coord.x, vert_lerp.text_coord.y);
		vert_lerp.normal.normalize();
		RBVector3 v = vert_lerp.position;
		v = v.get_abs();
		v.normalize();
		RBVector3 light(-1,-1,-1);
		float cost = RBVector3::dot_product(vert_lerp.normal, light);
		RBColorf oc = RBColorf::white;
		if (cost <= 0)
			oc = RBColorf::black;
		RBVector4 outc = oc;
		outc = outc * cost;
		outc.w = 1.0;
		if (outc.x > 1) outc.x = 1.f;
		if (outc.y > 1) outc.y = 1.f;
		if (outc.z > 1) outc.z = 1.f;
		if (tc.a < 0.0001)
			RBColorf f = RBColorf::black;
		RBColorf c =outc* tc;
		return c;
	}
};
```

Note
=======

数学库按照DX标准

默认绕序顺时针顶点顺序

左手系

右乘矩阵

行向量



Rencently TODO
=========
- 渲染至纹理
- 更高效的shader
- 双缓存
-	并行优化
- 完美裁剪
- MSAA
- 混合
- mipmap
- 多种纹理采样方案
- OIT



Platform Dependences
========

渲染器使用DirectX11渲染最终像素到屏幕。仅在最后阶段使用了一张D3D11纹理来承载最终渲染结果。

D3D11需要包含的库：

>d3d11.lib

>d3dx11d.lib

>D3DCompiler.lib

>dxerr.lib

>dxgi.lib

>dxguid.lib


Rendering results
========

渲染图样及其文档：[http://claireswallet.farbox.com/post/graphics/guang-zha-hua-xuan-ran-qi](http://claireswallet.farbox.com/post/graphics/guang-zha-hua-xuan-ran-qi)
