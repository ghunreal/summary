#pragma once

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "VisibleRect.h"

USING_NS_CC;
using namespace cocostudio;

namespace Tool{

	class GameObject :public Sprite{
	public:
		enum eState{
			NONE,
			RUN,
			ATTACK,
			STAND,
			OTHER
		};

		GameObject();
		virtual bool init();
	
		int getDir()const{ return m_curDir; };
		void setAniSpeed(float speed);
		void setMoveSpeed(int speed) { m_curMoveSpeed = speed; };
		CREATE_FUNC(GameObject);

		void initRole(const std::string& name);
		void playAction(const std::string& name, int dir);
		void rePlayAction(int dir);
		//void loadActions(const std::string& name);
		bool isRun(const std::string& name);
		void changeDir(const Vec2&, int dir);
		std::vector<std::string> getActions(const std::string name, int dir);
		void setActions(const std::vector<std::string>&);
	private:
		void setDir(int dir);
		void update(float dt);
	private:

		eState m_curState;
		int m_curDir;
		float m_curAniSpeed;
		Point m_targetPos;
		int m_curMoveSpeed;

		std::string m_roleName;
		std::string m_ActionName;
		std::vector<std::string> m_actions;
	};
}