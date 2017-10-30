#ifndef _EASYEDITOR_COMBINED_DROP_TARGET_H_
#define _EASYEDITOR_COMBINED_DROP_TARGET_H_

#include <SM_Vector.h>

#include <wx/dnd.h>

#include <memory>

namespace ee
{

class Symbol;

class CombinedDropTarget : public wxDropTarget
{
public:
	CombinedDropTarget(wxWindow* parent);
	virtual ~CombinedDropTarget() {}
 
	//
	//	interface wxDropTarget
	//
	virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def);

	virtual bool OnDropSymbol(const std::shared_ptr<Symbol>& sym, const sm::vec2& pos) { return false; }
	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text) = 0;
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) = 0;

private:
	wxWindow* m_parent;

}; // CombinedDropTarget 

}

#endif // _EASYEDITOR_COMBINED_DROP_TARGET_H_