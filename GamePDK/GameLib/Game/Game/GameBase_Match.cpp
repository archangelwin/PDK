#include "GameBase.h"

#include "Game/GameLib.h"


//����¼�
//ϵͳ������Ϣ
bool GameBase::OnGFTableMessage(const char* szMessage)
{

	return true;
}
//������Ϣ
bool GameBase::OnGFMatchInfo(tagMatchInfo* pMatchInfo)
{

	return true;
}
//�����ȴ���ʾ
bool GameBase::OnGFMatchWaitTips(tagMatchWaitTip* pMatchWaitTip)
{
	return true;
}
//�������
bool GameBase::OnGFMatchResult(tagMatchResult* pMatchResult)
{

	return true;
}

