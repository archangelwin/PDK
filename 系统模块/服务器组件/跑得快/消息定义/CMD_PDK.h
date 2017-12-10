#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE
#include <vector>

//////////////////////////////////////////////////////////////////////////
//�����궨��
#pragma pack(1)

#define GAME_RULE_SUIJIZHUANG		1
#define GAME_RULE_QIANGZHUANG		2
#define GAME_RULE_NOT_WAITE			3

#define KIND_ID							28									//��Ϸ I D
#define GAME_PLAYER						3									//��Ϸ����
#define GAME_NAME						TEXT("�ܵÿ�")						//��Ϸ����
#define MAX_COUNT						16									//�����Ŀ

#define VERSION_SERVER					PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT					PROCESS_VERSION(6,0,3)				//����汾

//����ԭ��
#define GER_NO_PLAYER					0x10								//û�����

//��Ϸ״̬
#define GS_TK_FREE						GAME_STATUS_FREE					//�ȴ���ʼ
#define GS_TK_PLAYING						GAME_STATUS_PLAY					//��ׯ״̬
#define  GAME_RULE_16ZHANG			      1
#define  GAME_RULE_15ZHANG			      2
#define  GAME_RULE_SHOUCHU_HEITAO3        3
#define  GAME_RULE_TWO_GAME               5
#define  GAME_RULE_THREE_GAME             6
#define  GAME_RULE_FOUR_LINE_THREE        7

#define  MAX_GAME_RULE				      4

#define SERVER_LEN						32 

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_GAME_START			101									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				102									//�û�����
#define SUB_S_PASS_CARD			    103									//��������
#define SUB_S_GAME_END				104									//��Ϸ����

//��Ϸ״̬
struct PDK_CMD_S_StatusFree
{
	LONGLONG							lCellScore;							//��������

	//��ʷ����
	LONGLONG							lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG							lCollectScore[GAME_PLAYER];			//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};
#pragma pack()

struct PDK_CardListInfo
{
	WORD  wChairID;
	std::vector<BYTE> cbCardList;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wChairID);
		Stream_VALUE(cbCardList);
	}
};

//��Ϸ״̬
struct PDK_CMD_S_StatusPlay
{
	WORD							wBankerUser;						//ׯ���û�
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	std::vector<BYTE>				cbLastOutCardList;
	std::vector<SCORE>              wlGameScores;
	std::vector<PDK_CardListInfo>  kHandCardInfo;					//����

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wBankerUser);
		Stream_VALUE(wLastOutUser);
		Stream_VALUE(wCurrentUser);
		Stream_VALUE(wlGameScores);
		Stream_VALUE(cbLastOutCardList);
		StructVecotrMember(PDK_CardListInfo,kHandCardInfo);
	}

};

//��Ϸ��ʼ
struct PDK_CMD_S_GameStart
{
	WORD								wBankerUser;						//ׯ���û�
	WORD				 				wCurrentUser;						//��ǰ���
	WORD								wChairID;
	std::vector<SCORE>                  wlGameScores;
	std::vector<BYTE>					cbHandCardList;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wBankerUser);
		Stream_VALUE(wCurrentUser);
		Stream_VALUE(wChairID);
		Stream_VALUE(wlGameScores);
		Stream_VALUE(cbHandCardList);
	}
};

struct PDK_CMD_S_OutCard
{
	WORD				 		wCurrentUser;						//��ǰ���
	WORD						wOutCardUser;						//�������
	BYTE                    cbCardType;
	std::vector<BYTE>        cbOutCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wCurrentUser);
		Stream_VALUE(wOutCardUser);
		Stream_VALUE(cbCardType);
		Stream_VALUE(cbOutCardList);
	}
};

//��������
struct PDK_CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(bNewTurn);
		Stream_VALUE(wPassUser);
		Stream_VALUE(wCurrentUser);
	}

};

struct PDK_EndInfo
{
	WORD  wChairID;
	SCORE lGameScore;
	BYTE  nBombCount;
	std::vector<BYTE> cbCardList;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wChairID);
		Stream_VALUE(lGameScore);
		Stream_VALUE(nBombCount);
		Stream_VALUE(cbCardList);
	}
};

struct PDK_CMD_S_GameEnd
{
	SCORE lGameTax;
	std::vector<PDK_EndInfo> kEndInfoList;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(lGameTax);
		StructVecotrMember(PDK_EndInfo,kEndInfoList);
	}
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_OutCard					1									//�û���ׯ
#define SUB_C_PASS_CARD				3									//��������

struct PDK_CMD_C_OutCard
{
	BYTE                    cbCardType;
	std::vector<BYTE>        cbOutCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbCardType);
		Stream_VALUE(cbOutCardList);
	}
};
#endif