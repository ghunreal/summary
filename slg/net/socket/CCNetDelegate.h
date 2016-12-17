/****************************************************************************
Copyright (c) 2014 Lijunlin - Jason lee

Created by Lijunlin - Jason lee on 2014

jason.lee.c@foxmail.com
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCNET_NETDELEGATE_H__
#define __CCNET_NETDELEGATE_H__

#include <string>
#include <thread>
#include "cocos2d.h"
#include "CCNetMacros.h"
#include "CCSocket.h"

#include "../msg/msg.h"

extern "C" {
#include "ringbuffer.h"
}

NS_CC_BEGIN

struct SocketConfig
{
	SocketConfig(int _bufSize, int _readPerFrame, int _id)
		: bufSize(_bufSize), readPerFrame(_readPerFrame), id(_id)
	{}
	int bufSize;
	int readPerFrame;
	int id;
};

class CCSocketMgr
{
public:
	CCSocketMgr();
	CCSocketMgr(const SocketConfig& config);
	void setInetAddress(const CCInetAddress& oInetAddress);
	void update(float dt);
	void send(uint16_t cmd, const std::string& msg);
	void connect();
	void disconnect();
	bool rcvSocketData(std::list<ProtoMsg>& msgList);
private:
	int _bufSize;    
	int _readPerFrame;
	int _id;
	std::list<std::string>	_sendBuffers;
	kringbuffer_t*			_readBuffer;
	CCSocketStatus			_eStatus;
	float					_fConnectingDuration;
	float					_fSoTimeout;

	CCInetAddress			_inetAddress;
	CCSocket				_socket;

	void sendData();
	void reconnect();
};

/**
 * class  : CCNetDelegate
 * author : Jason lee
 * email  : jason.lee.c@foxmail.com
 * descpt : the net delegate, use it as connector
 */
class CCNetDelegate : public Ref
{
public:
	CCNetDelegate();
	virtual ~CCNetDelegate();

public:
	// will calling when a package is coming
	virtual void onMessageReceived(const ProtoMsg& msg) = 0;

public:
	
	// send package to target address
	void send( int type, uint16_t cmd, const std::string& msg);

private:
	void rcvData();
	void processMsgList();

	// frame call
	void runSchedule(float dt);
	
	// registe the function of frame called
	void registerScheduler();

	// unregiste the function of frame called
	void unregisterScheduler(); 

protected:
	enum SocketType
	{
		Login = 0,
		GameServer,
		Max,
	};

	std::vector<CCSocketMgr*> _socketMgrList;

	bool					_isStop;
	std::thread*			_threadRecv;
	std::list<ProtoMsg>		_msgList;
	std::mutex				_msgMutex;
};

NS_CC_END

#endif //__CCNET_NETDELEGATE_H__