#include "StdAfx.h"
#include "TableFrameSink.h"
#include "FvMask.h"
#include <conio.h>

//////////////////////////////////////////////////////////////////////////

//��̬����
WORD m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
WORD cardCount;//15�Ż�16��
//////////////////////////////////////////////////////////////////////////

#define	IDI_SO_OPERATE							2							//����ʱ��
#define	TIME_SO_OPERATE							15000						//����ʱ��

//////////////////////////////////////////////////////////////////////////

#define	AI_EX									1

#define	BANKER_CARD                     			0x33
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	m_currentPlays = 0;
	RepositionSink(true);
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{

}

//�ӿڲ�ѯ--��������Ϣ�汾
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	//��Ϸ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();


#ifdef AI_EX
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//��ȡ����
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,sizeof(szFileName),TEXT("%s\\OxServer.ini"),szPath);

	//��ʼ�洢��Ŀ
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,sizeof(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);

#endif

	//�����ļ���
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\OxServer.ini"),szPath);

	//��������
	memcpy(m_szRoomName, m_pGameServiceOption->szServerName, sizeof(m_pGameServiceOption->szServerName));

	ReadConfig();

	return true;
}

//��λ����
void CTableFrameSink::RepositionSink(bool resizeScore)
{
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_bBaoPei=false;
	m_bOutCardType = CT_ERROR;
	m_bOutCardCount = 0;
	m_bOutCardDataList.clear();

	initPlayer(resizeScore);
	return;
}

