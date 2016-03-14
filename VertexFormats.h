#pragma once
#include "RBMath/Inc/Vector2.h"
#include "RBMath/Inc/Vector3.h"
#include "RBMath/Inc/Colorf.h"
#include <vector>
#include "Mesh.h"
#pragma warning(disable : 4005)


struct VertexFormats
{
	struct VertexBase
	{

	};

	struct Vertex_PC
	{
		RBVector3 Pos;
		RBColorf color;
		static void Construct_Vertices(std::vector<Vertex_PC>& _out,RBMesh* m)
		{
			RBMeshData* da = m->get_data();
			int n = da->indices.size();
			int nv = da->vertices.size();
			for (int i=0;i<nv;i++)
			{
				Vertex_PC v;
				v.Pos = da->vertices[i];
				v.color = RBColorf::black;
				_out.push_back(v);
			}
			
		}
	};

	struct Vertex_PCN 
	{
		RBVector3 Pos;
		RBColorf color;
		RBVector3 normal;
		static void Construct_Vertices(std::vector<Vertex_PCN>& _out,RBMesh* m)
		{
			if (false)
			{
				RBMeshData* da = m->get_data();
				int n = da->indices.size();
				int nv = da->vertices.size();
				for (int i = 0; i < nv; i++)
				{
					Vertex_PCN v;
					v.Pos = da->vertices[i];
					v.color = RBColorf::black;
					v.normal = RBVector3::zero_vector;
					_out.push_back(v);
				}

				while (n-- > 0)
				{
					int ii = da->indices[n];
					//int ti = da->t_indices[n];
					int ni = da->n_indices[n];
					if (_out[ii].normal == RBVector3::zero_vector)
					{
						_out[ii].normal = da->normal[ni];
					}
				}
			}
			else
			{
				////读取只要前面那个函数就够了，这种处理需要在生成顶点的时候按照f做处理
				RBMeshData* da = m->get_data();
				int n = da->indices.size();
				int i = 0;
				while (i<n)
				{
					int ii = da->indices[i];
					int ti = da->t_indices[i];
					int ni = da->n_indices[i];
					Vertex_PCN v;
					v.Pos = da->vertices[ii];
					v.color = RBColorf::gray;
					v.normal = da->normal[ni];
					_out.push_back(v);
					da->indices[i] = i;
					i++;
				}

			}
		}
	};

	struct Vertex_PCNT2D
	{
		RBVector3 Pos;
		RBColorf color;
		RBVector3 normal;
		RBVector2 texcoord;
		static void Construct_Vertices(std::vector<Vertex_PCNT2D>& _out, RBMesh* m)
		{
			RBMeshData* da = m->get_data();
			for (int i = 0; i < 6; ++i)
			{
				Vertex_PCNT2D v;
				v.Pos = da->vertices[i];
				v.texcoord = da->tex_coord[i];
				v.color = RBColorf::black;
				v.normal = RBVector3();
				_out.push_back(v);
			}
		}
	};

	//***
	struct Vertex_PCNT 
	{
		RBVector3 Pos;
		RBColorf color;
		RBVector3 normal;
		RBVector2 texcoord;
		static void Construct_Vertices(std::vector<Vertex_PCNT>& _out,RBMesh* m)
		{

			if (false)
			{
				RBMeshData* da = m->get_data();
				int n = da->indices.size();
				int nv = da->vertices.size();
				for (int i = 0; i < nv; i++)
				{
					Vertex_PCNT v;
					v.Pos = da->vertices[i];
					v.color = RBColorf::black;
					v.normal = RBVector3::zero_vector;
					v.texcoord = RBVector2::zero_vector;
					_out.push_back(v);
				}

				while (n-- > 0)
				{
					int ii = da->indices[n - 1];
					int ti = da->t_indices[n - 1];
					int ni = da->n_indices[n - 1];
					if (_out[ii].normal == RBVector3::zero_vector)
					{
						_out[ii].normal = da->normal[ni];
					}
					if (_out[ii].texcoord == RBVector2::zero_vector)
					{
						_out[ii].texcoord = da->tex_coord[ti];
					}
				}
			}
			else
			{
				RBMeshData* da = m->get_data();
				int n = da->indices.size();
				int i = 0;
				while (i<n)
				{
					/*
					int ii = da->indices[i];
					int ti = da->t_indices[i];
					int ni = da->n_indices[i];
					Vertex_PCNT v;
					v.Pos = da->vertices[ii];
					v.color = RBColorf::gray;
					v.normal = da->normal[ni];
					v.texcoord = da->tex_coord[ti];
					_out.push_back(v);
					da->indices[i] = i;
					i++;
					*/

					//此处翻转z轴和绕序，把模型从右手系转换到左手系
					int ii1 = da->indices[i];
					int ti1 = da->t_indices[i];
					int ni1 = da->n_indices[i];
					Vertex_PCNT v1;
					v1.Pos = da->vertices[ii1];
					v1.Pos.z = -v1.Pos.z;
					v1.color = RBColorf::white;
					v1.normal = da->normal[ni1];
					v1.normal.z = -v1.normal.z;
					v1.texcoord = da->tex_coord.size()>0?da->tex_coord[ti1]:RBVector2(0,0);
					_out.push_back(v1);

					int ii2 = da->indices[i+1];
					int ti2 = da->t_indices[i+1];
					int ni2 = da->n_indices[i+1];
					Vertex_PCNT v2;
					
					v2.Pos = da->vertices[ii2];
					v2.Pos.z = -v2.Pos.z;
					v2.color = RBColorf::white;
					v2.normal = da->normal[ni2];
					v2.normal.z = -v2.normal.z;
					v2.texcoord = da->tex_coord.size()>0 ? da->tex_coord[ti2] : RBVector2(0, 0);
					_out.push_back(v2);

					int ii3 = da->indices[i+2];
					int ti3 = da->t_indices[i+2];
					int ni3 = da->n_indices[i+2];
					Vertex_PCNT v3;
					v3.Pos = da->vertices[ii3];
					v3.Pos.z = -v3.Pos.z;
					v3.color = RBColorf::white;
					v3.normal = da->normal[ni3];
					v3.normal.z = -v3.normal.z;
					v3.texcoord = da->tex_coord.size()>0 ? da->tex_coord[ti3] : RBVector2(0, 0);
					_out.push_back(v3);

					da->indices[i] = i+2;
					da->indices[i + 1] = i + 1;
					da->indices[i + 2] = i;
					i += 3;

				}
				fflush(stdout);
			}
			
		}
	};

	struct Vertex_PCNTT
	{
		RBVector3 Pos;
		RBColorf color;
		RBVector3 normal;
		RBVector2 texcoord0;
		RBVector2 texcoord1;
		static void Construct_Vertices(std::vector<Vertex_PCNTT>& _out,RBMesh* m)
		{
			RBMeshData* da = m->get_data();

		}
	};

	struct Vertex_PT
	{
		RBVector3 Pos;
		RBVector2 texcoord;
		static void Construct_Vertices(std::vector<Vertex_PT>& _out,RBMesh* m)
		{
			RBMeshData* da = m->get_data();
			for (int i = 0; i < 6;++i)
			{
				Vertex_PT v;
				v.Pos = da->vertices[i];
				v.texcoord = da->tex_coord[i];
				_out.push_back(v);
			}
		}
	};
};