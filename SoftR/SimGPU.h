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
/*
�������⣬͸��������Ⱦhttp://zhuanlan.zhihu.com/sildenafil/20180902
http ://graphics.cs.cmu.edu/courses/15869/fall2014/article/1
*/

const int thread_num = 4;
//#define TRI
void func();
class SrSimGPU
{
public:
#ifdef TRI
	struct DataPak_Triangle
	{
		DataPak_Triangle(size_t queue_size) :queue(queue_size), color(nullptr), depth(nullptr),_raster(nullptr), _stage_om(nullptr), _stage_ps(nullptr),
			ter(false), done(false), total(0), total_frag(0), run(false),idx(-1) {}
		folly::ProducerConsumerQueue<SrTriangle*> queue;
		SrSSBuffer<RBColor32>* color;
		SrSSBuffer<float>* depth;
		SrStagePS* _stage_ps;
		SrStageOM* _stage_om;
		SrRasterizer* _raster;
		bool ter;
		bool done;
		int total;
		int total_frag;
		std::condition_variable cv;
		std::mutex mt;
		volatile bool run;

		int idx;

		RBSem sem;
		void set_container(SrSSBuffer<RBColor32>* color, SrSSBuffer<float>* depth)
		{
			this->color = color;
			this->depth = depth;
		}
		void set_stage(SrStagePS* stage_ps, SrStageOM* stage_om)
		{
			_stage_om = stage_om;
			_stage_ps = stage_ps;
		}
		void wait()
		{
			sem.wait();
		}
	};
#else
	struct DataPak
	{
		DataPak(size_t queue_size):queue(queue_size),color(nullptr),depth(nullptr),_stage_om(nullptr),_stage_ps(nullptr),
		ter(false),done(false),total(0), total_frag(0),run(false){}
		folly::ProducerConsumerQueue<VertexP3N3T2> queue;
		SrSSBuffer<RBColor32>* color;
		SrSSBuffer<float>* depth;
		SrStagePS* _stage_ps;
		SrStageOM* _stage_om;
		bool ter;
		bool done;
		int total;
		int total_frag;
		std::condition_variable cv;
		std::mutex mt;
		volatile bool run;

		RBSem sem;
		void set_container(SrSSBuffer<RBColor32>* color, SrSSBuffer<float>* depth)
		{
			this->color = color;
			this->depth = depth;
		}
		void set_stage(SrStagePS* stage_ps, SrStageOM* stage_om)
		{
			_stage_om = stage_om;
			_stage_ps = stage_ps;
		}
		void wait()
		{
			/*
			printf("run : %s\n", run ? "true" : "false");
			int i = 0;
			while (!run);
			printf("false %d\n",i);
			run = false;
			*/
			sem.wait();
		}
	};
#endif
	SrSimGPU(size_t queue_size)
	{
#ifdef TRI
		for (int i = 0; i < thread_num; ++i)
		{
			dtpks[i] = new DataPak_Triangle(queue_size);
			dtpks[i]->idx = i;
			t[i] = std::thread(&SrSimGPU::run, dtpks[i]);
		}
#else
		for (int i = 0; i < thread_num; ++i)
		{
			dtpks[i] = new DataPak(queue_size);
			t[i] = std::thread(&SrSimGPU::run,dtpks[i]);
		}
#endif
	}
	~SrSimGPU()
	{
		for (int i = 0; i < thread_num;++i)
		{
			//delete dtpks[i];
			t[i].detach();
		}
	}

	bool discard_invisible(const VertexP3N3T2& data)
	{
		if (data.position.z <= 0)
			return true;

	}

#ifdef TRI
	//д����С���ȵ�queue
	void write_min(SrTriangle* data)
	{
		int min = MAX_I32;
		int min_i = 3;
		for (int i = 0; i < thread_num; ++i)
		{
			int cur = dtpks[i]->queue.sizeGuess();
			if (cur < min)
			{
				min = cur;
				min_i = i;
			}
		}
		int k = dtpks[min_i]->queue.sizeGuess();
		//��Լ��queue�ĳ���
		if (k < 10239988)
			write(min_i, data);
		else
			printf("discard!\n");
	}



	inline void write(int index, SrTriangle* data)
	{
		write_num++;

		printf("write %d\n", index);
		dtpks[index]->queue.write(data);
	}
#else
	//д����С���ȵ�queue
	void write_min(VertexP3N3T2& data)
	{
		int min = MAX_I32;
		int min_i = 3;
		for (int i = 0; i < thread_num; ++i)
		{
			int cur = dtpks[i]->queue.sizeGuess();
			if (cur < min)
			{
				min = cur;
				min_i = i;
			}
		}
		int k = dtpks[min_i]->queue.sizeGuess();
		//��Լ��queue�ĳ���
		if (k < 10239988)
			write(min_i, data);
		else
			printf("discard!\n");
	}



	inline void write(int index,const VertexP3N3T2& data)
	{
		write_num++;
		
			dtpks[index]->queue.write(data);
	}
#endif


	void set_stage(int index,SrStagePS* _stage_ps,SrStageOM* _stage_om)
	{
		dtpks[index]->set_stage(_stage_ps, _stage_om);
	}

	void set_container(int index,SrSSBuffer<RBColor32>* color, SrSSBuffer<float>* depth)
	{
		dtpks[index]->set_container(color, depth);
	}

#ifdef TRI
	void set_raster(int index,SrRasterizer* raster)
	{
		dtpks[index]->_raster = raster;

	}
#endif

