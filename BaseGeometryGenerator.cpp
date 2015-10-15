#include "BaseGeometryGenerator.h"
#include "RBMath/Inc/RBMathBase.h"
#include <fstream>
#include "Mesh.h"
#include "assert.h"

void RBBaseGeometryGenerator::generate_cylinder(RBMeshData& md,float up_r,float down_r,float height,int ring_num,int h_num)
{
	std::vector<RBVector3>& vs = md.vertices;
	std::vector<int>& ins = md.indices;

	//清空原有项目
	std::vector<RBVector3>().swap(vs);
	std::vector<int>().swap(ins);

	float ddegree = 360.f/ring_num;
	float dh = height/h_num;
	int ni = 0;
	int nj =0;
	float hei;
	for (float i = 0;i<=360;i+=ddegree)
	{
		ni++;
		nj = 0;
		for (float j =0;j<=height;j+=dh)
		{
			nj++;
			float r = (down_r-up_r)*(height-j)/height + up_r;
			vs.push_back(RBVector3(r*RBMath::cos(DEG2RAD(i)),r*RBMath::sin(DEG2RAD(i)),j));
			if(j+dh>height)
			{
				hei = j;
			}
		}
	}

	vs.push_back(RBVector3(0,0,hei));
	vs.push_back(RBVector3(0,0,0));

	for (int j=0;j<nj;++j)
	{

		for(int i=0;i<ni;++i)
		{
			if(j==nj-1)
			{
				int ii = i*nj+j;
				ins.push_back(vs.size()-2);
				ins.push_back(ii+nj>=vs.size()-3?nj-1:ii+nj);
				ins.push_back(ii);
				continue;
			}
			if (j==0)
			{
				int ii = i*nj+j;
				ins.push_back(vs.size()-1);
				ins.push_back(ii+nj>=vs.size()-3?0:ii+nj);
				ins.push_back(ii);
			}
			if (i==ni-1)
			{
				int ii = i*nj+j;
				ins.push_back(ii);
				ins.push_back(ii+1);
				ins.push_back(j+1);
				ins.push_back(ii);
				ins.push_back(j+1);
				ins.push_back(j);
				

			}
			else
			{
				int ii = i*nj+j;
				ins.push_back(ii);
				ins.push_back(ii+1);
				ins.push_back(ii+nj+1);
				ins.push_back(ii);
				ins.push_back(ii+nj+1);
				ins.push_back(ii+nj);
				
			}
		}
	}

}

void RBBaseGeometryGenerator::generate_box(RBMeshData& md,float len,float width,float height,int w_num /*= 1*/,int h_num /*= 1*/)
{
	std::vector<RBVector3>& vs = md.vertices;
	std::vector<int>& ins = md.indices;

	std::vector<RBVector3>().swap(vs);
	std::vector<int>().swap(ins);

	float hl = len* 0.5;
	float hw = width*0.5;
	vs.push_back(RBVector3(hl,0.f,hw));
	vs.push_back(RBVector3(-hl,0.f,hw));
	vs.push_back(RBVector3(-hl,0.f,-hw));
	vs.push_back(RBVector3(hl,0.f,-hw));
	vs.push_back(RBVector3(hl,height,hw));
	vs.push_back(RBVector3(-hl, height, hw));
	vs.push_back(RBVector3(-hl, height, -hw));
	vs.push_back(RBVector3(hl, height, -hw));

	{
		ins.push_back(3);
		ins.push_back(2);
		ins.push_back(6);
		ins.push_back(3);
		ins.push_back(6);
		ins.push_back(7);
		ins.push_back(0);
		ins.push_back(4);
		ins.push_back(5);
		ins.push_back(0);
		ins.push_back(5);
		ins.push_back(1);
		ins.push_back(1);
		ins.push_back(5);
		ins.push_back(6);
		ins.push_back(1);
		ins.push_back(6);
		ins.push_back(2);
		ins.push_back(0);
		ins.push_back(3);
		ins.push_back(7);
		ins.push_back(0);
		ins.push_back(7);
		ins.push_back(4);
		ins.push_back(7);
		ins.push_back(6);
		ins.push_back(5);
		ins.push_back(7);
		ins.push_back(5);
		ins.push_back(4);
		ins.push_back(0);
		ins.push_back(1);
		ins.push_back(2);
		ins.push_back(0);
		ins.push_back(2);
		ins.push_back(3);
	}

}

void RBBaseGeometryGenerator::generate_sphere(RBMeshData& md,float radius,int ring_num,int h_num)
{
	std::vector<RBVector3>& vs = md.vertices;
	std::vector<int>& ins = md.indices;

	std::vector<RBVector3>().swap(vs);
	std::vector<int>().swap(ins);

	float ddegreew = 360.f/ring_num;
	float ddegreeh = 360.f/h_num;
	int ni = 0;
	int nj =0;
	for (float i = ddegreeh;i<=180-ddegreeh;i+=ddegreeh)
	{
		ni++;
		float r = radius*sin(DEG2RAD(i));
		nj = 0;
		for (float j =0;j<=360;j+=ddegreew)
		{
			if (RBMath::is_nearly_equal(j,360))
			{
				j = 360;
			}
			nj++;
			vs.push_back(RBVector3(r*RBMath::cos(DEG2RAD(j)),r*RBMath::sin(DEG2RAD(j)),radius*(1-RBMath::cos(DEG2RAD(i)))));
		}
	}

	vs.push_back(RBVector3(0,0,2*radius));
	vs.push_back(RBVector3(0,0,0));

		for (int i=0;i<ni;++i)
		{
			for (int j=0;j<nj;++j)
			{
				int jj = i*nj + j;
				if(i==0)
				{
					ins.push_back(jj);
					ins.push_back(j+1>nj-1?i*nj:jj+1);
					ins.push_back(vs.size()-1);
				}
				if(i==ni-1)
				{
					ins.push_back(jj);
					ins.push_back(j+1>nj-1?i*nj:jj+1);
					ins.push_back(vs.size()-2);
					continue;
				}

				if(j==nj-1)
				{
					ins.push_back(jj);
					ins.push_back(jj+nj);
					ins.push_back(i*nj+nj);
					ins.push_back(jj);
					ins.push_back(i*nj+nj);
					ins.push_back(i*nj);
					break;
				}
				ins.push_back(jj);
				ins.push_back(jj+nj);
				ins.push_back(jj+nj+1);
				ins.push_back(jj);
				ins.push_back(jj+nj+1);
				ins.push_back(jj+1);

			}
		}
}

