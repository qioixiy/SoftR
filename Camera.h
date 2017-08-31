#pragma once
#pragma warning(disable : 4005)


#define RW 800
#define  RH 600
#include "RBMath/Inc/Vector3.h"
class RBCamera
{
public:
	RBCamera(float x=0,float y=0,float z=0) 
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
		_up = RBVector3(0,1,0);
		_forward = RBVector4(0,0,1,1);
		_near_panel = 1;
		_far_panel = 100;
		_fovy = 90;
		_ratio = 9.f/16.f;
		
	}
	void translate(float x,float y,float z);
	void rotate(float x_deg,float y_deg,float  z_deg);

	void set_position(float x,float y,float z);
	void set_rotation(float x_deg,float y_deg,float  z_deg);

	void set_target(RBVector3 tar);

	RBVector3 get_position() const;
	RBVector4 get_rotaion();

	void update(float dt);
	void get_view_matrix(RBMatrix& out_mat) const;
	void get_perspective_matrix(RBMatrix& out_mat) const;
	void get_ortho(RBMatrix& out_mat) const;

	inline void set_near_panel(float n)
	{
		_near_panel = n;
	}

	inline void set_far_panel(float f)
	{
		_far_panel = f;
	}

	inline void set_fov_y(float deg)
	{
		_fovy = deg;
	}

	//x/y
	inline void set_ratio(float r)
	{
		_ratio = r;
	}

	inline float get_near_panel()
	{
		return _near_panel;
	}

	inline float get_far_panel()
	{
		return _far_panel;
	}

	inline float get_fovy()
	{
		return _fovy;
	}

	inline float get_ratio()
	{
		return _ratio;
	}

	inline RBVector4 get_forward()
	{
		return _forward;
	}

	inline RBVector3 get_up()
	{
		return _up;
	}

	inline RBVector3 get_right()
	{
		RBVector3 v = _forward;
		return RBVector3::cross_product(_up, v);
	}

	void pan(const RBVector3& target,float degree);
	void rotate_by_axis(float degree,RBVector3 axis); 
protected:

public:
	RBVector3 _position;
	RBVector3 _up;
	RBVector4 _forward;
	float _near_panel;
	float _far_panel;
	float _fovy;
	//w/h
	float _ratio;

};