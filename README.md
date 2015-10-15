Intro
=======

这是一个简单的可编程软件渲染器[SoftR](http://claireswallet.farbox.com/post/graphics/guang-zha-hua-xuan-ran-qi)。


Note
=======

数学库按照DX标准

默认绕序顺时针顶点顺序

左手系

右乘矩阵

行向量



Rencently TODO
=========
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
