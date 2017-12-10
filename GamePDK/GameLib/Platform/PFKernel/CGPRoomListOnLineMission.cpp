#include "CGPRoomListOnLineMission.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGPRoomListOnLineMission::CGPRoomListOnLineMission()
{
	//���ñ���
	m_bUpdateOnLine=false;

	return;
}

//��������
CGPRoomListOnLineMission::~CGPRoomListOnLineMission()
{
	return;
}

//�����¼�
void CGPRoomListOnLineMission::onEventTCPSocketLink()
{
	//���ñ���
	m_bUpdateOnLine=false;

	//��������
	CMD_GP_GetOnline GetOnline;
	zeromemory(&GetOnline,sizeof(GetOnline));

	//��������
	CServerListData * pServerListData=CServerListData::shared();

	CGameServerItemMap::iterator itor = pServerListData->GetServerItemMapBegin();

	CGameServerItem * pGameServerItem=pServerListData->EmunGameServerItem(itor);
	while(pGameServerItem)
	{
		//��ȡ����
		//����ж�
		if (GetOnline.wServerCount>=CountArray(GetOnline.wOnLineServerID))
		{
			ASSERT(FALSE);
			break;
		}

		tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
		GetOnline.wOnLineServerID[GetOnline.wServerCount++]=pGameServer->wServerID;

		pGameServerItem=pServerListData->EmunGameServerItem(itor);
	}

	//��������
	word wHeadSize=sizeof(GetOnline)-sizeof(GetOnline.wOnLineServerID);
	word wSendSize=wHeadSize+sizeof(GetOnline.wOnLineServerID[0])*GetOnline.wServerCount;
	send(MDM_GP_SERVER_LIST,SUB_GP_GET_ONLINE,&GetOnline,wSendSize);
}
void CGPRoomListOnLineMission::onEventTCPSocketShut()
{

}
void CGPRoomListOnLineMission::onEventTCPSocketError(int errorCode)
{

}
bool CGPRoomListOnLineMission::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	switch (sub)
	{
	//case SUB_GP_LIST_TYPE:		//������Ϣ
	//	{
	//		//Ч�����
	//		ASSERT(dataSize%sizeof(tagGameType)==0);
	//		if (dataSize%sizeof(tagGameType)!=0) return false;

	//		//��������
	//		WORD wItemCount=dataSize/sizeof(tagGameType);
	//		tagGameType * pGameType=(tagGameType *)(data);

	//		//��ȡ����
	//		CServerListData * pServerListData=CServerListData::shared();

	//		//��������
	//		for (WORD i=0;i<wItemCount;i++)
	//		{
	//			pServerListData->InsertGameType(pGameType++);
	//		}

	//		return true;
	//	}
	//case SUB_GP_LIST_KIND:		//������Ϣ
	//	{
	//		//Ч�����
	//		ASSERT(dataSize%sizeof(tagGameKind)==0);
	//		if (dataSize%sizeof(tagGameKind)!=0) return false;

	//		//��������
	//		WORD wItemCount=dataSize/sizeof(tagGameKind);
	//		tagGameKind * pGameKind=(tagGameKind *)(data);

	//		//��ȡ����
	//		CServerListData * pServerListData=CServerListData::shared();

	//		//��������
	//		for (WORD i=0;i<wItemCount;i++)
	//		{
	//			pServerListData->InsertGameKind(pGameKind++);
	//		}

	//		return true;
	//	}
	//case SUB_GP_LIST_SERVER:	//������Ϣ
	//	{
	//		//Ч�����
	//		ASSERT(dataSize%sizeof(tagGameServer)==0);
	//		if (dataSize%sizeof(tagGameServer)!=0) return false;

	//		//��������
	//		WORD wItemCount=dataSize/sizeof(tagGameServer);
	//		tagGameServer * pGameServer=(tagGameServer *)(data);

	//		//��ȡ����
	//		CServerListData * pServerListData=CServerListData::shared();

	//		//��������
	//		for (WORD i=0;i<wItemCount;i++)
	//		{
	//			pServerListData->InsertGameServer(pGameServer++);
	//		}

	//		return true;
	//	}
	//case SUB_GP_LIST_FINISH:	//�б���Ϣ
	//	{
	//		//�б����
	//		ASSERT(CServerListData::shared()!=NULL);
	//		if (CServerListData::shared()!=NULL) CServerListData::shared()->OnEventListFinish();

	//		return true;
	//	}
	//case SUB_GP_SERVER_FINISH:	//�������
	//	{
	//		//Ч������
	//		ASSERT(dataSize%sizeof(WORD)==0);
	//		if (dataSize%sizeof(WORD)!=0) return false;

	//		//��ȡ����
	//		ASSERT(CServerListData::shared()!=NULL);
	//		CServerListData * pServerListData=CServerListData::shared();

	//		//������
	//		for (UINT i=0;i<(dataSize/sizeof(WORD));i++)
	//		{
	//			m_KindIDPending.RemoveKey(((WORD *)data)[i]);
	//			pServerListData->OnEventKindFinish(((WORD *)data)[i]);
	//		}

	//		stop();

	//		return true;
	//	}
	case SUB_GR_KINE_ONLINE:	//��������
		{
			//��������
			CMD_GP_KindOnline * pKindOnline=(CMD_GP_KindOnline *)data;
			word wHeadSize=(sizeof(CMD_GP_KindOnline)-sizeof(pKindOnline->OnLineInfoKind));

			//Ч������
			ASSERT((dataSize>=wHeadSize)&&(dataSize==(wHeadSize+pKindOnline->wKindCount*sizeof(tagOnLineInfoKind))));
			if ((dataSize<wHeadSize)||(dataSize!=(wHeadSize+pKindOnline->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

			//��ȡ����
			CServerListData * pServerListData=CServerListData::shared();

			//��������
			for (word i=0;i<pKindOnline->wKindCount;i++)
			{
				tagOnLineInfoKind * pOnLineInfoKind=&pKindOnline->OnLineInfoKind[i];
				pServerListData->SetKindOnLineCount(pOnLineInfoKind->wKindID,pOnLineInfoKind->dwOnLineCount);
			}

			return true;
		}
	case SUB_GR_SERVER_ONLINE:	//��������
		{
			//��������
			CMD_GP_ServerOnline * pServerOnline=(CMD_GP_ServerOnline *)data;
			word wHeadSize=(sizeof(CMD_GP_ServerOnline)-sizeof(pServerOnline->OnLineInfoServer));

			//Ч������
			ASSERT((dataSize>=wHeadSize)&&(dataSize==(wHeadSize+pServerOnline->wServerCount*sizeof(tagOnLineInfoServer))));
			if ((dataSize<wHeadSize)||(dataSize!=(wHeadSize+pServerOnline->wServerCount*sizeof(tagOnLineInfoServer)))) return false;

			//��ȡ����
			CServerListData * pServerListData=CServerListData::shared();

			//��������
			for (word i=0;i<pServerOnline->wServerCount;i++)
			{
				tagOnLineInfoServer * pOnLineInfoServer=&pServerOnline->OnLineInfoServer[i];
				pServerListData->SetServerOnLineCount(pOnLineInfoServer->wServerID,pOnLineInfoServer->dwOnLineCount);
			}

			//���֪ͨ
			pServerListData->SetServerOnLineFinish();

			stop();
			return true;
		}
	}

	return true;
}

//��������
void CGPRoomListOnLineMission::UpdateOnLineInfo()
{
	start();
	return;
}
