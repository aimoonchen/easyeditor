#ifndef _EASYICON_EDIT_CHANGED_RECT_OP_H_
#define _EASYICON_EDIT_CHANGED_RECT_OP_H_

#include <ee/ZoomViewOP.h>

namespace eicon
{

class StagePanel;

class EditChangedRectOP : public ee::ZoomViewOP
{
public:
	EditChangedRectOP(StagePanel* stage);

	virtual bool OnActive() override;

private:
	StagePanel* m_stage;

}; // EditChangedRectOP

}

#endif // _EASYICON_EDIT_CHANGED_RECT_OP_H_