bool CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserEnter(pIServerUserItem->GetChairID());

	if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
	{

	}

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) 
	{
		m_HistoryScore.OnEventUserLeave(pIServerUserItem->GetChairID());
	}

	return true;
}
void CTableFrameSink::SetPrivateInfo(BYTE bGameTypeIdex,DWORD bGameRuleIdex)
{
	m_cbGameTypeIdex = bGameTypeIdex;
	m_dwGameRule = bGameRuleIdex;
	//�ж��Ƿ�Ϊ�����淨
	bool isTwo = hasRule(GAME_RULE_TWO_GAME);
	bool is16zhang = hasRule(GAME_RULE_16ZHANG);
	if(isTwo)
	{
		m_wPlayerCount = 2;
	}
	else
	{
       m_wPlayerCount = 3;
	}
	if(is16zhang)
	{
		cardCount = 16;
	}
	else
	{
		cardCount = 15;
	}
	m_currentPlays = 1;
	m_GameLogic.setPrivite(cardCount);
}
void CTableFrameSink::SetCreateUserID(DWORD	dwUserID)
{
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	ReadConfig();

	shufle();
	sendStartCard();

	if (m_wCurrentUser == INVALID_CHAIR)
	{
		OnEventGameConclude(m_wBankerUser,NULL,GER_NORMAL);
		return true;
	}
	//����״̬
	m_pITableFrame->SetGameStatus(GS_TK_PLAYING);
	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{

			//�������
			PDK_CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				PDK_EndInfo kInfo;
				kInfo.cbCardList = m_kPlayers[i].kHandCard;
				kInfo.wChairID = i;
				kInfo.nBombCount = m_kPlayers[i].nBombCount;
				kInfo.lGameScore = 0;

				GameEnd.kEndInfoList.push_back(kInfo);
			}

			//ͳ�ƻ���
			LONGLONG lCellScore=m_pGameServiceOption->lCellScore;
			if ((wChairID==m_wBankerUser)&&(m_kPlayers[wChairID].kHandCard.size()==12))
			{

				GameEnd.kEndInfoList[wChairID].lGameScore = 64L*lCellScore;
				GameEnd.kEndInfoList[(wChairID+1)%m_wPlayerCount].lGameScore = -32L*lCellScore;
				GameEnd.kEndInfoList[(wChairID+2)%m_wPlayerCount].lGameScore = -32L*lCellScore;

				//���⴦��
				if (m_bBaoPei==true)
				{
					WORD wBaoPeiUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

					GameEnd.kEndInfoList[wBaoPeiUser].lGameScore = -GameEnd.kEndInfoList[wChairID].lGameScore;
					GameEnd.kEndInfoList[(wBaoPeiUser+2)%m_wPlayerCount].lGameScore = 0;
				}
			}
			else
			{
				//����ͳ��
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					int nCardCount = m_kPlayers[i].kHandCard.size();
					//��Ϸ����
					if (nCardCount>1)
					{

						LONGLONG lScoreTimes=(nCardCount==cardCount)?2L:1L;

						SCORE nScore = nCardCount*lCellScore*lScoreTimes;

						GameEnd.kEndInfoList[i].lGameScore -= nScore;
						GameEnd.kEndInfoList[wChairID].lGameScore	+=nScore;
					}
				}
				//���⴦��
				if (m_bBaoPei==true)
				{
					WORD wBaoPeiUser=(wChairID+GAME_PLAYER-1)%GAME_PLAYER;
					GameEnd.kEndInfoList[wBaoPeiUser].lGameScore = -GameEnd.kEndInfoList[wChairID].lGameScore;
					GameEnd.kEndInfoList[(wBaoPeiUser+2)%GAME_PLAYER].lGameScore = 0;
				}

			}

			for(int i = 0; i < GAME_PLAYER; ++i)
			{
				m_kPlayers[i].lGameScore += GameEnd.kEndInfoList[i].lGameScore;
			}

			//����ը������
			for (WORD i=0;i<GAME_PLAYER;++i)
			{
				if(GameEnd.kEndInfoList[i].nBombCount > 0)
				{
					int cell = m_wPlayerCount == 2 ? 10 : 20;
					int gamescore = GameEnd.kEndInfoList[i].nBombCount * cell;
					GameEnd.kEndInfoList[i].lGameScore += gamescore;
					m_kPlayers[i].lGameScore += gamescore;

					for(WORD j=0;j<m_wPlayerCount;++j)
					{
					    if(GameEnd.kEndInfoList[i].wChairID != GameEnd.kEndInfoList[j].wChairID)
						{
							int fuScore = GameEnd.kEndInfoList[i].nBombCount * 10;
							GameEnd.kEndInfoList[j].lGameScore -= fuScore;
							m_kPlayers[j].lGameScore -= fuScore;
						}
					}
				}
			}

			for(int i = 0; i < GAME_PLAYER; ++i)
			{
				m_kPlayers[i].nBombCountConst += m_kPlayers[i].nBombCount;
				if(m_kPlayers[i].lGameHighScore < GameEnd.kEndInfoList[i].lGameScore)
				{
					m_kPlayers[i].lGameHighScore = GameEnd.kEndInfoList[i].lGameScore;
				}
				if(m_kPlayers[i].wChairID == wChairID)
				{
					++m_kPlayers[i].nWinNum;
				}
			}

			//��������
			LONGLONG lRevenue=0L;
			//ͳ�ƻ���
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				SCORE  lUserScore =  GameEnd.kEndInfoList[i].lGameScore;
				//���û���
				if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0&&lUserScore>0)
				{
					lRevenue =  lUserScore * m_pGameServiceOption->wRevenueRatio/1000;

					GameEnd.kEndInfoList[i].lGameScore =lUserScore-lRevenue;
					GameEnd.lGameTax+=lRevenue;
				}

				BYTE ScoreKind=(GameEnd.kEndInfoList[i].lGameScore>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				ScoreInfo[i].cbType = ScoreKind;
				ScoreInfo[i].lRevenue = lRevenue;
				ScoreInfo[i].lScore = GameEnd.kEndInfoList[i].lGameScore;
				ScoreInfo[i].kHandCardCost = m_kPlayers[i].kHandCardCost;
				
			}

			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			datastream kDataStream;
			GameEnd.StreamValue(kDataStream,true);

				//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&kDataStream[0],kDataStream.size());
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&kDataStream[0],kDataStream.size());

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_TK_FREE);


			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:
		return true;
	case GER_DISMISS:
		{

			//��������
			PDK_CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_TK_FREE);
			return true;
		}
	}

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//��������
			PDK_CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore=0L;

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));


			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{	//��������
			PDK_CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wCurrentUser=m_wCurrentUser;

			std::vector<SCORE> scores;
	        for(int i = 0;i<GAME_PLAYER;++i)
	        {
			    scores.push_back(m_kPlayers[i].lGameScore);
	        }
			StatusPlay.wlGameScores = scores;


			for (int i=0;i<GAME_PLAYER;i++)
			{
				PDK_CardListInfo kInfo;
				kInfo.wChairID = i;
				kInfo.cbCardList = m_kPlayers[i].kHandCard;
				StatusPlay.kHandCardInfo.push_back(kInfo);
			}
			StatusPlay.cbLastOutCardList = m_bOutCardDataList;
			datastream kDataStream;
			StatusPlay.StreamValue(kDataStream,true);

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&kDataStream[0],kDataStream.size());
		}
	}
	//Ч�����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{
	switch(dwTimerID)
	{
	case IDI_SO_OPERATE:
		{
			if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
				m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ���� 
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	bool bResult=false;
	switch (wSubCmdID)
	{
	case SUB_C_OutCard:			//�û���ׯ
		{
			datastream kStream(pDataBuffer,wDataSize);
			PDK_CMD_C_OutCard kOutCardInfo;
			kOutCardInfo.StreamValue(kStream,false);

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;
				
			//��Ϣ����
			return OnUserOutCard(pUserData->wChairID, kOutCardInfo);
		}
	case SUB_C_PASS_CARD:
		{

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;
			return OnUserPassCard(pUserData->wChairID);
		}
	}

	return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;
		
//		if (m_bBuckleServiceCharge[i]&&i==wChairID)
		{
			return true;
		}
		
	}
	return false;
}


