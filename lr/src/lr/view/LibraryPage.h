#ifndef _LR_LIBRARY_PAGE_H_
#define _LR_LIBRARY_PAGE_H_

#include "dataset/layer_type.h"

#include <ee/LibraryPage.h>

#include <vector>

namespace ee { class EditOP; }

namespace lr
{

class LibraryPanel;
class Layer;

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(LibraryPanel* library, const std::string& name, 
		LayerType type, int id, bool has_height);
	virtual ~LibraryPage();

	//
	//	interface ee::LibraryPage
	//
	virtual bool IsHandleSymbol(ee::Symbol* symbol) const;

	void UpdateStatusFromLayer();

	Layer* GetLayer() { return m_layer; }
	void SetLayer(Layer* layer);

	void AddEditOP(ee::EditOP* editop);
	ee::EditOP* GetNextEditOP();

	LayerType GetLayerType() const { return m_layer_type; }
	bool GetLayerPerspective() const { return m_has_height; }

protected:
	virtual void InitLayoutExtend(wxSizer* sizer);

	virtual void OnAddPress(wxCommandEvent& event);

private:
	void OnChangeVisible(wxCommandEvent& event);
	void OnChangeEditable(wxCommandEvent& event);

	void VisibleAllSprites(bool visible);
	void EditableAllSprites(bool visible);

private:
	LayerType m_layer_type;
	bool m_has_height;

	Layer* m_layer;

	wxCheckBox* m_visible_ctrl;
	wxCheckBox* m_editable_ctrl;

	int m_curr_op_idx;
	std::vector<ee::EditOP*> m_editops;

}; // LibraryPage 

}

#endif // _LR_LIBRARY_PAGE_H_