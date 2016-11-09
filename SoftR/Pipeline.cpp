#include "Pipeline.h"
#include "SimGPU.h"

#include "../Debug.h"


void SrPipeline::draw(const SrBufferVertex & vertex_buffer, const SrBufferIndex & index_buffer, int num_tri)
{
	rmt_BeginCPUSample(PipLine);
	//_backup_index = &index_buffer;
	//_backup_vertex = &vertex_buffer;
	

	_profler.set_begin();
	rmt_BeginCPUSample(IA);
	_stage_ia->proccess(vertex_buffer, index_buffer, _triangles, num_tri);
	rmt_EndCPUSample();//IA

	_profler.set_end("IA"); _profler.set_begin();
	rmt_BeginCPUSample(VS);
	_stage_vs->proccess(_triangles,num_tri);
	rmt_EndCPUSample();//VS
	
	_profler.set_end("VS"); _profler.set_begin();
	rmt_BeginCPUSample(GS);
	_stage_gs->proccess();
	rmt_EndCPUSample();//GS

	_profler.set_end("GS"); _profler.set_begin();
	rmt_BeginCPUSample(NEAR_FAR_CULL);
	_rasterizer->near_far_cull(_triangles, _triangles_near_far_cull,num_tri);
	rmt_EndCPUSample();//NEAR_FAR_CULL
	_profler.set_end("NFC"); _profler.set_begin();
	rmt_BeginCPUSample(BACK_CULL);
	_rasterizer->back_cull(_triangles_near_far_cull, _triangles_back_cull);
	rmt_EndCPUSample();//BACK_CULL
	_profler.set_end("BC"); _profler.set_begin();
	rmt_BeginCPUSample(CLIP);
	_rasterizer->clip(_triangles_back_cull, _triangles_clip);
	rmt_EndCPUSample();//CLIP

	_profler.set_end("CLIP"); _profler.set_begin();
	rmt_BeginCPUSample(TS);
	_rasterizer->triangle_setup(_triangles_clip, _triangles_fragments);
	rmt_EndCPUSample();//TS
	_profler.set_end("TS"); 
	

	s1.finish_all_pass();

	

	_profler.set_begin();
	rmt_BeginCPUSample(thread_wait);
	int f1 = s1.wait();
	rmt_EndCPUSample();//thread wait
	_profler.set_end("WAIT");

	rmt_EndCPUSample();//Pipline
	//�����Ŀ���merge������
	//_profler.set_begin();
	//s1.merge_result_color(_color_buffer);
	//s1.merge_result_depth(_depth_buffer);
	//_profler.set_end("Merge");
	/*���̴߳�
	g_gpu->finish_pass();
	_profler.set_begin();
	g_gpu->wait();
	_profler.set_end(Profiler::e_tsp);
	*/
	//_rasterizer->shade(_triangles_fragments);
	//_triangles_fragments:px.position.w = c.a;px.normal.x = c.r;px.normal.y = c.g;px.normal.z = c.b;
	//_rasterizer->merge(_triangles_fragments,_color_buffer,_depth_buffer);
	//_profler.out_put_after_time(20);
	_rasterizer->set_last_ts_time(_profler.get_time());

	_show_buffer(_bfidx,_out_tex);

	_clear();

	int f2 = _rasterizer->get_total();

	
	if (f1 != f2)
	{
		printf("Diff:%d\n", f2 - f1);
		//return;
	}
	//s1.print_write_num();
	s1.clear_write_num();
	_rasterizer->clear_total();
}

void SrPipeline::_show_buffer(int index,RBD3D11Texture2D* out_tex)
{
	if(index==0)
		out_tex->write_data(reinterpret_cast<RBColor32*>(&_color_buffer.get_buffer()[0]), _rasterizer->get_width(), _rasterizer->get_height());
	else
		out_tex->write_data(reinterpret_cast<RBColor32*>(&_color_buffers[index-1]->get_buffer()[0]), _rasterizer->get_width(), _rasterizer->get_height());
}

void SrPipeline::_clear_SSBuffer()
{
	memcpy(&_color_buffer.get_buffer()[0], &_o_color_buffer.get_buffer()[0], _color_buffer.size);
	memcpy(&_depth_buffer.get_buffer()[0], &_o_depth_buffer.get_buffer()[0], _depth_buffer.size);

	//���߳�
	for (int i = 0; i < thread_num; ++i)
	{
		memcpy(&_color_buffers[i]->get_buffer()[0], &_o_color_buffer.get_buffer()[0], _color_buffers[i]->size);
		memcpy(&_depth_buffers[i]->get_buffer()[0], &_o_depth_buffer.get_buffer()[0], _depth_buffers[i]->size);
	}
}

#define MAX_QUEUE_SIZE 10240000/4
SrPipeline::SrPipeline():s1(MAX_QUEUE_SIZE)
{
	_bfidx = 0;
	/*
	for(int i=0;i<102400;++i)
	{3124
		_triangles.push_back(new SrTriangle(VertexP3N3T2(),VertexP3N3T2(),VertexP3N3T2()));
	}
	*/

	_profler.init();
	_profler.set_output_inter(10);

	SrTriangle::Init();

	_backup_index = 0;
	_backup_vertex = 0;
	_stage_ia = new SrStageIA();
	_stage_vs = new SrStageVS();
	_stage_gs = new SrStageGS();
	//���߳�
	//_rasterizer = new SrRasterizer();
	_rasterizer = new SrRasterizer(&s1);
	int w = RW;
	int h = RH;
	_rasterizer->set_viewport_shape(w, h);

	_depth_buffer.init(w,h);
	_color_buffer.init(w,h);


	//���߳�
	for (int i = 0; i < thread_num; ++i)
	{
		auto cbf = new SrSSBuffer<RBColor32>;
		auto dbf = new SrSSBuffer<float>;
		cbf->init(w,h);
		dbf->init(w,h);
		_depth_buffers.push_back(dbf);
		_color_buffers.push_back(cbf);
		//s1.set_container(i,cbf,dbf);
		s1.set_container(i,&_color_buffer,&_depth_buffer);
	}

	_co_depth_buffer.init(w, h);
	_co_color_buffer.init(w, h);

	_o_depth_buffer.init(w, h);
	_o_color_buffer.init(w, h);
	_o_depth_buffer.set_vals(1.1);
	_o_color_buffer.set_vals(125);

	_clear_SSBuffer();

	_rasterizer->set_depth_buffer(&_depth_buffer);
	_rasterizer->set_color_buffer(&_color_buffer);

	//���̴߳�
	//g_gpu->set_container(&_color_buffer,&_depth_buffer);
	

}

SrPipeline::~SrPipeline()
{
	
	for (int i = 0; i < thread_num; ++i)
	{
		delete _depth_buffers[i];
		delete _color_buffers[i];
	}
	delete _rasterizer;
	delete _stage_gs;
	delete _stage_vs;
	delete _stage_ia;
	s1.terminal();
#ifndef POOL
	SrTriangle::Deinit();
#endif
}

void SrPipeline::_clear()
{	
	//SrTriangle::print_mem();
#ifdef POOL
	for (auto& tri : _triangles)
	{
		delete tri;
		tri  = nullptr;
	}
#else
	SrTriangle::ReleaseAll();
#endif


	_triangles.clear();
	_triangles_near_far_cull.clear();
	_triangles_back_cull.clear();
	_triangles_clip.clear();
	_triangles_fragments.clear();
	


}