void RBBaseGeometryGenerator::generate_sphere_ea(RBMeshData& md,float r,int sub_n)
{
	//每次细分就将原来的1个索引替换为新的4个索引
}

#include "windows.h"
void RBBaseGeometryGenerator::generate_from_obj_file(RBMeshData& md,const char* file_name)
{
	std::ifstream ifile(file_name,std::ios::in);

	if (!ifile)
	{
		MessageBox(NULL, L"读取失败，没有这个obj文件，即将退出！", L"警告", MB_OK);
		exit(0);
	}

	char in[128];

	while (ifile.getline(in,128))
	{
		if(in[0]=='v'&&in[1]==' ')
		{
			RBVector3 v;
			sscanf(&in[2],"%f %f %f",&v.x,&v.y,&v.z);
			md.vertices.push_back(v);
		}
		else if(in[0]=='v'&&in[1]=='t'&&in[2]==' ')
		{
			RBVector2 v;
			sscanf(&in[3],"%f %f",&v.x,&v.y);
			v.y = 1 - v.y;
			md.tex_coord.push_back(v);
		}
		else if(in[0]=='v'&&in[1]=='n'&&in[2]==' ')
		{
			RBVector3 v;
			sscanf(&in[3],"%f %f %f",&v.x,&v.y,&v.z);
			md.normal.push_back(v);
		}
		else if(in[0]=='f'&&in[1]==' ')
		{
			bool no_tex = false;
			for(int i=2;i<32;++i)
			{
				if(in[i]=='/'&&in[i+1]=='/')
				{
					no_tex = true;
					break;
				}
			}

			if(no_tex)
			{
				int c1[3],c3[3];
				sscanf(&in[2],"%d//%d %d//%d %d//%d",
					&c1[0],&c3[0],
					&c1[1],&c3[1],
					&c1[2],&c3[2]);
				md.indices.push_back(c1[0]-1);
				md.indices.push_back(c1[1]-1);
				md.indices.push_back(c1[2]-1);
				md.t_indices.push_back(0);
				md.t_indices.push_back(0);
				md.t_indices.push_back(0);
				md.n_indices.push_back(c3[0]-1);
				md.n_indices.push_back(c3[1]-1);
				md.n_indices.push_back(c3[2]-1);
			}
			else
			{
				int c1[3],c2[3],c3[3];
				sscanf(&in[2],"%d/%d/%d %d/%d/%d %d/%d/%d",
					&c1[0],&c2[0],&c3[0],
					&c1[1],&c2[1],&c3[1],
					&c1[2],&c2[2],&c3[2]);
				md.indices.push_back(c1[0]-1);
				md.indices.push_back(c1[1]-1);
				md.indices.push_back(c1[2]-1);
				md.t_indices.push_back(c2[0]-1);
				md.t_indices.push_back(c2[1]-1);
				md.t_indices.push_back(c2[2]-1);
				md.n_indices.push_back(c3[0]-1);
				md.n_indices.push_back(c3[1]-1);
				md.n_indices.push_back(c3[2]-1);
			}
		}
	}

	ifile.close();

}


void RBBaseGeometryGenerator::generate_from_obj_file_fix(RBMeshData& md,const char* file_name)
{
	//读取只要前面那个函数就够了，这种处理需要在生成顶点的时候按照f做处理
	generate_from_obj_file(md,file_name);
}

void RBBaseGeometryGenerator::generate_quad(RBMeshData& md, float w, float h)
{
	float hw = w / 2.f;
	float hh = h / 2.f;
	md.vertices.push_back(RBVector3(-hw,-hh,0.f));
	md.vertices.push_back(RBVector3(-hw,hh,0.f));
	md.vertices.push_back(RBVector3(hw, -hh, 0.f));
	md.vertices.push_back(RBVector3(hw, -hh, 0.f));
	md.vertices.push_back(RBVector3(-hw, hh, 0.f));
	md.vertices.push_back(RBVector3(hw, hh, 0.f));

	md.indices.push_back(0);
	md.indices.push_back(1);
	md.indices.push_back(2);
	md.indices.push_back(3);
	md.indices.push_back(4);
	md.indices.push_back(5);

	md.tex_coord.push_back(RBVector2(0.f,1.f));
	md.tex_coord.push_back(RBVector2(0.f, 0.f));
	md.tex_coord.push_back(RBVector2(1.f, 1.f));
	md.tex_coord.push_back(RBVector2(1.f, 1.f));
	md.tex_coord.push_back(RBVector2(0.f, 0.f));
	md.tex_coord.push_back(RBVector2(1.f, 0.f));


}
