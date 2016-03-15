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
  //出于方便使用名字索引shader uniform变量，但是效率及其低下
  //vs->set_constant_buffer("matrix", bf);
  //ps->set_texture("texture", tf);
  //使用数组直接用索引来处理提升50%性能
  vs->set_constant_buffer_index(0,bf);
  ps->set_texture_index(0,tf);
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
- **完善管线，按照标准的现代管线完善，clipping space，clipping等等**
- 添加一个帧率太低就直接discard，并且记录下来的功能
- 各个部件的Release测试
- 线程安全处理
- 新的并行模式
- SIMD优化
- Blending:这个在并行优化之后再做，有一种方案的优化并不允许做blending而只允许test。
- 渲染至纹理
- 双缓存
- 并行优化
- 完美裁剪
- MSAA
- 混合
- mipmap
- 多种纹理采样方案
- OIT

Rencetly Update
=========
**2016/2/12**
-	添加内存池（非线程安全），用在了SrTriangle上，重载了new和delete
-	logger添加了一个简单的print函数，logger没有线程安全
-	工程调整为main启动的程序，RBLog被重定向到logger
-	工程移到VS2015支持c++11，去掉了dxerr的支持，现在所有的dx调试信息都移除了，包括HR宏和Shader中的DXTRACE_ERR_MSGBOX都已经失效
-	更新了profiler，现在可以更在自由的使用字符串标识测试结果

**2016/2/13**
-	添加多线程支持，在scanline阶段之后程序并行，暂时考虑为片元处理为并行，这种方法还要解决多线程结果merge的问题，如果逐像素merge消耗太高，不实际，不如直接对原buffer进行加锁读取，还有一种考虑是实现一个无锁的SSBuffer。还有一种方案是图元并行，这种方法应该更加高效简单
-	Release模式无法运行无法测试，需要修改，其中FreeImage运行出错，怀疑是编译器版本不对

**2016/2/14**
-	使用while语句同步，这句同步在Release会被优化，所以Release优化已关闭

**2016/3/1**
-	更新了多线程，使用信号量进行同步。

**2016/3/15**
-	添加了Remotery来Profile。运来的Profile依然在工作。

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
