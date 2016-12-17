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

#include "navi_agent_impl.h"
#include "navi_impl.h"

namespace knavi {

NaviAgentImpl::NaviAgentImpl(float radius, NaviImpl* navi)
: _radius(radius),
  _height(1.0f),
  _maxAcceleration(.1f),
  _maxSpeed(3.5f),
  _collisionQueryRange(radius * 8.0f),
  _pathOptimizationRange(radius * 30.0f),
  _separationWeight(.0f),
  _updateFlags(DT_CROWD_SEPARATION),
  _obstacleAvoidanceType(0),
  _queryFilterType(0),
  _userData(0),
  _handler(0),
  _navi(navi),
  _idx(-1),
  _stopped(true),
  _collideLevel(1) {
}

NaviAgentImpl::~NaviAgentImpl() {
}

bool NaviAgentImpl::start() {
    float pos[3] = {.0f};
    pos[0] = _position.x;
    pos[1] = _position.y;
    pos[2] = _position.z;
    _params.userData = this;
    // 添加Agent到dtCrowd
    _idx = _navi->getCrowd()->addAgent(pos, &_params);
    if (_idx < 0) {
        return false;
    }
	setCollideLevel(_collideLevel);
    reset();
    // 目标点不为零，发起移动
    if (!_moveTarget.isZero()) {
        move();
    }
    return true;
}

void NaviAgentImpl::stopWalk() {
    if (_idx < 0) {
        return;
    }
    _stopped = true;
    _navi->getCrowd()->resetMoveTarget(_idx);
}

bool NaviAgentImpl::isStopped() {
    return _stopped;
}

void NaviAgentImpl::setRadius(float radius) {
    _radius = radius;
    reset();
}

float NaviAgentImpl::getRadius() {
    return _radius;
}

void NaviAgentImpl::setHeight(float height) {
    _height = height;
    reset();
}

float NaviAgentImpl::getHeight() {
    return _height;
}

void NaviAgentImpl::setMaxAcceleration(float maxAcceleration) {
    _maxAcceleration = maxAcceleration;
    reset();
}

float NaviAgentImpl::getMaxAcceleration() {
    return _maxAcceleration;
}

void NaviAgentImpl::setMaxSpeed(float maxSpeed) {
    _maxSpeed = maxSpeed;
    reset();
}

float NaviAgentImpl::getMaxSpeed() {
    return _maxSpeed;
}

void NaviAgentImpl::setCollisionQueryRange(float collisionQueryRange) {
    _collisionQueryRange = collisionQueryRange;
    reset();
}

float NaviAgentImpl::getCollisionQueryRange() {
    return _collisionQueryRange;
}

void NaviAgentImpl::setPathOptimizationRange(float pathOptimizationRange) {
    _pathOptimizationRange = pathOptimizationRange;
    reset();
}

float NaviAgentImpl::getPathOptimizationRange() {
    return _pathOptimizationRange;
}

void NaviAgentImpl::setSeparationWeight(float separationWeight) {
    _separationWeight = separationWeight;
    reset();
}

float NaviAgentImpl::getSeparationWeight() {
    return _separationWeight;
}

void NaviAgentImpl::setUpdateFlags(uint8_t updateFlags) {
    _updateFlags |= updateFlags;
    reset();
}

uint8_t NaviAgentImpl::getUpdateFlags() {
    return _updateFlags;
}

void NaviAgentImpl::setObstacleAvoidanceType(uint8_t obstacleAvoidanceType) {
    _obstacleAvoidanceType = obstacleAvoidanceType;
    reset();
}

uint8_t NaviAgentImpl::getObstacleAvoidanceType() {
    return _obstacleAvoidanceType;
}

void NaviAgentImpl::setQueryFilterType(uint8_t queryFilterType) {
    _queryFilterType = queryFilterType;
    reset();
}

uint8_t NaviAgentImpl::getQueryFilterType() {
    return _queryFilterType;
}

void NaviAgentImpl::setUserData(void* userData) {
    _userData = userData;
}

void* NaviAgentImpl::getUserData() {
    return _userData;
}

void NaviAgentImpl::setHandler(NaviAgentHandler* handler) {
    _handler = handler;
}

NaviAgentHandler* NaviAgentImpl::getHandler() {
    return _handler;
}

void NaviAgentImpl::setVelocity(const NaviPoint3D& velocity) {
    _velocity = velocity;
    if (_idx >= 0) {
        float v[3] = {.0f};
        v[0] = velocity.x;
        v[1] = velocity.y;
        v[2] = velocity.z;
        // 更改移动速度
        _navi->getCrowd()->requestMoveVelocity(_idx, v);
    }
}

const NaviPoint3D& NaviAgentImpl::getVelocity() {
    return _velocity;
}

void NaviAgentImpl::setMoveTarget(const NaviPoint3D& moveTarget) {
    _moveTarget = moveTarget;
    move();
}

const NaviPoint3D& NaviAgentImpl::getMoveTarget() {
    if (_idx >= 0) {
        const dtCrowdAgent* agent = _navi->getCrowd()->getAgent(_idx);
        _moveTarget.x = agent->targetPos[0];
        _moveTarget.y = agent->targetPos[1];
        _moveTarget.z = agent->targetPos[2];
    }
    return _moveTarget;
}

void NaviAgentImpl::changePosition(const NaviPoint3D& pos)
{
	_position = pos;
	if (_idx >= 0) {
		dtCrowdAgent* crowAgent = _navi->getCrowd()->getEditableAgent(_idx);
		if (crowAgent)
		{
			crowAgent->npos[0] = pos.x;
			crowAgent->npos[1] = pos.y;
			crowAgent->npos[2] = pos.z;
		}
	}
}

const NaviPoint3D& NaviAgentImpl::getPosition() {
    return _position;
}

const NaviPoint3D& NaviAgentImpl::getSpeed() {
    return _speed;
}

void NaviAgentImpl::setCollideLevel(int level)
{
	_collideLevel = level;
	if (_idx >= 0)
	{
		dtCrowdAgent* dtAgent = _navi->getCrowd()->getEditableAgent(_idx);
		dtAgent->ncollide = _collideLevel;
	}
}

void NaviAgentImpl::reset() {
    _params.radius                = _radius;
    _params.collisionQueryRange   = _collisionQueryRange;
    _params.height                = _height;
    _params.maxAcceleration       = _maxAcceleration;
    _params.maxSpeed              = _maxSpeed;
    _params.obstacleAvoidanceType = _obstacleAvoidanceType;
    _params.pathOptimizationRange = _pathOptimizationRange;
    _params.queryFilterType       = _queryFilterType;
    _params.separationWeight      = _separationWeight;
    _params.updateFlags           = _updateFlags;
    _params.userData              = this;
    if (_idx >= 0) {
        // 更新Agent参数
        _navi->getCrowd()->updateAgentParameters(_idx, &_params);
    }
}

void NaviAgentImpl::move() {
    if (_idx >= 0) {
        _stopped = false;
        float posArray[3] = {.0f};
        posArray[0] = _moveTarget.x;
        posArray[1] = _moveTarget.y;
        posArray[2] = _moveTarget.z;
        // 找到目标点的Polygon
        dtPolyRef polyRef = _navi->getPolyRefByPos(_moveTarget);
        // 发起移动请求
        _navi->getCrowd()->requestMoveTarget(_idx, polyRef, posArray);
    }
}

}
