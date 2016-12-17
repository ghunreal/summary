//
// Generated code, PLEASE DO NOT MODIFY
//
#ifndef DataPool_1465182952
#define DataPool_1465182952

#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include <string>

#include "csv_helper.hpp"
#include "cocos2d.h"

namespace CsvCoder {

class buildingsLoader;

class buildings {
    friend class buildingsLoader;
    friend class buildingsLoaderNonSingleton;

    int32_t TID; ///< TID
    std::string name; ///< 中文名称
    int32_t SightRange; ///< 视力范围
    int32_t DeployTime; ///< 部署时间
    int32_t Speed; ///< 移动速度
    int32_t Hitpoint32s; ///< 生命
    int32_t HitSpeed; ///< 攻速
    int32_t LoadTime; ///< 部署时间
    int32_t Damage; ///< 伤害
    std::string CardIcon; ///< 资源图片
    int32_t LifeTime; ///< 存在时间（建筑用）
    int32_t Scale; ///< 比例尺
    int32_t CollisionRadius; ///< 碰撞半径
    int32_t Mass; ///< 碰撞权重

public:
    buildings() {}
    ~buildings() {}

public:
    inline int32_t getTID() const {
        return TID;
    }

    inline const std::string& getname() const {
        return name;
    }

    inline int32_t getSightRange() const {
        return SightRange;
    }

    inline int32_t getDeployTime() const {
        return DeployTime;
    }

    inline int32_t getSpeed() const {
        return Speed;
    }

    inline int32_t getHitpoint32s() const {
        return Hitpoint32s;
    }

    inline int32_t getHitSpeed() const {
        return HitSpeed;
    }

    inline int32_t getLoadTime() const {
        return LoadTime;
    }

    inline int32_t getDamage() const {
        return Damage;
    }

    inline const std::string& getCardIcon() const {
        return CardIcon;
    }

    inline int32_t getLifeTime() const {
        return LifeTime;
    }

    inline int32_t getScale() const {
        return Scale;
    }

    inline int32_t getCollisionRadius() const {
        return CollisionRadius;
    }

    inline int32_t getMass() const {
        return Mass;
    }

private:
    inline void setTID(int32_t v) {
        TID = v;
    }
    inline void setname(const std::string& v) {
        name = v;
    }

    inline void setSightRange(int32_t v) {
        SightRange = v;
    }
    inline void setDeployTime(int32_t v) {
        DeployTime = v;
    }
    inline void setSpeed(int32_t v) {
        Speed = v;
    }
    inline void setHitpoint32s(int32_t v) {
        Hitpoint32s = v;
    }
    inline void setHitSpeed(int32_t v) {
        HitSpeed = v;
    }
    inline void setLoadTime(int32_t v) {
        LoadTime = v;
    }
    inline void setDamage(int32_t v) {
        Damage = v;
    }
    inline void setCardIcon(const std::string& v) {
        CardIcon = v;
    }

    inline void setLifeTime(int32_t v) {
        LifeTime = v;
    }
    inline void setScale(int32_t v) {
        Scale = v;
    }
    inline void setCollisionRadius(int32_t v) {
        CollisionRadius = v;
    }
    inline void setMass(int32_t v) {
        Mass = v;
    }
};

class buildingsLoader : public Singleton<buildingsLoader> {
    friend class Singleton<buildingsLoader>;
    std::map<int32_t, buildings*> _indixes0;
    std::vector<buildings*> _array;
public:
    bool load(const std::string& path) {
        std::string data = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
        if (data.size() == 0) {
            return false;
        }
        int index = 0;
        while ((index = data.find("\r\n", index)) != std::string::npos)
        {
            data.erase(data.begin() + index);
        }
        std::string line;
        std::stringstream ss;
        ss << data;
        for (int i = 0; (i < 3) && getline(ss, line); i++) {}
        for (int i = 0; getline(ss, line); i++) {
            std::vector<std::string> strs;
            csv::split(line, ",", strs);
            buildings* o = new buildings();
            int32_t index0 = atoi(strs[0].c_str());
            o->setTID(atoi(strs[0].c_str()));
            o->setname(strs[1].c_str());
            o->setSightRange(atoi(strs[2].c_str()));
            o->setDeployTime(atoi(strs[3].c_str()));
            o->setSpeed(atoi(strs[4].c_str()));
            o->setHitpoint32s(atoi(strs[5].c_str()));
            o->setHitSpeed(atoi(strs[6].c_str()));
            o->setLoadTime(atoi(strs[7].c_str()));
            o->setDamage(atoi(strs[8].c_str()));
            o->setCardIcon(strs[9].c_str());
            o->setLifeTime(atoi(strs[10].c_str()));
            o->setScale(atoi(strs[11].c_str()));
            o->setCollisionRadius(atoi(strs[12].c_str()));
            o->setMass(atoi(strs[13].c_str()));
            _indixes0.insert(std::make_pair(index0, o));
            _array.push_back(o);
        }
        return true;
    }

