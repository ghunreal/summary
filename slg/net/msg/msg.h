#ifndef MSG_H
#define MSG_H

#ifdef WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>  
#endif

#define MAX_BODY_SIZE 1024 * 64

#pragma pack (1)

/**
* 协议头保留字段
*/
typedef union _Reserve {
	_Reserve()
		: reserve0(0) {
	}

	_Reserve(const _Reserve& rht) {
		reserve0 = rht.reserve0;
	}

	const _Reserve& operator=(const _Reserve& rht) {
		reserve0 = rht.reserve0;
		return *this;
	}

	const _Reserve& operator=(uint64_t value) {
		reserve0 = value;
		return *this;
	}

	const _Reserve& convert() {
		uint16_t* sp = (uint16_t*)&reserve0;
		reserve1.crc = ntohs(sp[0]);
		reserve1.rsa = ntohs(sp[1]);
		return *this;
	}

	uint64_t reserve0; ///< 保留0, 集群节点间传输将设置为角色ID, 客户端与集群间的传输可能用于加密等安全策略
	struct {
		uint16_t crc;      ///< 16位用于CRC16校验
		uint16_t rsa;      ///< 是否经过RSA加密, 1或者0
		uint16_t zip;      ///< 协议是否经过压缩
		uint16_t reserve3;
	} reserve1; ///< 保留1
} Reserve;

/**
* 消息头
*
* 系统默认的消息头格式
*/
struct MsgHeader {
	uint16_t size;     ///< 包长度,包含MsgHeader长度
	uint16_t type;     ///< 消息类型(消息ID)
	Reserve  reserve;  ///< 保留字段

	MsgHeader()
		: size(0),
		type(0) {
	}

	MsgHeader(const MsgHeader& rht) {
		size = rht.size;
		type = rht.type;
		reserve = rht.reserve;
	}

	~MsgHeader() {
		size = 0;
		type = 0;
		reserve = 0;
	}

	const MsgHeader& operator=(const MsgHeader& rht) {
		size = rht.size;
		type = rht.type;
		reserve = rht.reserve;
		return *this;
	}
};

//msg struct
class ProtoMsg{
public:
	static const int bufferLength = MAX_BODY_SIZE;
	static const int HeadLen = sizeof(MsgHeader);
	static const int bodyLength = bufferLength-HeadLen; 

private:
	char _body[bodyLength]; //body
	MsgHeader _header;

public:
	ProtoMsg()
	{
		memset(_body, 0, sizeof(_body));
	}

	void initMsg(uint16_t size, uint16_t type, uint64_t reserve, const std::string& body)
	{
		memset(_body, 0, sizeof(_body));
		_header.size = size;
		_header.type = type;
		_header.reserve.reserve0 = reserve;
		memcpy(_body, body.c_str(), _header.size - HeadLen);
	}

	bool makeMsg(const char* buffer,int bufSize)
	{
		memset(_body, 0, sizeof(_body));

		memcpy(&_header, buffer, HeadLen);
		_header.size = ntohs(_header.size);
		CCAssert(_header.size <= bufferLength, "msg size is beyond bufferLength!");

		if (_header.size <= bufSize) {
			_header.type = ntohs(_header.type);
			_header.reserve.reserve0 = ntohl(_header.reserve.reserve0);

			memcpy(_body, buffer + HeadLen, getBodyLength());
			return getMsgSize()>0;
		}
		return false;
	}

	uint16_t getCmd() const{
		return _header.type;
	}

	void setHeader(const MsgHeader& other)
	{
		_header = other;
	}

	const MsgHeader& getHeader()const
	{
		return _header;
	}

	void setBody(const std::string& msg)
	{
		setBody(msg.c_str(), msg.size());
	}

	void setBody(const char* msg, int size)
	{
		memcpy(_body, msg, size);
	}

	const char* getBody() const{
		return _body;
	}

	uint16_t getBodyLength() const{
		return _header.size - HeadLen;
	}

	uint16_t getMsgSize()const
	{
		return _header.size;
	}

	uint64_t getReserve()const
	{
		return _header.reserve.reserve0;
	}

	std::string getSerializeStr() const
	{
		std::string ret;
		uint16_t size = htons(_header.size);
		ret.append((char*)(&size), sizeof(uint16_t));
		uint16_t type = htons(_header.type);
		ret.append((char*)(&type), sizeof(uint16_t));
		uint64_t reserve = htonl(_header.reserve.reserve0);
		ret.append((char*)(&reserve), sizeof(uint64_t));
		ret.append(_body, getBodyLength());

		return ret;
	}
};

#pragma pack ()
#endif