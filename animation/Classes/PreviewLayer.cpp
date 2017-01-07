#include "PreviewLayer.h"
#include "common.h"
#include "HelperFunc.h"
#include "Preference.h"

#include "JSON/CustomAnimation.h"
namespace Tool{

	Scene* PreviewLayer::createScene()
	{
		auto scene = Scene::create();
		auto pLayer = PreviewLayer::create();

		scene->addChild(pLayer);

		return scene;
	}

	bool PreviewLayer::init()
	{
		if (!Layer::init())
		{
			return false;
		}

		initUi();
		initMenu();
		initActionLayer();
		//m_roleList = HelperFunc::GetSubDirFiles(FileUtils::getInstance()->getDefaultResourceRootPath() + g_path);
		getRoleNames();
		refreshRoleList();
		initMoseEvt();

		return true;
	}

	void PreviewLayer::getRoleNames()
	{
		m_roleList = HelperFunc::TraverseFloder(FileUtils::getInstance()->getDefaultResourceRootPath() + g_custom, "/*.json");
		for (int i = 0; i < m_roleList.size(); ++i)
		{
			std::string& str = m_roleList[i];
			str = str.substr(0, str.rfind('.'));
		}
	}

	void PreviewLayer::initUi()
	{
		m_root = CSLoader::createNode("preivewUi.csb");
		addChild(m_root);

		auto pBtn = static_cast<ui::Button*>(m_root->getChildByName("Menu"));
		pBtn->addTouchEventListener([this](Ref* pSender, Widget::TouchEventType eType){
			auto pBtn = static_cast<ui::Button*>(pSender);
			if (eType == Widget::TouchEventType::BEGAN)
			{
				pBtn->setOpacity(230);
				pBtn->setScale(0.95f);
			}
			else if (eType == Widget::TouchEventType::ENDED)
			{
				pBtn->setOpacity(255);
				pBtn->setScale(1.0f);
				m_menu->setVisible(!m_menu->isVisible());
			}
			else if (eType == Widget::TouchEventType::CANCELED)
			{
				pBtn->setOpacity(255);
				pBtn->setScale(1.0f);
			}
		});

		pBtn = static_cast<ui::Button*>(m_root->getChildByName("refresh"));
		pBtn->addTouchEventListener([this](Ref* pSender, Widget::TouchEventType eType){
			auto pBtn = static_cast<ui::Button*>(pSender);
			if (eType == Widget::TouchEventType::BEGAN)
			{
				pBtn->setOpacity(230);
				pBtn->setScale(0.75f);
			}
			else if (eType == Widget::TouchEventType::ENDED)
			{
				pBtn->setOpacity(255);
				pBtn->setScale(0.8f);
				m_roleList.clear();
				//m_roleList = HelperFunc::GetSubDirFiles(FileUtils::getInstance()->getDefaultResourceRootPath() + g_path);
				getRoleNames();
				refreshRoleList();
			}
			else if (eType == Widget::TouchEventType::CANCELED)
			{
				pBtn->setOpacity(255);
				pBtn->setScale(0.8f);
			}
		});
	}

	void PreviewLayer::initMenu()
	{
		m_menu = static_cast<Layout*>(m_root->getChildByName("menuLayout"));
		
		m_menu->setSwallowTouches(false);
		m_menu->addClickEventListener([this](Ref* pSender){
			m_menu->setVisible(false);
		});

		initMenuScale();
		initMenuAniSpeed();
		initMenuFps();
		initMenuMoveSpeed();
	}

	void PreviewLayer::initMenuScale()
	{
		auto pScale = static_cast<Slider*>(m_menu->getChildByName("scale"));
		static_cast<Text*>(m_menu->getChildByName("scaleMin"))->setString(StringUtils::format("%d", minScale / 10));
		static_cast<Text*>(m_menu->getChildByName("scaleMax"))->setString(StringUtils::format("%d", maxScale / 10));
		auto pScaleNum = static_cast<Text*>(m_menu->getChildByName("scaleTxt"));
		pScaleNum->setString(StringUtils::format("scale:%2.1f", Preference::getScale() / 10.0f));
		pScale->setMaxPercent(maxScale - minScale);
		pScale->setPercent(Preference::getScale() - minScale);
		pScale->addEventListener([=](Ref *pSender, Slider::EventType type){
			Slider* slider = dynamic_cast<Slider*>(pSender);
			if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
			{
				int percent = slider->getPercent() + minScale;
				float scale = percent / 10.0f;
				m_actLayer->getPlayer()->setScale(scale);
				pScaleNum->setString(StringUtils::format("scale:%2.1f", scale));
			}
			else if (type == Slider::EventType::ON_SLIDEBALL_UP)
			{
				Preference::setScale(slider->getPercent() + minScale);
			}
		});
	}