    inline const buildings& getByTID(int32_t i) {
        return *_indixes0.at(i);
    }

    inline bool isValidTID(int32_t i) {
        return (_indixes0.find(i) != _indixes0.end());
    }
    inline int size() {
        return (int)_array.size();
    }

    inline const buildings& operator[](int i) {
        return *_array.at(i);
    }

private:
    buildingsLoader() {}
    ~buildingsLoader() {
        for (int i = 0; i < size(); i++) {
            delete _array[i];
        }
    }
};

#define buildingsLoaderRef (*CsvCoder::buildingsLoader::instance())


class buildingsLoaderNonSingleton {
public:
    buildingsLoaderNonSingleton() {}
    ~buildingsLoaderNonSingleton() {
        for (int i = 0; i < size(); i++) {
            delete _array[i];
        }
    }
private:
    std::map<int32_t, buildings*> _indixes0;
    std::vector<buildings*> _array;
public:
    bool load(const std::string& path) {
        std::string data = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
        if (data.size() == 0) {
            return false;
        }
        int index = 0;
        while ((index = data.find("\r\n", index)) != std::string::npos)
        {
            data.erase(data.begin() + index);
        }
        std::string line;
        std::stringstream ss;
        ss << data;
        for (int i = 0; (i < 3) && getline(ss, line); i++) {}
        for (int i = 0; getline(ss, line); i++) {
            std::vector<std::string> strs;
            csv::split(line, ",", strs);
            buildings* o = new buildings();
            int32_t index0 = atoi(strs[0].c_str());
            o->setTID(atoi(strs[0].c_str()));
            o->setname(strs[1].c_str());
            o->setSightRange(atoi(strs[2].c_str()));
            o->setDeployTime(atoi(strs[3].c_str()));
            o->setSpeed(atoi(strs[4].c_str()));
            o->setHitpoint32s(atoi(strs[5].c_str()));
            o->setHitSpeed(atoi(strs[6].c_str()));
            o->setLoadTime(atoi(strs[7].c_str()));
            o->setDamage(atoi(strs[8].c_str()));
            o->setCardIcon(strs[9].c_str());
            o->setLifeTime(atoi(strs[10].c_str()));
            o->setScale(atoi(strs[11].c_str()));
            o->setCollisionRadius(atoi(strs[12].c_str()));
            o->setMass(atoi(strs[13].c_str()));
            _indixes0.insert(std::make_pair(index0, o));
            _array.push_back(o);
        }
        return true;
    }

    inline const buildings& getByTID(int32_t i) {
        return *_indixes0.at(i);
    }

    inline bool isValidTID(int32_t i) {
        return (_indixes0.find(i) != _indixes0.end());
    }
    inline int size() {
        return (int)_array.size();
    }

    inline const buildings& operator[](int i) {
        return *_array.at(i);
    }

};

class CardHeroLoader;

class CardHero {
    friend class CardHeroLoader;
    friend class CardHeroLoaderNonSingleton;

