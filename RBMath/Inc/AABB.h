#pragma once
#include "Vector2.h"

struct RBAABB
{
	RBVector2 lb;
	f32 width,height;
	RBAABB();
	void set(f32 lb_x,f32 lb_y,f32 w,f32 h)
	{
		lb.x = lb_x;
		lb.y = lb_y;
		width = w;
		height = h;
	}
	
};

template <> struct TIsPODType<RBAABB> { enum { v = true }; };