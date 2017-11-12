#ifndef _EASYANIM_KEYS_CONTENT_EDIT_H_
#define _EASYANIM_KEYS_CONTENT_EDIT_H_

#include <ee/Observer.h>

class wxWindow;

namespace eanim
{

class KeysContentEdit : public ee::Observer
{
public:
	KeysContentEdit(wxWindow* parent);

	void OnMouseLeftDown(int row, int col);
	void OnMouseDragging(int row, int col);
	void OnMouseLeftClick(int row, int col);

	void CopySelection();
	void PasteSelection();
	void DeleteSelection();

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void UpdateRegion(int layer, int frame);

	bool IsSelectionValid() const;

private:
	wxWindow* m_parent;

	int m_row, m_col;
	int m_col_min, m_col_max;

}; // KeysContentEdit

}

#endif // _EASYANIM_KEYS_CONTENT_EDIT_H_