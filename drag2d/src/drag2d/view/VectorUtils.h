#pragma once

#include <vector>

#include "interfaces.h"

#include "view/EditPanelDefs.h"

namespace d2d
{
	class VectorUtils
	{
	public:
		template<class T>
		static void traverse(const std::vector<T*>& objs, IVisitor& visitor, 
			bool order = true);
		template<class T>
		static void traverse(const std::vector<T*>& objs, IVisitor& visitor, 
			TraverseType type = e_allExisting, bool order = true);

		template<class T>
		static void remove(std::vector<T*>& objs, T* obj);

		template<class T>
		static void insert(std::vector<T*>& objs, T* obj);

		template<class T>
		static void clear(std::vector<T*>& objs);

		template<class T>
		static void resetOrder(std::vector<T*>& objs, const T* obj, 
			bool up);
	};

	template<class T>
	inline void VectorUtils::traverse(const std::vector<T*>& objs,
		IVisitor& visitor, bool order/* = true*/)
	{
		if (order)
		{
			std::vector<T*>::const_iterator itr = objs.begin();
			for ( ; itr != objs.end(); ++itr)
			{
				bool hasNext;
				visitor.visit(*itr, hasNext);
				if (!hasNext) break;
			}
		}
		else
		{
			std::vector<T*>::const_reverse_iterator itr = objs.rbegin();
			for ( ; itr != objs.rend(); ++itr)
			{
				bool hasNext;
				visitor.visit(*itr, hasNext);
				if (!hasNext) break;
			}
		}
	}

	template<class T>
	inline void VectorUtils::traverse(const std::vector<T*>& objs,
		IVisitor& visitor, TraverseType type, bool order)
	{
		if (order)
		{
			std::vector<T*>::const_iterator itr = objs.begin();
			for ( ; itr != objs.end(); ++itr)
			{
				if (type == e_editable && (*itr)->editable ||
					type == e_visible && (*itr)->visiable ||
					type == e_allExisting || type == e_selectable)
				{
					bool hasNext;
					visitor.visit(*itr, hasNext);
					if (!hasNext) break;
				}
			}
		}
		else
		{
			std::vector<T*>::const_reverse_iterator itr = objs.rbegin();
			for ( ; itr != objs.rend(); ++itr)
			{
				bool hasNext;
				visitor.visit(*itr, hasNext);
				if (!hasNext) break;
			}
		}
	}

	template<class T>
	inline void VectorUtils::remove(std::vector<T*>& objs, T* obj)
	{
		for (size_t i = 0, n = objs.size(); i < n; ++i)
		{
			if (objs[i] == obj)
			{
				obj->Release();
				objs.erase(objs.begin() + i);
				break;
			}
		}
	}

	template<class T>
	inline void VectorUtils::insert(std::vector<T*>& objs, T* obj)
	{
		obj->Retain();
		objs.push_back(obj);
	}

	template<class T>
	inline void VectorUtils::clear(std::vector<T*>& objs)
	{
		for (size_t i = 0, n = objs.size(); i < n; ++i)
			objs[i]->Release();
		objs.clear();
	}

	template<class T>
	inline void VectorUtils::resetOrder(std::vector<T*>& objs, 
		const T* obj, bool up)
	{
		for (size_t i = 0, n = objs.size(); i < n; ++i)
		{
			if (objs[i] == obj)
			{
				if (up && i != n - 1)
				{
					T* tmp = objs[i];
					objs[i] = objs[i+1];
					objs[i+1] = tmp;
				}
				else if (!up && i != 0)
				{
					T* tmp = objs[i];
					objs[i] = objs[i-1];
					objs[i-1] = tmp;
				}

				break;
			}
		}
	}
}