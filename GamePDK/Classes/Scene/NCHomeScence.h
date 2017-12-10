#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPLoginMission.h"
#include "Platform/PFKernel/CGPublicNoticMission.h"

class NCHomeScence
	:public cocos2d::Node
	,public IServerListDataSink
    ,public CGPublicNoticMissionSink
	,public FvSingleton<NCHomeScence> 
{
public:
	NCHomeScence();
	~NCHomeScence();
public:
	virtual bool init();
public:
	//���֪ͨ
	virtual void OnGameItemFinish();
	//���֪ͨ
	virtual void OnGameKindFinish(uint16 wKindID);
	//����֪ͨ
	virtual void OnGameItemUpdateFinish();

	//����֪ͨ
public:
	//����֪ͨ
	virtual void OnGameItemInsert(CGameListItem * pGameListItem);
	//����֪ͨ
	virtual void OnGameItemUpdate(CGameListItem * pGameListItem);
	//ɾ��֪ͨ
	virtual void OnGameItemDelete(CGameListItem * pGameListItem);

	virtual void onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr );
    void NextSpeaker();
    void showSysSpeakerTxt(std::string kTxt,std::string kColor);
    
    virtual void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription);
public:
	void EnterScence();
	void upPlayerInfo();
	void LogonSucess();
	void upGameSever();

	void upJoinNumTxt();
    
    void UpdatePaomadeng(std::string paomadeng);
    void UpdateChongKaNotice(std::string notice);
public:
    CGPublicNoticMission m_kPublicNoticMission;
    std::vector<std::string> m_kMsgList;
	void init_Button();
	void Button_GameRoom(cocos2d::Ref*,WidgetUserInfo*);
	void Button_BackHome(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HomeHead(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HomeExit(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HomeGameHall(cocos2d::Ref*,WidgetUserInfo*);
    void Button_sharePengYou(cocos2d::Ref*,WidgetUserInfo*);
    void Button_shareQuan(cocos2d::Ref*,WidgetUserInfo*);
    
private:
};
