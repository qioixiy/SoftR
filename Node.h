#pragma once
#pragma warning(disable : 4005)
#include "RBMath/Inc/Matrix.h"
#include "RBMath/Inc/Vector3.h"

/*
 *����������������ռ��еķ�λ��Ϣ��
 */
class RBNode
{
public:
	RBNode();
	//local
	void set_position(float x,float y,float z);
	void set_rotation(float dex,float dey,float dez);
	void translate(float x,float y,float z);
	void rotate(float dex,float dey,float dez);
	void scale(float sca);
	inline const RBMatrix* get_mat(){return &_model;}
	inline const RBVector4* get_position(){return &_position;}
	inline void get_mat(RBMatrix& m)
	{
		m = _model;
	}
protected:

private:
	RBMatrix _model;
	//����λ�ø���
	RBVector4 _position;
};
