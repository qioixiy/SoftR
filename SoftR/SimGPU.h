#pragma once
#include <thread>
#include "VertexFormat.h"
#include "InnerData.h"
#include <deque>
#include <mutex>
#include "StageOM.h"
#include "StagePS.h"
#include "Rasterizer.h"
#include <condition_variable>
#include "..\\Logger.h"
#include "..\\ProducerConsumerQueue.h"
#include "../RBMath/Inc/Color32.h"
#include <condition_variable>
#include "../Uitilities.h"
#include "../ThreadPool.h"
#include "../Mem/MemoryFrame.h"
/*
保序问题，透明物体渲染http://zhuanlan.zhihu.com/sildenafil/20180902
http ://graphics.cs.cmu.edu/courses/15869/fall2014/article/1
*/
#include <atomic>




const int thread_num = 2;
#define TRI

class SrSimGPU
{
public:
	struct DataPak
	{
		DataPak(SrSSBuffer<RBColor32>* colort, 
			SrSSBuffer<float>* deptht, SrStagePS*sp, SrStageOM* so, SrRasterizer* rs,
			RBSem* semt,SrTriangle* vt) :
			color(colort), depth(deptht),
			_stage_om(so), _stage_ps(sp), _raster(rs),sem(semt),v(vt) {}
		SrSSBuffer<RBColor32>* color;
		SrSSBuffer<float>* depth;
		SrStagePS* _stage_ps;
		SrStageOM* _stage_om;
		SrRasterizer* _raster;
		SrTriangle* v;
		RBSem *sem;
	};



	class RenderTask : public Task
	{
	public:
		RenderTask() { }
		virtual int run(int id) override
		{
			DataPak* args = static_cast<DataPak*>(this->arg_);
			args->_raster->trangle_setup_gpu(args->v);
			SrSimGPU::handle_count++;
			args->sem->signal();
			return 0;
		}
		
	};

	static void mem_list_init();
	static void mem_list_deinit();
	static ThreadMem *mem_list;

	SrSimGPU(int thread_n):excutor(thread_n)
	{
		sem.reset();
		total_task = 0;
	}
	~SrSimGPU()
	{
	}

	void add_task(SrTriangle* v)
	{
		DataPak* pak = new(
			mem_list->frame.alloc(sizeof(DataPak),false)
			) DataPak(_color, _depth, _stage_ps, _stage_om,_raster, &sem, v);
		RenderTask* task = new(
			mem_list->frame.alloc(sizeof(RenderTask), false)
			)
			RenderTask();
		task->setArg(pak);
		excutor.addTask(task);
		total_task++;
	}

	void wait()
	{
		for (int i = 0; i < total_task; ++i)
		{
			sem.wait();
		}
		if (total_task != handle_count)
		{
			printf("%d/%d\n", SrSimGPU::handle_count,total_task);
		}
		total_task = 0;
		handle_count = 0;
		mem_list->frame.release(mem_list->mf);
	}

	//extern static const int thread_num;
private:

	int write_num;

	

public:
	void set_color_buffer(SrSSBuffer<RBColor32>* colort)
	{
		_color = colort;
	}
	void set_depth_buffer(SrSSBuffer<float>* deptht)
	{
		_depth = deptht;
	}
	void set_stage_ps(SrStagePS* stage_ps)
	{
		_stage_ps = stage_ps;
	}
	void set_stage_om(SrStageOM* stage_om)
	{
		_stage_om = stage_om;
	}
	void set_raster(SrRasterizer* rs)
	{
		_raster = rs;
	}
private:

	ThreadPool excutor;
	SrSSBuffer<RBColor32>* _color;
	SrSSBuffer<float>* _depth;
	SrStagePS* _stage_ps;
	SrStageOM* _stage_om;
	SrRasterizer* _raster;
	RBSem sem;

	int total_task;

	static atomic_int handle_count;
};
