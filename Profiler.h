#pragma once
#include "Uitilities.h"
#include "RBTimer.h"
#include <map>
#include <string>

class Profiler
{
private:
	typedef std::map<std::string,float> GartherMap;
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
	void set_end(std::string name)
	{
		
		GartherMap::iterator it = _gather_element_ex.find(name);
		if(it==_gather_element_ex.end())
		{
			_gather_element_ex.insert(GartherMap::value_type(name,_timer.end()));
		}
		else
		{
			_gather_element_ex[name] += _timer.end();
		}
	}
	void gather_element(int index, float time)
	{
		_gather_elements[index] += time;
	}
	float get_element(int index)
	{
		return _gather_elements[index];
	}
	void clear()
	{
		for (int i = 0; i < max_elements; ++i)
		{
			_gather_elements[i] = 0.f;
		}
		_gather_element_ex.clear();
	}

	//循环多少次输出一次
	void out_put_after_time(int times)
	{
		_out_cur_times += 1;
		if (_out_cur_times >= times)
		{
			float it = 1.f / times;
			std::string str;
			for(GartherMap::iterator i = _gather_element_ex.begin();i!=_gather_element_ex.end();++i)
			{
				std::string a = i->first;
				char num[64];
				sprintf(num,": %f  ",i->second*it);
				a += num;
				i->second = 0.f;
				str += a;
			}
			str+="\n";
			//char a[128];
			//sprintf(a, "TriSet:%.0f,ScanLine:%.0f,PS+OM:%.0f,PS:%.2f,OM:%.0f,Scanline_lerp:%.0f,TSP:%.0f\n", it*_gather_elements[0],it*_gather_elements[1],it*_gather_elements[2],it*_gather_elements[3],it*_gather_elements[4],it*_gather_elements[5],it*_gather_elements[6]);
			RBlog(str.c_str());
			/*
			for (int i = 0; i < max_elements;++i )
			{
				_gather_elements[i] = 0.f;
			}
			*/
			_out_cur_times = 0;
		}
	}
private:
	float _cur_time;
	float _out_cur_times;
	float _gather_elements[max_elements];
	rb::rb_timer_t _timer;

	GartherMap _gather_element_ex;
};