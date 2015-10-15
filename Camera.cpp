#include "Camera.h"
#include "RBMath/Inc/Matrix.h"
#include "Uitilities.h"
#include "stdio.h"

void RBCamera::translate(float x,float y,float z)
{
	RBVector3 v = _forward;
	v = _position + v;
	_position.x += x;
	_position.y += y;
	_position.z += z;

	v.x += x;
	v.y += y;
	v.z += z;
	_forward = RBVector4( v - _position,1);

}


void RBCamera::rotate_by_axis(float degree, RBVector3 axis)
{
	RBVector3 v = _forward;
	axis.normalize();
	v = v.rotate_angle_around_axis(degree, axis);
	_forward = RBVector4(v,1);
}

void RBCamera::rotate(float x,float y,float  z)
{

	RBMatrix m;
	m.rotate(x,y,z);
	_forward.apply_matrix(m);


	/*
	RBVector4 v4(_up,1.f);
	v4.apply_matrix(m);
	_up.x = v4.x;
	_up.y = v4.y;
	_up.z = v4.z;
	*/

}

void RBCamera::set_position(float x,float y,float z)
{
	RBVector4 _v(x-_position.x,y-_position.y,z-_position.z,1);
	_position.x = x;
	_position.y = y;
	_position.z = z;
	_forward = _forward - _v;
}

void RBCamera::set_rotation(float x,float y,float z)
{
	_forward = RBVector4(0,0,1,1);
	RBMatrix m;
	m.rotate(x,y,z);
	_forward=_forward*m;
}

void RBCamera::set_target(RBVector3 tar)
{
	 _forward = RBVector4(tar - _position,1);
}

RBVector3 RBCamera::get_position() const
{
	return _position;
}

RBVector4 RBCamera::get_rotaion()
{
	return _forward;
}

void RBCamera::update(float dt)
{

}

void RBCamera::get_view_matrix(RBMatrix& out_mat) const
{
	out_mat = RBMatrix::get_lookat(_position,_forward,_up);
}

void RBCamera::get_perspective_matrix(RBMatrix& out_mat) const
{
	out_mat = RBMatrix::get_perspective(_fovy,_ratio,_near_panel,_far_panel);
}

void RBCamera::get_ortho(RBMatrix& out_mat) const
{
	float h = _near_panel*RBMath::tan(DEG2RAD(_fovy));
	out_mat = RBMatrix::get_ortho(0,h*_ratio , 0, h, _near_panel, _far_panel);
}

void RBCamera::pan(const RBVector3& target, float degree)
{
	RBVector4 _t(_position,1.f);
	RBVector4 _t2(target,1.f);
	RBMatrix m1,m2,m3;
	m1.traslate(-target.x,-target.y,-target.z);
	_t.apply_matrix(m1);
	_t2.apply_matrix(m1);
	m2.rotate(0,degree,0);
	_t.apply_matrix(m2);
	_t2.apply_matrix(m2);
	m3.traslate(target.x,target.y,target.z);
	_t.apply_matrix(m3);
	_t2.apply_matrix(m3);
	_forward = _t2-_t;
	_forward.w = 1.f;
	_position.x = _t.x;
	_position.y = _t.y;
	_position.z = _t.z;
}

