#ifndef PACKET_AIDE_HEAD_FILE
#define PACKET_AIDE_HEAD_FILE

#include "types.h"

#pragma once

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

//���ݶ���
#define DTP_NULL					0								//��Ч����

//��������
struct tagDataDescribe
{
	word							wDataSize;						//���ݴ�С
	word							wDataDescribe;					//��������
};

//////////////////////////////////////////////////////////////////////////////////

//���͸�����
class CSendPacketHelper
{
	//��������
protected:
	word							m_wDataSize;					//���ݴ�С
	word							m_wMaxBytes;					//�����С
	byte*							m_pcbBuffer;					//����ָ��

	//��������
public:
	//���캯��
	inline CSendPacketHelper(void * pcbBuffer, word wMaxBytes);

	//���ܺ���
public:
	//��������
	inline void CleanData() { m_wDataSize=0; }
	//��ȡ��С
	inline word GetDataSize() { return m_wDataSize; }
	//��ȡ����
	inline void * GetDataBuffer() { return m_pcbBuffer; }

	//���ܺ���
public:
	//�����ַ�
	inline bool AddPacket(char* pszString , word wDataType);
	//��������
	inline bool AddPacket(void * pData, word wDataSize, word wDataType);
};

//////////////////////////////////////////////////////////////////////////////////

//���ո�����
class CRecvPacketHelper
{
	//��������
protected:
	word							m_wDataPos;						//���ݵ�
	word							m_wDataSize;					//���ݴ�С
	byte*							m_pcbBuffer;					//����ָ��

	//��������
public:
	//���캯��
	inline CRecvPacketHelper(void * pcbBuffer, word wDataSize);

	//���ܺ���
public:
	//��ȡ����
	inline void * GetData(tagDataDescribe & DataDescribe);
};

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSendPacketHelper::CSendPacketHelper(void * pcbBuffer, word wMaxBytes)
{
	//���ñ���
	m_wDataSize=0;
	m_wMaxBytes=wMaxBytes;
	m_pcbBuffer=(byte *)pcbBuffer;

	return;
}

//�����ַ�
bool CSendPacketHelper::AddPacket(char* pszString,word wDataType)
{
	//�����ж�
	ASSERT(pszString!=NULL);
	if (pszString[0]==0) return true;

	//��������
	return AddPacket((void *)pszString,strlen(pszString),wDataType);
}

//��������
bool CSendPacketHelper::AddPacket(void * pData, word wDataSize, word wDataType)
{
	//Ч������
	ASSERT(wDataType!=DTP_NULL);
	ASSERT((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)<=m_wMaxBytes);
	if ((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)>m_wMaxBytes) return false;

	//��������
	ASSERT(m_pcbBuffer!=NULL);
	tagDataDescribe * pDataDescribe=(tagDataDescribe *)(m_pcbBuffer+m_wDataSize);
	pDataDescribe->wDataSize=wDataSize;
	pDataDescribe->wDataDescribe=wDataType;

	//��������
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		memcpy(pDataDescribe+1,pData,wDataSize);
	}

	//��������
	m_wDataSize+=sizeof(tagDataDescribe)+wDataSize;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CRecvPacketHelper::CRecvPacketHelper(void* pcbBuffer, word wDataSize)
{
	//���ñ���
	m_wDataPos=0;
	m_wDataSize=wDataSize;
	m_pcbBuffer=(byte *)pcbBuffer;

	return;
}

//��ȡ����
void * CRecvPacketHelper::GetData(tagDataDescribe & DataDescribe)
{
	//Ч������
	if (m_wDataPos>=m_wDataSize) 
	{
		ASSERT(m_wDataPos==m_wDataSize);
		DataDescribe.wDataSize=0;
		DataDescribe.wDataDescribe=DTP_NULL;
		return NULL;
	}

	//��ȡ����
	ASSERT((m_wDataPos+sizeof(tagDataDescribe))<=m_wDataSize);
	memcpy(&DataDescribe,m_pcbBuffer+m_wDataPos,sizeof(tagDataDescribe));
	ASSERT((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)<=m_wDataSize);

	//Ч������
	if ((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)>m_wDataSize)
	{
		DataDescribe.wDataSize=0;
		DataDescribe.wDataDescribe=DTP_NULL;
		return NULL;
	}

	//��������
	void * pData=NULL;
	if (DataDescribe.wDataSize>0) pData=m_pcbBuffer+m_wDataPos+sizeof(tagDataDescribe);
	m_wDataPos+=sizeof(tagDataDescribe)+DataDescribe.wDataSize;

	return pData;
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif