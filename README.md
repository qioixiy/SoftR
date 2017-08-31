Intro
=======

简单的可编程软件渲染器[SoftR](http://claireswallet.farbox.com/post/graphics/guang-zha-hua-xuan-ran-qi)。

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
  //出于方便使用名字索引shader uniform变量，但效率低
  //vs->set_constant_buffer("matrix", bf);
  //ps->set_texture("texture", tf);
  vs->set_constant_buffer_index(0,bf);
  ps->set_texture_index(0,tf);
  pip->set_ps(ps);
  pip->set_vs(vs);
  //draw call
  pip->draw(*obj->get_softr_vertex_buffer(), *obj->get_softr_index_buffer(),obj->get_index_count()/3);

```

## Shader例程

目前shader使用C++写成，继承shader父类。

VS例子：
```cpp
#include "ShaderVertex.h"
#include "Matrix.h"
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
		//SrBufferConstant* mb = get_cbuffer("matrix");
        SrBufferConstant* mb = get_cbuffer_index(0);
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
		//SrTexture2D* tex = get_texture2d("texture");
    SrTexture2D* tex = get_texture2d_index(0);
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

-   裁剪有bug，参考神庙场景
-   效率优化，线扫方法，神庙全纹理，基本着色器，起码20fps!参考[https://github.com/gameknife/SoftRenderer](https://github.com/gameknife/SoftRenderer)
-   整理重构！

Future works
=========

-	**实用性改善！** ：**等价的并行架构，可以用于在CPU上等价模拟DirectCompute计算。**
-   **完善基本的管线，按照标准的现代管线完善，clipping space，clipping等等，标准API**
-   SIMD加速，单独编译的Shader
-   CommandQueue
-   Blending
-   更完整的渲染功能（如支持**渲染至纹理**，**多缓冲**，**mipmap**，**
MSAA**等）
-   更多的纹理采样方案
-   OIT
-   新的光栅化/并行架构
-   API D3D12化


Platform Dependences
========

渲染器使用DirectX11渲染最终像素到屏幕。仅在最后阶段使用了一张D3D11纹理来承载最终渲染结果。


Rendering results
========
![](https://github.com/wubugui/FXXKTracer/raw/master/pic/19-45-26.jpg)
![](https://github.com/wubugui/FXXKTracer/raw/master/pic/15-51-03.jpg)
![](https://github.com/wubugui/FXXKTracer/raw/master/pic/QQ%E5%9B%BE%E7%89%8720161123215229.png)
![](https://github.com/wubugui/FXXKTracer/raw/master/pic/softr1.png)

实现笔记：[http://claireswallet.farbox.com/post/graphics/guang-zha-hua-xuan-ran-qi](http://claireswallet.farbox.com/post/graphics/guang-zha-hua-xuan-ran-qi)
