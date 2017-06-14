#ifndef _EASYEDITOR_OBJECT_VECTOR_H_
#define _EASYEDITOR_OBJECT_VECTOR_H_

#include "DataTraverseType.h"
#include "Visitor.h"

#include <vector>

namespace ee
{

template<class T>
class ObjectVector
{
public:
	ObjectVector();
	virtual ~ObjectVector();
	
	void Traverse(Visitor<T>& visitor, bool order = true) const;
	void Traverse(Visitor<T>& visitor, DataTraverseType type = DT_ALL, bool order = true) const;

	bool Remove(T* obj);

	bool Insert(T* obj);
	bool Insert(T* obj, int idx);

	bool Clear();

	bool ResetOrder(const T* obj, bool up);
	bool ResetOrderMost(const T* obj, bool up);
	bool Sort(std::vector<T*>& list);

	bool IsExist(T* obj) const;

	int Size() const;

	const std::vector<T*>& GetObjs() const { return m_objs; }
	void SetObjs(const std::vector<T*>& objs) { m_objs = objs; }

public:
	static void Traverse(const std::vector<T*>& objs, Visitor<T>& visitor, bool order = true);
	static void Traverse(const std::vector<T*>& objs, Visitor<T>& visitor, DataTraverseType type = DT_ALL, bool order = true);

	static bool Remove(std::vector<T*>& objs, T* obj);

	static bool Insert(std::vector<T*>& objs, T* obj);
	static bool Insert(std::vector<T*>& objs, T* obj, int idx);

	static bool Clear(std::vector<T*>& objs);

	static bool ResetOrder(std::vector<T*>& objs, const T* obj, bool up);
	static bool ResetOrderMost(std::vector<T*>& objs, const T* obj, bool up);
	static bool Sort(std::vector<T*>& objs, std::vector<T*>& list);

private:
	std::vector<T*> m_objs;

}; // ObjectVector

}

#include "ObjectVector.inl"

#endif // _EASYEDITOR_OBJECT_VECTOR_H_