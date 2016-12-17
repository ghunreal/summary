#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include <string>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace cocostudio;

class GameObject : public Node
{
public:
    GameObject();
    virtual ~GameObject()=0;

public:
	void stop();
	void pause();
	void resume();

	void replay();
	void armatureRunAction(Action* action);
	
	virtual void setColor(const Color3B& color);
	void revertColor();
	bool containsPt( const Point& pt );
	void setOpacity(GLubyte opacity);
protected:   

	Armature* m_armature;		
	virtual void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);

	void play(int startFrame = 0, int index = 0);
	virtual void play(const std::string& name, float speed, int durationTo = -1);
	virtual void play(const std::string& name, Action* pAction, float speed, int durationTo = -1);
	virtual void playWithCB(const std::string& name, float speed, int durationTo = -1, int loop = -1);
	void playWithCB(float speed, int durationTo = -1, int loops = -1);
};

class EffectObject : public GameObject
{
public:
	static EffectObject* create( const std::string& effect );
	void setCompleteCB(const std::function<void()>& func);
	Vec2 getEffectPos() const;
protected:
	void initArmatureByEffect( const std::string& effect );
	virtual void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
	std::function<void()> m_function;	
};

class RoleObject : public GameObject
{
public:
	RoleObject();
	static RoleObject* create( const std::string& looks );
	virtual void initArmatureByLooks(const std::string& looks);
	void initHpBar(int maxHp, int curHp, float scale, Point pos, Node* target = NULL, bool isRedTeam = false);
	void updateHpBar(int maxHp, int curHp);
	const std::string& getLooks() const;
	Size getObjectSize();
	virtual ~RoleObject();

	int getCardId() const { return _cardId; };
	int getId() const { return _id; }
	void setId(int cardId, bool isEnemy); 

protected: 
	std::string	m_looks;
	ProgressTimer* m_hpBar;
	Label* m_hpLabel;
	Node* m_hpNode;
	int _cardId;
	int _id;
};

#endif /* defined(__GAME_OBJECT__) */