bool CTableFrameSink::TryWriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount)
{

	//�޸Ļ���
	tagScoreInfo ScoreInfo[GAME_PLAYER];
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

	memcpy(&ScoreInfo,ScoreInfoArray,sizeof(ScoreInfo));

	//��¼�쳣
	LONGLONG beforeScore[GAME_PLAYER];
	ZeroMemory(beforeScore,sizeof(beforeScore));

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem *pItem=m_pITableFrame->GetTableUserItem(i);
		if(pItem!=NULL)
		{
			beforeScore[i]=pItem->GetUserScore();

		}
	}

	m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));


	LONGLONG afterScore[GAME_PLAYER];
	ZeroMemory(afterScore,sizeof(afterScore));

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem *pItem=m_pITableFrame->GetTableUserItem(i);
		if(pItem!=NULL)
		{
			afterScore[i]=pItem->GetUserScore();

			if(afterScore[i]<0)
			{
				//�쳣д����־

				CString strInfo;
				strInfo.Format(TEXT("[%s] ���ָ���"),pItem->GetNickName());
			    //NcaTextOut(strInfo);

				strInfo.Format(TEXT("д��ǰ������%I64d"),beforeScore[i]);
				//NcaTextOut(strInfo);

				strInfo.Format(TEXT("д����Ϣ��д����� %I64d��˰�� %I64d"),ScoreInfoArray[i].lScore,ScoreInfoArray[i].lRevenue);
				//NcaTextOut(strInfo);

				strInfo.Format(TEXT("д�ֺ������%I64d"),afterScore[i]);
				//NcaTextOut(strInfo);
			}

		}
	}

	return true;

}


//��ȡ����
VOID CTableFrameSink::ReadConfig()
{
}


//��ȡ���Ӻ�
WORD CTableFrameSink::GetChairIDFromGameID(DWORD dwGameID)
{
	for(int i=0; i<GAME_PLAYER; i++)
	{
		IServerUserItem * pUser=m_pITableFrame->GetTableUserItem(i);
		if(pUser==NULL)continue;
		if(pUser->GetGameID()==dwGameID)
		{
			return i;
		}
	}
	return INVALID_CHAIR;
}

void CTableFrameSink::initPlayer(bool resizeScore)
{
	//��ʾ��Ϣ
	for(int i = 0;i<GAME_PLAYER;i++)
	{
		m_kPlayers[i].wChairID = i;
		m_kPlayers[i].kHandCard.clear();
		m_kPlayers[i].kHandCardCost.clear();
		m_kPlayers[i].lBombScore = 0;
		m_kPlayers[i].nBombCount = 0;
		if(resizeScore)
		{
			m_kPlayers[i].lGameHighScore = 0;
			m_kPlayers[i].nWinNum = 0;
			m_kPlayers[i].nBombCountConst = 0;
			m_kPlayers[i].lGameScore = 0;
		}
	}
}

//�����û��ܷ���
SCORE CTableFrameSink::getPlayScore(int index)
{
	return m_kPlayers[index].lGameScore;
}

//�����û�������߷�
SCORE CTableFrameSink::getHighScore(int index)
{
	return m_kPlayers[index].lGameHighScore;
}

//�����û��Ļ���
WORD CTableFrameSink::getChairID(int index)
{
	return m_kPlayers[index].wChairID;
}

//�����û�Ӯ������
WORD CTableFrameSink::getWinNum(int index)
{
	return m_kPlayers[index].nWinNum;
}

//�����û����ը������
WORD CTableFrameSink::getBoombNum(int index)
{
	return m_kPlayers[index].nBombCountConst;
}


