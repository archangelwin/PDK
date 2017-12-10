#ifndef PDKGAMELOGIC_H
#define PDKGAMELOGIC_H

#pragma once

#include "CMD_PDK.h"
#include "Game/Game/GameDefine.h"
#include "Game/FV/FvSingleton.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0

#pragma once

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_SINGLE_LINE				2									//��������
#define CT_DOUBLE_LINE				3									//��������
#define CT_THREE_LINE				4									//��������
#define CT_THREE_LINE_TAKE_SINGLE	     5									//����һ��
#define CT_THREE_LINE_TAKE_DOUBLE	     6									//����һ��
#define CT_BOMB					7									//ը������
#define CT_FOUR_LINE_TAKE_THREE		8									//�Ĵ�������

//�궨��
#define MAX_COUNT					16									//�����Ŀ

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							bFourCount;							//������Ŀ
	BYTE 							bThreeCount;						//������Ŀ
	BYTE 							bDoubleCount;						//������Ŀ
	BYTE							bSignedCount;						//������Ŀ
	BYTE 							bFourLogicVolue[4];					//�����б�
	BYTE 							bThreeLogicVolue[5];				//�����б�
	BYTE 							bDoubleLogicVolue[8];				//�����б�
	BYTE 							bSignedLogicVolue[16];				//�����б�
	BYTE							bFourCardData[MAX_COUNT];			//�����б�
	BYTE							bThreeCardData[MAX_COUNT];			//�����б�
	BYTE							bDoubleCardData[MAX_COUNT];			//�����б�
	BYTE							bSignedCardData[MAX_COUNT];			//������Ŀ
};



//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class PDKGameLogic:public FvSingleton<PDKGameLogic>
{
	//��������
protected:
	static const BYTE				m_cbCardListData[48];				//�˿�����

	//��������
public:
	//���캯��
	PDKGameLogic();
	//��������
	virtual ~PDKGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount);
	//��ȡ����
	BYTE GetCardType(const std::vector<BYTE>& cbCardList);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return (bCardData&LOGIC_MASK_COLOR)>>4; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount);
	void SortCardList(std::vector<BYTE>& cbCardList);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//�����˿�
	void RandCardList(std::vector<BYTE>& cbCardList);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//ɾ���˿�
	bool RemoveCard(const std::vector<BYTE>& kRemoveCardList, std::vector<BYTE>& kCardList);

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE bCardData);

	//���ܺ���
public:
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//�Ա��˿�
	bool CompareCard(const std::vector<BYTE>& kFirstList , const std::vector<BYTE>& kNextList);
	//�����˿�
	void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);

	//�����˿�
	void AnalysebCardData(const std::vector<BYTE>& cbCardList, tagAnalyseResult & AnalyseResult);

	//�����ж�
	bool SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, tagOutCardResult & OutCardResultList);

	//�����ж�
	bool SearchOutCard(const std::vector<BYTE>& cbCardList, const std::vector<BYTE>& cbTurnCardList, BYTE bTurnOutType, std::vector<tagOutCardResult> & OutCardResultList);
};

//////////////////////////////////////////////////////////////////////////
#endif	
