#ifndef _EASYSKELETON_TOOLBAR_PANEL_H_
#define _EASYSKELETON_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace eskeleton
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

protected:
	virtual wxSizer* InitLayout() override;

}; // ToolbarPanel

}

#endif // _EASYSKELETON_TOOLBAR_PANEL_H_
