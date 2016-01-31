#ifndef _EASYPARTICLE2D_EDIT_OP_H_
#define _EASYPARTICLE2D_EDIT_OP_H_



namespace eparticle2d
{

class StagePanel;

class EditOP : public ee::ZoomViewOP
{
public:
	EditOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

private:
	StagePanel* m_stage;

}; // EditOP

}

#endif // _EASYPARTICLE2D_EDIT_OP_H_