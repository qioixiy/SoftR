#pragma once
#pragma warning(disable : 4005)
#include <vector>
#include "RBMath\Inc\Vector3.h"
#include "RBMath\Inc\Vector2.h"

class RBMeshData
{
public:
	~RBMeshData();
	void release_data();
protected:

public:
	std::vector<RBVector3> vertices;
	std::vector<int> indices;
	std::vector<RBVector3>  normal;
	std::vector<int> n_indices;
	std::vector<RBVector2> tex_coord;
	std::vector<int> t_indices;
};


class RBMesh
{
public:
	enum GeoLoadTypes
	{
		e_cylinder = 0,
		e_box,
		e_sphere,
		e_sphere_ar,
		e_quad,
		e_total,
	};

public:
	RBMesh();
	~RBMesh();
	bool load_obj(const char* filename);
	bool load_base_geometry(GeoLoadTypes type);
	bool load_2d_quad(float w,float h);

	inline RBMeshData* get_data()
	{
		return _mesh_data;
	}
	//...some function to change mesh
protected:

private:
	RBMeshData* _mesh_data;

};

