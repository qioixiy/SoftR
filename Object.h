#pragma once
#pragma warning(disable : 4005)
#include "Mesh.h"
#include "Node.h"
#include "D3D11Buffer.h"
#include "Uitilities.h"
#include ".\\SoftR\\BufferIndex.h"
#include ".\\SoftR\\BufferVertex.h"

class RBObject
{
public:
	enum BufferGetTypes
	{
		e_vertex = 0,
		e_index,
		e_tex_coord,
		e_normal,
		e_total,
	};

	
	static RBObject* create_object();

	void load_mesh(const char* filename);
	void load_box();
	void load_cylinder();
	void load_sphere();
	void load_quad(float w,float h);

	inline void set_visible(bool val){ _visible = val; }
	inline bool is_visible(){ return _visible; }

	template<class T>
	void generate_buffer();

	template<class T>
	void generate_softr_buffer();

	RBD3D11Buffer* get_buffer(BufferGetTypes type=e_vertex) const;
	inline RBMesh* get_mesh();

	void draw(UINT stride,UINT offset=0);
	inline int get_stride() const 
	{
		return _buffer_stride;
	}
	inline int get_index_count() const 
	{
		return _index_count;
	}
	inline int get_vertex_count() const 
	{
		return _vertex_count;
	}

	inline SrBufferIndex* get_softr_index_buffer()
	{
		return _softr_index_buffer;
	}

	inline SrBufferVertex* get_softr_vertex_buffer()
	{
		return _softr_vertex_buffer;
	}
	RBNode* _node;
	RBMesh *_mesh;
	~RBObject();
	bool debug_draw;
protected:
	RBObject();
private:
	
	RBD3D11Buffer* _vertex_buffer;
	RBD3D11Buffer* _index_buffer;
	RBD3D11Buffer* _tex_buffer;
	RBD3D11Buffer* _normal_buffer;

	SrBufferIndex* _softr_index_buffer;
	SrBufferVertex* _softr_vertex_buffer;

	bool _visible;

	int _buffer_stride;
	int _index_count;
	int _vertex_count;

public:
	bool _flip;

};

RBMesh* RBObject::get_mesh()
{
	return _mesh;
}


template<class T>
void RBObject::generate_softr_buffer()
{
	if (!_mesh)
	{
		return;
	}
	//typedef TypeTraits<T>::type Vertex;
	typedef typename T Vertex;

	//填充顶点数据
	std::vector<Vertex> _v;
	Vertex::Construct_Vertices(_v, _mesh);

	for (auto i : _v)
	{
		VertexP3N3T2* v = new VertexP3N3T2();
		v->position = RBVector4(i.Pos, 1);
		v->normal = i.normal;
		v->text_coord = i.texcoord;

		_softr_vertex_buffer->put_data(v);


	}

	int vn = _v.size();
	_vertex_count = vn;

	int in = _mesh->get_data()->indices.size();
	_index_count = in;

	for (auto i : _mesh->get_data()->indices)
	{
		_softr_index_buffer->put_data(i);
	}

	_buffer_stride = sizeof(Vertex);

	_mesh->get_data()->release_data();
}

template<class T>
void RBObject::generate_buffer()
{
	if (!_mesh)
	{
		return;
	}
	//typedef TypeTraits<T>::type Vertex;
	typedef typename T::vertex_format Vertex;

	//填充顶点数据
	std::vector<Vertex> _v;
	Vertex::Construct_Vertices(_v,_mesh);

	int vn = _v.size();
	_vertex_buffer->set_property(vn*sizeof(Vertex),
		D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_IMMUTABLE);
	_vertex_count = vn;
	_vertex_buffer->create_with_data(&_v[0]);

	int in = _mesh->get_data()->indices.size();
	_index_count = in;

	_index_buffer->set_property(in*sizeof(int), D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT);
	_index_buffer->create_with_data(&(_mesh->get_data()->indices[0]));

	_buffer_stride = sizeof(Vertex);

	_mesh->get_data()->release_data();
}