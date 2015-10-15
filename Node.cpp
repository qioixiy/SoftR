#include "Node.h"

RBNode::RBNode()
{
	_position.set(0.f,0.f,0.f,1.f);
	_model.set_identity();
}

void RBNode::set_position(float x,float y,float z)
{
	_position.set(x,y,z,1);
	_model.set_translation(x,y,z);
}

void RBNode::set_rotation(float dex,float dey,float dez)
{
	_model.set_rotation(dex,dey,dez);
}

void RBNode::translate(float x,float y,float z)
{
	RBMatrix m;
	m.traslate(x,y,z);
	_model*=m;
	_position.apply_matrix(m);
}

void RBNode::rotate(float dex,float dey,float dez)
{
	RBVector3 v;
	_model.get_translate(v);
	float x = -v[0];
	float y = -v[1];
	float z = -v[2];
	RBMatrix m;
	m.traslate(x,y,z);
	_model*=m;
	m.set_identity();
	m.rotate(dex,dey,dez);
	_model*=m;
	m.traslate(-x,-y,-z);
	_model*=m;

}

void RBNode::scale(float sca)
{
	_model.set_scale(sca, sca, sca);
}