    int32_t TID; ///< TID
    std::string Name; ///< 中文名称
    int32_t SightRange; ///< 视力范围
    int32_t DeployTime; ///< 部署时间
    int32_t Speed; ///< 移动速度
    int32_t HP; ///< 生命
    int32_t HitSpeed; ///< 攻速
    int32_t LoadTime; ///< 部署时间
    int32_t TargetOnlyBuildings; ///< 只攻击建筑物
    int32_t AttacksGround; ///< 攻击地面
    int32_t AttacksAir; ///< 攻击空中
    int32_t Damage; ///< 伤害
    int32_t SightClip; ///< 搜索视野内建筑范围
    int32_t SightClipSide; ///< 视角
    int32_t IgnorePushback; ///< 忽略推回
    int32_t Scale; ///< 比例尺
    int32_t CollisionRadius; ///< 碰撞半径
    int32_t Mass; ///< 碰撞权重
    int32_t Skill1; ///< 主动技能1
    int32_t Skill2; ///< 主动技能2
    int32_t Passive; ///< 被动技能
    std::string AnimationBlue; ///< 蓝方动作资源
    std::string AnimationRed; ///< 红方人物动作资源
    std::string CardIcon; ///< 战斗中卡牌显示icon
    int32_t Consume; ///< 紫水消耗
    int32_t GridsNum; ///< 占用的格子

public:
    CardHero() {}
    ~CardHero() {}

public:
    inline int32_t getTID() const {
        return TID;
    }

    inline const std::string& getName() const {
        return Name;
    }

    inline int32_t getSightRange() const {
        return SightRange;
    }

    inline int32_t getDeployTime() const {
        return DeployTime;
    }

    inline int32_t getSpeed() const {
        return Speed;
    }

    inline int32_t getHP() const {
        return HP;
    }

    inline int32_t getHitSpeed() const {
        return HitSpeed;
    }

    inline int32_t getLoadTime() const {
        return LoadTime;
    }

    inline int32_t getTargetOnlyBuildings() const {
        return TargetOnlyBuildings;
    }

    inline int32_t getAttacksGround() const {
        return AttacksGround;
    }

    inline int32_t getAttacksAir() const {
        return AttacksAir;
    }

    inline int32_t getDamage() const {
        return Damage;
    }

    inline int32_t getSightClip() const {
        return SightClip;
    }

    inline int32_t getSightClipSide() const {
        return SightClipSide;
    }

    inline int32_t getIgnorePushback() const {
        return IgnorePushback;
    }

    inline int32_t getScale() const {
        return Scale;
    }

    inline int32_t getCollisionRadius() const {
        return CollisionRadius;
    }

    inline int32_t getMass() const {
        return Mass;
    }

    inline int32_t getSkill1() const {
        return Skill1;
    }

    inline int32_t getSkill2() const {
        return Skill2;
    }

    inline int32_t getPassive() const {
        return Passive;
    }

    inline const std::string& getAnimationBlue() const {
        return AnimationBlue;
    }

    inline const std::string& getAnimationRed() const {
        return AnimationRed;
    }

    inline const std::string& getCardIcon() const {
        return CardIcon;
    }

    inline int32_t getConsume() const {
        return Consume;
    }

    inline int32_t getGridsNum() const {
        return GridsNum;
    }

private:
    inline void setTID(int32_t v) {
        TID = v;
    }
    inline void setName(const std::string& v) {
        Name = v;
    }

    inline void setSightRange(int32_t v) {
        SightRange = v;
    }
    inline void setDeployTime(int32_t v) {
        DeployTime = v;
    }
    inline void setSpeed(int32_t v) {
        Speed = v;
    }
    inline void setHP(int32_t v) {
        HP = v;
    }
    inline void setHitSpeed(int32_t v) {
        HitSpeed = v;
    }
    inline void setLoadTime(int32_t v) {
        LoadTime = v;
    }
    inline void setTargetOnlyBuildings(int32_t v) {
        TargetOnlyBuildings = v;
    }
    inline void setAttacksGround(int32_t v) {
        AttacksGround = v;
    }
    inline void setAttacksAir(int32_t v) {
        AttacksAir = v;
    }
    inline void setDamage(int32_t v) {
        Damage = v;
    }
    inline void setSightClip(int32_t v) {
        SightClip = v;
    }
    inline void setSightClipSide(int32_t v) {
        SightClipSide = v;
    }
    inline void setIgnorePushback(int32_t v) {
        IgnorePushback = v;
    }
    inline void setScale(int32_t v) {
        Scale = v;
    }
    inline void setCollisionRadius(int32_t v) {
        CollisionRadius = v;
    }
    inline void setMass(int32_t v) {
        Mass = v;
    }
    inline void setSkill1(int32_t v) {
        Skill1 = v;
    }
    inline void setSkill2(int32_t v) {
        Skill2 = v;
    }
    inline void setPassive(int32_t v) {
        Passive = v;
    }
    inline void setAnimationBlue(const std::string& v) {
        AnimationBlue = v;
    }

