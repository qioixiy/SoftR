#include "Mesh.h"
#include "BaseGeometryGenerator.h"

RBMesh::RBMesh()
{
	_mesh_data = new RBMeshData();
}

RBMesh::~RBMesh()
{
	delete _mesh_data;
}

bool RBMesh::load_obj(const char* filename)
{
	RBBaseGeometryGenerator::generate_from_obj_file(*_mesh_data,filename);
	return true;
}

bool RBMesh::load_base_geometry(GeoLoadTypes type)
{
	switch (type)
	{
	case RBMesh::e_cylinder:
		RBBaseGeometryGenerator::generate_cylinder(*_mesh_data,1,2,10,24,4);
		break;
	case RBMesh::e_box:
		RBBaseGeometryGenerator::generate_box(*_mesh_data,1,1,1);
		break;
	case RBMesh::e_sphere:
		RBBaseGeometryGenerator::generate_sphere(*_mesh_data,5,12,12);
		break;
	case RBMesh::e_sphere_ar:
		RBBaseGeometryGenerator::generate_sphere_ea(*_mesh_data,1,6);
		break;
	case RBMesh::e_quad:
		
	case RBMesh::e_total:
		return false;
	default:
		break;
	}
	return true;
}


bool RBMesh::load_2d_quad(float w, float h)
{
	RBBaseGeometryGenerator::generate_quad(*_mesh_data, w, h);
	return true;
}

/*
 *MeshData Implementation
 */
RBMeshData::~RBMeshData()
{

}

void RBMeshData::release_data()
{
	std::vector<RBVector3>().swap(vertices);
	std::vector<int>().swap( indices);
	std::vector<RBVector3> ().swap( normal);
	std::vector<int> ().swap(n_indices);
	std::vector<RBVector2> ().swap(tex_coord);
	std::vector<int>().swap( t_indices);


}
