#pragma once
#include "Effect.h"
#include "VertexFormats.h"
class RBCamera;
class RBD3D11Texture;
class RBD3D11Texture2D;
class RB2DRenderingEffect : public RBEffect
{
public:
	typedef VertexFormats::Vertex_PT vertex_format;
	~RB2DRenderingEffect();
	static RB2DRenderingEffect* create();
	bool init(const char* shader_file);
	void set_matrices(const RBMatrix& model, const RBMatrix& view, const RBMatrix& ortho);
	void apply() const;
	void update(float dt, RBCamera* cam);
	void release_layout();
	void set_texture(RBD3D11Texture* _tex);
	void render(const RBCamera* cam, EffectObjects::_SingleList& obj);
	void render(const RBCamera* cam, std::vector<RBD3D11Texture2D*>& objs);
	void clear();
protected:

private:
	RBVertexShader* _vs;
	RBPixelShader* _ps;
	ID3D11InputLayout* _layout;
	RBD3D11Texture* _tex;
};