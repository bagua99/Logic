#include <assert.h>
#include <math.h>
#include "GameLogic.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////

// �˿�����
const BYTE CGameLogic::m_byCardDataArray[MAX_REPERTORY] =
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						// ����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						// ����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						// ����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						// ����
																		   
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						// ����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						// ����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						// ����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						// ����
																		   
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						// ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						// ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						// ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						// ͬ��
																		   
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									// ���������з���
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									// ���������з���
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									// ���������з���
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									// ���������з���
};

//////////////////////////////////////////////////////////////////////////

// ���캯��
CGameLogic::CGameLogic()
{
	m_byKingCardValue = 0;
	m_byKingCardIndex = 0;
	m_HuPaiLogic.InitCard();
}

// ��������
CGameLogic::~CGameLogic()
{
}

// �����˿�
void CGameLogic::RandCardData(BYTE byCardData[], BYTE byMaxCount)
{
	// ����׼��
	BYTE byCardDataTemp[MAX_REPERTORY] = {0};
	memcpy(byCardDataTemp, m_byCardDataArray, sizeof(m_byCardDataArray));

	// �����˿�
	BYTE byRandCount = 0;
	BYTE byPosition = 0;
	do
	{
		byRandCount = rand() % (byMaxCount - byRandCount);
		byCardData[byRandCount++] = byCardDataTemp[byPosition];
		byCardDataTemp[byPosition] = byCardDataTemp[byMaxCount - byRandCount];
	} while (byRandCount < byMaxCount);
}

// ɾ���˿�
bool CGameLogic::RemoveCard(BYTE byCardIndex[MAX_INDEX], BYTE byRemoveCard)
{
	if (!IsValidCommonCard(byRemoveCard))
	{
		return false;
	}

	// ɾ���˿�
	BYTE byRemoveIndex = SwitchToCardIndex(byRemoveCard);
	if (byCardIndex[byRemoveIndex] <= 0)
	{
		return false;
	}

	byCardIndex[byRemoveIndex]--;
	return true;
}

// ɾ���˿�
bool CGameLogic::RemoveCard(BYTE byCardIndex[MAX_INDEX], BYTE byRemoveCard[], BYTE byRemoveCount)
{
	// ��ֵ��ʱ�ģ�����У��
	BYTE byTempIndex[MAX_INDEX] = { 0 };
	memcpy(byTempIndex, byCardIndex, sizeof(byTempIndex));
	for (BYTE i = 0; i < byRemoveCount; i++)
	{
		BYTE byCard = byRemoveCard[i];
		if (!IsValidCommonCard(byCard))
		{
			return false;
		}

		BYTE byIndex = SwitchToCardIndex(byCard);
		if (byTempIndex[byIndex] <= 0)
		{
			return false;
		}

		--byTempIndex[byIndex];
	}

	// ����ɾ���˿�
	for (BYTE i=0; i<byRemoveCount; i++)
	{
		// ɾ���˿�
		--byCardIndex[SwitchToCardIndex(byRemoveCard[i])];
	}

	return true;
}

// ɾ���˿�
bool CGameLogic::RemoveCard(BYTE byCardData[], BYTE byCardCount, BYTE byRemoveCard[], BYTE byRemoveCount)
{
	// ��������
	if (byCardCount > MAX_COUNT || byRemoveCount > byCardCount)
	{
		return false;
	}

	// �������
	BYTE byTempCardData[MAX_COUNT] = {0};
	memcpy(byTempCardData, byCardData, byCardCount*sizeof(BYTE));

	BYTE byDeleteCount = 0;
	// �����˿�
	for (BYTE i=0; i<byRemoveCount; i++)
	{
		for (BYTE j = 0; j < byCardCount; j++)
		{
			if (byRemoveCard[i] == byTempCardData[j])
			{
				byDeleteCount++;
				byTempCardData[j] = 0;
				break;
			}
		}
	}

	// �ɹ��ж�
	if (byDeleteCount != byRemoveCount)
	{
		return false;
	}

	// �����˿�
	BYTE bCardPos = 0;
	for (BYTE i = 0; i < byCardCount; i++)
	{
		if (byTempCardData[i] != 0)
		{
			byCardData[bCardPos++] = byTempCardData[i];
		}
	}

	return true;
}

