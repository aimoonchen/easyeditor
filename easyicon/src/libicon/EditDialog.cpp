#include "EditDialog.h"
#include "StagePanel.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Icon.h"
#include "IconType.h"
#include "FileIO.h"
#include "EditRectOP.h"
#include "EditQuadOP.h"
#include "EditChangedRectOP.h"
#include "EditChangedSectorOP.h"

#include <ee/panel_msg.h>
#include <ee/ConfirmDialog.h>
#include <ee/SpriteFactory.h>

namespace eicon
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx,
					   ee::Sprite* edited, const ee::MultiSpritesImpl* sprite_impl)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_stage(NULL)
{
	InitLayout(glctx, edited, sprite_impl);
	InitEditOP(edited);

	ee::SetWndDirtySJ::Instance()->SetDirty();
}

void EditDialog::InitLayout(wxGLContext* glctx, ee::Sprite* edited, 
							const ee::MultiSpritesImpl* sprite_impl)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_stage = new StagePanel(this, this, glctx, edited, sprite_impl);
	sizer->Add(m_stage, 1, wxEXPAND);

	SetSizer(sizer);
}

void EditDialog::InitEditOP(ee::Sprite* edited)
{
	ee::EditOP* op = NULL;

	Symbol* sym = dynamic_cast<Symbol*>(edited->GetSymbol());
	const Icon* icon = dynamic_cast<const Icon*>(sym->GetIcon());
	IconType type = get_icon_type(icon->GetIconDesc());
	switch (type)
	{
	case IT_RECT:
		op = new EditRectOP(m_stage);
		break;
	case IT_QUAD:
		op = new EditQuadOP(m_stage);
		break;
	case IT_CHANGED_RECT:
		op = new EditChangedRectOP(m_stage);
		break;
	case IT_CHANGED_SECTOR:
		op = new EditChangedSectorOP(m_stage);
		break;
	}

	m_stage->SetEditOP(op);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	Symbol& sym = m_stage->GetSymbol();
	const std::string& filepath = sym.GetFilepath();

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		FileIO::StoreToFile(filepath.c_str(), dynamic_cast<Icon*>(sym.GetIcon()));
		sym.RefreshThumbnail(filepath);
		ee::SpriteFactory::Instance()->UpdateBoundings(sym);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		sym.LoadFromFile(filepath);
		Destroy();
	}
}

}