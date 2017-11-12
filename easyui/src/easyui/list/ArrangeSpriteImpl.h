#ifndef _EASYUI_LIST_ARRANGE_SPRITE_IMPL_H_
#define _EASYUI_LIST_ARRANGE_SPRITE_IMPL_H_

#include <ee/ArrangeSpriteImpl.h>

namespace eui
{
namespace list
{

class StagePanel;

class ArrangeSpriteImpl : public ee::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* stage, ee::PropertySettingPanel* property);

private:
	virtual ee::ArrangeSpriteState* CreateTranslateState(ee::SpriteSelection* selection, 
		const sm::vec2& first_pos) const override;

private:
	StagePanel* m_stage;

}; // ArrangeSpriteImpl

}
}

#endif // _EASYUI_LIST_ARRANGE_SPRITE_IMPL_H_