// �˿���Ŀ
BYTE CGameLogic::GetCardCount(BYTE byCardIndex[MAX_INDEX])
{
	// ��Ŀͳ��
	BYTE byCount = 0;
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		byCount += byCardIndex[i];
	}

	return byCount;
}

// ��ȡ���
BYTE CGameLogic::GetWeaveCardData(BYTE byWeaveKind, BYTE byCenterCard, BYTE byCardBuffer[MAX_WEAVE_CARD_COUNT])
{
	// ����˿�
	switch (byWeaveKind)
	{
	case ACK_LEFT:		// ���Ʋ���
		{
			// ���ñ���
			byCardBuffer[0] = byCenterCard;
			byCardBuffer[1] = byCenterCard + 1;
			byCardBuffer[2]  =byCenterCard + 2;

			return 3;
		}
		break;

	case ACK_RIGHT:		// ���Ʋ���
		{
			// ���ñ���
			byCardBuffer[0] = byCenterCard;
			byCardBuffer[1] = byCenterCard - 1;
			byCardBuffer[2] = byCenterCard - 2;

			return 3;
		}
		break;

	case ACK_CENTER:	// ���Ʋ���
		{
			// ���ñ���
			byCardBuffer[0] = byCenterCard;
			byCardBuffer[1] = byCenterCard - 1;
			byCardBuffer[2] = byCenterCard + 1;

			return 3;
		}
		break;

	case ACK_PENG:		// ���Ʋ���
		{
			// ���ñ���
			byCardBuffer[0] = byCenterCard;
			byCardBuffer[1] = byCenterCard;
			byCardBuffer[2] = byCenterCard;

			return 3;
		}
		break;

	case ACK_GANG:		// ���Ʋ���
		{
			// ���ñ���
			byCardBuffer[0] = byCenterCard;
			byCardBuffer[1] = byCenterCard;
			byCardBuffer[2] = byCenterCard;
			byCardBuffer[3] = byCenterCard;

			return 4;
		}
		break;

	default:
		break;
	}

	return 0;
}


// ��������
BYTE CGameLogic::GetEatCardKind(BYTE byCardIndex[MAX_INDEX], BYTE byCurrentCard)
{
	if(IsKingCardData(byCurrentCard))
	{
		return ACK_NULL;
	}

	// �����ж�
	if (byCurrentCard >= 0x31) 
	{
		return ACK_NULL;
	}

	// ��������
	BYTE byExcursion[3] = {0, 1, 2};
	BYTE byItemKind[3] = {ACK_LEFT, ACK_CENTER, ACK_RIGHT};

	// �����ж�
	BYTE bEatKind = 0;
	BYTE byFirstIndex = 0;
	BYTE byCurrentIndex = SwitchToCardIndex(byCurrentCard);
	for (BYTE i = 0; i < CountArray(byItemKind); i++)
	{
		BYTE byValueIndex = byCurrentIndex % 9;
		if (byValueIndex >= byExcursion[i] && byValueIndex - byExcursion[i] <= 6)
		{
			// �����ж�
			byFirstIndex = byCurrentIndex - byExcursion[i];
			if (byCurrentIndex != byFirstIndex && byCardIndex[byFirstIndex] == 0)
			{
				continue;
			}
			if (byCurrentIndex != byFirstIndex + 1 && byCardIndex[byFirstIndex + 1] == 0)
			{
				continue;
			}
			if (byCurrentIndex != byFirstIndex + 2 && byCardIndex[byFirstIndex + 2] == 0)
			{
				continue;
			}

			// ��������
			bEatKind |= byItemKind[i];
		}
	}

	return bEatKind;
}