    inline void setAnimationRed(const std::string& v) {
        AnimationRed = v;
    }

    inline void setCardIcon(const std::string& v) {
        CardIcon = v;
    }

    inline void setConsume(int32_t v) {
        Consume = v;
    }
    inline void setGridsNum(int32_t v) {
        GridsNum = v;
    }
};

class CardHeroLoader : public Singleton<CardHeroLoader> {
    friend class Singleton<CardHeroLoader>;
    std::map<int32_t, CardHero*> _indixes0;
    std::vector<CardHero*> _array;
public:
    bool load(const std::string& path) {
        std::string data = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
        if (data.size() == 0) {
            return false;
        }
        int index = 0;
        while ((index = data.find("\r\n", index)) != std::string::npos)
        {
            data.erase(data.begin() + index);
        }
        std::string line;
        std::stringstream ss;
        ss << data;
        for (int i = 0; (i < 3) && getline(ss, line); i++) {}
        for (int i = 0; getline(ss, line); i++) {
            std::vector<std::string> strs;
            csv::split(line, ",", strs);
            CardHero* o = new CardHero();
            int32_t index0 = atoi(strs[0].c_str());
            o->setTID(atoi(strs[0].c_str()));
            o->setName(strs[1].c_str());
            o->setSightRange(atoi(strs[2].c_str()));
            o->setDeployTime(atoi(strs[3].c_str()));
            o->setSpeed(atoi(strs[4].c_str()));
            o->setHP(atoi(strs[5].c_str()));
            o->setHitSpeed(atoi(strs[6].c_str()));
            o->setLoadTime(atoi(strs[7].c_str()));
            o->setTargetOnlyBuildings(atoi(strs[8].c_str()));
            o->setAttacksGround(atoi(strs[9].c_str()));
            o->setAttacksAir(atoi(strs[10].c_str()));
            o->setDamage(atoi(strs[11].c_str()));
            o->setSightClip(atoi(strs[12].c_str()));
            o->setSightClipSide(atoi(strs[13].c_str()));
            o->setIgnorePushback(atoi(strs[14].c_str()));
            o->setScale(atoi(strs[15].c_str()));
            o->setCollisionRadius(atoi(strs[16].c_str()));
            o->setMass(atoi(strs[17].c_str()));
            o->setSkill1(atoi(strs[18].c_str()));
            o->setSkill2(atoi(strs[19].c_str()));
            o->setPassive(atoi(strs[20].c_str()));
            o->setAnimationBlue(strs[21].c_str());
            o->setAnimationRed(strs[22].c_str());
            o->setCardIcon(strs[23].c_str());
            o->setConsume(atoi(strs[24].c_str()));
            o->setGridsNum(atoi(strs[25].c_str()));
            _indixes0.insert(std::make_pair(index0, o));
            _array.push_back(o);
        }
        return true;
    }

    inline const CardHero& getByTID(int32_t i) {
        return *_indixes0.at(i);
    }

    inline bool isValidTID(int32_t i) {
        return (_indixes0.find(i) != _indixes0.end());
    }
    inline int size() {
        return (int)_array.size();
    }

