#pragma once
#include "cocos2d.h"
#include "Platform/PlatformHeader.h"
#include "Kernel/network/CSocketMission.h"

//////////////////////////////////////////////////////////////////////////////////

//�б�����
class CGPRoomListOnLineMission
	: public CSocketMission
{
public:
	CGPRoomListOnLineMission(const char* url, int port);

	//��������
protected:
	bool							m_bUpdateOnLine;					//��������

	//��������
public:
	//���캯��
	CGPRoomListOnLineMission();
	//��������
	virtual ~CGPRoomListOnLineMission();

	//���غ���
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);

	//���ܺ���
public:
	//��������
	void UpdateOnLineInfo();
};
