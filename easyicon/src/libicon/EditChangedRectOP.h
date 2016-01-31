#ifndef _EASYICON_EDIT_CHANGED_RECT_OP_H_
#define _EASYICON_EDIT_CHANGED_RECT_OP_H_



namespace eicon
{

class StagePanel;

class EditChangedRectOP : public ee::ZoomViewOP
{
public:
	EditChangedRectOP(StagePanel* stage);

	virtual bool OnActive();

private:
	StagePanel* m_stage;

}; // EditChangedRectOP

}

#endif // _EASYICON_EDIT_CHANGED_RECT_OP_H_