    inline const CardHero& operator[](int i) {
        return *_array.at(i);
    }

private:
    CardHeroLoader() {}
    ~CardHeroLoader() {
        for (int i = 0; i < size(); i++) {
            delete _array[i];
        }
    }
};

#define CardHeroLoaderRef (*CsvCoder::CardHeroLoader::instance())


class CardHeroLoaderNonSingleton {
public:
    CardHeroLoaderNonSingleton() {}
    ~CardHeroLoaderNonSingleton() {
        for (int i = 0; i < size(); i++) {
            delete _array[i];
        }
    }
private:
    std::map<int32_t, CardHero*> _indixes0;
    std::vector<CardHero*> _array;
public:
    bool load(const std::string& path) {
        std::string data = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
        if (data.size() == 0) {
            return false;
        }
        int index = 0;
        while ((index = data.find("\r\n", index)) != std::string::npos)
        {
            data.erase(data.begin() + index);
        }
        std::string line;
        std::stringstream ss;
        ss << data;
        for (int i = 0; (i < 3) && getline(ss, line); i++) {}
        for (int i = 0; getline(ss, line); i++) {
            std::vector<std::string> strs;
            csv::split(line, ",", strs);
            CardHero* o = new CardHero();
            int32_t index0 = atoi(strs[0].c_str());
            o->setTID(atoi(strs[0].c_str()));
            o->setName(strs[1].c_str());
            o->setSightRange(atoi(strs[2].c_str()));
            o->setDeployTime(atoi(strs[3].c_str()));
            o->setSpeed(atoi(strs[4].c_str()));
            o->setHP(atoi(strs[5].c_str()));
            o->setHitSpeed(atoi(strs[6].c_str()));
            o->setLoadTime(atoi(strs[7].c_str()));
            o->setTargetOnlyBuildings(atoi(strs[8].c_str()));
            o->setAttacksGround(atoi(strs[9].c_str()));
            o->setAttacksAir(atoi(strs[10].c_str()));
            o->setDamage(atoi(strs[11].c_str()));
            o->setSightClip(atoi(strs[12].c_str()));
            o->setSightClipSide(atoi(strs[13].c_str()));
            o->setIgnorePushback(atoi(strs[14].c_str()));
            o->setScale(atoi(strs[15].c_str()));
            o->setCollisionRadius(atoi(strs[16].c_str()));
            o->setMass(atoi(strs[17].c_str()));
            o->setSkill1(atoi(strs[18].c_str()));
            o->setSkill2(atoi(strs[19].c_str()));
            o->setPassive(atoi(strs[20].c_str()));
            o->setAnimationBlue(strs[21].c_str());
            o->setAnimationRed(strs[22].c_str());
            o->setCardIcon(strs[23].c_str());
            o->setConsume(atoi(strs[24].c_str()));
            o->setGridsNum(atoi(strs[25].c_str()));
            _indixes0.insert(std::make_pair(index0, o));
            _array.push_back(o);
        }
        return true;
    }

    inline const CardHero& getByTID(int32_t i) {
        return *_indixes0.at(i);
    }

    inline bool isValidTID(int32_t i) {
        return (_indixes0.find(i) != _indixes0.end());
    }
    inline int size() {
        return (int)_array.size();
    }

    inline const CardHero& operator[](int i) {
        return *_array.at(i);
    }

};

class DataPool : public Singleton<DataPool> {
    friend class Singleton<DataPool>;
public:
    bool loadAll(const std::string& path) {
         if (path.empty()) {
             return false;
         }
        _path = csv::completePath(path);
        if (!buildingsLoaderRef.load(_path + "buildings.csv")) {
            _error = "load cvs file: buildings.csv failed";
            return false;
        }
        if (!CardHeroLoaderRef.load(_path + "CardHero.csv")) {
            _error = "load cvs file: CardHero.csv failed";
            return false;
        }
        return true;
    }

    void unloadAll() {
        buildingsLoader::destroy();
        CardHeroLoader::destroy();
    }

    void reloadAll() {
        unloadAll();
        loadAll(_path);
    }

    const std::string& getError() {
        return _error;
    }

private:
    DataPool() {}
    ~DataPool() {
        unloadAll();
    }
    std::string _path;
    std::string _error;
};

#define DataPoolRef (*CsvCoder::DataPool::instance())

} // namespace CsvCoder

#endif // DataPool_1465182952
