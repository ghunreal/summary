#include "ActionLayer.h"
#include "VisibleRect.h"
#include "Preference.h"
#include "common.h"
#include "HelperFunc.h"

namespace Tool{

	bool ActionLayer::init()
	{
		if (!Layer::init())
		{
			return false;
		}

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [](Touch* touch, Event* event){
			return true;
		};
		listener->onTouchEnded = [this](Touch* touch, Event* event){
			auto target = static_cast<Node*>(event->getCurrentTarget());

			Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
			Size s = target->getContentSize();
			Rect rect = Rect(0, 0, s.width, s.height);

			if (rect.containsPoint(locationInNode))
			{
				log("ActionLayer onTouchEnded... x = %f, y = %f", locationInNode.x, locationInNode.y);
				target->setOpacity(180);

				m_curDir->setString(StringUtils::format("Direction:%d", getDirection(locationInNode)));
				m_player->changeDir(locationInNode, getDirection(locationInNode));
			}
		};
		listener->onTouchCancelled = [](Touch* touch, Event* event){};
		listener->onTouchMoved = [](Touch* touch, Event* event){};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
		
		addDebugItmes();
		addPlayer();
		return true;
	}

	void ActionLayer::addDebugItmes()
	{
		m_linesLayer = Layer::create();
		auto center = VisibleRect::center();

		auto node = DrawNode::create();
		/*auto length = (VisibleRect::leftTop() - center).length();
		for (int	i = 1; i < 9;++i)
		{
		float x = length * sin(i * M_PI / 8);
		float y = length * cos(i * M_PI / 8);
		node->drawLine(center, center + Point(x, y), Color4F::RED);
		}*/

		m_curDir = LabelTTF::create("Direction:1", "arial.ttf", 24);
		m_curDir->setPosition(VisibleRect::top().x, VisibleRect::top().y - 30);
		m_linesLayer->addChild(m_curDir);

		m_linesLayer->addChild(node);

		addChild(m_linesLayer);
		displayQuadrant(true);
	}

	void ActionLayer::displayQuadrant(bool bDis /* = true */)
	{
		m_linesLayer->setVisible(bDis);
	}

	void ActionLayer::addPlayer()
	{
		m_player = GameObject::create();
		m_player->setPosition(VisibleRect::center());
		m_player->setScale(Preference::getScale() / 10.0f);
		this->addChild(m_player);
	}

	int ActionLayer::getDirection(const Point& targetPos)
	{
		auto originalPos = VisibleRect::center();

		if (targetPos == originalPos)
		{
			return 0;
		}

		auto dir = targetPos - originalPos;
		float angle = dir.getAngle();
		if (angle >= -M_PI / 8 && angle < M_PI / 8) // right
		{
			return 3;
		}
		else if (angle >= M_PI / 8 && angle < M_PI * 3 / 8) // top-right
		{
			return 2;
		}
		else if (angle >= M_PI * 3 / 8 && angle < M_PI * 5 / 8) // top
		{
			return 1;
		}
		else if (angle >= M_PI * 5 / 8 && angle < M_PI * 7 / 8) // top-left
		{
			return 8;
		}
		else if (angle >= M_PI * 7 / 8 || angle < -M_PI * 7 / 8) // left
		{
			return 7;
		}
		else if (angle >= -M_PI * 7 / 8 && angle < -M_PI * 5 / 8) // bottom-left
		{
			return 6;
		}
		else if (angle >= -M_PI * 5 / 8 && angle < -M_PI * 3 / 8) // bottom
		{
			return 5;
		}
		else if (angle >= -M_PI * 3 / 8 && angle < -M_PI / 8) // bottom-right
		{
			return 4;
		}

		return 0;
	}

}