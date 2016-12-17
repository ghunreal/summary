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
#include "CCNetDelegate.h"
#include "JSGameBind.h"

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
#include <unistd.h>
#endif
NS_CC_BEGIN

static void SleepFunc(int ms)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	Sleep(ms);
#else
	usleep(ms*1000);
#endif
}

CCSocketMgr::CCSocketMgr()
	: _bufSize(0)
	, _readPerFrame(0)
	, _readBuffer(NULL)
	, _eStatus(eSocketDisconnected)
{

}

CCSocketMgr::CCSocketMgr(const SocketConfig& config)
	: _bufSize(config.bufSize)
	, _readPerFrame(config.readPerFrame)
	, _eStatus(eSocketDisconnected)
{
	_readBuffer = ringbuffer_create(_bufSize); // 512K
	_id = config.id;
}

void CCSocketMgr::send(uint16_t cmd, const std::string& msg)
{
	connect();

	ProtoMsg protoMsg;
	protoMsg.initMsg(ProtoMsg::HeadLen + msg.size(), cmd, 123456, msg);
	_sendBuffers.push_back(protoMsg.getSerializeStr());
}

void CCSocketMgr::setInetAddress(const CCInetAddress& oInetAddress)
{
	_socket.setInetAddress(oInetAddress);
}

void CCSocketMgr::update(float dt)
{
	switch (_eStatus)
	{
	case eSocketConnecting:
	{
		switch (_socket.isConnected())
		{
		case eSocketConnected:
		{
			_eStatus = eSocketConnected;
//			onConnected();
		}
		break;
		case eSocketConnectFailed:
		{
			_socket.closeSocket();
			_eStatus = eSocketConnectFailed;
		}
		break;
		case eSocketConnecting:
		{
			if (_fConnectingDuration > _fSoTimeout)
			{
				disconnect();
				_fConnectingDuration = 0.0f;
			}
			else
			{
				_fConnectingDuration += dt;
			}
		}
		break;
		default:
			break;
		}
	}
	break;
	case eSocketConnected:
	{
		if (!_sendBuffers.empty() && _socket.isWritable())
		{
			sendData();
		}
	}
	break;
	default:
		break;
	}
}

void CCSocketMgr::connect()
{
	if (_eStatus != eSocketConnected && _eStatus != eSocketConnecting)
	{
		_socket.connect();
		_eStatus = eSocketConnecting;
	}
}

void CCSocketMgr::disconnect()
{
	if (_eStatus != eSocketDisconnected)
	{
		_socket.disconnect();
		_eStatus = eSocketDisconnected;
	}	
}

bool CCSocketMgr::rcvSocketData(std::list<ProtoMsg>& msgList)
{
	int bufNum = 0;
	if (_eStatus == eSocketConnected && _socket.isReadable())
	{
		char socketBuffer[SOCKET_READ_BUFFER_SIZE];
		int nRet = _socket.read(socketBuffer, SOCKET_READ_BUFFER_SIZE);
		if (nRet == eSocketIoError || nRet == eSocketIoClosed)
		{
			// do nothing now
		}
		else
		{
			CCLOG("Socket readdata len is %d", nRet);
			ringbuffer_write(_readBuffer, socketBuffer, (uint32_t)nRet);
		}
	}
	uint32_t size = ringbuffer_available(_readBuffer);
	if (size > ProtoMsg::HeadLen)
	{
		char socketBuffer[SOCKET_READ_BUFFER_SIZE];
		while ((size = ringbuffer_available(_readBuffer)) >= ProtoMsg::HeadLen
			&& bufNum < _readPerFrame)
		{
			uint32_t copySize = ringbuffer_copy(_readBuffer, socketBuffer, sizeof(socketBuffer));
			ProtoMsg msg;
			if (msg.makeMsg(socketBuffer, copySize))
			{
				CCLOG("onMessageReceived, cmd id %d, msg total size = %d, push msg", msg.getCmd(), msg.getMsgSize());
				msgList.push_back(msg);
				ringbuffer_remove(_readBuffer, msg.getMsgSize());
				++bufNum;
			}
			else {
				CCLOG("onMessageReceived, cmd id %d, msg total size = %d, not enough", msg.getCmd(), msg.getMsgSize());
				break;
			}
		}
	}
	return bufNum>0;
}

void CCSocketMgr::sendData()
{
	while (_sendBuffers.size() > 0)
	{
		const std::string& buf = _sendBuffers.front();

		int nRet = _socket.write((char*)buf.c_str(), buf.size());
		if (nRet == buf.size())
		{
			log("Write socket complete, len is %d, server type is %d", nRet, _id);
			_sendBuffers.pop_front();
		}
		else
		{
			CCLOG("Write socket error, len is %d, write len is %d", buf.size(), nRet);
			disconnect();
			reconnect();
			break;
		}
	}
}

void CCSocketMgr::reconnect()
{

}

/*****************************************CCSocketMgr End**************************************/

CCNetDelegate::CCNetDelegate()
	: _isStop(false)
{	
	_threadRecv = new std::thread(&CCNetDelegate::rcvData, this);

	// Login
	_socketMgrList.push_back(new CCSocketMgr(SocketConfig(0.5f*SOCKET_READ_BUFFER_SIZE, 8, Login)));

	// GameServer
	_socketMgrList.push_back(new CCSocketMgr(SocketConfig(1.5f*SOCKET_READ_BUFFER_SIZE, 8, GameServer)));

	registerScheduler();
}

CCNetDelegate::~CCNetDelegate()
{
	_isStop = true;
	_threadRecv->join();
	delete _threadRecv;
	for (auto it : _socketMgrList)
	{
		delete it;
	}
}

void CCNetDelegate::send(int type, uint16_t cmd, const std::string& msg)
{
	if (type >= Login && type < Max) {
		if (_socketMgrList[type])
		{
			_socketMgrList[type]->send(cmd, msg);
		}
		else
		{
			CCLOG("_socketMgrList %d is NULL", type);
		}
	}
	else
	{
		CCLOG("Invalid server type : %d", type);
	}
}

void CCNetDelegate::runSchedule(float dt)
{
	processMsgList(); 
	for (auto it : _socketMgrList)
	{
		it->update(dt);
	}
}

void CCNetDelegate::rcvData()
{
	while (true)
	{
		if (_isStop)
			break;

		_msgMutex.lock();
		bool isRcv = false;
		for (auto it : _socketMgrList)
		{
			isRcv = isRcv || it->rcvSocketData(_msgList);
		}
		_msgMutex.unlock();

		if (isRcv)
		{
			SleepFunc(5);
		}
		else
		{
			SleepFunc(30);
		}
	}
}

void CCNetDelegate::processMsgList()
{
	if (_msgList.empty())
		return;

	std::lock_guard<std::mutex> lck(_msgMutex);
	auto it = _msgList.begin();
	for (int i = 0; i < _msgList.size(); ++i, ++it)
	{
		onMessageReceived(*it);
	}
	_msgList.clear();
}

void CCNetDelegate::registerScheduler()
{
	CCDirector::getInstance()->getScheduler()->schedule(
		schedule_selector(CCNetDelegate::runSchedule), 
		this, 
		0.0f, 
		false
	);
}

void CCNetDelegate::unregisterScheduler()
{
	CCDirector::getInstance()->getScheduler()->unschedule(
		schedule_selector(CCNetDelegate::runSchedule),
		this
	);
}


NS_CC_END