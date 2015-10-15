#include "Object.h"
#include "GlobalEnv.h"

RBObject::RBObject()
{
	_mesh = new RBMesh();
	_vertex_buffer = new RBD3D11Buffer();
	_index_buffer = new RBD3D11Buffer();
	_tex_buffer = new RBD3D11Buffer();
	_normal_buffer = new RBD3D11Buffer();

	_softr_index_buffer = new SrBufferIndex();
	_softr_vertex_buffer = new SrBufferVertex();

	_buffer_stride = 0;
	_index_count = 0;
	_vertex_count = 0;

	debug_draw = false;

	_visible = true;
	_node = new RBNode();

	_flip = false;
}

RBObject* RBObject::create_object()
{
	RBObject* ret = new RBObject();
	return ret;
}

void RBObject::load_mesh(const char* filename)
{
	_mesh->load_obj(filename);
}

void RBObject::load_box()
{
	_mesh->load_base_geometry(RBMesh::e_box);
}

void RBObject::load_cylinder()
{
	_mesh->load_base_geometry(RBMesh::e_cylinder);
}

void RBObject::load_sphere()
{
	_mesh->load_base_geometry(RBMesh::e_sphere);
}



void RBObject::load_quad(float w,float h)
{
	_mesh->load_2d_quad(w, h);
}


RBD3D11Buffer* RBObject::get_buffer(BufferGetTypes type/*=e_vertex*/) const
{
	RBD3D11Buffer* ret = NULL;
	switch (type)
	{
	case RBObject::e_vertex:
		ret = _vertex_buffer;
		break;
	case RBObject::e_index:
		ret = _index_buffer;
		break;
	case RBObject::e_tex_coord:
		ret = _tex_buffer;
		break;
	case RBObject::e_normal:
		ret = _normal_buffer;
		break;
	case RBObject::e_total:

		break;
	default:

		break;
	}
	return ret;
}

void RBObject::draw(UINT stride,UINT offset/*= 0*/)
{
	ID3D11Buffer* vb = _vertex_buffer->get_buffer();
	ID3D11Buffer* ib = _index_buffer->get_buffer();
	g_env->d3d_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_env->d3d_context->IASetVertexBuffers(0,1,&vb,&stride,&offset);
	g_env->d3d_context->IASetIndexBuffer(ib,DXGI_FORMAT_R32_UINT,0);

}

RBObject::~RBObject()
{
	if (_mesh)
	{
		delete _mesh;
		_mesh = NULL;
	}
	if(_vertex_buffer)
	{
		delete _vertex_buffer;
		_vertex_buffer = NULL;
	}
	if (_index_buffer)
	{
		delete _index_buffer;
		_index_buffer = NULL;
	}
	if(_tex_buffer)
	{
		delete _tex_buffer;
		_tex_buffer = NULL;
	}
	if(_normal_buffer)
	{
		delete _normal_buffer;
		_normal_buffer = NULL;
	}
}

