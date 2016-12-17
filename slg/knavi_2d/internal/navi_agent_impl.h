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

#ifndef NAVI_AGENT_IMPL_H
#define NAVI_AGENT_IMPL_H

#include <DetourCrowd.h>
#include "navi.h"

namespace knavi {

class NaviImpl;

/**
 * Agent实现
 */
class NaviAgentImpl : public NaviAgent {
public:
    NaviAgentImpl(float radius, NaviImpl* navi);
    virtual ~NaviAgentImpl();
    virtual bool start();
    virtual void stopWalk();
    virtual bool isStopped();
    virtual void setRadius(float radius);
    virtual float getRadius();
    virtual void setHeight(float height);
    virtual float getHeight();
    virtual void setMaxAcceleration(float maxAcceleration);
    virtual float getMaxAcceleration();
    virtual void setMaxSpeed(float maxSpeed);
    virtual float getMaxSpeed();
    virtual void setCollisionQueryRange(float collisionQueryRange);
    virtual float getCollisionQueryRange();
    virtual void setPathOptimizationRange(float pathOptimizationRange);
    virtual float getPathOptimizationRange();
    virtual void setSeparationWeight(float separationWeight);
    virtual float getSeparationWeight();
    virtual void setUpdateFlags(uint8_t updateFlags);
    virtual uint8_t getUpdateFlags();
    virtual void setObstacleAvoidanceType(uint8_t obstacleAvoidanceType);
    virtual uint8_t getObstacleAvoidanceType();
    virtual void setQueryFilterType(uint8_t queryFilterType);
    virtual uint8_t getQueryFilterType();
    virtual void setUserData(void* userData);
    virtual void* getUserData();
    virtual void setHandler(NaviAgentHandler* handler);
    virtual NaviAgentHandler* getHandler();
    virtual void setVelocity(const NaviPoint3D& velocity);
    virtual const NaviPoint3D& getVelocity();
    virtual void setMoveTarget(const NaviPoint3D& moveTarget);
    virtual const NaviPoint3D& getMoveTarget();
    virtual const NaviPoint3D& getPosition();
    virtual const NaviPoint3D& getSpeed();

	virtual void changePosition(const NaviPoint3D& pos);
	virtual void setCollideLevel(int level);

private:
    /**
     * 重置
     */
    void reset();

    /**
     * 设置在dtCrowd内的索引
     * @param index 索引
     */
    inline void setIndex(int index) {
        _idx = index;
    }

    /**
     * 获取在dtCrowd内的索引
     * @return 索引
     */
    inline int getIndex() {
        return _idx;
    }

    /**
     * 设置当前位置坐标
     * @param position 位置坐标
     */
    inline void setPosition(const NaviPoint3D& position) {
        _position = position;
    }
	
    /**
     * 设置当前速度
     * @param speed 速度
     */
    inline void setSpeed(const NaviPoint3D& speed) {
        _speed = speed;
    }

    /**
     * 重启寻路
     */
    void move();

    friend class NaviImpl;

private:
    float              _radius;                ///< Agent radius. [Limit: >= 0]
    float              _height;                ///< Agent height. [Limit: > 0]
    float              _maxAcceleration;       ///< Agent height. [Limit: > 0]
    float              _maxSpeed;              ///< Maximum allowed speed. [Limit: >= 0]
    float              _collisionQueryRange;   ///< Defines how close a collision element must be before it is considered for steering behaviors. [Limits: > 0]
    float              _pathOptimizationRange; ///< The path visibility optimization range. [Limit: > 0]
    float              _separationWeight;      ///< How aggresive the agent manager should be at avoiding collisions with this agent. [Limit: >= 0]
    uint8_t            _updateFlags;           ///< Flags that impact steering behavior
    uint8_t            _obstacleAvoidanceType; ///< The index of the avoidance configuration to use for the agent
    uint8_t            _queryFilterType;       ///< The index of the query filter used by this agent
    void*              _userData;              ///< User defined data attached to the agent
    NaviAgentHandler*  _handler;               ///< 回调
    NaviPoint3D        _velocity;              ///< 速度
    NaviPoint3D        _moveTarget;            ///< 目标
    NaviPoint3D        _position;              ///< 当前位置
    NaviPoint3D        _speed;                 ///< 当前速度
    dtCrowdAgentParams _params;                ///< 参数
    NaviImpl*          _navi;                  ///< 导航网格
    int                _idx;                   ///< 在dtCrowd内的索引
    bool               _stopped;               ///< 停止标志
	int				   _collideLevel;		   ///< 碰撞等级
};

}

#endif // NAVI_AGENT_IMPL_H