	inline void finish_pass(int index)
	{
#ifdef TRI
		SrTriangle* t = new SrTriangle();
		printf("write a finish %d\n",index);
		dtpks[index]->queue.write(t);
#else
		VertexP3N3T2 v;
		v.finish = true;
		write(index,v);
#endif
	}

	void finish_all_pass()
	{
		for (int i = 0; i < thread_num; ++i)
		{
			finish_pass(i);
		}
	}

	inline int gauss_size(int index)
	{
		return dtpks[index]->queue.sizeGuess();
	}

	void print_write_num()
	{
		printf("write_num : %d\n",write_num);
	}

	void clear_write_num()
	{
		write_num = 0;
	}

	int wait()
	{
		int res=0;
		for (int i = 0; i < thread_num; ++i)
		{
			/*
			if (dtpks[i]->total_frag > 20000)
			{
				dtpks[i]->queue.clear();
				VertexP3N3T2 v;
				v.finish = true;
				dtpks[i]->queue.write(v);
			}
			*/
			//printf("wait:%d\n",i);
			dtpks[i]->sem.wait();
			//dtpks[i]->wait();
			//printf("Thread %d finished!\n",i);
			
			res += dtpks[i]->total_frag;
			
			dtpks[i]->total_frag = 0;
		}
		return res;
	}
#ifdef TRI



	static void run(DataPak_Triangle* s)
	{
		//s���ܻᱻ����
		while (!s->ter)
		{

			SrTriangle* v;
			if (s->queue.read(v))
			{
				printf("read %d\n", s->idx);
				v->v[0].position.out();
				v->v[1].position.out();
				v->v[2].position.out();

				if (v->v[0].finish)
				{
					printf("finish %d\n",s->idx);
					if (s->queue.read(v))
					{
						if (!v->v[0].finish)
						{
							printf("break\n");
							getchar();
						}
					}
					s->run = true;
					s->sem.signal();
				}
				else
				{
					if (s->queue.sizeGuess() > s->total)
						s->total = s->queue.sizeGuess();
					s->total_frag++;
					s->_raster->new_set_tri2(v, s->_stage_ps, s->_stage_om);
				}

			}
			else
			{
				//s->done = true;
				continue;
			}

		}
		printf("Queue Num : %d\n", s->total);
	}
#else
	static void run(DataPak* s)
	{
		//s���ܻᱻ����
		while (!s->ter)
		{
			
			VertexP3N3T2 v;
			//if(s->run)
				//printf("s->run : %s\n", s->run ? "true" : "false");
			if (s->queue.read(v))
			{
				
				if (v.finish)
				{
					//printf("finish\n");
					if (s->queue.read(v))
					{
						printf("break\n");
						getchar();
					}
					s->run = true;
					//printf("set : %s\n",s->run?"true":"false");

					s->sem.signal();
				}
				else
				{

					//s->done = false;
					if (s->queue.sizeGuess() > s->total)
						s->total = s->queue.sizeGuess();
					//s->depth
					s->_stage_ps->proccess(v);
					s->_stage_om->proccess(v, *(s->color), *(s->depth));
					s->total_frag++;
					
					
				}
			}
			else
			{
				//s->done = true;
				continue;
			}

		}
		printf("Queue Num : %d\n",s->total);
		//g_logger->debug_print(WIP_INFO, "Queue Num:%d", s->total);
	}
#endif

	inline void terminal(int index)
	{
		dtpks[index]->ter = true;
	}

	void terminal()
	{
		for (int i = 0; i < thread_num; ++i)
		{
			dtpks[i]->ter = true;
		}

	}

	void merge_result_color(SrSSBuffer<RBColor32>& out)
	{
		for (int i = 0; i < out.h; ++i)
		{
			for (int j = 0; j < out.w; ++j)
			{
				int index = _get_min_depth_index(j,i);
				out.set_data(j,i,dtpks[index]->color->get_data(j, i));
			}
		}
	}

	void merge_result_depth(SrSSBuffer<float>& out)
	{
		for (int i = 0; i < out.h; ++i)
		{
			for (int j = 0; j < out.w; ++j)
			{
				out.set_data(j, i, _get_min_depth(j, i));
			}
		}
	}

	int get_total_frag()
	{
		int res = 0;
		for (int i = 0; i < thread_num; ++i)
			res += dtpks[i]->total_frag;
		return res;
	}

	//extern static const int thread_num;
private:
	//std::mutex lock_queue;
	//std::deque<VertexP3N3T2> queue;
	int write_num;
	static std::mutex mu;
	static std::condition_variable g_pass_done;
	
	std::thread t[thread_num];
#ifdef  TRI
	DataPak_Triangle* dtpks[thread_num];
#else

	DataPak* dtpks[thread_num];
#endif
	int _get_min_depth_index(int x,int y)
	{
		float min = MAX_F32;
		int min_i = 0;
		for (int i = 0; i < thread_num; ++i)
		{
			float cur = dtpks[i]->depth->get_data(x,y);
			if (cur < min)
			{
				min = cur;
				min_i = i;
			}
		}
		return min_i;
	}

	int _get_min_depth(int x, int y)
	{
		float min = MAX_F32;
		int min_i = 0;
		for (int i = 0; i < thread_num; ++i)
		{
			float cur = dtpks[i]->depth->get_data(x, y);
			if (cur < min)
			{
				min = cur;
				min_i = i;
			}
		}
		return min;
	}
};
