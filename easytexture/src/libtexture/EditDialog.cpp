#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileSaver.h"

#include <ee/panel_msg.h>
#include <ee/Config.h>
#include <ee/SettingData.h>
#include <ee/ConfirmDialog.h>
#include <ee/SpritePool.h>
#include <ee/CameraCanvas.h>

#include <sprite2/OrthoCamera.h>

#include <wx/splitter.h>

namespace etexture
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx,
					   const std::shared_ptr<Sprite>& edited, const ee::MultiSpritesImpl* sprite_impl)
	: wxDialog(parent, wxID_ANY, "Edit Texture", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_sym(NULL)
	, m_stage(NULL)
{
	assert(edited);

	auto sym = std::dynamic_pointer_cast<Symbol>(edited->GetSymbol());
	m_sym = sym;
//	m_sym->ReloadTexture();
	SetTitle(sym->GetFilepath());

	InitLayout(glctx, edited, sprite_impl);

	ee::SetWndDirtySJ::Instance()->SetDirty();

	m_visible_tex_edge = ee::Config::Instance()->GetSettings().visible_tex_edge;
	ee::Config::Instance()->GetSettings().visible_tex_edge = true;
}

EditDialog::~EditDialog()
{
	ee::Config::Instance()->GetSettings().visible_tex_edge = m_visible_tex_edge;
}

void EditDialog::InitLayout(wxGLContext* glctx, const ee::SprPtr& edited, const ee::MultiSpritesImpl* sprite_impl)
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(this);
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);

	LibraryPanel* library = new LibraryPanel(left_splitter);
	StagePanel* stage = new StagePanel(left_splitter, this, glctx, edited, sprite_impl, library);
	m_stage = stage;

	InitCamera(edited);

	left_splitter->SetSashGravity(0.15f);
	left_splitter->SplitVertically(library, stage);

	ToolbarPanel* toolbar = new ToolbarPanel(right_splitter, stage);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_splitter, toolbar);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		const std::string& filepath = m_sym->GetFilepath();
		FileSaver::Store(filepath.c_str(), m_sym);
		m_sym->RefreshThumbnail(filepath);
		ee::SpritePool::Instance()->UpdateBoundings(*m_sym);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		m_sym->LoadFromFile(m_sym->GetFilepath());
		Destroy();
	}
}

void EditDialog::InitCamera(const ee::SprPtr& spr) const
{
	auto canvas = std::dynamic_pointer_cast<ee::CameraCanvas>(m_stage->GetCanvas());
	auto cam = canvas->GetCamera();
	if (cam->Type() == s2::CAM_PSEUDO3D) {
		return;
	}

	wxSize sz = GetSize();
	sm::vec2 r_sz = spr->GetBounding().GetSize().Size();
	float scale = std::min(sz.GetWidth() / r_sz.x, sz.GetHeight() / r_sz.y);

	auto ortho_cam = std::dynamic_pointer_cast<s2::OrthoCamera>(cam);
	ortho_cam->Set(sm::vec2(0, 0), 1 / scale);
}

}