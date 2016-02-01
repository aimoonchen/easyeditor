#ifndef _EASYSHAPE_TASK_H_
#define _EASYSHAPE_TASK_H_


#include <easyshape.h>

namespace eshape
{

class Task : public ee::Task
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const ee::Sprite*>& sprites) const;

	virtual const ee::EditPanel* GetEditPanel() const;

private:
	void InitLayout(wxFrame* parent);

private:
	ee::LibraryPanel* m_library;

	libshape::StagePanel* m_stage;

}; // Task

}

#endif // _EASYSHAPE_TASK_H_