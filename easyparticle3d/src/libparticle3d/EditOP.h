#ifndef _EASYPARTICLE3D_EDIT_OP_H_
#define _EASYPARTICLE3D_EDIT_OP_H_

#include <ee/ZoomViewOP.h>

namespace eparticle3d
{

class StagePanel;

class EditOP : public ee::ZoomViewOP
{
public:
	EditOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	StagePanel* m_stage;

}; // EditOP

}

#endif // _EASYPARTICLE3D_EDIT_OP_H_