// ��������
BYTE CGameLogic::GetPengCardKind(BYTE byCardIndex[MAX_INDEX], BYTE byCurrentCard)
{
	if(IsKingCardData(byCurrentCard))
	{
		return ACK_NULL;
	}

	if (!IsValidCommonCard(byCurrentCard))
	{
		return ACK_NULL;
	}

	return (byCardIndex[SwitchToCardIndex(byCurrentCard)] >= 2) ? ACK_PENG : ACK_NULL;
}

// ��������
BYTE CGameLogic::GetGangCardKind(BYTE byCardIndex[MAX_INDEX], BYTE byCurrentCard)
{
	if(IsKingCardData(byCurrentCard))
	{
		return ACK_NULL;
	}

	if (!IsValidCommonCard(byCurrentCard))
	{
		return ACK_NULL;
	}

	return (byCardIndex[SwitchToCardIndex(byCurrentCard)] >= 3) ? ACK_GANG : ACK_NULL;
}

// ����״̬
bool CGameLogic::IsTingPaiStatus(BYTE byCardIndex[MAX_INDEX])
{
	// �����ƴ��룬�ж���
	tagHuInput HuInput;
	HuInput.bJudgeKingWait = false;
	HuInput.bInsertCurrentCard = true;
	HuInput.byCurrentCard = m_byKingCardValue;
	HuInput.pCardIndexSeq = byCardIndex;

	if (CGameLogic::GetHuInfo(&HuInput))
	{
		return true;
	}

	return false;
}

// ������Ϣ
BYTE CGameLogic::GetGangCardInfo(BYTE byCardIndex[MAX_INDEX], tagWeaveItem *pWeaveItem, BYTE byItemCount, BYTE byGangCard[MAX_WEAVE])
{
	// ���ñ���
	BYTE byGangCount = 0;
	memset(byGangCard, 0, sizeof(byGangCard));

	// ���ظ���
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (i == m_byKingCardIndex)
		{
			continue;
		}

		if (byGangCount >= MAX_WEAVE)
		{
			break;
		}

		if (byCardIndex[i] >= 4)
		{
			byGangCard[byGangCount++] = SwitchToCardData(i);
		}
	}

	// ��ϸ���
	for (BYTE i = 0; i < byItemCount; i++)
	{
		if (byGangCount >= MAX_WEAVE)
		{
			break;
		}

		if (pWeaveItem[i].byWeaveKind == ACK_PENG)
		{
			BYTE byCenterIndex = SwitchToCardIndex(pWeaveItem[i].byCenterCard);
			if (byCardIndex[byCenterIndex] == 1)
			{
				byGangCard[byGangCount++] = pWeaveItem[i].byCenterCard;
			}
		}
	}

	return byGangCount;
}

// �˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE byCardIndex)
{
	if (byCardIndex >= MAX_INDEX)
	{
		return 0;
	}

	// �ж�����
	if(IsKingCardIndex(byCardIndex))
	{
		return m_byKingCardValue;
	}

	return ((byCardIndex/9)<<4)|(byCardIndex%9+1);
}

// �˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE byCardData)
{
	if (!IsValidCommonCard(byCardData))
	{
		return MAX_INDEX;
	}

	// �ж�����
	if(IsKingCardData(byCardData))
	{
		return m_byKingCardIndex;
	}

	return ((byCardData&MASK_COLOR) >> 4)*9 + (byCardData&MASK_VALUE) - 1;
}

// �˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE byCardIndex[MAX_INDEX], BYTE byCardData[], BYTE bMaxCount)
{
	// ת���˿� ���Ʒŵ���ǰ��
	BYTE bPosition = 0;
	if(byCardIndex[m_byKingCardIndex] > 0)
	{
		for (BYTE j = 0; j < byCardIndex[m_byKingCardIndex]; j++)
		{
			if (bPosition >= bMaxCount)
			{
				break;
			}
			byCardData[bPosition++] = SwitchToCardData(m_byKingCardIndex);
		}
	}
	
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (byCardIndex[i] != 0)
		{
			for (BYTE j = 0; j < byCardIndex[i]; j++)
			{
				if (bPosition >= bMaxCount)
				{
					break;
				}
				byCardData[bPosition++] = SwitchToCardData(i);
			}
		}
	}

	return bPosition;
}

// �˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE byCardData[], BYTE bCardCount, BYTE byCardIndex[MAX_INDEX])
{
	// ���ñ���
	memset(byCardIndex, 0, sizeof(byCardIndex));

	// ת���˿�
	for (BYTE i = 0; i < bCardCount; i++)
	{
		byCardIndex[SwitchToCardIndex(byCardData[i])]++;
	}

	return bCardCount;
}

// ��Ч�ж�
bool CGameLogic::IsValidCommonCard(BYTE byCardData)
{
	if (byCardData == m_byKingCardValue)
	{
		return true;
	}

	BYTE byValue = (byCardData&MASK_VALUE);
	BYTE byColor = (byCardData&MASK_COLOR) >> 4;

	return ((byValue >= 1 && byValue <= 9 && byColor <= 2) || (byValue >= 1 && byValue <= 7 && byColor == 3));
}

// �ж�����
void CGameLogic::SetKingCardData(BYTE byCardData)
{
	m_byKingCardValue = byCardData;
	m_byKingCardIndex = ((byCardData&MASK_COLOR) >> 4) * 9 + (byCardData&MASK_VALUE) - 1;
}

// ��ȡ����
BYTE CGameLogic::GetKingCardData()
{
	return m_byKingCardValue;
}

// ��ȡ��������
BYTE CGameLogic::GetKingCardIndex()
{
	return m_byKingCardIndex;
}

// �ж�����
bool CGameLogic::IsKingCardData(BYTE byCardData)
{
	return (byCardData == m_byKingCardValue ? true : false);
}

// �ж���������
bool CGameLogic::IsKingCardIndex(BYTE byCardIndex)
{
	return (byCardIndex == m_byKingCardIndex ? true : false);
}

