#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "HistoryScore.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////

struct PDKPlayer
{
	WORD             wChairID;
	std::vector<BYTE> kHandCard;
	std::vector<BYTE> kHandCardCost;
	SCORE            lBombScore;
	SCORE            lGameScore;
	SCORE            lGameHighScore;//������߷�
	int              nBombCount;
	int              nBombCountConst;//ը������
	int              nWinNum;//Ӯ�ľ���

	PDKPlayer()
	{
		wChairID = INVALID_CHAIR;
		lBombScore = 0;
		lGameScore = 0;
		lGameHighScore = 0;
		nBombCount = 0;
		nBombCountConst = 0;
		nWinNum = 0;
	}
};


//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
public:
	BYTE							m_cbGameTypeIdex;	//��Ϸ����
	DWORD                           m_dwGameRule;
	//��Ϸ����
protected:
	WORD							m_wBankerUser;							//ׯ���û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD                            m_currentPlays;                         //��ǰ���� 
	bool							m_bBaoPei;							    //�����־

	PDKPlayer                    m_kPlayers[GAME_PLAYER];
	//�˿˱���
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�

	WORD							m_wTurnWiner;					//ʤ�����
	BYTE							m_bOutCardType;					//��������
	BYTE							m_bOutCardCount;				//������Ŀ
	std::vector<BYTE>				m_bOutCardDataList;				//�����б�
	//���Ʊ���
protected:
	TCHAR							m_szConfigFileName[MAX_PATH];		//�����ļ�
	TCHAR							m_szRoomName[32];					//���÷���

	//AI����
protected:
	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	CHistoryScore					m_HistoryScore;							//��ʷ�ɼ�

	tagGameServiceOption		    *m_pGameServiceOption;					//���ò���
	tagGameServiceAttrib			*m_pGameServiceAttrib;					//��Ϸ����

	//���Ա���
protected:
	WORD				m_wPlayerCount;							//��Ϸ����

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() {}
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	virtual SCORE getPlayScore(int index);//�����û��ܷ���

	virtual WORD getChairID(int index);//�����û�ID

	virtual SCORE getHighScore(int index);//�����û�������߷�

	virtual WORD getWinNum(int index);//�����û�Ӯ������

	virtual WORD getBoombNum(int index);//�����û����ը������
	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void RepositionSink(bool resizeScore = false);

	virtual bool IsUserPlaying(WORD wChairID);
	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);


	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize){return true;}


	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//��ѯ�����
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){return;}


	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	virtual void SetPrivateInfo(BYTE bGameTypeIdex,DWORD bGameRuleIdex);
	virtual void SetCreateUserID(DWORD	dwUserID);

	//��Ϸ�¼�
protected:
	//��ȡ����
	VOID ReadConfig();

	//��ȡ��������ܷ�
	LONGLONG GetAllUserScore(bool bAddRevenue=false);
	//д�ֺ���
	bool TryWriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount);
	
	//��ȡ���Ӻ�
	WORD GetChairIDFromGameID(DWORD dwGameID);

public:
	void initPlayer(bool resizeScore);
	void shufle();
	void sendStartCard();
	bool OnUserOutCard(WORD wChairID,PDK_CMD_C_OutCard pOutCardInfo);
	bool OnUserPassCard(WORD wChairID);
	DWORD getNextPlayer(WORD wChairID);
public:
	bool hasRule(BYTE cbRule);
};

//////////////////////////////////////////////////////////////////////////

#endif
