#include "NCHomeScence.h"
#include "NCScenceManager.h"
#include "GameLib/Game/GameLib.h"


//���֪ͨ
void NCHomeScence::OnGameItemFinish()
{
}
//���֪ͨ
void NCHomeScence::OnGameKindFinish(uint16 wKindID)
{

}
//����֪ͨ
void NCHomeScence::OnGameItemUpdateFinish()
{

}
//����֪ͨ
void NCHomeScence::OnGameItemInsert(CGameListItem * pGameListItem)
{
}
//����֪ͨ
void NCHomeScence::OnGameItemUpdate(CGameListItem * pGameListItem)
{

}
//ɾ��֪ͨ
void NCHomeScence::OnGameItemDelete(CGameListItem * pGameListItem)
{

}
void NCHomeScence::onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr )
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
}