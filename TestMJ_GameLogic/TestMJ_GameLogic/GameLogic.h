#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "GameMessage.h"
#include "HuPaiLogic.h"

// ��Ϸ�߼���
class CGameLogic
{
	// ��������
public:
	// ���캯��
	CGameLogic();
	// ��������
	virtual ~CGameLogic();

	//���ƺ���
public:
	// �����˿�
	void RandCardData(BYTE byCardData[], BYTE byMaxCount);
	// ɾ���˿�
	bool RemoveCard(BYTE byCardIndex[MAX_INDEX], BYTE byRemoveCard);
	//ɾ���˿�
	bool RemoveCard(BYTE byCardIndex[MAX_INDEX], BYTE byRemoveCard[], BYTE bRemoveCount);
	// ɾ���˿�
	bool RemoveCard(BYTE byCardData[], BYTE bCardCount, BYTE byRemoveCard[], BYTE bRemoveCount);

	// ��������
public:
	// ��Ч�ж�
	bool IsValidCommonCard(BYTE byCardData);
	// �ж�����
	void SetKingCardData(BYTE byCardData);
	// ��ȡ����
	BYTE GetKingCardData();
	// ��ȡ����
	BYTE GetKingCardIndex();
	// �ж�����
	bool IsKingCardData(BYTE byCardData);
	// �ж�������
	bool IsKingCardIndex(BYTE byCardIndex);
	// �˿���Ŀ
	BYTE GetCardCount(BYTE byCardIndex[MAX_INDEX]);
	// ��ȡ���
	BYTE GetWeaveCardData(BYTE byWeaveKind, BYTE byCenterCard, BYTE byCardBuffer[MAX_WEAVE_CARD_COUNT]);
	
	// ��������
public:
	// ��������
	BYTE GetEatCardKind(BYTE byCardIndex[MAX_INDEX], BYTE byCurrentCard);
	// ��������
	BYTE GetPengCardKind(BYTE byCardIndex[MAX_INDEX], BYTE byCurrentCard);
	// ��������
	BYTE GetGangCardKind(BYTE byCardIndex[MAX_INDEX], BYTE byCurrentCard);
	// ������Ϣ
	bool GetHuInfo(tagHuInput *pInput);

	// ��������
public:
	// ����״̬
	bool IsTingPaiStatus(BYTE byCardIndex[MAX_INDEX]);
	// ������Ϣ
	BYTE GetGangCardInfo(BYTE byCardIndex[MAX_INDEX], tagWeaveItem *pWeaveItem, BYTE byItemCount, BYTE byGangCard[MAX_WEAVE]);

	// ת������
public:
	// �˿�ת��
	BYTE SwitchToCardData(BYTE byCardIndex);
	// �˿�ת��
	BYTE SwitchToCardIndex(BYTE byCardData);
	// �˿�ת��
	BYTE SwitchToCardData(BYTE byCardIndex[MAX_INDEX], BYTE byCardData[], BYTE bMaxCount);
	// �˿�ת��
	BYTE SwitchToCardIndex(BYTE byCardData[], BYTE bCardCount, BYTE byCardIndex[MAX_INDEX]);

private:
	// 7�Ժ�
	bool _GetHuInfo7Dui(tagHuInput* pInput, bool &bKingWaitHu);

	// ��������
private:
	static const BYTE				m_byCardDataArray[MAX_REPERTORY];					// �˿�����
	BYTE							m_byKingCardValue;
	BYTE							m_byKingCardIndex;

	HuPaiLogic						m_HuPaiLogic;										// �����߼�
};

#endif