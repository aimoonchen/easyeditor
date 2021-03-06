
#include "ActorsList.h"
#include "ActorsPage.h"
#include "ActorItem.h"
#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "ActorPropertySetting.h"
#include "Context.h"

#include "dataset/Actor.h"
#include "dataset/Layer.h"
#include "output/DesignerPage.h"

using namespace ebuilder;

ActorsList::ActorsList(wxWindow* parent)
	: ee::LibraryList(parent, "actor", false)
{
}

void ActorsList::onListSelected(wxCommandEvent& event)
{
	if (m_items.empty()) return;

	// property panel
	ActorItem* item = static_cast<ActorItem*>(m_items[event.GetInt()]);
	ee::IPropertySetting* setting 
		= new ActorPropertySetting(item->getActor(), this);
	Context::Instance()->property->setPropertySetting(setting);

	// stage panel
	StagePanel* stage = Context::Instance()->stage;
	ee::SpriteSelection* selection = stage->getDesignerPage()->getSpriteSelection();
	selection->clear();
	selection->insert(item->getActor());
	stage->getDesignerPage()->Refresh();
}

void ActorsList::loadFromLayer(Layer* layer)
{
	clear();

	const std::vector<Actor*>& actors = layer->getActors();
	for (size_t i = 0, n = actors.size(); i < n; ++i)
	{
		insert(new ActorItem(actors[i]));
	}
}