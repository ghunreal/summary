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
 * �������Ƿ�Ϊ��
 * @retval true ��
 * @retval false ����
 */
inline bool zero(float a) {
    static const float EPSINON = 0.000001f;
    return ((a >= -EPSINON) && (a <= EPSINON));
}

/**
 * ��������������
 */
struct NaviPoint3D {
    float x;  ///< x����
    float y;  ///< y����
    float z;  ///< z����

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
     * ����
     * @return ����
     */
    float getLength() const {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    /**
     * �Ƿ�Ϊ������
     * @retval true ��
     * @retval false ��
     */
    bool isZero() const {
        return (zero(x) && zero(y) && zero(z));
    }

    /**
     * �ɶ��ַ���
     * @return �ַ���
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
 * Agent�ص�
 */
class NaviAgentHandler {
public:
    virtual ~NaviAgentHandler() {}

    /**
     * ÿ��Navi::updateִ�ж��ᱻ�ص�
     * @param agent Agent
     */
    virtual void onUpdate(knavi::NaviAgent* agent) = 0;

    /**
     * Agent������ǰ����
     * @param agent Agent
     */
    virtual void onDestroy(knavi::NaviAgent* agent) = 0;
};

/**
 * ���������ڵ�Agent
 */
class NaviAgent {
public:
    /**
     * ������
     */
    virtual ~NaviAgent() {}

    /**
     * ����Agent
     * @retval true �ɹ�
     * @retval false ʧ��
     */
    virtual bool start() = 0;

    /**
     * ֹͣ����
     */
    virtual void stopWalk() = 0;

    /**
     * Agent�Ƿ�ֹͣ�ƶ�
     * @retval true ��
     * @retval false ��
     */
    virtual bool isStopped() = 0;

    /**
     * ���ý�����뾶
     * @param radius ������뾶
     */
    virtual void setRadius(float radius) = 0;

    /**
     * ��ȡ������뾶
     * @return ������뾶
     */
    virtual float getRadius() = 0;

    /**
     * ����Agent�߶�
     * @param height �߶�
     */
    virtual void setHeight(float height) = 0;

    /**
     * ��ȡAgent�߶�
     * @return �߶�
     */
    virtual float getHeight() = 0;

    /**
     * ���������ٶ�(��/��)
     * @param maxAcceleration �����ٶ�
     */
    virtual void setMaxAcceleration(float maxAcceleration) = 0;

    /**
     * ��ȡ�����ٶ�(��/��)
     * @return �����ٶ�
     */
    virtual float getMaxAcceleration() = 0;

    /**
     * ��������ƶ��ٶ�(��/��)
     * @param maxSpeed ����ƶ��ٶ�
     */
    virtual void setMaxSpeed(float maxSpeed) = 0;

    /**
     * ��ȡ����ƶ��ٶ�(��/��)
     * @return ����ƶ��ٶ�
     */
    virtual float getMaxSpeed() = 0;

    /**
     * �����ƶ��ٶ�����
     * @param velocity �ƶ�����
     */
    virtual void setVelocity(const NaviPoint3D& velocity) = 0;

    /**
     * ��ȡ�ƶ��ٶ�����
     * @return �ƶ�����
     */
    virtual const NaviPoint3D& getVelocity() = 0;

    /**
     * �����û�����
     * @param userData �û�����
     */
    virtual void setUserData(void* userData) = 0;

    /**
     * ���ö�̬��ײ���ķ�Χ
     * @param collisionQueryRange ��̬��ײ���ķ�Χ
     */
    virtual void setCollisionQueryRange(float collisionQueryRange) = 0;

    /**
     * ��ȡ��̬��ײ���ķ�Χ
     * @return ��̬��ײ���ķ�Χ
     */
    virtual float getCollisionQueryRange() = 0;

    /**
     * ����·���Ż���Χ
     * @param pathOptimizationRange ·���Ż���Χ
     */
    virtual void setPathOptimizationRange(float pathOptimizationRange) = 0;

    /**
    * ��ȡ·���Ż���Χ
    * @return ·���Ż���Χ
    */
    virtual float getPathOptimizationRange() = 0;

    /**
     * ����AgentվλȨ��
     * @param separationWeight AgentվλȨ��
     */
    virtual void setSeparationWeight(float separationWeight) = 0;

    /**
     * ����Agent������Ϊ��־
     * @param updateFlags ������Ϊ��־
     */
    virtual void setUpdateFlags(uint8_t updateFlags) = 0;

    /**
    * ��ȡAgent������Ϊ��־
    * @return ������Ϊ��־
    */
    virtual uint8_t getUpdateFlags() = 0;

    /**
     * ��ȡAgentվλȨ��
     * @return AgentվλȨ��
     */
    virtual float getSeparationWeight() = 0;

    /**
     * ��ȡ�û�����
     * @return �û�����
     */
    virtual void* getUserData() = 0;

    /**
     * ����Ѱ·Ŀ���
     * @param moveTarget Ŀ��
     */
    virtual void setMoveTarget(const NaviPoint3D& moveTarget) = 0;

    /**
     * ��ȡѰ·Ŀ���
     * @return Ŀ��
     */
    virtual const NaviPoint3D& getMoveTarget() = 0;

    /**
     * ��ȡ��ǰλ������
     * @return ��ǰλ������
     */
    virtual const NaviPoint3D& getPosition() = 0;

	/**
	* ���ĵ�ǰ���꣬�ڲ�����Ҳ�ᷢ���仯
	* @param pos ����
	*/
	virtual void changePosition(const NaviPoint3D& pos)=0;

	/**
	* ������ײ�ȼ�
	* @param level 0: ������ײ��Ĭ��1��Խ�����ȼ�Խ��
	*/
	virtual void setCollideLevel(int level)=0;

    /**
     * ��ȡ��ǰ�ٶ�����
     * @return ��ǰ�ٶ�����
     */
    virtual const NaviPoint3D& getSpeed() = 0;

    /**
     * ���ûص�������
     * @param handler �ص�������
     */
    virtual void setHandler(NaviAgentHandler* handler) = 0;

    /**
     * ��ȡ�ص�������
     * @return �ص�������
     */
    virtual NaviAgentHandler* getHandler() = 0;
};

/**
 * ��������ӿ�
 */
class Navi {
public:
    /**
     * ����
     */
    virtual ~Navi() {}

    /**
     * ����OBJ�ļ�
     * @param filePath �ļ�����·��
     * @retval true �ɹ�
     * @retval false ʧ��
     */
    virtual bool load(const char* filePath) = 0;

    /**
     * ���������ͼ��Agent���߸���
     * @param ms ����Ƶ��(����/֡)
     */
    virtual void update(uint32_t ms) = 0;

    /**
     * ����Agent����Agent���������ͼ
     * @param radius Agent������뾶
     * @param position Agent����λ������
     * @return NaviAgentʵ��
     */
    virtual NaviAgent* addAgent(float radius, const NaviPoint3D& position) = 0;
    
    /**
     * ����Agent����Agent�������ͼ���Ƴ�
     * @param agent Agent
     */
    virtual void removeAgent(NaviAgent* agent) = 0;

    /**
     * Ѱ·
     * @param start ��ʼ����
     * @param end Ŀ������
     * @param path ·��������
     * @param size ���鳤��, ����ʵ�ʳ���
     * @retval true �ɹ�
     * @retval false ʧ��
     */
    virtual bool find(const NaviPoint3D& start, const NaviPoint3D& end, NaviPoint3D* path, int* size) = 0;

    /**
     * �����߶�
     * @param start ��ʼ����
     * @param end Ŀ������
     * @param hitPoint �����߶�ʵ���յ㣨�������ϰ����ཻ��
     * @retval true �ɹ�
     * @retval false ʧ��
     */
    virtual bool raycast(const NaviPoint3D& start, const NaviPoint3D& end, NaviPoint3D& hitPoint) = 0;

    /**
     * ��ȡ����
     * @return ����ԭ��
     */
    virtual const char* getError() = 0;
};

/**
 * �������񹤳�
 */
class NaviFactory {
public:
    /**
     * ��ȡ��������
     * @return NaviFactoryʵ��
     */
    static NaviFactory* instance();

    /**
     * ���ٹ�������
     */
    static void destroy();

    /**
     * ����
     */
    ~NaviFactory();

    /**
     * ������������ʵ��
     * @param maxAgent Agent�������
     * @return Navi�ӿ�
     */
    Navi* createNavi(int maxAgent = 25);

    /**
     * ���ٵ�������ʵ��
     * @param navi Navi�ӿ�
     */
    void destroyNavi(Navi* navi);

private:
    NaviFactory();
    NaviFactory(const NaviFactory&);
    static NaviFactory* _instance; ///< ����
};

/*! ��ȡ�������񹤳����������� */
#define NaviFactoryPtr knavi::NaviFactory::instance()

}

#endif // NAVI_H
