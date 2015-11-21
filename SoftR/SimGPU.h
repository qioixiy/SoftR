#pragma once
#include <thread>
#include "VertexFormat.h"
#include "InnerData.h"
#include <deque>
#include <mutex>
#include "StageOM.h"
#include "StagePS.h"
#include <condition_variable>
#include "..\\Logger.h"

class SrSimGPU
{
public:
	SrSimGPU()
	{
		done = false;
		ter = false;
		total = 0;
	}
	~SrSimGPU()
	{

	}

	void set_container(SrSSBuffer<RBColor32>* color,SrSSBuffer<float>* depth)
	{
		this->color = color;
		this->depth = depth;
	}

	void access(bool read, VertexP3N3T2& data)
	{
		lock_queue.lock();
		if (read)
		{
			data = queue.front();
			queue.pop_front();
		}
		else
		{
			queue.push_back(data);
		}
		lock_queue.unlock();
	}

	void set_stage(SrStagePS* _stage_ps,SrStageOM* _stage_om)
	{
		this->_stage_ps = _stage_ps;
		this->_stage_om = _stage_om;
	}

	void finish_pass()
	{
		VertexP3N3T2 v;
		v.finish = true;
		access(false,v);
	}

	void wait()
	{
		while (!done);
		done = false;
	}

	void run()
	{
		while (!ter)
		{
			if (queue.empty()) 
				continue;
			VertexP3N3T2 v;
			access(true,v);
			if (!v.finish)
			{
				if (queue.size() > total)
					total = queue.size();
				_stage_ps->proccess(v);
				_stage_om->proccess(v, *color, *depth);
				
			}
			else
			{
				
				//g_pass_done.notify_one();
				done = true;
			}
		}
		std::deque<VertexP3N3T2>().swap(queue);
		g_logger->debug_log(WIP_INFO, "Queue Num:%d", total);
	}

	void terminal()
	{
		ter = true;
	}
private:
	std::mutex lock_queue;
	//可能使用std::queue
	std::deque<VertexP3N3T2> queue;
	SrSSBuffer<RBColor32>* color;
	SrSSBuffer<float>* depth;
	SrStagePS* _stage_ps;
	SrStageOM* _stage_om;
	bool done;
	std::mutex mu;
	std::condition_variable g_pass_done;
	bool ter;
	//std::thread& t;

	int total;
};

extern SrSimGPU* g_gpu;