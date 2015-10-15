#include "Pipeline.h"

void SrPipeline::draw(const SrBufferVertex & vertex_buffer, const SrBufferIndex & index_buffer, int num_tri)
{

	//_backup_index = &index_buffer;
	//_backup_vertex = &vertex_buffer;
	_stage_ia->proccess(vertex_buffer, index_buffer, _triangles, num_tri);
	_stage_vs->proccess(_triangles);
	_stage_gs->proccess();
	_resterizer->near_far_cull(_triangles, _triangles_near_far_cull);
	_resterizer->back_cull(_triangles_near_far_cull, _triangles_back_cull);
	_resterizer->clip(_triangles_back_cull, _triangles_clip);
	_resterizer->triangle_setup(_triangles_clip, _triangles_fragments);
	//_resterizer->shade(_triangles_fragments);
	//_triangles_fragments:px.position.w = c.a;px.normal.x = c.r;px.normal.y = c.g;px.normal.z = c.b;
	//_resterizer->merge(_triangles_fragments,_color_buffer,_depth_buffer);

	_show_buffer(_out_tex);

	clear();
}

void SrPipeline::_show_buffer(RBD3D11Texture2D* out_tex)
{
	out_tex->write_data(reinterpret_cast<RBColor32*>(&_color_buffer.get_buffer()[0]), _resterizer->get_width(), _resterizer->get_height());
}

void SrPipeline::_clear_SSBuffer()
{
	memcpy(&_color_buffer.get_buffer()[0], &_o_color_buffer.get_buffer()[0], _color_buffer.size);
	memcpy(&_depth_buffer.get_buffer()[0], &_o_depth_buffer.get_buffer()[0], _depth_buffer.size);
}

SrPipeline::SrPipeline()
{
	_backup_index = 0;
	_backup_vertex = 0;
	_stage_ia = new SrStageIA();
	_stage_vs = new SrStageVS();
	_stage_gs = new SrStageGS();
	_resterizer = new SrRasterizer();
	int w = 800;
	int h = 600;
	_resterizer->set_viewport_shape(w, h);

	_depth_buffer.init(w,h);
	_color_buffer.init(w,h);
	_o_depth_buffer.init(w, h);
	_o_color_buffer.init(w, h);
	_o_depth_buffer.set_vals(1.1);
	_o_color_buffer.set_vals(125);

	_clear_SSBuffer();

	_resterizer->set_depth_buffer(&_depth_buffer);
	_resterizer->set_color_buffer(&_color_buffer);
}

SrPipeline::~SrPipeline()
{
	delete _resterizer;
	delete _stage_gs;
	delete _stage_vs;
	delete _stage_ia;
}

void SrPipeline::clear()
{
	for (auto tri : _triangles)
		delete tri;
	_triangles.clear();
	_triangles_near_far_cull.clear();
	_triangles_back_cull.clear();
	_triangles_clip.clear();
	_triangles_fragments.clear();
	//批量修改内存，下列函数非常耗时
	//_color_buffer.set_vals(0.f);
	//_depth_buffer.set_vals(1.1f);
	_clear_SSBuffer();

}