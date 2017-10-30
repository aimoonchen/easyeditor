#include "Shape.h"
#include "StringHelper.h"

namespace ee
{

Shape::Shape() 
{ 
	static int index = 0;
	m_name = "shape" + StringHelper::ToString(index++);
}

Shape::Shape(const Shape& shape)
	: m_name(shape.m_name)
{
}

Shape::~Shape() 
{ 
	ClearUserData(true); 
}

void Shape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	m_name = value["name"].asString().c_str();
}

void Shape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	value["name"] = m_name.c_str();
	value["type"] = GetShapeDesc();
}

}