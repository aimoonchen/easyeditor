#include "Frame.h"
#include "SettingsDialog.h"
#include "SettingCfg.h"
#include "config.h"
#include "ToolBar.h"
#include "Task.h"

#include "preview/MainDialog.h"
#include "view/StagePanel.h"
#include "view/typedef.h"

#include <ee/FileHelper.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/StageCanvas.h>
#include <ee/StringHelper.h>
#include <ee/EditedFileStack.h>

#include <easyshape.h>

#include <sprite2/BoundingBox.h>
#include <sprite2/DrawRT.h>

namespace lr
{

BEGIN_EVENT_TABLE(Frame, ee::Frame)
	EVT_MENU(ID_PREVIEW, Frame::OnPreview)
	EVT_MENU(ID_SETING_EXTEND, Frame::OnExtendSetting)
	EVT_MENU(ID_STATISTICS, Frame::OnStatistics)
	EVT_MENU_RANGE(ID_TOOLBAR+1, ID_TOOLBAR+9, Frame::OnToolBarClick)
END_EVENT_TABLE()

Frame::Frame(const std::string& title, const std::string& filetag)
	: ee::Frame(title, filetag)
{
	m_view_menu->Append(ID_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxT("Play"));
	m_setting_menu->Append(ID_SETING_EXTEND, wxT("Extend"), wxT("Extend"));
	m_code_menu->Append(ID_STATISTICS, "Statistics", "Statistics");

	m_toolbar = new ToolBar(this, ID_TOOLBAR);
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		std::string filter = GetFileFilter() + "|PNG files (*.png)|*.png";
		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK)
		{
			std::string filename = dlg.GetPath();
			std::string ext = ee::FileHelper::GetExtension(filename);
			if (ext == "png") {
				SaveAsPNG(filename);
			} else {
				SaveAsJson(filename);
			}
		}
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::OnFullView(wxCommandEvent& event)
{
	static_cast<Task*>(m_task)->OnFullView();
}

void Frame::OnToolBarClick(wxCommandEvent& event)
{
	int idx = event.GetId() - ID_TOOLBAR - 1;
	m_toolbar->OnClick(idx);
}

void Frame::OnPreview(wxCommandEvent& event)
{
	SettingCfg* cfg = SettingCfg::Instance();

	std::vector<ee::Sprite*> sprs;
	m_task->GetAllSprite(sprs);

	wxGLContext* ctx = m_task->GetEditPanel()->GetCanvas()->GetGLContext();
	preview::MainDialog dlg(this, ctx, cfg->m_view_width * PREVIEW_SCALE, cfg->m_view_height * PREVIEW_SCALE, sprs);
	dlg.ShowModal();
}

void Frame::OnExtendSetting(wxCommandEvent& event)
{
	SettingDialog dlg(this, (StagePanel*)(m_task->GetEditPanel()));
	dlg.ShowModal();
}

void Frame::OnStatistics(wxCommandEvent& event)
{
	std::vector<ee::Sprite*> sprs;
	StagePanel* stage = (StagePanel*)(m_task->GetEditPanel());
	stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs), ee::DT_ALL);
	int count = sprs.size();
	int area = 0;
	for (int i = 0; i < count; ++i) {
		const ee::Sprite* spr = sprs[i];
		sm::rect r = spr->GetBounding()->GetSize();
		area += (r.xmax - r.xmin) * (r.ymax - r.ymin);
	}

	SettingCfg* cfg = SettingCfg::Instance();
 	float coverage = static_cast<float>(area) / (cfg->m_map_width * cfg->m_map_height);
	std::string msg = ee::StringHelper::Format("count %d, area %d, coverage %f", count, area, coverage);
	wxMessageBox(msg);
}

void Frame::SaveAsPNG(const std::string& filepath) const
{
	SettingCfg* cfg = SettingCfg::Instance();
	s2::DrawRT rt(cfg->m_view_width, cfg->m_view_height);
	StagePanel* stage = (StagePanel*)(m_task->GetEditPanel());

	std::vector<ee::Sprite*> cover_layer, top_layer;

	std::vector<ee::Sprite*> all_sprites;
	stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(all_sprites), ee::DT_VISIBLE);
	for (int i = 0, n = all_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = all_sprites[i];

		const std::string& tag = spr->GetTag();
		if (tag.find(TOP_LAYER_TAG) != std::string::npos) {
			top_layer.push_back(spr);
		} else if (tag.find(COVER_LAYER_TAG) != std::string::npos) {
			cover_layer.push_back(spr);
		} else {
			rt.Draw(spr);
		}
	}
	std::sort(cover_layer.begin(), cover_layer.end(), ee::SpriteCmp(ee::SpriteCmp::e_y_invert));
	for (int i = 0, n = cover_layer.size(); i < n; ++i) {
		rt.Draw(cover_layer[i]);
	}
	for (int i = 0, n = top_layer.size(); i < n; ++i) {
		rt.Draw(top_layer[i]);
	}

	std::vector<ee::Shape*> shapes;
	stage->TraverseShapes(ee::FetchAllVisitor<ee::Shape>(shapes), ee::DT_VISIBLE);
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		rt.Draw(shapes[i]);		
	}

	eshape::RectShape rect(sm::vec2(0, 0), cfg->m_view_width * 0.5f, cfg->m_view_height * 0.5f);
	rt.Draw(&rect);

	rt.StoreToFile(filepath);
}

void Frame::SaveAsJson(const std::string& filepath) const
{
	std::string fixed = ee::FileHelper::GetFilenameAddTag(filepath, m_filetag, "json");
	ee::EditedFileStack::Instance()->SetBottom(fixed);
	m_task->Store(fixed.c_str());
}

}