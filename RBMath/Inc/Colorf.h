#pragma once

#include "./Platform/RBBasedata.h"


class RBColor32;
class RBVector4;

class RBColorf
{
public:
	//0.0f-1.0f
	f32 r, g, b, a;
	RBColorf();
	RBColorf(f32 r, f32 g, f32 b, f32 a);
	RBColorf(f32 r, f32 g, f32 b);
	RBColorf(const RBVector4& v);
	bool equal(const RBColorf& other);
	f32 get_grayscale();
	//[Kinds of operators(+,-,*,/)] 

	RBColorf(const RBColor32& color32);

	
	static const RBColorf
		red,
		green,
		blue,
		white,
		black,
		yellow,
		cyan,
		magenta,
		gray,
		grey,
		blank;
		

	void out_cyan();



};


/*
RBColorf::green(0.f, 1.f, 0.f, 1.f),
RBColorf::blue(0.f, 0.f, 1.f, 1.f),
RBColorf::white(1.f, 1.f, 1.f, 1.f),
RBColorf::black(0.f, 0.f, 0.f, 1.f),
RBColorf::yellow(1.f, 0.9215686f, 0.01568628f, 1.f),
//Çà
RBColorf::cyan(0.f, 1.f, 1.f, 1.f),
//Ñóºì
RBColorf::magenta(1.f, 0.f, 1.f, 1.f),
//»Ò
RBColorf::grey(0.5f, 0.5f, 0.5f, 1.f),
//»Ò
RBColorf::gray(0.5f, 0.5f, 0.5f, 1.f),
//clear
RBColorf::blank(0.f, 0.f, 0.f, 0.f);
*/
template <> struct TIsPODType < RBColorf > { enum { v = true }; };