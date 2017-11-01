#ifndef _EASYSCALE9_SPRITE_H_
#define _EASYSCALE9_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/Scale9Sprite.h>

namespace escale9
{

class Symbol;

class Sprite : public s2::Scale9Sprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const CU_STR& dir = "");
	virtual void Store(Json::Value& val, const CU_STR& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);	

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);
	
	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYSCALE9_SPRITE_H_