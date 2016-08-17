#include "Mesh.h"
#include "Triangle.h"
#include "color_config.h"

#include <ee/Symbol.h>
#include <ee/std_functor.h>
#include <ee/JsonSerializer.h>

#include <shaderlab.h>
#include <sprite2/S2_RVG.h>
#include <SM_Test.h>
#include <SM_Calc.h>

#include <set>
#include <algorithm>

#include <assert.h>

namespace emesh
{

Mesh::Mesh()
	: m_base(NULL)
	, m_width(0)
	, m_height(0)
	, m_node_radius(5)
{
}

Mesh::Mesh(const Mesh& mesh)
	: m_width(mesh.m_width)
	, m_height(mesh.m_height)
	, m_node_radius(mesh.m_node_radius)
{
	m_base = mesh.m_base;
	m_base->AddReference();
}

Mesh::Mesh(const ee::Symbol* base)
{
	m_base = base;
	m_base->AddReference();

	sm::vec2 sz = m_base->GetBounding().Size();
	m_width = sz.x;
	m_height = sz.y;

	m_node_radius = std::min(sz.x * 0.1f, 5.0f);
}

Mesh::~Mesh()
{
	if (m_base) {
		m_base->RemoveReference();
	}

	ClearTriangles();
}

Node* Mesh::PointQueryNode(const sm::vec2& p)
{
	Node* node = NULL;
	float nearest = FLT_MAX;
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j) {
			float dis = sm::dis_pos_to_pos(tri->nodes[j]->xy, p);
			if (dis < m_node_radius && dis < nearest) {
				nearest = dis;
				node = tri->nodes[j];
			}
		}
	}

	return node;
}

void Mesh::RectQueryNodes(const sm::rect& r, std::vector<Node*>& nodes)
{
	std::set<Node*> unique;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j) {
			Node* node = tri->nodes[j];
			if (sm::is_point_in_rect(node->xy, r) &&
				unique.find(node) == unique.end()) {
				nodes.push_back(node);
				unique.insert(node);
			}
		}
	}
}

sm::rect Mesh::GetRegion() const
{
	sm::rect r;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i) {
			r.Combine(tri->nodes[i]->xy);
		}
	}	
	return r;
}

void Mesh::ClearTriangles()
{
	for_each(m_tris.begin(), m_tris.end(), ee::DeletePointerFunctor<Triangle>());
	m_tris.clear();
}

void Mesh::StoreTriangles(Json::Value& value) const
{
	std::vector<sm::vec2> transform;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			transform.push_back(tri->nodes[i]->xy);
	}
	ee::JsonSerializer::Store(transform, value);
}

void Mesh::LoadTriangles(const Json::Value& value)
{
	std::vector<sm::vec2> transform;
	ee::JsonSerializer::Load(value, transform);
	int itr = 0;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			tri->nodes[i]->xy = transform[itr++];
	}
}

}