	void PreviewLayer::initMenuAniSpeed()
	{
		auto pSpeed = static_cast<Slider*>(m_menu->getChildByName("animationSpeed"));
		static_cast<Text*>(m_menu->getChildByName("animationSpeedMin"))->setString(StringUtils::format("%d", minAniSpeed / 10));
		static_cast<Text*>(m_menu->getChildByName("animationSpeedMax"))->setString(StringUtils::format("%d", maxAniSpeed / 10));
		auto pSpeedNum = static_cast<Text*>(m_menu->getChildByName("animationSpeedTxt"));

		pSpeedNum->setString(StringUtils::format("animationSpeed:%2.1f", Preference::getAniSpeed() / 10.0f));
		pSpeed->setMaxPercent(maxAniSpeed - minAniSpeed);
		pSpeed->setPercent(Preference::getAniSpeed() - minAniSpeed);
		pSpeed->addEventListener([=](Ref *pSender, Slider::EventType type){
			Slider* slider = dynamic_cast<Slider*>(pSender);
			if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
			{
				int percent = slider->getPercent() + minAniSpeed;
				pSpeedNum->setString(StringUtils::format("animationSpeed:%2.1f", percent / 10.0f));
			}
			else if (type == Slider::EventType::ON_SLIDEBALL_UP)
			{
				int percent = slider->getPercent() + minAniSpeed;
				Preference::setAniSpeed(percent);
				m_actLayer->getPlayer()->setAniSpeed(percent / 10.0f);
			}
		});
	}


	void PreviewLayer::initMenuFps()
	{
		auto pFps = static_cast<Slider*>(m_menu->getChildByName("frameRate"));
		static_cast<Text*>(m_menu->getChildByName("frameRateMin"))->setString(StringUtils::format("%d", minFps));
		static_cast<Text*>(m_menu->getChildByName("frameRateMax"))->setString(StringUtils::format("%d", maxFps));
		auto pFpsNum = static_cast<Text*>(m_menu->getChildByName("frameRateTxt"));
		pFpsNum->setString(StringUtils::format("fps:%d", Preference::getFps()));
		pFps->setMaxPercent(maxFps - minFps);
		pFps->setPercent(Preference::getFps() - minFps);
		pFps->addEventListener([=](Ref *pSender, Slider::EventType type){
			Slider* slider = dynamic_cast<Slider*>(pSender);
			if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
			{
				int percent = slider->getPercent() + minFps;
				pFpsNum->setString(StringUtils::format("fps:%d", percent));

				auto player = m_actLayer->getPlayer();
				player->rePlayAction(player->getDir());
			}
			else if (type == Slider::EventType::ON_SLIDEBALL_UP)
			{
				Preference::setFps(slider->getPercent() + minFps);
				auto player = m_actLayer->getPlayer();
				player->rePlayAction(player->getDir());
			}
		});
	}

	void PreviewLayer::initMenuMoveSpeed()
	{
		auto pFps = static_cast<Slider*>(m_menu->getChildByName("moveSpeed"));
		static_cast<Text*>(m_menu->getChildByName("moveSpeedMin"))->setString(StringUtils::format("%d", minMoveSpeed));
		static_cast<Text*>(m_menu->getChildByName("moveSpeedMax"))->setString(StringUtils::format("%d", maxMoveSpeed));
		auto pFpsNum = static_cast<Text*>(m_menu->getChildByName("moveSpeedTxt"));
		pFpsNum->setString(StringUtils::format("moveSpeed:%d", Preference::getMoveSpeed()));
		pFps->setMaxPercent(maxMoveSpeed - minMoveSpeed);
		pFps->setPercent(Preference::getMoveSpeed() - minMoveSpeed);
		pFps->addEventListener([=](Ref *pSender, Slider::EventType type){
			Slider* slider = dynamic_cast<Slider*>(pSender);
			if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
			{
				int percent = slider->getPercent() + minMoveSpeed;
				pFpsNum->setString(StringUtils::format("moveSpeed:%d", percent));

				auto player = m_actLayer->getPlayer();
				player->setMoveSpeed(percent);
			}
			else if (type == Slider::EventType::ON_SLIDEBALL_UP)
			{
				int speed = slider->getPercent() + minMoveSpeed;
				Preference::setMoveSpeed(speed);
				auto player = m_actLayer->getPlayer();
				player->setMoveSpeed(speed);
			}
		});
	}

