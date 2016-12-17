//
//  MsgHandler.h
//  gamedemo
//
//  Created by xin on 14-8-11.
//
//

#ifndef __gamedemo__NetManager__
#define __gamedemo__NetManager__

#include <map>
#include "Utils/Singleton.hpp"
#include "net/Socket/CCNetDelegate.h"

class ProtoMsg;
class NetManager : public cocos2d::CCNetDelegate
{
public:
    NetManager();
    ~NetManager();   

	static NetManager* getInstance();

	void connectServer(int srvIdx, const std::string& ip, unsigned int port);
	virtual void onMessageReceived(const ProtoMsg& msg);	
private:
	void initLoginNet(const std::string& ip, unsigned int port);
	void initGSNet(const std::string& ip, unsigned int port);

	bool m_bReconnect;
	int m_nPhase;
    //std::map</*const google::protobuf::Descriptor**/unsigned int,HandlerOp> m_msgMap;
};

#define NetManagerIns Singleton<NetManager>::instance()

#endif /* defined(__gamedemo__MsgHandler__) */
