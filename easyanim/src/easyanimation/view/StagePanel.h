#pragma once

#include <drag2d.h>

namespace eanim
{
	class Controller;
	class KeyFrame;
	class SkeletonData;

	class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
			d2d::PropertySettingPanel* property, Controller* ctrl);
		virtual ~StagePanel();

		//
		// d2d::EditPanel interface
		//
		virtual void clear();

		//
		// d2d::MultiSpritesImpl interface
		//
		virtual void traverseSprites(d2d::IVisitor& visitor, 
			d2d::DataTraverseType type = d2d::DT_ALL,
			bool order = true) const;
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite);
		virtual void clearSprites();

		virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up);

		SkeletonData& getSkeletonData();

		Controller* GetController() { return m_ctrl; }

	private:
		void onMenuAddJointNode(wxCommandEvent& event);
		void onMenuDelJointNode(wxCommandEvent& event);

	public:
		enum
		{
			Menu_AddJointNode = EditPanel::Menu_End,
			Menu_DelJointNode,
			Menu_End
		};

		static std::string menu_entries[];

	private:
		Controller* m_ctrl;

	}; // StagePanel
}