bool CGameLogic::GetHuInfo(tagHuInput *pInput)
{
	// Ч������
	if(pInput == NULL)
	{
		return false;
	}

	BYTE byCardIndexTmp[MAX_INDEX] = {0};
	memcpy(byCardIndexTmp, pInput->pCardIndexSeq, sizeof(byCardIndexTmp));

	tagHuInput HuInput = *pInput;
	HuInput.pCardIndexSeq = byCardIndexTmp;
	
	if(pInput->bInsertCurrentCard)
	{
		if (pInput->byCurrentCard == 0)
		{
			return false;
		}

		// ������
		byCardIndexTmp[SwitchToCardIndex(pInput->byCurrentCard)]++;
	}

	CardData cardData;
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		for (BYTE j = 0; j < byCardIndexTmp[i]; j++)
		{
			BYTE byCard = SwitchToCardData(i);
			cardData.AddCard(byCard);
		}
	}
	
	CardAnswer cardAnswer;
	bool bCanHu = false;
	BYTE byJiangCount = 0;
	// �ж�������
	do
	{
		BYTE byCard = pInput->byCurrentCard;
		if (byCard == 0)
		{
			break;
		}
		
		BYTE byCardIndex_0 = cardData.GetCardIndex(byCard);
		BYTE byCardCount_0 = cardData.GetCardIndexArrayCountInIndex(byCardIndex_0);
		BYTE *pCardIndexArray_0 = cardData.GetCardIndexArrayInIndex(byCardIndex_0);
		if (byCardCount_0 <= 0)
		{
			break;
		}

		BYTE byCardCount_King = cardData.GetCardIndexArrayCountInIndex(m_byKingCardIndex);
		BYTE *pCardIndexArray_King = cardData.GetCardIndexArrayInIndex(m_byKingCardIndex);
		if (byCardCount_King <= 0)
		{
			break;
		}

		BYTE cbKeyIndex[MAX_WEAVE_CARD_COUNT] = { 0 };
		cbKeyIndex[0] = pCardIndexArray_0[0];
		// ������һ������,����Ҫ2��������
		if (byCard == m_byKingCardValue)
		{
			if (byCardCount_King <= 1)
			{
				break;
			}
			// ��ʵ������������
			cbKeyIndex[1] = pCardIndexArray_0[1];
		}
		else
		{
			cbKeyIndex[1] = pCardIndexArray_King[0];
		}

		pInput->pWeaveCardItemSeq[pInput->byWeaveCardItemCount].byCard[0] = byCard;
		pInput->pWeaveCardItemSeq[pInput->byWeaveCardItemCount].byCard[1] = m_byKingCardValue;
		pInput->byWeaveCardItemCount++;
		byJiangCount = pInput->byWeaveCardItemCount;

		CardKey key(KING_DIAO_KEY, cbKeyIndex[0], cbKeyIndex[1]);
		cardAnswer.AddCardData(&key, &cardData);
		bCanHu = m_HuPaiLogic.InitHuCard(&cardData, &cardAnswer, m_byKingCardValue, m_byKingCardIndex, byJiangCount);
		if (!bCanHu)
		{
			cardAnswer.RemoveCardData(&cardData);
		}

	} while (false);

	// û�����������ж�������
	if (!bCanHu)
	{
		for (BYTE i = 0; i < MAX_ANSWER_WEAVE; i++)
		{
			memset(&pInput->pWeaveCardItemSeq[i], 0, sizeof(tagWeaveCardItem));
		}
		pInput->byWeaveCardItemCount = 0;
		byJiangCount = 0;
		cardAnswer.Clear();

		if (!m_HuPaiLogic.InitHuCard(&cardData, &cardAnswer, m_byKingCardValue, m_byKingCardIndex, byJiangCount))
		{
			return false;
		}
	}

	BYTE byCardKeyCount = cardAnswer.GetCardKeyCount();
	pInput->byWeaveCardItemCount = byCardKeyCount;
	for (BYTE i = 0; i < byCardKeyCount; ++i)
	{
		CardKey *pCardKey = cardAnswer.GetCardKey(i);
		if (pCardKey == NULL)
		{
			continue;
		}

		BYTE byCardCount = 0;
		// ȡ������������
		BYTE byCardIndexCount = pCardKey->GetCardIndexCount();
		for (BYTE j = 0; j < byCardIndexCount; j++)
		{
			// ȡ��������
			BYTE byCardIndex = pCardKey->GetCardIndex(j);
			pInput->pWeaveCardItemSeq[i].byCard[byCardCount++] = cardData.GetCardValue(byCardIndex);
		}
	}

	return true;
}

// 7�Ժ�
bool CGameLogic::_GetHuInfo7Dui(tagHuInput* pInput, bool &bKingWaitHu)
{
	if (pInput->byWeaveCardItemCount != 0)
	{
		return false;
	}

	BYTE byCardIndex[MAX_INDEX] = { 0 };
	memcpy(byCardIndex, pInput->pCardIndexSeq, sizeof(byCardIndex));
	int nKingCount = byCardIndex[m_byKingCardIndex];
	int nOnlyCount = 0;
	for (BYTE i = 0; i < m_byKingCardIndex; i++)
	{
		if (byCardIndex[i] != 0 && byCardIndex[i] % 2 != 0)
		{
			++nOnlyCount;
		}
	}

	// ����С�ڵ�������
	if (nKingCount < nOnlyCount)
	{
		return false;
	}

	// ����ת��
	BYTE byCurrentIndex = SwitchToCardIndex(pInput->byCurrentCard);
	// ��ȥ������
	--byCardIndex[byCurrentIndex];
	nOnlyCount = 0;
	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (i == m_byKingCardIndex)
		{
			continue;
		}

		if (byCardIndex[i] != 0 && (byCardIndex[i] % 2 != 0))
		{
			++nOnlyCount;
		}
	}

	if (nKingCount >= nOnlyCount + 1)
	{
		bKingWaitHu = true;
	}

	return true;
}