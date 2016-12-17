/*
 * Copyright (c) 2014-2015, dennis wang
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NAVI_H
#define NAVI_H

#include <cstdint>
#include <cmath>
#include <sstream>

namespace knavi {

struct NaviPoint3D;
class Navi;
class NaviFactory;
class NaviAgent;
class NaviAgentHandler;

/**
 * 浮点数是否为零
 * @retval true 是
 * @retval false 不是
 */
inline bool zero(float a) {
    static const float EPSINON = 0.000001f;
    return ((a >= -EPSINON) && (a <= EPSINON));
}

/**
 * 导航网格内坐标
 */
struct NaviPoint3D {
    float x;  ///< x坐标
    float y;  ///< y坐标
    float z;  ///< z坐标

    NaviPoint3D()
    : x(.0f),
      y(.0f),
      z(.0f) {
    }

    NaviPoint3D(float f3[]) {
        x = f3[0];
        y = f3[1];
        z = f3[2];
    }

    NaviPoint3D(float a, float b, float c)
    : x(a),
      y(b),
      z(c) {
    }

    NaviPoint3D(const NaviPoint3D& rht) {
        x = rht.x;
        y = rht.y;
        z = rht.z;
    }

    const NaviPoint3D& operator=(const NaviPoint3D& rht) {
        x = rht.x;
        y = rht.y;
        z = rht.z;
        return *this;
    }