	void PreviewLayer::initActionLayer()
	{
		m_actionNode = static_cast<Node*>(m_root->getChildByName("actionNode"));
		
		m_actLayer = ActionLayer::create();
		m_actionNode->addChild(m_actLayer);
	}

	void PreviewLayer::onSelectedHero(Ref* pSender, ListView::EventType type)
	{
		if (type == ListView::EventType::ON_SELECTED_ITEM_END)
		{
			auto sender = static_cast<ListView*>(pSender);
			auto idx = sender->getCurSelectedIndex();
			if (m_curIdx == idx)
			{
				return;
			}
			m_curIdx = idx;
			auto item = static_cast<Button*>(sender->getItem(idx));
			auto player = m_actLayer->getPlayer();

			refreshActionsList(item->getTitleText());
		}
	}

	void PreviewLayer::refreshRoleList()
	{
		ListView* pList = static_cast<ListView*>(m_root->getChildByName("roleList"));
		pList->addEventListener((ListView::ccListViewCallback)CC_CALLBACK_2(PreviewLayer::onSelectedHero, this));
		auto item = static_cast<Button*>(m_root->getChildByName("item"));

		assert(m_roleList.size() > 0);
		
		auto num = pList->getChildrenCount();
		int reuseNum = num > m_roleList.size() ? m_roleList.size() : num;
		
		for (int i = reuseNum; i < m_roleList.size(); ++i)
		{
			item->setTitleText(m_roleList[i]);
			auto copy = item->clone();
			pList->pushBackCustomItem(copy);
		}

		for (int i = num; i > reuseNum; --i)
		{
			pList->removeItem(i - 1);
		}
		for (int m = 0; m < m_roleList.size(); ++m)
		{
			auto btn = static_cast<Button*>(pList->getItem(m));
			btn->setTitleText(m_roleList[m]);
		}

		m_curIdx = 0;
		auto player = m_actLayer->getPlayer();
		refreshActionsList(m_roleList[m_curIdx]);
	}

	void PreviewLayer::initMoseEvt()
	{
		Layout* pLayout = static_cast<Layout*>(m_root->getChildByName("listPanel"));
		
		auto moueListener = EventListenerMouse::create();
		moueListener->onMouseDown = [](EventMouse* event){};
		moueListener->onMouseUp = [](EventMouse* event){};
		moueListener->onMouseScroll = [](EventMouse* event){};

		moueListener->onMouseMove = [=](EventMouse* event){
			Rect rect(pLayout->getPosition(), pLayout->getContentSize());
			auto curPos = event->getLocation();
			bool cur = rect.containsPoint(curPos), pre = rect.containsPoint(m_pos);
			if (cur && !pre)
			{
				FadeIn *act = FadeIn::create(0.5f);
				pLayout->runAction(act);
			}
			else  if (!cur && pre)
			{
				FadeOut *act = FadeOut::create(0.8f);
				pLayout->runAction(act);
			}
			m_pos = curPos;
		};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(moueListener, pLayout);
	}

	void PreviewLayer::refreshActionsList(const std::string& name)
	{
		/*auto names = HelperFunc::GetSubDirFiles(FileUtils::getInstance()->getDefaultResourceRootPath() + g_path + "/" + name);*/
		auto names = CustomAnimation::getInstance()->getActions(name);
		auto player = m_actLayer->getPlayer();
		player->setActions(names);

		ListView* pList = static_cast<ListView*>(m_root->getChildByName("actionList"));
		/*pList->addEventListener([=](Ref* pSender, ListView::EventType type){

		});*/
		pList->addEventListener((ListView::ccListViewCallback)CC_CALLBACK_2(PreviewLayer::onSelectedAction, this));
		pList->removeAllChildren();

		auto item = static_cast<Button*>(m_root->getChildByName("actionItem"));
		for (int i = 0; i < names.size(); ++i)
		{
			item->setTitleText(names[i]);
			auto copy = item->clone();
			pList->pushBackCustomItem(copy);
		}

		player->initRole(m_roleList[m_curIdx]);
	}

	void PreviewLayer::onSelectedAction(Ref* pSender, ListView::EventType type)
	{
		if (type == ListView::EventType::ON_SELECTED_ITEM_END)
		{
			CCLOG("press actions button.");
			auto sender = static_cast<ListView*>(pSender);
			auto idx = sender->getCurSelectedIndex();

			auto item = static_cast<Button*>(sender->getItem(idx));
			auto player = m_actLayer->getPlayer();

			player->playAction(item->getTitleText(), player->getDir());
		}
	}
}