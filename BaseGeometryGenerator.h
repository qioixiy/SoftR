
#pragma once
#pragma warning(disable : 4005)
#include <vector>
#include "Mesh.h"

class RBBaseGeometryGenerator
{
public:
	static void generate_cylinder(RBMeshData& md,float up_r,float down_r,float height,int ring_num,int h_num);
	static void generate_box(RBMeshData& md,float len,float width,float height,int w_num = 1,int h_num = 1);
	static void generate_sphere(RBMeshData& md,float r,int ring_num,int h_num);
	//生成等面积三角形的球
	static void generate_sphere_ea(RBMeshData& md,float r,int sub_n);
	//以最小顶点数量读取
	static void generate_from_obj_file(RBMeshData& md,const char* file_name);
	//按照face读取，一个立方体将会读取36个顶点
	static void generate_from_obj_file_fix(RBMeshData& md,const char* file_name);

	static void generate_from_txt_file(RBMeshData& md, const char* file_name);

	static void generate_quad(RBMeshData& md,float w,float h);

protected:

private:

};
