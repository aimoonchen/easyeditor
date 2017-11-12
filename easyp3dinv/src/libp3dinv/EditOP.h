#ifndef _EASYP3DINV_EDIT_OP_H_
#define _EASYP3DINV_EDIT_OP_H_

#include <ee/ZoomViewOP.h>

namespace ep3dinv
{

class StagePanel;

class EditOP : public ee::ZoomViewOP
{
public:
	EditOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y) override;

private:
	StagePanel* m_stage;

}; // EditOP

}

#endif // _EASYP3DINV_EDIT_OP_H_