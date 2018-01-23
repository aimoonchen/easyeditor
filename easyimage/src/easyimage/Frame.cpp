#include "Frame.h"
#include "RectCutOP.h"
#include "RectCutCMPT.h"

#include <ee/EditPanel.h>
#include <ee/Task.h>

namespace eimage
{

Frame::Frame(const std::string& title)
	: ee::Frame(title, "")
{
}

void Frame::OnNew(wxCommandEvent& event)
{
	auto editop = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	editop->Clear();
}

void Frame::OnOpen(wxCommandEvent& event)
{
	auto editop = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	if (auto op = std::dynamic_pointer_cast<RectCutOP>(editop))
	{
		op->GetEditCMPT()->OnLoadEditOP(event);
	}
}

void Frame::OnSave(wxCommandEvent& event)
{
	auto editop = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	if (auto op = std::dynamic_pointer_cast<RectCutOP>(editop))
	{
		op->GetEditCMPT()->OnSaveEditOP(event);
	}
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	auto editop = const_cast<ee::EditPanel*>(m_task->GetEditPanel())->GetEditOP();
	if (auto op = std::dynamic_pointer_cast<RectCutOP>(editop))
	{
		op->GetEditCMPT()->OnSaveEditOP(event);
	}
}

}