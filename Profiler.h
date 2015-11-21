#pragma once
#include "Uitilities.h"
#include "RBTimer.h"


class Profiler
{
public:
	static const int max_elements = 10;
	enum ProfTypes
	{
		e_ia,
		e_vs,
		e_gs,
		e_nfc,
		e_bc,
		e_clip,
		e_tsp,
	};
	void init()
	{
		_timer.init();
		_out_cur_times = 0.f;
		_cur_time = 0.f;
		for (int i = 0; i < max_elements; ++i)
		{
			_gather_elements[i] = 0.f;
		}
	}
	void set_begin()
	{
		_timer.begin();
	}
	void set_end(int index)
	{
		
		_gather_elements[index] += _timer.end();
	}
	void gather_element(int index, float time)
	{
		_gather_elements[index] += time;
	}
	float get_element(int index)
	{
		return _gather_elements[index];
	}
	//循环多少次输出一次
	void out_put_after_time(int times)
	{
		_out_cur_times += 1;
		if (_out_cur_times >= times)
		{
			float it = 1.f / times;
			char a[128];
			sprintf(a, "TriSet:%.0f,ScanLine:%.0f,PS+OM:%.0f,PS:%.2f,OM:%.0f,Scanline_lerp:%.0f,TSP:%.0f\n", it*_gather_elements[0],it*_gather_elements[1],it*_gather_elements[2],it*_gather_elements[3],it*_gather_elements[4],it*_gather_elements[5],it*_gather_elements[6]);
			RBlog(a);

			for (int i = 0; i < max_elements;++i )
			{
				_gather_elements[i] = 0.f;
			}

			_out_cur_times = 0;
		}
	}
private:
	float _cur_time;
	float _out_cur_times;
	float _gather_elements[max_elements];
	rb::rb_timer_t _timer;
};