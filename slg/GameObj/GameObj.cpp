#include "GameObj.h"

#include "Battle/BattleData.h"

GameObject::GameObject()
{    
	this->setCascadeOpacityEnabled(true);
	m_armature = NULL;
}

GameObject::~GameObject()
{
	this->cleanup();
	this->removeFromParent();
}

void GameObject::pause()
{
	Node::pause();
	if ( m_armature!= NULL )
	{
		m_armature->pause();
		m_armature->getAnimation()->pause();	
	}
}

void GameObject::resume()
{
	Node::resume();
	if ( m_armature!=NULL )
	{
		m_armature->resume();
		m_armature->getAnimation()->resume();
	}
}

void GameObject::stop()
{
	if ( m_armature!= NULL )
	{
		m_armature->stopAllActions();
		m_armature->getAnimation()->stop();	
	}
}

void GameObject::play( const std::string& name, float speed, int durationTo  )
{
	if ( m_armature!= NULL )
	{
		m_armature->stopAllActions();
		m_armature->getAnimation()->setMovementEventCallFunc(nullptr);
		m_armature->getAnimation()->setSpeedScale(speed);
		m_armature->getAnimation()->play(name,durationTo);
	}
}

void GameObject::play( int startFrame, int index )
{
	startFrame < 0 ? 0 : startFrame;
	if ( m_armature!= NULL )
	{
		m_armature->stopAllActions();
		m_armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(GameObject::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		m_armature->getAnimation()->playWithIndex(index);
		m_armature->getAnimation()->gotoAndPlay(startFrame);
	}
}

void GameObject::replay()
{
	play(0,0);
}

void GameObject::armatureRunAction(Action* action)
{
	if ( m_armature!= NULL )
	{
		m_armature->runAction(action);
	}
}

void GameObject::play( const std::string& name, Action* pAction, float speed, int durationTo )
{
	if ( m_armature!= NULL )
	{
		m_armature->stopAllActions();
		m_armature->getAnimation()->setMovementEventCallFunc(nullptr);
		m_armature->getAnimation()->setSpeedScale(speed);
		m_armature->getAnimation()->play(name,durationTo);
		m_armature->runAction( pAction );	
	}	
}

void GameObject::playWithCB(const std::string& name, float speed, int durationTo, int loop)
{
	if ( m_armature!= NULL )
	{
		m_armature->stopAllActions();
		m_armature->getAnimation()->setMovementEventCallFunc(
			CC_CALLBACK_0(GameObject::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );

		m_armature->getAnimation()->setSpeedScale(speed);
		m_armature->getAnimation()->play(name, durationTo, loop);
	}
}

void GameObject::playWithCB( float speed, int durationTo, int loops )
{
	if ( m_armature!= NULL )
	{
		m_armature->stopAllActions();
		m_armature->getAnimation()->setMovementEventCallFunc(
			CC_CALLBACK_0(GameObject::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );

		m_armature->getAnimation()->setSpeedScale(speed);
		m_armature->getAnimation()->playWithIndex(0,durationTo,loops);
	}
}

bool GameObject::containsPt( const Point& pt )
{
/*	if( getParent() == NULL )
	{
		return false;
	}

	CCPoint realPt = getParent()->convertToNodeSpaceAR(pt);

	CCPoint pos = getPosition();

	CCRect palyerRect = getTextureRect();
	CCPoint anchor = getAnchorPoint();

	float scale = CCDirector::getInstance()->getContentScaleFactor();

	int width = palyerRect.size.width*getScaleX();
	int height = palyerRect.size.height*getScaleY();

	palyerRect.setRect( pos.x - anchor.x*width, pos.y, width, height ); 

	if( palyerRect.containsPoint(realPt) )
	{
		return true;
	} 
*/
	return false;
}

void GameObject::setColor(const Color3B& color)
{
	if ( m_armature!=NULL )
	{
		m_armature->updateDisplayedColor(color);		
	}
}

void GameObject::revertColor()
{
	if ( m_armature!=NULL )
	{
		m_armature->setColor(Color3B(255,255,255));		
	}
}

void GameObject::setOpacity(GLubyte opacity)
{
	if (m_armature != NULL)
	{
		m_armature->setOpacity(opacity);
	}
}

void GameObject::animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID)
{
}

/**************************************************End of GameObject********************************************/

EffectObject* EffectObject::create(const std::string& effect)
{
    EffectObject* ret = new EffectObject();	
    if( ret )
    {
		ret->initArmatureByEffect(effect);
		ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void EffectObject::initArmatureByEffect( const std::string& effect )
{
	m_function = nullptr;
	std::string path = "effect/";
	path += effect + ".ExportJson";

	ArmatureDataManager::getInstance()->addArmatureFileInfo(path);
	std::vector<std::string> nameVec;
	m_armature = Armature::create(nameVec[nameVec.size()-1]);
    addChild(m_armature);
}

void EffectObject::setCompleteCB(const std::function<void()>& func)
{
	m_function = func;
}

Vec2 EffectObject::getEffectPos() const
{
	return m_armature->getPosition(); //this->getPosition()+
}

void EffectObject::animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID)
{
	if (movementType == COMPLETE || movementType == LOOP_COMPLETE)
	{
		if (m_function)
		{
			m_function();
		}
	}
}

/**************************************************End of EffectObject********************************************/
RoleObject::RoleObject():
m_hpBar(NULL),
m_hpLabel(NULL),
m_hpNode(NULL)
{
}
RoleObject::~RoleObject()
{
	this->setOpacity(255);
}

RoleObject* RoleObject::create(const std::string& looks)
{
    RoleObject* ret = new RoleObject();	
    if( ret )
    {
		ret->initArmatureByLooks(looks);
		ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void RoleObject::initArmatureByLooks( const std::string& looks )
{
	m_looks = looks.substr(looks.rfind('/') + 1, looks.rfind('.'));
	std::string path = "res/role/animation/" + looks + ".ExportJson";

	ArmatureDataManager::getInstance()->addArmatureFileInfo(path);
	m_armature = Armature::create(m_looks);
	//m_armature->setDisplayShadow(true);

	addChild(m_armature);
}

const std::string& RoleObject::getLooks() const
{
	return m_looks;
}

void RoleObject::initHpBar(int maxHp, int curHp, float scale, Point pos, Node* target, bool isRedTeam /* = false */)
{
	std::string img_path("res/ui/demo/battle/");
	std::string res = isRedTeam ? "health_red.png" : "health_blue.png" ;

	m_hpNode = Node::create();
	auto bg = Sprite::create(img_path + "health_bg.png");
	Size size = bg->getContentSize();
	m_hpNode->setContentSize(size);
	m_hpNode->addChild(bg);
	
	auto hpImg = Sprite::create(img_path + res);
	m_hpBar = ProgressTimer::create(hpImg);
	m_hpBar->setType(ProgressTimer::Type::BAR);
	m_hpBar->setMidpoint(Point(0, 0.5));
	m_hpBar->setBarChangeRate(Point(1.0f, 0.0f));
	m_hpBar->setPercentage(100.0f);
	m_hpNode->addChild(m_hpBar);

	m_hpLabel = Label::createWithSystemFont(StringUtils::format("%d", maxHp), "res/fonts/arial.ttf", 24);
	m_hpNode->addChild(m_hpLabel);

	m_hpNode->setPosition(pos);
	m_hpNode->setCascadeOpacityEnabled(true);
	m_hpNode->setScale(scale);
	if (target)
	{
		target->addChild(m_hpNode);
	}
	else
	{
		addChild(m_hpNode);
	}
}

void RoleObject::updateHpBar(int maxHp, int curHp)
{
	curHp = curHp > 0 ? curHp : 0 ;
	if (m_hpBar)
	{
		m_hpBar->stopAllActions();
		float cur = curHp * 100.0f / maxHp;
		float pre = m_hpBar->getPercentage();
		m_hpBar->runAction(ProgressFromTo::create(
			.5f,
			pre,
			cur));
		//m_hpBar->setPercentage(curHp * 100.0f / maxHp);
	}

	if (m_hpLabel)
	{
		m_hpLabel->setString(StringUtils::format("%d", curHp));
	}
}

Size RoleObject::getObjectSize()
{
	if (m_armature)
	{
		return m_armature->getContentSize();
	}
	
	return Size::ZERO;
}

void RoleObject::setId(int cardId, bool isEnemy)
{
	static int seq = 1;
	_cardId = cardId;
	_id = !isEnemy ? cardId : cardId+10000;
	_id = _id*100+seq++;
	seq = seq >= 100 ? 1 : seq;
}

////////////////////////////////////////////////////////
