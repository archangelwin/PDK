#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/MissionWeiXin.h"
#include "Platform/PFKernel/CGPGameRecord.h"
#include "Game/Game/GameDefine.h"

class NCGameRecord
	:public cocos2d::Node
	,public CGGameRecordSink
	,public FvSingleton<NCGameRecord> 
{
public:
	NCGameRecord();
	~NCGameRecord();
public:
	virtual bool init();
public:
	void EnterScence();
	void hideAll();
	std::string timeToString(systemtime kSystem);
	std::string scoreToString(int iScore);
public:
	virtual void onGPBackGameRecordList(tagPrivateRandTotalRecordList* pNetInfo);
	virtual void onGPBackGameTotalRecord(tagPrivateRandTotalRecord* pNetInfo);

public:
	void Button_zhanji(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ShowTotal(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordBack(cocos2d::Ref*,WidgetUserInfo*);
	void Button_StartGameRecord(cocos2d::Ref*,WidgetUserInfo*);
public:
    void showOutCard(std::vector<BYTE> kOutCard,int m_iIdex,std::string nickName);
    void setCardImgic(cocos2d::Node* pCard,int nCardData);
private:
	CGPGameRecordMission m_kGameRecordMission;
	tagPrivateRandTotalRecord m_kPrivateRandTotalRecord;
};
