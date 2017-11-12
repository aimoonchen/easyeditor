#ifndef _EASYTRAIL_EDIT_OP_H_
#define _EASYTRAIL_EDIT_OP_H_

#include <ee/ZoomViewOP.h>

namespace etrail
{

class StagePanel;

class EditOP : public ee::ZoomViewOP
{
public:
	EditOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	StagePanel* m_stage;

}; // EditOP

}

#endif // _EASYTRAIL_EDIT_OP_H_