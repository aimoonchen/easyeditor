#include "EditDialog.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "InputPanel.h"
#include "ToolBarPanel.h"

#include <wx/splitter.h>

namespace etext
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx,
					   const std::shared_ptr<Sprite>& spr, const ee::MultiSpritesImpl* sprite_impl)
	: wxDialog(parent, wxID_ANY, "Edit Text", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
{
	InitLayout(glctx, spr, sprite_impl);
}

void EditDialog::InitLayout(wxGLContext* glctx, const std::shared_ptr<Sprite>& spr, 
							const ee::MultiSpritesImpl* sprite_impl)
{
	wxSplitterWindow* right_split = new wxSplitterWindow(this);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split, glctx, spr, sprite_impl);
	wxWindow* right = InitLayoutRight(right_split, spr);

	left_split->SetSashGravity(0.15f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);
}

wxWindow* EditDialog::InitLayoutLeft(wxWindow* parent)
{
	m_library = new LibraryPanel(parent);
	return m_library;
}

wxWindow* EditDialog::InitLayoutCenter(wxWindow* parent, wxGLContext* glctx,
									   const std::shared_ptr<Sprite>& spr, const ee::MultiSpritesImpl* sprite_impl)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_stage = new StagePanel(split, this, glctx, spr, sprite_impl, m_library);

	m_input = new InputPanel(split, spr, m_stage->GetStageImpl());

	split->SetSashGravity(0.6f);
	split->SplitHorizontally(m_stage, m_input);

	return split;
}

wxWindow* EditDialog::InitLayoutRight(wxWindow* parent, const std::shared_ptr<Sprite>& spr)
{
	m_toolbar = new ToolbarPanel(parent, m_stage, spr);
	return m_toolbar;
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

// 	ee::ConfirmDialog dlg(this);
// 	int val = dlg.ShowModal();
// 	if (val == wxID_YES) 
// 	{
// 		m_sym->InitBounding();
// 		const std::string& filepath = m_sym->GetFilepath();
// 		FileStorer::Store(filepath.c_str(), m_sym);
// 		m_sym->RefreshThumbnail(filepath, true);
// 		ee::SpritePool::Instance()->UpdateBoundings(*m_sym);
// 		Destroy();
// 	} 
// 	else if (val == wxID_NO) 
// 	{
// 		m_sym->LoadFromFile(m_sym->GetFilepath());
// 		Destroy();
// 	}
}

}