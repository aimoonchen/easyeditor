#include "Shape.h"
#include "Triangle.h"

#include <drag2d.h>

namespace emesh
{

Shape::Shape()
	: m_texid(0)
	, m_width(0)
	, m_height(0)
{
}

Shape::Shape(const Shape& shape)
	: m_texid(shape.m_texid)
	, m_width(shape.m_width)
	, m_height(shape.m_height)
{
}

Shape::Shape(const d2d::Image& image)
{
	m_texid = image.textureID();

	m_width = image.getRegion().xLength();
	m_height = image.getRegion().yLength();
}

Shape::~Shape()
{
	ClearTriangles();
}

Node* Shape::QueryNode(const d2d::Vector& p)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			if (d2d::Math::getDistance(tri->nodes[j]->xy, p) < Node::RADIUS)
				return tri->nodes[j];
		}
	}
	return NULL;
}

void Shape::QueryNode(const d2d::Rect& r, std::vector<Node*>& nodes)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			if (d2d::Math::isPointInRect(tri->nodes[j]->xy, r))
				nodes.push_back(tri->nodes[j]);
		}
	}
}

void Shape::DrawInfoUV() const
{
	std::set<d2d::Vector, d2d::VectorCmp> unique;
	std::vector<d2d::Vector> tmp(3);
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i].x = (tri->nodes[i]->uv.x - 0.5f) * m_width;
			tmp[i].y = (tri->nodes[i]->uv.y - 0.5f) * m_height;
			unique.insert(tmp[i]);
		}
		d2d::PrimitiveDraw::drawPolyline(tmp, d2d::Colorf(0.8f, 0.2f, 0.4f, 0.5f), true);
	}
	std::vector<d2d::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	d2d::PrimitiveDraw::drawCircles(nodes, Node::RADIUS, true, 2, d2d::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
}

void Shape::DrawInfoXY() const
{
	std::set<d2d::Vector, d2d::VectorCmp> unique;
	std::vector<d2d::Vector> tmp(3);
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i] = tri->nodes[i]->xy;
			unique.insert(tmp[i]);
		}
		d2d::PrimitiveDraw::drawPolyline(tmp, d2d::Colorf(0.8f, 0.2f, 0.4f, 0.5f), true);
	}
	std::vector<d2d::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	d2d::PrimitiveDraw::drawCircles(nodes, Node::RADIUS, true, 2, d2d::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
}

void Shape::DrawTexture() const
{
	std::vector<d2d::Vector> vertices, texcoords;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			texcoords.push_back(tri->nodes[i]->uv);
			vertices.push_back(tri->nodes[i]->xy);
		}
	}

	d2d::PrimitiveDraw::drawTriangles(m_texid, vertices, texcoords);
}

void Shape::ClearTriangles()
{
	for_each(m_tris.begin(), m_tris.end(), DeletePointerFunctor<Triangle>());
	m_tris.clear();
}

void Shape::LoadTriangles(const std::vector<d2d::Vector>& tris)
{
	std::map<d2d::Vector, Node*, d2d::VectorCmp> map2Node;
	Node null;
	for (int i = 0, n = tris.size(); i < n; ++i)
		map2Node.insert(std::make_pair(tris[i], &null));

	for (int i = 0, n = tris.size() / 3, ptr = 0; i < n; ++i)
	{
		Triangle* tri = new Triangle;
		for (int j = 0; j < 3; ++j)
		{
			std::map<d2d::Vector, Node*, d2d::VectorCmp>::iterator itr 
				= map2Node.find(tris[ptr++]);
			assert(itr != map2Node.end());
			if (itr->second == &null)
				itr->second = new Node(itr->first, m_width, m_height);
			tri->nodes[j] = itr->second;
		}
		m_tris.push_back(tri);
	}
}

}