    /**
     * 长度
     * @return 长度
     */
    float getLength() const {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    /**
     * 是否为零向量
     * @retval true 是
     * @retval false 否
     */
    bool isZero() const {
        return (zero(x) && zero(y) && zero(z));
    }

    /**
     * 可读字符串
     * @return 字符串
     */
    std::string toString() const {
        std::stringstream ss;
        ss << "x:" << x << ",y:" << y << ",z:" << z;
        return ss.str();
    }
};

inline float distance(const NaviPoint3D& a, const NaviPoint3D& b) {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    const float dz = a.z - b.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

/**
 * Agent回调
 */
class NaviAgentHandler {
public:
    virtual ~NaviAgentHandler() {}

    /**
     * 每轮Navi::update执行都会被回调
     * @param agent Agent
     */
    virtual void onUpdate(knavi::NaviAgent* agent) = 0;

    /**
     * Agent被销毁前调用
     * @param agent Agent
     */
    virtual void onDestroy(knavi::NaviAgent* agent) = 0;
};

/**
 * 导航网格内的Agent
 */
class NaviAgent {
public:
    /**
     * 虚析构
     */
    virtual ~NaviAgent() {}

    /**
     * 启动Agent
     * @retval true 成功
     * @retval false 失败
     */
    virtual bool start() = 0;

    /**
     * 停止行走
     */
    virtual void stopWalk() = 0;

    /**
     * Agent是否停止移动
     * @retval true 是
     * @retval false 否
     */
    virtual bool isStopped() = 0;

    /**
     * 设置胶囊体半径
     * @param radius 胶囊体半径
     */
    virtual void setRadius(float radius) = 0;

    /**
     * 获取胶囊体半径
     * @return 胶囊体半径
     */
    virtual float getRadius() = 0;

    /**
     * 设置Agent高度
     * @param height 高度
     */
    virtual void setHeight(float height) = 0;

    /**
     * 获取Agent高度
     * @return 高度
     */
    virtual float getHeight() = 0;

    /**
     * 设置最大加速度(米/秒)
     * @param maxAcceleration 最大加速度
     */
    virtual void setMaxAcceleration(float maxAcceleration) = 0;

    /**
     * 获取最大加速度(米/秒)
     * @return 最大加速度
     */
    virtual float getMaxAcceleration() = 0;

    /**
     * 设置最大移动速度(米/秒)
     * @param maxSpeed 最大移动速度
     */
    virtual void setMaxSpeed(float maxSpeed) = 0;

    /**
     * 获取最大移动速度(米/秒)
     * @return 最大移动速度
     */
    virtual float getMaxSpeed() = 0;

    /**
     * 设置移动速度向量
     * @param velocity 移动向量
     */
    virtual void setVelocity(const NaviPoint3D& velocity) = 0;

    /**
     * 获取移动速度向量
     * @return 移动向量
     */
    virtual const NaviPoint3D& getVelocity() = 0;

    /**
     * 设置用户数据
     * @param userData 用户数据
     */
    virtual void setUserData(void* userData) = 0;

    /**
     * 设置动态碰撞检测的范围
     * @param collisionQueryRange 动态碰撞检测的范围
     */
    virtual void setCollisionQueryRange(float collisionQueryRange) = 0;

    /**
     * 获取动态碰撞检测的范围
     * @return 动态碰撞检测的范围
     */
    virtual float getCollisionQueryRange() = 0;

    /**
     * 设置路径优化范围
     * @param pathOptimizationRange 路径优化范围
     */
    virtual void setPathOptimizationRange(float pathOptimizationRange) = 0;

    /**
    * 获取路径优化范围
    * @return 路径优化范围
    */
    virtual float getPathOptimizationRange() = 0;

    /**
     * 设置Agent站位权重
     * @param separationWeight Agent站位权重
     */
    virtual void setSeparationWeight(float separationWeight) = 0;

    /**
     * 设置Agent更新行为标志
     * @param updateFlags 更新行为标志
     */
    virtual void setUpdateFlags(uint8_t updateFlags) = 0;

    /**
    * 获取Agent更新行为标志
    * @return 更新行为标志
    */
    virtual uint8_t getUpdateFlags() = 0;

    /**
     * 获取Agent站位权重
     * @return Agent站位权重
     */
    virtual float getSeparationWeight() = 0;

    /**
     * 获取用户数据
     * @return 用户数据
     */
    virtual void* getUserData() = 0;

    /**
     * 设置寻路目标点
     * @param moveTarget 目标
     */
    virtual void setMoveTarget(const NaviPoint3D& moveTarget) = 0;

    /**
     * 获取寻路目标点
     * @return 目标
     */
    virtual const NaviPoint3D& getMoveTarget() = 0;

    /**
     * 获取当前位置坐标
     * @return 当前位置坐标
     */
    virtual const NaviPoint3D& getPosition() = 0;

	/**
	* 更改当前坐标，内部坐标也会发生变化
	* @param pos 坐标
	*/
	virtual void changePosition(const NaviPoint3D& pos)=0;

	/**
	* 设置碰撞等级
	* @param level 0: 不会碰撞；默认1，越大优先级越高
	*/
	virtual void setCollideLevel(int level)=0;

    /**
     * 获取当前速度向量
     * @return 当前速度向量
     */
    virtual const NaviPoint3D& getSpeed() = 0;

    /**
     * 设置回调处理器
     * @param handler 回调处理器
     */
    virtual void setHandler(NaviAgentHandler* handler) = 0;

    /**
     * 获取回调处理器
     * @return 回调处理器
     */
    virtual NaviAgentHandler* getHandler() = 0;
};

/**
 * 导航网格接口
 */
class Navi {
public:
    /**
     * 析构
     */
    virtual ~Navi() {}

    /**
     * 加载OBJ文件
     * @param filePath 文件绝对路径
     * @retval true 成功
     * @retval false 失败
     */
    virtual bool load(const char* filePath) = 0;

    /**
     * 导航网格地图内Agent行走更新
     * @param ms 更新频率(毫秒/帧)
     */
    virtual void update(uint32_t ms) = 0;

    /**
     * 建立Agent并将Agent加入网格地图
     * @param radius Agent胶囊体半径
     * @param position Agent所在位置坐标
     * @return NaviAgent实例
     */
    virtual NaviAgent* addAgent(float radius, const NaviPoint3D& position) = 0;
    
    /**
     * 销毁Agent并将Agent从网格地图内移除
     * @param agent Agent
     */
    virtual void removeAgent(NaviAgent* agent) = 0;

    /**
     * 寻路
     * @param start 起始坐标
     * @param end 目的坐标
     * @param path 路径点数组
     * @param size 数组长度, 返回实际长度
     * @retval true 成功
     * @retval false 失败
     */
    virtual bool find(const NaviPoint3D& start, const NaviPoint3D& end, NaviPoint3D* path, int* size) = 0;

    /**
     * 射线线段
     * @param start 起始坐标
     * @param end 目的坐标
     * @param hitPoint 射线线段实际终点（可能与障碍物相交）
     * @retval true 成功
     * @retval false 失败
     */
    virtual bool raycast(const NaviPoint3D& start, const NaviPoint3D& end, NaviPoint3D& hitPoint) = 0;

    /**
     * 获取错误
     * @return 错误原因
     */
    virtual const char* getError() = 0;
};

/**
 * 导航网格工厂
 */
class NaviFactory {
public:
    /**
     * 获取工厂单件
     * @return NaviFactory实例
     */
    static NaviFactory* instance();

    /**
     * 销毁工厂单件
     */
    static void destroy();

    /**
     * 析构
     */
    ~NaviFactory();

    /**
     * 建立导航网格实例
     * @param maxAgent Agent最大数量
     * @return Navi接口
     */
    Navi* createNavi(int maxAgent = 25);

    /**
     * 销毁导航网格实例
     * @param navi Navi接口
     */
    void destroyNavi(Navi* navi);

private:
    NaviFactory();
    NaviFactory(const NaviFactory&);
    static NaviFactory* _instance; ///< 单件
};

/*! 获取导航网格工厂单件帮助宏 */
#define NaviFactoryPtr knavi::NaviFactory::instance()

}

#endif // NAVI_H