void CTableFrameSink::shufle()
{
	std::vector<BYTE> kCardList;
	m_GameLogic.RandCardList(kCardList);
	for (int i=0;i<m_wPlayerCount;i++)
    {
	   m_kPlayers[i].lBombScore=0;
	   m_kPlayers[i].nBombCount=0;
	}

	ASSERT(kCardList.size() == cardCount*GAME_PLAYER);
	if (kCardList.size() != cardCount*GAME_PLAYER)
	{
		return;
	}


	//�׾��жϺ�����������ׯ��
	if(m_currentPlays == 1 && hasRule(GAME_RULE_SHOUCHU_HEITAO3))
	{
		bool bankers = false;
		for (int i=0;i<m_wPlayerCount;i++)
	   {
		  for (int n=0;n<cardCount;n++)
		  {
			  BYTE nCard = kCardList[n+cardCount*i];
			  m_kPlayers[i].kHandCard.push_back(nCard);
			  if (nCard == BANKER_CARD)
			  {
				  m_wCurrentUser = i;
				  m_wBankerUser = i;
				  bankers = true;
			  }
		  }
	   }
		//����������ڵ�������������
		if(!bankers)
		{
			int num=rand()%2;
			for (int n=0;n<cardCount;n++)
		  {
			  BYTE nCard = kCardList[n+cardCount*2];
			  if (nCard == BANKER_CARD)
			  {
				  BYTE temp = m_kPlayers[num].kHandCard[cardCount - 1];
				  m_kPlayers[num].kHandCard.push_back(nCard);
				  kCardList[n+cardCount*2] = temp;
				  m_wCurrentUser = num;
				  m_wBankerUser = num;
			  }
		  }
		}
	}
	//�����׾���Ӯ���ȳ�
	else
	{
		if(m_currentPlays == 1)
		{
			m_wCurrentUser = 0;
		    m_wBankerUser = 0;
		}
		else
		{
			m_wCurrentUser = m_wTurnWiner;
		    m_wBankerUser = m_wTurnWiner;
		}
		 
		// ����ǰ����������ֺ���
		 for (int i=0;i<m_wPlayerCount;i++)
	     {
		    for (int n=0;n<cardCount;n++)
		    {
			    BYTE nCard = kCardList[n+cardCount*i];
			    m_kPlayers[i].kHandCard.push_back(nCard);
		    }
	     }
	}
	
	//�����˿�
	tagAnalyseResult AnalyseResult;
	m_GameLogic.AnalysebCardData(m_kPlayers[m_wCurrentUser].kHandCard,AnalyseResult);

	//���ƴ���
	if ((AnalyseResult.bFourCount>0)&&(AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount-1]==3))
	{
		//�����û�
		m_wCurrentUser=INVALID_CHAIR;

		//ɾ���˿�
		std::vector<BYTE> cbCardData;
		cbCardData.push_back(0x33);
		cbCardData.push_back(0x23);
		cbCardData.push_back(0x13);
		cbCardData.push_back(0x03);
		m_GameLogic.RemoveCard(cbCardData,m_kPlayers[m_wBankerUser].kHandCard);
	}

	//�������(������Ҫ)
	for(int i = 0;i<GAME_PLAYER;++i)
	{
		for(int j = 0;j<m_kPlayers[i].kHandCard.size(); ++j)
		{
			m_kPlayers[i].kHandCardCost.push_back(m_kPlayers[i].kHandCard[j]);
		}
	}

	m_wTurnWiner = INVALID_CHAIR;
	//��ǰ����+1 ���ж��׾֣�
	++m_currentPlays;
}

