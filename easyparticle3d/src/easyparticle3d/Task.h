#ifndef _EASYPARTICLE3D_TASK_H_
#define _EASYPARTICLE3D_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace eparticle3d
{

class LibraryPanel;
class PropertySettingPanel;
class StagePanel;
class ToolbarPanel;

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const {}

	virtual const d2d::EditPanel* GetEditPanel() const;

	void StoreAsAnim(const wxString& filepath) const;
	void StoreAsInvert(const wxString& filepath) const;

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	LibraryPanel* m_library;
	PropertySettingPanel* m_property;
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYPARTICLE3D_TASK_H_
