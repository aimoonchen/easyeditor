#ifndef _EASYEDITOR_SHAPES_PANEL_IMPL_H_
#define _EASYEDITOR_SHAPES_PANEL_IMPL_H_

#include "MultiShapesImpl.h"
#include "DataContainer.h"

namespace ee
{

class Shape;

class ShapesPanelImpl : public MultiShapesImpl
{
public:
	ShapesPanelImpl();
	ShapesPanelImpl(DataContainer<Shape>* container);
	virtual ~ShapesPanelImpl();

	//
	// MultiShapesImpl interface
	//
	virtual void TraverseShapes(Visitor<ee::Shape>& visitor, 
		DataTraverseType type = DT_ALL) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitSubjects();

private:
	DataContainer<Shape>* m_container;

}; // ShapesPanelImpl

}

#endif // _EASYEDITOR_SHAPES_PANEL_IMPL_H_