
#include "CustomAnimation.h"
#include "../HelperFunc.h"
#include "../common.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
#include "cocos2d.h"

#define MAX_ACTION_DIRECTION	8
#define		DEFAULT_FPS	8
using namespace rapidjson;

namespace Tool{	
	typedef std::vector<std::string> vec_str;
	typedef rapidjson::Value	RJSValue;

	static CustomAnimation* s_CustomAnimation = nullptr;

	CustomAnimation* CustomAnimation::getInstance()
	{
		if (!s_CustomAnimation)
		{
			s_CustomAnimation = new (std::nothrow) CustomAnimation();
			CCASSERT(s_CustomAnimation, "FATAL: Not enough memory");
			s_CustomAnimation->init();
		}

		return s_CustomAnimation;
	}

	void CustomAnimation::init()
	{
		_jsonPath = FileUtils::getInstance()->getDefaultResourceRootPath() + "res/custom/";
	}

	bool CustomAnimation::addPlist(const std::string& roleName)
	{
		const auto& iter = _plistMap.find(roleName);
		if (iter != _plistMap.end())
		{
			SpriteFrameCache* pCache = SpriteFrameCache::getInstance();
			const auto& plists = _plistMap[roleName];
			for (int i = 0; i < plists.size(); ++i)
			{
				pCache->addSpriteFramesWithFile(_jsonPath + plists[i]);
			}

			return true;
		}

		return false;
	}
	Animate* CustomAnimation::getAnimate(const std::string& roleName, const std::string& actionName, int direction, float fps)
	{
		if (roleName.empty() || actionName.empty() || direction < 1 || direction > MAX_ACTION_DIRECTION)
		{
			return NULL;
		}
		const auto& iter = _roleActionMap.find(roleName);
		if (iter == _roleActionMap.end())
		{
			initRole(roleName);
		}
		auto roleAcions = _roleActionMap[roleName];
		
		if (addPlist(roleName))
		{
			SpriteFrameCache* pCache = SpriteFrameCache::getInstance();
			const auto& imgVec = roleAcions[actionName][actionName + StringUtils::format("%d", direction)];
			
			Vector<AnimationFrame*> animationFrames;
			float fRate = fps / DEFAULT_FPS;
			for (int n = 0; n < imgVec.size(); ++n)
			{
				const FrameImgTime& temp = imgVec[n];
				AnimationFrame* frame = AnimationFrame::create(pCache->getSpriteFrameByName(temp.name), temp.delay / fRate);
				animationFrames.pushBack(frame);
			}
			auto ani = Animation::create(animationFrames);
			ani->setRestoreOriginalFrame(true);
			//ani->setDelayPerUnit(delayPerUnit);

			return Animate::create(ani);
		}
		else
		{
			return NULL;
		}
	}

	vec_str CustomAnimation::getActions(const std::string& roleName)
	{
		const auto& iter = _roleActionMap.find(roleName);
		
		if (iter == _roleActionMap.end())
		{
			initRole(roleName);
		}
		vec_str ret;
		for (const auto& actIter : _roleActionMap[roleName])
		{
			ret.push_back(actIter.first);
		}

		return ret;
	}
	void CustomAnimation::initRole(const std::string& roleName)
	{
		std::string fileData = FileUtils::getInstance()->getStringFromFile(_jsonPath + roleName + ".json");
			
		Document doc;
		doc.Parse<0>(fileData.c_str());
		if (doc.HasParseError())
		{
			CCLOG("parse error : %s\n", doc.GetParseError());
		}
		else
		{
			Actions_map actions;
			const auto& animations = doc[jsonAni];

			for (size_t i = 0; i < animations.Capacity(); i++)
			{
				const auto& subActs = animations[i][jsonSubActs];
				std::unordered_map<std::string, std::vector<FrameImgTime>> actVec;

				std::string actName = animations[i][jsonActName].GetString();
				
				for (auto iter = subActs.MemberBegin(); iter != subActs.MemberEnd(); ++iter)
				{
					auto key = iter->name.GetString();
					auto& arr = subActs[key];
					std::vector<FrameImgTime> imgs;
					for (size_t j = 0; j < arr.Capacity(); ++j)
					{
						FrameImgTime temp;
						temp.name = arr[j][jsonImgName].GetString();
						temp.delay = arr[j][jsonDelayTimes].GetDouble();
						imgs.push_back(temp);
					}
					actVec[key] = imgs;
				}

				actions[actName] = actVec;
			}

			_roleActionMap[roleName] = actions;

			//cache plist name
			const auto& plist = doc[jsonPlistPath];
			vec_str name_vec;
			for (int i = 0; i < plist.Capacity();++i)
			{
				name_vec.push_back(plist[i].GetString());
			}
			_plistMap[roleName] = name_vec;
		}
	}

