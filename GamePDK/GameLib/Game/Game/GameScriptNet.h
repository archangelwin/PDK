#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPublicNoticMission.h"

class GameScriptNet
	:public cocos2d::Ref
	,public CGPublicNoticMissionSink
	,public FvSingletonBase<GameScriptNet>
{
public:
	GameScriptNet();
	~GameScriptNet();

public:
	void LogonSucess();
	void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription);
	void OnGetConfig();
public:
	CGPublicNoticMission m_kPublicNoticMission;
};