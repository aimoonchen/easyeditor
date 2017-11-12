#ifndef _EASYSHADOW_STAGE_PANEL_H_
#define _EASYSHADOW_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/MultiShapesImpl.h>

namespace ee { class LibraryPanel; class MultiSpritesImpl; }

class wxGLContext;

namespace eshadow
{

class Symbol;

class StagePanel : public ee::EditPanel, public ee::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, const ee::SprPtr& edited, 
		const ee::MultiSpritesImpl* bg_sprites);
	virtual ~StagePanel();

	//
	//	wxWindows interface
	//
	virtual void Refresh(bool eraseBackground=true, const wxRect *rect=NULL) override;

	//
	// ee::MultiShapesImpl interface
	//
	virtual void TraverseShapes(ee::RefVisitor<ee::Shape>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL) const override;

	auto& GetSymbol() { return m_sym; }

	void LoadFromShadow();

protected:
	//
	//	interface MultiShapesImpl
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void InsertShape(ee::Shape* shape);

private:
	std::shared_ptr<Symbol> m_sym;

	ee::Shape* m_loop;

}; // StagePanel

}

#endif // _EASYSHADOW_STAGE_PANEL_H_