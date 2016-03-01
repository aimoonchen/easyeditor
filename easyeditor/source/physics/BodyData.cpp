#include "BodyData.h"
#include "std_functor.h"
#include "FileHelper.h"
#include "Exception.h"

#include <json/json.h>

#include <string>

namespace ee
{

BodyData::BodyData()
{
}

BodyData::~BodyData()
{
	BodyDataMgr::Instance()->RemoveItem(m_filename);
	clear();
}

void BodyData::clear()
{
	for_each(m_fixtures.begin(), m_fixtures.end(), DeletePointerFunctor<FixtureData>());
	m_fixtures.clear();
}

bool BodyData::LoadFromFile(const std::string& filename)
{
	if (!FileHelper::IsFileExist(filename)) {
		throw Exception("File: %s don't exist!", filename.c_str());
	}

	clear();

	m_filename = filename;

	FileType::Type type = FileType::GetType(filename);
	switch (type)
	{
	case FileType::e_mesh:
		loadFromMeshFile(filename);
		break;
	case FileType::e_shape:
		loadFromShapeFile(filename);
	default:
		return false;
	}

	return true;
}

// todo ȥ��MeshSymbol���Ժ���easymesh�ģ����ⲻ��Ҫ�Ķ�easymesh������
void BodyData::loadFromMeshFile(const std::string& filename)
{
	//m_type = e_mesh;

	//MeshSymbol symbol;
	//symbol.LoadFromFile(filename);

	//const std::vector<Vector>& triangles = symbol.getVertices();

	//size_t size = triangles.size() / 3;
	//m_fixtures.reserve(size);
	//size_t index = 0;
	//for (size_t i = 0; i < size; ++i)
	//{
	//	FixtureData* fd = new FixtureData;
	//	fd->vertices.push_back(triangles[index++]);
	//	fd->vertices.push_back(triangles[index++]);
	//	fd->vertices.push_back(triangles[index++]);
	//	m_fixtures.push_back(fd);
	//}
}

// todo �Ƶ�libshape�У����ⲻ��Ҫ�Ķ�libshape������
void BodyData::loadFromShapeFile(const std::string& filename)
{
// 	m_type = e_shapes;
// 
// 	std::vector<Shape*> shapes;
// 	eshape::FileAdapter adapter(shapes);
// 
// 	adapter.load(filename.c_str());
// 	for (size_t i = 0, n = shapes.size();  i< n; ++i)
// 	{
// 		if (ChainShape* chain = dynamic_cast<ChainShape*>(shapes[i]))
// 		{
// 			FixtureData* fd = new FixtureData;
// 			fd->vertices = chain->getVertices();
// 			m_fixtures.push_back(fd);
// 		}
// 		else if (RectShape* rect = dynamic_cast<RectShape*>(shapes[i]))
// 		{
// 			FixtureData* fd = new FixtureData;
// 			fd->vertices.resize(4);
// 			fd->vertices[0] = Vector(rect->m_rect.xmin, rect->m_rect.ymin);
// 			fd->vertices[1] = Vector(rect->m_rect.xmax, rect->m_rect.ymin);
// 			fd->vertices[2] = Vector(rect->m_rect.xmax, rect->m_rect.ymax);
// 			fd->vertices[3] = Vector(rect->m_rect.xmin, rect->m_rect.ymax);
// 			m_fixtures.push_back(fd);
// 		}
// 		else if (CircleShape* circle = dynamic_cast<CircleShape*>(shapes[i]))
// 		{
// 			FixtureData* fd = new FixtureData;
// 			fd->vertices.push_back(Vector(circle->radius*2, circle->radius*2));
// 			m_fixtures.push_back(fd);
// 		}
// 
// 		shapes[i]->Release();
// 	}
}

}