#pragma once

#include "cocos2d.h"
#include "GameObject.h"
USING_NS_CC;

namespace Tool{
	
	class ActionLayer :public Layer
	{
	public:
		virtual bool	init();
		CREATE_FUNC(ActionLayer);

		GameObject* getPlayer()const { return m_player; };
	private:
		void addDebugItmes();
		void displayQuadrant(bool bDis = true);

		void addPlayer();
		int getDirection(const Point&);
	private:
		Layer* m_linesLayer;
		GameObject* m_player;
		Vector<Animation*> m_runAnis;
		LabelTTF* m_curDir;
	};
}


