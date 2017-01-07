#include "GameObject.h"
#include "common.h"
#include "Preference.h"
#include "HelperFunc.h"

#include "JSON/CustomAnimation.h"

namespace Tool{
	GameObject::GameObject()
		:m_curState(NONE),
		m_curDir(0),
		m_curAniSpeed(1.0f),
		m_curMoveSpeed(40),
		m_targetPos(VisibleRect::center())
	{
	}
	bool GameObject::init()
	{
		return Sprite::init();
	}

	void GameObject::setAniSpeed(float speed)
	{
		if (speed == m_curAniSpeed)
		{
			return;
		}
		if (speed < 0.00001f)
		{
			m_curAniSpeed = 0.00001f;
		}
		m_curAniSpeed = speed;
		rePlayAction(m_curDir);
	}

	void GameObject::setDir(int dir)
	{
		m_curDir = dir > 8 ? 8 : dir < 1 ? 1 : dir;
	}

	void GameObject::update(float dt)
	{
		auto curPos = getPosition();
		Point diff = m_targetPos - curPos;
		if (abs(diff.x) <= 2)	diff.x = 0;
		if (abs(diff.y) <= 2)	diff.y = 0;
		float xFactor = diff.x > 0 ? 1 : diff.x == 0 ? 0 : -1;
		float yFactor = diff.y > 0 ? 1 : diff.y == 0 ? 0 : -1;
		auto next = Point(curPos.x + xFactor * dt * m_curMoveSpeed, curPos.y + yFactor * dt * m_curMoveSpeed);
		setPosition(next);
	}


	void GameObject::setActions(const std::vector<std::string>& actions)
	{
		m_actions.clear();
		m_actions = actions;
	}

	void GameObject::initRole(const std::string& name)
	{
		m_roleName = name;
		setDir(8);
		playAction(m_actions[0], 1);
	}

	bool GameObject::isRun(const std::string& name)
	{
		return name.find(g_strRun) != std::string::npos || name.find(g_strRun1) != std::string::npos;
	}

	std::vector<std::string> GameObject::getActions(const std::string name, int dir)
	{
		std::vector<std::string> ret;

		char str[16] = { 0 };
		itoa(dir, str, 10);
		auto framesPath = StringUtils::format("%s/%s/%s/%s", g_path, name.c_str(), g_strAttack, str);

		return HelperFunc::TraverseFloder(framesPath);
	}

	void GameObject::playAction(const std::string& actionName, int dir)
	{
		if (m_ActionName == actionName && dir == m_curDir)
		{
			return;
		}

		m_ActionName = actionName;
		setDir(dir);

		/*char str[16] = { 0 };
		std::string path = Preference::getResRootPath();
		itoa(dir, str, 10);
		auto framesPath = StringUtils::format("%s/%s/%s/%s", g_path, m_roleName.c_str(), actionName.c_str(), str);
		auto fileUtil = FileUtils::getInstance();
		if (!fileUtil->isDirectoryExist(framesPath))
		{
			CCLOG(" %s  is none!", actionName.c_str());
			return;
		}
		std::vector<std::string> fileNames = HelperFunc::TraverseFloder(path + framesPath);

		auto ani = Animation::create();
		for (int n = 0; n < fileNames.size(); ++n)
		{
		ani->addSpriteFrameWithFile(framesPath + "/" + fileNames[n]);
		}
		auto animate = Animate::create(ani);
		*/
		auto animate = CustomAnimation::getInstance()->getAnimate(m_roleName, actionName, m_curDir,  Preference::getFps() * m_curAniSpeed);

		this->addChild(CSLoader::createNodeAndPlayRepeat("res/effect/battle/Cha001_attack_001.csb"));
		stopAllActions();
		unscheduleUpdate();
		setPosition(VisibleRect::center());
		if (isRun(actionName))
		{
			scheduleUpdate();
		}
		else
		{
			m_targetPos = VisibleRect::center();
		}

		runAction(RepeatForever::create(animate));
	}

	void GameObject::rePlayAction(int dir)
	{
		dir = -1;
		playAction(m_ActionName, dir);
	}

	void GameObject::changeDir(const Vec2& pos, int dir)
	{
		m_targetPos = pos;
		playAction(m_ActionName, dir);
	}
};