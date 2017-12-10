#pragma once
#include "cocos2d.h"
#include "Kernel/kernel/user/IClientUserItem.h"

class GamePlayer
{
public:
	GamePlayer(IClientUserItem* pUserItem);
	~GamePlayer();
public:
	void setUserItem(IClientUserItem* pItem);
	IClientUserItem* getUserItem(bool bAssert = true);
public:
	word GetTableID();
	word GetChairID();
	byte GetUserStatus();
	SCORE GetUserScore();
	word GetFaceID();
	byte GetGender();
	dword GetUserID();
	dword GetGameID();
	dword GetUserStarValue();
	std::string GetHeadHttp();
	std::string GetNickName();
	std::string GetGPS();
	//�û���Ϣ
	tagUserInfo * GetUserInfo();
public:
	virtual void PlayerEnter(){};
	virtual void PlayerLeave(){};
	virtual void upPlayerInfo(){};
	virtual void upPlayerState(){};
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo){}
protected:
	IClientUserItem* m_pUserItem;
};