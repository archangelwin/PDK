#pragma once
#include "cocos2d.h"
#include "Platform/PlatformHeader.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Script/TimeNode.h"
#include "Platform/PFKernel/CGPIndividualMission.h"
#include "Platform/PFKernel/CGUserInGameServerInfo.h"


typedef void (cocos2d::Ref::*QYSEL_CallFunc)();
#define QY_CALLFUNC_SELECTOR(_SELECTOR) static_cast<QYSEL_CallFunc>(&_SELECTOR)

struct UserCallInfo
{
	UserCallInfo()
		:pPoint(0)
		,pFun(NULL)
	{}
	cocos2d::Ref* pPoint;
	QYSEL_CallFunc pFun;
};

class UserInfo
	:public GlobalUserInfoSink
	,public ICGUserInGameServerInfoSink
	,public TimeNode 
	,public FvSingleton<UserInfo>
{
public:
	UserInfo();
	~UserInfo();
public:
	void reqAccountInfo(float fWaiteTime = 0.0f);
	void reqIndividual();
	void on_reqAccountInfo();
	tagGlobalUserData* getUserData();
	tagUserInsureInfo* GetUserInsureInfo();
	std::string getUserNicName();
	std::string getAccounts();
	SCORE getUserScore();
	SCORE getUserInsure();
	dword getUserID();
	dword getSpreakerID();
	byte getGender();
	void modeGender(byte cbGender);
	void modeName(std::string strName);
	void modeHeadHttp(std::string strHttp);
	void modeGPS(std::string strGps);
	std::string getHeadHttp();
	std::string getUserChannel();
	std::string getUserIP();
	void setPhoneNumber(std::string strNumber);
	std::string getPhoneNumber();
	void checkInGameServer();
	void setPrivateInGameServerID(int iServerID);
	void setUserRight(dword dwRight);
	void setMasterRight(dword dwRight);
	dword getUserRight();
	dword getMasterRight();
public:
	void addLoginSucessCB(cocos2d::Ref* pPoint,QYSEL_CallFunc pFun);
	void addUpPlayerInfoCB(cocos2d::Ref* pPoint,QYSEL_CallFunc pFun);
	void delCallByPoint(cocos2d::Ref* pPoint);
public:
	virtual void upPlayerInfo();
	virtual void LoginSucess();
private:
	std::vector<UserCallInfo> m_kUpPlayerInfoCB;
	std::vector<UserCallInfo> m_kLoginSucessCB;
	CGPIndividualMission m_kIndividualMission;

	dword				m_dwUserRight;						//�û�Ȩ��
	dword				m_dwMasterRight;					//����Ȩ��
};