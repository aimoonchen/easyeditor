#ifndef _ESHADER_TASK_H_
#define _ESHADER_TASK_H_

#include <drag2d.h>

namespace eshader 
{

class LibraryPanel;
class ToolbarPanel;

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void load(const char* filepath);
	virtual void store(const char* filepath) const;

	virtual bool isDirty() const;

	virtual void clear();

	virtual void getAllSprite(std::vector<const d2d::ISprite*>& sprites) const;

	virtual const d2d::EditPanel* getEditPanel() const;

private:
	void initLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	bool m_is_2d;

	LibraryPanel* m_library;
	d2d::EditPanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _ESHADER_TASK_H_