void CTableFrameSink::sendStartCard()
{
	std::vector<SCORE> scores;
	for(int i = 0;i<GAME_PLAYER;++i)
	{
		scores.push_back(m_kPlayers[i].lGameScore);
	}

	for (int i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(m_kPlayers[i].wChairID);	
		if(pIServerUser==NULL)continue;

		PDK_CMD_S_GameStart kGameStart;
		ZeroMemory(&kGameStart,sizeof(kGameStart));
		kGameStart.wBankerUser = m_wBankerUser;
		kGameStart.wCurrentUser = m_wCurrentUser;

		kGameStart.wlGameScores = scores;

		kGameStart.wChairID		= m_kPlayers[i].wChairID;
		kGameStart.cbHandCardList  = m_kPlayers[i].kHandCard;
		datastream kData;
		kGameStart.StreamValue(kData,true);
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&kData[0],kData.size());
	}
}
bool CTableFrameSink::OnUserOutCard(WORD wChairID,PDK_CMD_C_OutCard kOutCardInfo)
{
	
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(kOutCardInfo.cbOutCardList);
	if (bCardType==CT_ERROR) return false;

	int nCardCount = kOutCardInfo.cbOutCardList.size();
		//�׳��ж�
	if ((m_currentPlays == 1)&&(wChairID==m_wBankerUser)&&(m_kPlayers[wChairID].kHandCard.size() == cardCount)&&hasRule(GAME_RULE_SHOUCHU_HEITAO3))
	{
		int i = 0;
		for (;i<nCardCount;i++)
		{
			if (kOutCardInfo.cbOutCardList[i]==BANKER_CARD) break;
		}
		if (i==nCardCount) return false;
	}
	if (m_bOutCardCount ==0 )
	{
		m_bOutCardCount=nCardCount;
	}
	else if ((m_GameLogic.CompareCard(kOutCardInfo.cbOutCardList,m_bOutCardDataList)==false) )
	{
		return false;
	}
	//ɾ���˿�
	if (m_GameLogic.RemoveCard(kOutCardInfo.cbOutCardList,m_kPlayers[wChairID].kHandCard)==false) 
	{
		return false;
	}

	//�����ж�
	if ((m_kPlayers[wChairID].kHandCard.size() !=0 ))
	{
		//��ȡ�û�
		WORD wNextPlayer=getNextPlayer(wChairID);

		//�����ж�
		if ((nCardCount==1)&&(m_kPlayers[wNextPlayer].kHandCard.size()==1))
		{
			m_bBaoPei=(m_GameLogic.CompareCard(m_kPlayers[wChairID].kHandCard,kOutCardInfo.cbOutCardList)==true);
		}
		else 
		{
			//���ñ���
			m_bBaoPei=false;
		}
	}

	m_bOutCardType = bCardType;
	m_bOutCardCount = nCardCount;
	m_bOutCardDataList = kOutCardInfo.cbOutCardList;

	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_kPlayers[wChairID].kHandCard.size()!=0)
	{
		m_wCurrentUser= getNextPlayer(m_wCurrentUser);
	}
	else
	{
		m_wCurrentUser=INVALID_CHAIR;
	}


	PDK_CMD_S_OutCard kOutCard;
	kOutCard.wCurrentUser = m_wCurrentUser;
	kOutCard.wOutCardUser = wChairID;
	kOutCard.cbCardType = bCardType;
	kOutCard.cbOutCardList = kOutCardInfo.cbOutCardList;

	datastream kDatastream;
	kOutCard.StreamValue(kDatastream,true);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&kDatastream[0],kDatastream.size());
	
	//�����ж�
	if (m_wCurrentUser == INVALID_CHAIR)
	{
		
		OnEventGameConclude(wChairID,NULL,GER_NORMAL);
	}
	
	return true;
}

bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING)
	{
		return true;
	}
	if ((wChairID!=m_wCurrentUser)||(m_bOutCardCount==0))
	{
		return false;
	}
	//�˿˷���
	tagOutCardResult OutCardResult;
	m_GameLogic.SearchOutCard(m_kPlayers[wChairID].kHandCard,m_bOutCardDataList,m_bOutCardType,OutCardResult);
	//�����ж�
	if (OutCardResult.cbCardCount!=0)
	{
		//������ʾ
		TCHAR szString[512]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("Ч��״̬-��������: [ %d ]"),m_bOutCardType);

		//��ʾ��Ϣ
		CTraceService::TraceString(szString,TraceLevel_Exception);
		return false;
	}
	//���ñ���
	m_bBaoPei=false;
	m_wCurrentUser= getNextPlayer(m_wCurrentUser);
	if (m_wCurrentUser==m_wTurnWiner)
	{
		//ը���ж�
		if (m_GameLogic.GetCardType(m_bOutCardDataList)==CT_BOMB)
		{
			++m_kPlayers[m_wTurnWiner].nBombCount;
			m_kPlayers[m_wTurnWiner].lBombScore += 10*m_pGameServiceOption->lCellScore;
			m_kPlayers[(m_wTurnWiner+1)%m_wPlayerCount].lBombScore +=-10*m_pGameServiceOption->lCellScore;
			m_kPlayers[(m_wTurnWiner+2)%m_wPlayerCount].lBombScore +=-10*m_pGameServiceOption->lCellScore;
		}

		//���ñ���
		m_bOutCardCount = 0;
	}

	//��������
	PDK_CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bOutCardCount==0)?TRUE:FALSE;

	datastream kStream;
	PassCard.StreamValue(kStream,true);

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&kStream[0],kStream.size());
	return true;
}

DWORD CTableFrameSink::getNextPlayer(WORD wChairID)
{
	return (wChairID-1 + m_wPlayerCount)%m_wPlayerCount;
}
bool CTableFrameSink::hasRule(BYTE cbRule)
{
	return FvMask::HasAny(m_dwGameRule,_MASK_(cbRule));
}
//////////////////////////////////////////////////////////////////////////
