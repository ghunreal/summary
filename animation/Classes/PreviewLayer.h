#pragma  once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ActionLayer.h"

USING_NS_CC;
using namespace ui;

namespace Tool
{
	
	class PreviewLayer :public	Layer
	{
	public:
		static Scene* createScene();
		virtual bool init();
	private:
		void initUi();
		void initMenu();
		void initMenuScale();
		void initMenuAniSpeed();
		void initMenuFps();
		void initMenuMoveSpeed();
		void initActionLayer();
		void initMoseEvt();
		void refreshRoleList();
		void onSelectedHero(Ref* , ListView::EventType);
		void refreshActionsList(const std::string& name);
		void onSelectedAction(Ref*, ListView::EventType);
		CREATE_FUNC(PreviewLayer);

		void getRoleNames();
	private:
		Node*	m_root;
		Node*	m_actionNode;
		Layout*	 m_menu;
		ActionLayer* m_actLayer;
		int m_curIdx;
		std::vector<std::string> m_roleList;
		Point m_pos;
	};
}