	//==============   export ===============
	void CustomAnimation::exportToJson(const std::string& resPath, const std::string& roleName, const std::string& plistPath, const std::string& savePath, int frameRate)
	{
		std::string res = resPath + roleName + "/" + roleName;
		Document doc;
		doc.SetObject();
		Document::AllocatorType& alloc = doc.GetAllocator();

		vec_str actions = Tool::HelperFunc::GetSubDirFiles(res);
		//animations 
		RJSValue array(kArrayType);
		for (int i = 0; i < actions.size(); ++i)	//run, idle, run actions
		{
			RJSValue aniObj(kObjectType);
			//actName
			aniObj.AddMember(RJSValue(jsonActName, alloc), RJSValue().SetString(actions[i].c_str(), alloc), alloc);
			//subActions -- 8 directions
			RJSValue subAniArr(kObjectType);
			std::string subPath(res);
			subPath += "/" + actions[i];

			std::vector<std::string> subActions = Tool::HelperFunc::GetSubDirFiles(subPath);
			for (int j = 0; j < subActions.size(); ++j)
			{
				//std::string direction = cocos2d::StringUtils::format("/%s/", subActions[j]);
				std::string direction = "/" + subActions[j] + "/";
				vec_str imgs = Tool::HelperFunc::TraverseFloder(subPath + direction);
				RJSValue imgArr(kArrayType);
				for (int n = 0; n < imgs.size(); ++n)//frames
				{
					//format: role_name/action_name/direction/img_name     eg:char01/run/1/1.png
					RJSValue imgName((roleName + "/" + actions[i] + direction + imgs[n]).c_str(), alloc);
					//¶¯»­Êý¾Ý {imageName:xxx, delayTimes:n}
					RJSValue data(kObjectType);
					data.AddMember(RJSValue(jsonImgName, alloc), imgName, alloc);
					//add delay times
					data.AddMember(RJSValue(jsonDelayTimes, alloc), 1.0f / frameRate, alloc);
					imgArr.PushBack(data, alloc);
				}
				subAniArr.AddMember(RJSValue((actions[i] + subActions[j]).c_str(), alloc), imgArr, alloc);
			}
			aniObj.AddMember(RJSValue(jsonSubActs, alloc), subAniArr, alloc);

			array.PushBack(aniObj, alloc);
		}

		doc.AddMember(RJSValue(jsonAni, alloc), array, alloc);
		//add plist
		RJSValue plist(kArrayType);
		vec_str files = Tool::HelperFunc::TraverseFloder(plistPath, cocos2d::StringUtils::format("/*%s-?.plist", roleName.c_str()));
		for (int i = 0; i < files.size(); ++i)
		{
			plist.PushBack(RJSValue(files[i].c_str(), alloc), alloc);
		}
		doc.AddMember(RJSValue(jsonPlistPath, alloc), plist, alloc);

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		doc.Accept(writer);

		std::ofstream os;
		CCLOG("json output path = %s", (savePath +  roleName).c_str());
		os.open(savePath + roleName + ".json");
		os << buffer.GetString();
		os.close();
	}
}
