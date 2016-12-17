#include "NetManager.h"

#include "JSGameBind.h"

using namespace cocos2d;

NetManager::NetManager()
{
}

NetManager::~NetManager()
{
}

NetManager* NetManager::getInstance()
{
	return NetManagerIns;
}

void NetManager::initLoginNet(const std::string& ip, unsigned int port)
{
	m_bReconnect = false;
	m_nPhase = 1;
	CCInetAddress addr;

	addr.setIp(ip.c_str());
	addr.setPort(port);

	_socketMgrList[Login]->setInetAddress(addr);
	_socketMgrList[Login]->connect();
}

void NetManager::initGSNet(const std::string& ip,unsigned int port)
{
	_socketMgrList[Login]->disconnect();
	m_nPhase = 2;
	CCInetAddress addr;
	addr.setIp(ip.c_str());
	addr.setPort(port);
	_socketMgrList[GameServer]->setInetAddress(addr);
	_socketMgrList[GameServer]->connect();
}

void NetManager::connectServer(int srvIdx, const std::string& ip, unsigned int port)
{
	switch (srvIdx)
	{
	case Login:
		log("Connect Login: ip : %s  port %d", ip.c_str(), port);
		initLoginNet(ip, port);
		break;
	case GameServer:
		log("Connect GameServer: ip : %s  port %d", ip.c_str(), port);
		initGSNet(ip, port);
		break;

	default:
		log("Not found in server type %d", srvIdx);
		break;
	}
}
/*/
void NetManager::connectBattle(const std::string& ip, unsigned int port)
{
	//_socketMgrList[BattleServer]->disconnect();
	CCInetAddress addr;
	addr.setIp(ip.c_str());
	addr.setPort(port);
	_socketMgrList[BattleServer]->setInetAddress(addr);
	_socketMgrList[BattleServer]->connect();
}
*/
void NetManager::onMessageReceived(const ProtoMsg& msg)
{
	int msgId = msg.getCmd();
	std::string msgStr = std::string(msg.getBody(), msg.getBodyLength());
	CodeNullChar(msgStr);
	jsval args[] = {
		args[0] = INT_TO_JSVAL(JSBBindMsgID::RECV_NET_MSG),
		args[1] = INT_TO_JSVAL(msgId),
		args[2] = getUTF8JSString(msgStr)
	};

	/*test code*/
/*	if (msgId == 1112)
	{
		auto path = FileUtils::getInstance()->getDefaultResourceRootPath() + "res/proto/test.txt";
		FileUtils::getInstance()->writeStringToFile(msgStr, path);
	}
*/
	send_js_msg(3, args);
}
