#ifndef __HUPAI_LOGIC_H__
#define __HUPAI_LOGIC_H__

#pragma once

#include <wtypes.h>
#include <vector>
#include <map>
#include <algorithm>
#include "GameMessage.h"
using namespace std;

// ��������
enum enPlayType
{
	enPlayType_NORMAL = 0,			// һ��
	enPlayType_XDD,					// С���
	enPlayType_27A,					// ����ʮ
	enPlayType_123,					// һ����
	enPlayType_Max,
};

// ���ù���
#define PLAY_TYPE	(1 + (1<<enPlayType_XDD) + (1<<enPlayType_27A) + (1<<enPlayType_123))

struct stHuKeyData
{
	stHuKeyData()
	{
		clear();
	}

	stHuKeyData(BYTE cbHuXi, BYTE cbCardIndex0, BYTE cbCardIndex1, BYTE cbCardIndex2 = MAX_INDEX, BYTE cbCardIndex3 = MAX_INDEX)
	{
		clear();

		m_cbHuXi = cbHuXi;
		m_cbCardIndex[m_cbCardIndexCount++] = cbCardIndex0;
		m_cbCardIndex[m_cbCardIndexCount++] = cbCardIndex1;
		if (cbCardIndex2 < MAX_INDEX)
		{
			m_cbCardIndex[m_cbCardIndexCount++] = cbCardIndex2;
		}
		if (cbCardIndex3 < MAX_INDEX)
		{
			m_cbCardIndex[m_cbCardIndexCount++] = cbCardIndex3;
		}
	}

	void clear()
	{
		memset(&m_cbCardIndex, 0, sizeof(m_cbCardIndex));
		m_cbCardIndexCount = 0;
		m_cbHuXi = 0;
	}

	BYTE getCardIndexCount()
	{
		return m_cbCardIndexCount;
	}

	BYTE getCardIndex(BYTE cbIndex)
	{
		if (cbIndex >= m_cbCardIndexCount)
		{
			return MAX_INDEX;
		}

		return m_cbCardIndex[cbIndex];
	}

	BYTE getHuXi()
	{
		return m_cbHuXi;
	}

private:
	// ������
	BYTE m_cbCardIndex[MAX_WEAVE_CARD_COUNT];
	// ����������
	BYTE m_cbCardIndexCount;
	// ��Ϻ�Ϣ
	BYTE m_cbHuXi;
};


struct stCardData
{
	stCardData()
	{
		m_nFlag = 0;
		memset(&m_cbCard, 0, sizeof(m_cbCard));
		m_cbCardCount = 0;
		memset(&m_cbCardIndex, 0, sizeof(m_cbCardIndex));
		memset(&m_cbCardIndexCount, 0, sizeof(m_cbCardIndexCount));
		memset(&m_cbLeftIndex, 0, sizeof(m_cbLeftIndex));
		m_cbLeftIndexCount = 0;
		m_cbLeft = 0;
	}

	void push(BYTE cbValue)
	{
		BYTE cbIndex = getCardIndex(cbValue);
		if (cbIndex < 0 || cbIndex >= MAX_INDEX)
		{
			return;
		}
		if (m_cbCardCount >= MAX_COUNT)
		{
			return;
		}

		if (m_cbCardIndexCount[cbIndex] >= MAX_WEAVE_CARD_COUNT)
		{
			return;
		}

		// ��¼λ��
		m_cbCardIndex[cbIndex][m_cbCardIndexCount[cbIndex]++] = m_cbCardCount;
		// ������
		m_cbCard[m_cbCardCount++] = cbValue;

		m_cbLeft++;
	}

	BYTE getCardValue(BYTE cbCardIndex)
	{
		if (cbCardIndex >= m_cbCardCount)
		{
			return 0;
		}

		return m_cbCard[cbCardIndex];
	}

	BYTE *getLeftIndex()
	{
		memset(&m_cbLeftIndex, 0, sizeof(m_cbLeftIndex));
		m_cbLeftIndexCount = 0;
		for (BYTE i = 0; i < m_cbCardCount; ++i)
		{
			if (!getFlag(i))
			{
				m_cbLeftIndex[m_cbLeftIndexCount++] = i;
			}
		}
		return (BYTE *)&m_cbLeftIndex;
	}

	BYTE getLeftIndexCount()
	{
		return m_cbLeftIndexCount;
	}

	void setFlag(BYTE cbIndex, bool bFlag)
	{
		if (cbIndex >= m_cbCardCount || cbIndex >= MAX_COUNT)
		{
			return;
		}

		BYTE cbCardNumIndex = getCardIndex(m_cbCard[cbIndex]);
		if (cbCardNumIndex >= MAX_INDEX)
		{
			return;
		}

		if (bFlag)
		{
			// �Ѿ���true��
			if (getFlag(cbIndex))
			{
				return;
			}

			// �ҵ��������
			for (int i = 0; i<MAX_WEAVE_CARD_COUNT; ++i)
			{
				if (m_cbCardIndex[cbCardNumIndex][i] == cbIndex)
				{
					if (i + 1 < MAX_WEAVE_CARD_COUNT)
					{
						memmove(&m_cbCardIndex[cbCardNumIndex][i], &m_cbCardIndex[cbCardNumIndex][i + 1], (MAX_WEAVE_CARD_COUNT - i - 1) * sizeof(m_cbCardIndex[cbCardNumIndex][i]));
					}
					--m_cbCardIndexCount[cbCardNumIndex];
					memset(&m_cbCardIndex[cbCardNumIndex][m_cbCardIndexCount[cbCardNumIndex]], 0, sizeof(m_cbCardIndex[cbCardNumIndex][m_cbCardIndexCount[cbCardNumIndex]]));

					break;
				}
			}
			--m_cbLeft;
			m_nFlag |= (1 << cbIndex);
		}
		else
		{
			// �Ѿ���false��
			if (!getFlag(cbIndex))
			{
				return;
			}

			if (m_cbCardIndexCount[cbCardNumIndex] >= MAX_WEAVE_CARD_COUNT)
			{
				return;
			}

			m_cbCardIndex[cbCardNumIndex][m_cbCardIndexCount[cbCardNumIndex]] = cbIndex;
			m_cbCardIndexCount[cbCardNumIndex]++;

			++m_cbLeft;
			m_nFlag &= ~(1 << cbIndex);
		}
	}

	bool getFlag(BYTE cbIndex)
	{
		return (m_nFlag &(1 << cbIndex)) != 0;
	}

	bool isFinised()
	{
		int nFlagAll = ((1 << (m_cbCardCount)) - 1);
		return (nFlagAll & m_nFlag) == nFlagAll;
	}

	// ȡ������������(ͨ������ֵȡ��)
	BYTE *getCardIndexArrayInValue(BYTE cbCardValue)
	{
		BYTE cbCardIndex = getCardIndex(cbCardValue);
		if (cbCardIndex >= MAX_INDEX)
		{
			return NULL;
		}

		return (BYTE *)&m_cbCardIndex[cbCardIndex];
	}

	// ȡ��������ֵ(ͨ������ֵȡ��)
	BYTE getCardIndexValueInValue(BYTE cbCardValue, BYTE cbCardCountIndex)
	{
		BYTE cbCardIndex = getCardIndex(cbCardValue);
		if (cbCardIndex >= MAX_INDEX)
		{
			return 0;
		}

		if (cbCardCountIndex >= MAX_WEAVE_CARD_COUNT)
		{
			return 0;
		}

		return m_cbCardIndex[cbCardIndex][cbCardCountIndex];
	}

	// ȡ������������
	BYTE getCardIndexCountInValue(BYTE cbCardValue)
	{
		BYTE cbCardIndex = getCardIndex(cbCardValue);
		if (cbCardIndex >= MAX_INDEX)
		{
			return 0;
		}

		return m_cbCardIndexCount[cbCardIndex];
	}

	// ȡ������������(ͨ����������ȡ��)
	BYTE *getCardIndexArrayInIndex(BYTE cbCardIndex)
	{
		if (cbCardIndex >= MAX_INDEX)
		{
			return NULL;
		}

		return (BYTE *)&m_cbCardIndex[cbCardIndex];
	}

	// ȡ��������ֵ(ͨ����������ȡ��)
	BYTE getCardIndexValueInIndex(BYTE cbCardIndex, BYTE cbCardCountIndex)
	{
		if (cbCardIndex >= MAX_INDEX)
		{
			return 0;
		}

		if (cbCardCountIndex >= MAX_WEAVE_CARD_COUNT)
		{
			return 0;
		}

		return m_cbCardIndex[cbCardIndex][cbCardCountIndex];
	}

	// ȡ������������(ͨ����������ȡ��)
	BYTE getCardIndexCountInIndex(BYTE cbCardIndex)
	{
		if (cbCardIndex >= MAX_INDEX)
		{
			return 0;
		}

		return m_cbCardIndexCount[cbCardIndex];
	}

	// ȡ����ֵ����
	BYTE getCardIndex(BYTE cbValue)
	{
		// ������
		if (cbValue == KING_VALUE)
		{
			return KING_INDEX;
		}
		else if (cbValue >= 0x01 && cbValue <= 0x1A)
		{
			return ((cbValue&MASK_COLOR) >> 4) * 10 + (cbValue&MASK_VALUE) - 1;
		}

		return MAX_INDEX;
	}

private:
	int					m_nFlag;
	BYTE				m_cbCard[MAX_COUNT];
	BYTE				m_cbCardCount;
	BYTE				m_cbCardIndex[MAX_INDEX][MAX_WEAVE_CARD_COUNT];
	BYTE				m_cbCardIndexCount[MAX_INDEX];
	BYTE				m_cbLeftIndex[MAX_COUNT];
	BYTE				m_cbLeftIndexCount;
	BYTE				m_cbLeft;
};

// ͳ����Ϣ
struct tagCountInfo
{
	BYTE				cbCardList[MAX_WEAVE_CARD_COUNT];	// ���б�
	BYTE				cbCardCount;						// ������
	BYTE				cbHuXi;								// �ƺ�Ϣ
	BYTE				cbHuXingCount;						// ��������
	BYTE				cbHongCardCount;					// ��������
	BYTE				cbHeiCardCount;						// ��������

	tagCountInfo()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct stResult
{
	BYTE	cbIndex[MAX_WEAVE_CARD_COUNT];
	BYTE	cbIndexCount;

	stResult()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct stAnswer
{
	void push(stHuKeyData stKey, stCardData *pVctCard = NULL)
	{
		if (pVctCard == NULL || m_cbHuKeyDataCount >= MAX_WEAVE)
		{
			return;
		}

		// ����
		m_cbHuXi += stKey.getHuXi();
		memcpy(&m_HuKeyData[m_cbHuKeyDataCount], &stKey, sizeof(m_HuKeyData[m_cbHuKeyDataCount]));
		++m_cbHuKeyDataCount;

		if (pVctCard != NULL)
		{
			BYTE cbCardIndexCount = stKey.getCardIndexCount();
			for (size_t i = 0; i < cbCardIndexCount; ++i)
			{
				BYTE cbCardIndex = stKey.getCardIndex(i);
				if (cbCardIndex >= MAX_INDEX)
				{
					continue;
				}

				(*pVctCard).setFlag(cbCardIndex, true);
			}
		}
	}

	void pop(stCardData *pVctCard = NULL)
	{
		if (m_cbHuKeyDataCount <= 0 || pVctCard == NULL)
		{
			return;
		}

		// ������ǰ��Ϣ
		BYTE cbCurrentHuKeyDataCount = m_cbHuKeyDataCount - 1;

		BYTE cbCardIndexCount = m_HuKeyData[cbCurrentHuKeyDataCount].getCardIndexCount();
		for (size_t i = 0; i < cbCardIndexCount; ++i)
		{
			BYTE cbCardIndex = m_HuKeyData[cbCurrentHuKeyDataCount].getCardIndex(i);
			if (cbCardIndex >= MAX_INDEX)
			{
				continue;
			}

			(*pVctCard).setFlag(cbCardIndex, false);
		}

		// ������
		m_cbHuXi -= m_HuKeyData[cbCurrentHuKeyDataCount].getHuXi();
		memset(&m_HuKeyData[cbCurrentHuKeyDataCount], 0, sizeof(m_HuKeyData[cbCurrentHuKeyDataCount]));
		--m_cbHuKeyDataCount;
	}

	stAnswer()
	{
		clear();
	}

	void clear()
	{
		memset(&m_HuFinish, 0, sizeof(m_HuFinish));
		memset(&m_InitialHuFinish, 0, sizeof(m_InitialHuFinish));
		memset(&m_TempHuFinish, 0, sizeof(m_TempHuFinish));

		memset(&m_HuKeyData, 0, sizeof(m_HuKeyData));
		m_cbHuKeyDataCount = 0;

		memset(&m_CountInfo, 0, sizeof(m_CountInfo));
		memset(&m_cbCountInfoIndex, 0, sizeof(m_cbCountInfoIndex));
		m_cbCountInfoIndexCount = 0;

		memset(&m_cbFinishIndex, 0, sizeof(m_cbFinishIndex));
		memset(&m_cbFinishIndexCount, 0, sizeof(m_cbFinishIndexCount));
		m_cbFinishAllIndexCount = 0;
		memset(&m_cbTempFinishIndex, 0, sizeof(m_cbTempFinishIndex));
		memset(&m_cbTempFinishIndexCount, 0, sizeof(m_cbTempFinishIndexCount));
		m_cbTempFinishAllIndexCount = 0;

		m_cbHuXi = 0;
		m_nHuScore = 0;
	}

	void setHuFinshInfo(tagHuFinishInfo &HuFinish)
	{
		memcpy(&m_HuFinish, &HuFinish, sizeof(m_HuFinish));
		memcpy(&m_InitialHuFinish, &HuFinish, sizeof(m_InitialHuFinish));
		memcpy(&m_TempHuFinish, &HuFinish, sizeof(m_TempHuFinish));

		m_cbHuXi = HuFinish.cbHuXi;
	}

	tagHuFinishInfo *getHuFinshInfo()
	{
		return &m_HuFinish;
	}

	BYTE getHuEyeCardCount()
	{
		return m_HuFinish.cbEyeCardCount;
	}

	BYTE getFinishAllIndexCount()
	{
		return m_cbFinishAllIndexCount;
	}

	BYTE getFinishIndexCount(BYTE cbIndexCount)
	{
		if (cbIndexCount >= m_cbFinishAllIndexCount)
		{
			return 0;
		}

		return m_cbFinishIndexCount[cbIndexCount];
	}

	BYTE getFinishIndex(BYTE cbIndexCount, BYTE cbIndex)
	{
		if (cbIndexCount >= m_cbFinishAllIndexCount)
		{
			return MAX_INDEX;
		}

		if (cbIndex >= m_cbFinishIndexCount[cbIndexCount])
		{
			return MAX_INDEX;
		}

		return m_cbFinishIndex[cbIndexCount][cbIndex];
	}

	// ֻ�жϺ�,��Ҫ�ж�����
	int getHuScore(stCardData *pVctCard)
	{
		if (pVctCard == NULL)
		{
			return -1;
		}

		// ��֦,������Ϣ,������
		if (m_cbHuXi + m_InitialHuFinish.cbEyeCardHuXi < MIN_HU_XI)
		{
			return -1;
		}

		memcpy(&m_TempHuFinish, &m_InitialHuFinish, sizeof(m_TempHuFinish));

		// ���ͳ������
		memset(&m_cbTempFinishIndex, 0, sizeof(m_cbTempFinishIndex));
		memset(&m_cbTempFinishIndexCount, 0, sizeof(m_cbTempFinishIndexCount));
		m_cbTempFinishAllIndexCount = 0;
		BYTE cbTempFinishIndex[MAX_WEAVE][MAX_WEAVE_CARD_COUNT];
		memset(&cbTempFinishIndex, 0, sizeof(cbTempFinishIndex));
		BYTE cbTempFinishIndexCount[MAX_WEAVE];
		memset(&cbTempFinishIndexCount, 0, sizeof(cbTempFinishIndexCount));
		BYTE cbTempFinishAllIndexCount = 0;

		// ͳ�����������
		/*
		0x05,0x06,0x07
		0x06,0x07,0x08
		0x07,0x08,0x09
		0x07,0x07,0x07
		0x07,0x07,0x17
		0x07,0x07,0x17
		0x02,0x07,0x0A
		*/
		memset(&m_CountInfo, 0, sizeof(m_CountInfo));
		memset(&m_cbCountInfoIndex, 0, sizeof(m_cbCountInfoIndex));
		m_cbCountInfoIndexCount = 0;

		// �����������
		BYTE cbAllCardIndexCount = 0;
		for (BYTE i = 0; i<m_cbHuKeyDataCount; ++i)
		{
			BYTE cbTempWangCardCount = 0;
			BYTE cbTempCardList[MAX_WEAVE_CARD_COUNT] = { 0 };

			stHuKeyData &data = m_HuKeyData[i];
			// ȡ���������������
			BYTE cbCardIndexCount = data.getCardIndexCount();
			// ��Ϣ
			BYTE cbTempHuXi = data.getHuXi();
			// �����������
			cbAllCardIndexCount += cbCardIndexCount;
			for (BYTE j = 0; j<cbCardIndexCount; ++j)
			{
				// ȡ��������
				BYTE cbCardIndex = data.getCardIndex(j);
				// ȡ����ֵ
				BYTE cbCardData = pVctCard->getCardValue(cbCardIndex);
				if (cbCardData == 0)
				{
					continue;
				}
				// �������б�
				cbTempCardList[j] = cbCardData;
				// ������
				if (cbCardData == KING_VALUE)
				{
					++cbTempWangCardCount;
				}
			}

			// �����˿�(����1��ֱ��������ϵ�,��¼����)
			if (analysisCard(cbTempCardList, cbCardIndexCount, cbTempHuXi, cbTempWangCardCount) == 1)
			{
				for (BYTE j = 0; j<cbCardIndexCount; ++j)
				{
					m_cbTempFinishIndex[m_cbTempFinishAllIndexCount][j] = data.getCardIndex(j);
				}
				m_cbTempFinishIndexCount[m_cbTempFinishAllIndexCount] = cbCardIndexCount;
				++m_cbTempFinishAllIndexCount;
			}
			else
			{
				for (BYTE j = 0; j<cbCardIndexCount; ++j)
				{
					cbTempFinishIndex[cbTempFinishAllIndexCount][j] = data.getCardIndex(j);
				}
				cbTempFinishIndexCount[cbTempFinishAllIndexCount] = cbCardIndexCount;
				++cbTempFinishAllIndexCount;
			}
		}

		// ��֦,������,������
		if (m_InitialHuFinish.cbHongCardCount + m_InitialHuFinish.cbHeiCardCount + cbAllCardIndexCount + m_TempHuFinish.cbEyeCardCount < MAX_COUNT)
		{
			return 0;
		}

		// ����ϵļ������
		for (BYTE i = 0; i<cbTempFinishAllIndexCount; ++i)
		{
			memcpy(&m_cbTempFinishIndex[m_cbTempFinishAllIndexCount], &cbTempFinishIndex[i], sizeof(m_cbTempFinishIndex[m_cbTempFinishAllIndexCount]));
			memcpy(&m_cbTempFinishIndexCount[m_cbTempFinishAllIndexCount], &cbTempFinishIndexCount[i], sizeof(m_cbTempFinishIndexCount[m_cbTempFinishAllIndexCount]));
			++m_cbTempFinishAllIndexCount;
		}

		// �������۷���
		if (m_TempHuFinish.cbEyeCardCount > 0)
		{
			BYTE cbTempWangCardCount = 0;
			BYTE cbTempCardList[MAX_WEAVE_CARD_COUNT] = { 0 };
			BYTE cbCardIndexCount = 0;
			// ������ֵ,�������
			for (int i = 0; i<MAX_WEAVE_CARD_COUNT; ++i)
			{
				// ȡ����ֵ
				BYTE cbCardData = m_TempHuFinish.cbEyeCard[i];
				if (cbCardData == 0)
				{
					continue;
				}
				// �������б�
				cbTempCardList[cbCardIndexCount++] = cbCardData;
				// ������
				if (cbCardData == KING_VALUE)
				{
					++cbTempWangCardCount;
				}
			}

			// ����ǰ���ת���ɾ���ֵ,����û����,ֱ��ת������,��,������
			if (cbTempWangCardCount <= 0)
			{
				// ���Ӽ�������Ϣ
				addCardCountInfo(cbTempCardList, cbCardIndexCount);

				BYTE cbCard = cbTempCardList[0];
				BYTE cbHuXi = 0;
				if (cbCardIndexCount == 3)
				{
					cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;
				}
				else if (cbCardIndexCount == 4)
				{
					cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_TI_HU_XI : TYPE_X_TI_HU_XI;
				}
				// ���Ӻ�Ϣ
				m_TempHuFinish.cbHuXi += cbHuXi;
			}
			else
			{
				// ������,˵������������,���洦��
				BYTE cbCardValue[7] = { 0 };
				BYTE cbCardValueIndex = 0;

				// �������ó�����
				cbCardValue[cbCardValueIndex++] = m_TempHuFinish.cbXingCard;
				// ���Ʋ�����������Ĭ������
				if (m_TempHuFinish.cbXingCard != KING_VALUE)
				{
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// �������óɴ�1(��)
						cbCardValue[cbCardValueIndex++] = 0x11;
					}
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// �������óɴ�2(��)
						cbCardValue[cbCardValueIndex++] = 0x12;
					}
				}

				for (BYTE nIndex = 0; nIndex<cbCardValueIndex; ++nIndex)
				{
					BYTE cbCard = cbCardValue[nIndex];

					for (BYTE i = 0; i<cbCardIndexCount; ++i)
					{
						// �������ó�������
						cbTempCardList[i] = cbCard;
					}

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// ���Ӽ�������Ϣ
					addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

					BYTE cbHuXi = 0;
					if (cbCardIndexCount == 3)
					{
						cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;
					}
					else if (cbCardIndexCount == 4)
					{
						cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_TI_HU_XI : TYPE_X_TI_HU_XI;
					}
					tagCountInfo CountInfo;
					memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
					CountInfo.cbCardCount = cbCardIndexCount;
					CountInfo.cbHuXi = cbHuXi;
					CountInfo.cbHongCardCount = cbHongCardCount;
					CountInfo.cbHeiCardCount = cbHeiCardCount;
					CountInfo.cbHuXingCount = cbHuXingCount;
					m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
				}
				m_cbCountInfoIndexCount++;
			}
		}

		// ����������
		analysisMaxScore();

		return m_nHuScore;
	}

	int getHuScore()
	{
		return m_nHuScore;
	}

	// ���Ӽ�������Ϣ
	void addCardCountInfo(BYTE cbCardList[], BYTE cbCardCount)
	{
		BYTE cbHuXingCount = 0;
		BYTE cbHongCardCount = 0;
		BYTE cbHeiCardCount = 0;
		for (BYTE i = 0; i<cbCardCount; ++i)
		{
			BYTE cbCardData = cbCardList[i];
			// ���������(�������������)
			if (cbCardData == m_TempHuFinish.cbXingCard)
			{
				cbHuXingCount++;
			}
			// ��ת��С��
			BYTE cbCardValue = cbCardData&MASK_VALUE;
			// ��2,7,A
			if (cbCardValue == 0x02 || cbCardValue == 0x07 || cbCardValue == 0x0A)
			{
				cbHongCardCount++;
			}
			else
			{
				cbHeiCardCount++;
			}
		}
		m_TempHuFinish.cbHuXingCount += cbHuXingCount;
		m_TempHuFinish.cbHongCardCount += cbHongCardCount;
		m_TempHuFinish.cbHeiCardCount += cbHeiCardCount;
	}

	// ���Ӽ�������Ϣ
	void addCardCountInfo(BYTE cbCardList[], BYTE cbCardCount, BYTE &cbHuXingCount, BYTE &cbHongCardCount, BYTE &cbHeiCardCount)
	{
		cbHuXingCount = 0;
		cbHongCardCount = 0;
		cbHeiCardCount = 0;
		for (BYTE i = 0; i<cbCardCount; ++i)
		{
			BYTE cbCardData = cbCardList[i];
			// ���������(�������������)
			if (cbCardData == m_TempHuFinish.cbXingCard)
			{
				cbHuXingCount++;
			}
			// ��ת��С��
			BYTE cbCardValue = cbCardData&MASK_VALUE;
			// ��2,7,A
			if (cbCardValue == 0x02 || cbCardValue == 0x07 || cbCardValue == 0x0A)
			{
				cbHongCardCount++;
			}
			else
			{
				cbHeiCardCount++;
			}
		}
	}

	BYTE analysisCard(BYTE cbTempCardList[], BYTE cbCardIndexCount, BYTE cbTempHuXi, BYTE cbTempWangCardCount)
	{
		// û����,ֱ�ӷ���������
		if (cbTempWangCardCount == 0)
		{
			int nWeaveKind = ACK_NULL;
			if (cbCardIndexCount == 3)
			{
				if (cbTempCardList[0] == cbTempCardList[1] && cbTempCardList[1] == cbTempCardList[2])
				{
					// ����
					if ((cbTempCardList[0] & MASK_COLOR) > 0)
					{
						if (cbTempHuXi == TYPE_D_PENG_HU_XI)
						{
							nWeaveKind = ACK_PENG;
						}
						else
						{
							nWeaveKind = ACK_WEI;
						}
					}
					else
					{
						if (cbTempHuXi == TYPE_X_PENG_HU_XI)
						{
							nWeaveKind = ACK_PENG;
						}
						else
						{
							nWeaveKind = ACK_WEI;
						}
					}
				}
				else
				{
					nWeaveKind = ACK_CHI;
				}
			}
			else if (cbCardIndexCount == 4)
			{
				nWeaveKind = ACK_TI;
			}

			// ���Ӽ�������Ϣ
			addCardCountInfo(cbTempCardList, cbCardIndexCount);

			m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].nWeaveKind = nWeaveKind;
			m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardCount = cbCardIndexCount;
			m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCenterCard = cbTempCardList[0];
			memcpy(&m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList, cbTempCardList, sizeof(m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList));
			++m_TempHuFinish.cbWeaveCount;

			return 1;
		}
		if (cbTempWangCardCount == 1)
		{
			// ֻ��2����(ֱ��������)
			if (cbCardIndexCount == 2)
			{
				vector<BYTE> vctTemp;
				vctTemp.push_back(cbTempCardList[0]);
				vctTemp.push_back(cbTempCardList[1]);
				sort(vctTemp.begin(), vctTemp.end());

				// ������ض�����������ƣ��ն�
				cbTempCardList[0] = vctTemp[0];
				cbTempCardList[1] = vctTemp[0];

				// ���Ӽ�������Ϣ
				addCardCountInfo(cbTempCardList, cbCardIndexCount);

				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].nWeaveKind = ACK_NULL;
				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardCount = cbCardIndexCount;
				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCenterCard = cbTempCardList[0];
				memcpy(&m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList, cbTempCardList, sizeof(m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList));
				++m_TempHuFinish.cbWeaveCount;

				return 1;
			}
			else if (cbCardIndexCount == 4)	//(ֱ��������)
			{
				vector<BYTE> vctTemp;
				vctTemp.push_back(cbTempCardList[0]);
				vctTemp.push_back(cbTempCardList[1]);
				vctTemp.push_back(cbTempCardList[2]);
				vctTemp.push_back(cbTempCardList[3]);
				sort(vctTemp.begin(), vctTemp.end());

				cbTempCardList[0] = vctTemp[0];
				cbTempCardList[1] = vctTemp[1];
				cbTempCardList[2] = vctTemp[2];
				cbTempCardList[3] = vctTemp[0];

				// ���Ӽ�������Ϣ
				addCardCountInfo(cbTempCardList, cbCardIndexCount);

				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].nWeaveKind = ACK_TI;
				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardCount = cbCardIndexCount;
				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCenterCard = cbTempCardList[0];
				memcpy(&m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList, cbTempCardList, sizeof(m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList));
				++m_TempHuFinish.cbWeaveCount;

				return 1;
			}
			else
			{
				vector<BYTE> vctTemp;
				vctTemp.push_back(cbTempCardList[0]);
				vctTemp.push_back(cbTempCardList[1]);
				vctTemp.push_back(cbTempCardList[2]);
				sort(vctTemp.begin(), vctTemp.end());

				BYTE cbCardValue[7] = { 0 };
				BYTE cbCardValueIndex = 0;

				// 3���Ʊ��ʾ���x+y=z��֪2�����3����

				if (vctTemp[0] == vctTemp[1])
				{
					// ��1�Ÿ���2��һ������Ҳ�����ţ��������Ĵ�С����
					cbCardValue[cbCardValueIndex++] = vctTemp[0];
					cbCardValue[cbCardValueIndex++] = (vctTemp[0] + 0x10) % 0x20;
				}
				else	// 2�Ų�һ����
				{
					// ��ֵ����2˵���Ǵ�С��,����27A���͵�
					int nValue = abs(vctTemp[0] - vctTemp[1]);
					if (nValue > 2)
					{
						BYTE cbCardValue0 = vctTemp[0] & MASK_VALUE;
						BYTE cbCardValue1 = vctTemp[1] & MASK_VALUE;
						// 0x02,0x12
						if (cbCardValue0 == 0x02)
						{
							// 0x07,0x17
							if (cbCardValue1 == 0x07)
							{
								cbCardValue[cbCardValueIndex++] = vctTemp[0] + 0x08;	// 0x0A,0x1A
							}
							else if (cbCardValue1 == 0x0A)	// 0x0A,0x1A
							{
								cbCardValue[cbCardValueIndex++] = vctTemp[0] + 0x05;	// 0x07,0x17
							}
							else
							{
								// �������óɵ�1����
								cbCardValue[cbCardValueIndex++] = vctTemp[0];
								// �������óɵ�2����
								cbCardValue[cbCardValueIndex++] = vctTemp[1];
							}
						}
						// 0x07,0x17
						else if (cbCardValue0 == 0x07)
						{
							if (cbCardValue1 == 0x0A)	// 0x0A,0x1A
							{
								cbCardValue[cbCardValueIndex++] = vctTemp[0] - 0x05;	// 0x02,0x12
							}
							else
							{
								// �������óɵ�1����
								cbCardValue[cbCardValueIndex++] = vctTemp[0];
								// �������óɵ�2����
								cbCardValue[cbCardValueIndex++] = vctTemp[1];
							}
						}
						else	// ��Ӧ���ж�0x0A,����Ϊ����������,���Բ����ڴ��ֿ���
						{
							// �������óɵ�1����
							cbCardValue[cbCardValueIndex++] = vctTemp[0];
							// �������óɵ�2����
							cbCardValue[cbCardValueIndex++] = vctTemp[1];
						}
					}
					else if (nValue == 2)	// ���Ϊ2
					{
						cbCardValue[cbCardValueIndex++] = vctTemp[0] + 1;
					}
					else
					{
						// ����ֵ
						// ��1������С��ֵ
						BYTE cbValue = vctTemp[0] & MASK_VALUE;
						// ��С��ֵ��1
						if (cbValue <= 0x01)
						{
							// ��ֻ����������ֵ
							// �������óɵ�1����ֵ+2
							cbCardValue[cbCardValueIndex++] = vctTemp[0] + 2;
						}
						else if (cbValue >= 0x09)	// ��С��ֵ��9
						{
							// ֻ���������
							// �������óɵ�1����ֵ-1
							cbCardValue[cbCardValueIndex++] = vctTemp[0] - 1;
						}
						else
						{
							// �������óɵ�1����ֵ+2
							cbCardValue[cbCardValueIndex++] = vctTemp[0] + 2;
							// �������óɵ�1����ֵ-1
							cbCardValue[cbCardValueIndex++] = vctTemp[0] - 1;
						}
					}
				}

				for (BYTE nIndex = 0; nIndex<cbCardValueIndex; ++nIndex)
				{
					BYTE cbCard = cbCardValue[nIndex];

					// �������ó�������
					cbTempCardList[0] = vctTemp[0];
					cbTempCardList[1] = vctTemp[1];
					cbTempCardList[2] = cbCard;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// ���Ӽ�������Ϣ
					addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

					BYTE cbHuXi = 0;
					// 3����ͬ
					if (cbTempCardList[0] == cbTempCardList[1] && cbTempCardList[1] == cbTempCardList[2])
					{
						if ((cbTempCardList[0] & MASK_COLOR) > 0)
						{
							cbHuXi = TYPE_D_WEI_HU_XI;
						}
						else
						{
							cbHuXi = TYPE_X_WEI_HU_XI;
						}
					}
					else
					{
						//��ȡ��ֵ
						BYTE cbValue1 = cbTempCardList[0] & MASK_VALUE;
						BYTE cbValue2 = cbTempCardList[1] & MASK_VALUE;
						BYTE cbValue3 = cbTempCardList[2] & MASK_VALUE;

						BYTE cbCardIndex[MAX_INDEX] = { 0 };
						// ��ת����С������
						cbCardIndex[cbValue1 - 1]++;
						cbCardIndex[cbValue2 - 1]++;
						cbCardIndex[cbValue3 - 1]++;

						//һ����
						if (cbCardIndex[0] == 1 && cbCardIndex[1] == 1 && cbCardIndex[2] == 1)
						{
							cbHuXi = ((cbTempCardList[0] & MASK_COLOR) > 0) ? TYPE_D_123_HU_XI : TYPE_X_123_HU_XI;
						}

						//����ʮ
						if (cbCardIndex[1] == 1 && cbCardIndex[6] == 1 && cbCardIndex[9] == 1)
						{
							cbHuXi = ((cbTempCardList[0] & MASK_COLOR) > 0) ? TYPE_D_27A_HU_XI : TYPE_X_27A_HU_XI;
						}
					}

					tagCountInfo CountInfo;
					memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
					CountInfo.cbCardCount = cbCardIndexCount;
					CountInfo.cbHuXi = cbHuXi;
					CountInfo.cbHongCardCount = cbHongCardCount;
					CountInfo.cbHeiCardCount = cbHeiCardCount;
					CountInfo.cbHuXingCount = cbHuXingCount;
					m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
				}
				m_cbCountInfoIndexCount++;
			}	// end cbCardIndexCount == 3
		}	// end cbTempWangCardCount==1
		else if (cbTempWangCardCount == 2)
		{
			// ֻ��2����
			if (cbCardIndexCount == 2)
			{
				BYTE cbCardValue[7] = { 0 };
				BYTE cbCardValueIndex = 0;

				// �������ó�����
				cbCardValue[cbCardValueIndex++] = m_TempHuFinish.cbXingCard;
				// ���Ʋ�����������Ĭ������
				if (m_TempHuFinish.cbXingCard != KING_VALUE)
				{
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// �������óɴ�1(��)
						cbCardValue[cbCardValueIndex++] = 0x11;
					}
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// �������óɴ�2(��)
						cbCardValue[cbCardValueIndex++] = 0x12;
					}
				}

				for (BYTE nIndex = 0; nIndex<cbCardValueIndex; ++nIndex)
				{
					BYTE cbCard = cbCardValue[nIndex];

					// �������ó�������
					cbTempCardList[0] = cbCard;
					cbTempCardList[1] = cbCard;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// ���Ӽ�������Ϣ
					addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

					tagCountInfo CountInfo;
					memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
					CountInfo.cbCardCount = cbCardIndexCount;
					CountInfo.cbHuXi = 0;
					CountInfo.cbHongCardCount = cbHongCardCount;
					CountInfo.cbHeiCardCount = cbHeiCardCount;
					CountInfo.cbHuXingCount = cbHuXingCount;
					m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
				}
				m_cbCountInfoIndexCount++;
			}
			else if (cbCardIndexCount == 4)
			{
				vector<BYTE> vctTemp;
				vctTemp.push_back(cbTempCardList[0]);
				vctTemp.push_back(cbTempCardList[1]);
				vctTemp.push_back(cbTempCardList[2]);
				vctTemp.push_back(cbTempCardList[3]);
				sort(vctTemp.begin(), vctTemp.end());

				cbTempCardList[0] = vctTemp[0];
				cbTempCardList[1] = vctTemp[1];
				cbTempCardList[2] = vctTemp[0];
				cbTempCardList[3] = vctTemp[0];

				// ���Ӽ�������Ϣ
				addCardCountInfo(cbTempCardList, cbCardIndexCount);

				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].nWeaveKind = ACK_TI;
				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardCount = cbCardIndexCount;
				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCenterCard = cbTempCardList[0];
				memcpy(&m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList, cbTempCardList, sizeof(m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList));
				++m_TempHuFinish.cbWeaveCount;

				return 1;
			}
			else
			{
				vector<BYTE> vctTemp;
				vctTemp.push_back(cbTempCardList[0]);
				vctTemp.push_back(cbTempCardList[1]);
				vctTemp.push_back(cbTempCardList[2]);
				sort(vctTemp.begin(), vctTemp.end());

				cbTempCardList[0] = vctTemp[0];

				// 3���Ʊ��ʾ���x+y=z��֪2�����3����
				BYTE cbCard = vctTemp[0];
				BYTE cbValue = cbCard&MASK_VALUE;

				// ����
				{
					// ��һ����������
					// �ڶ�����������
					BYTE cbCardData = cbTempCardList[0];
					cbTempCardList[1] = cbCardData;
					cbTempCardList[2] = cbCardData;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// ���Ӽ�������Ϣ
					addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

					tagCountInfo CountInfo;
					memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
					CountInfo.cbCardCount = cbCardIndexCount;
					CountInfo.cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;
					CountInfo.cbHongCardCount = cbHongCardCount;
					CountInfo.cbHeiCardCount = cbHeiCardCount;
					CountInfo.cbHuXingCount = cbHuXingCount;
					m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
				}

				// 27A
				{
					// ��2
					if (cbValue == 0x02)
					{
						// ��һ����������
						cbTempCardList[1] = cbTempCardList[0] + 0x05;
						// �ڶ�����������
						cbTempCardList[2] = cbTempCardList[0] + 0x08;

						BYTE cbHuXingCount = 0;
						BYTE cbHongCardCount = 0;
						BYTE cbHeiCardCount = 0;
						// ���Ӽ�������Ϣ
						addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

						tagCountInfo CountInfo;
						memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
						CountInfo.cbCardCount = cbCardIndexCount;
						CountInfo.cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_27A_HU_XI : TYPE_X_27A_HU_XI;
						CountInfo.cbHongCardCount = cbHongCardCount;
						CountInfo.cbHeiCardCount = cbHeiCardCount;
						CountInfo.cbHuXingCount = cbHuXingCount;
						m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
					}
					else if (cbValue == 0x07)
					{
						// ��һ����������
						cbTempCardList[1] = cbTempCardList[0] - 0x05;
						// �ڶ�����������
						cbTempCardList[2] = cbTempCardList[0] + 0x03;

						BYTE cbHuXingCount = 0;
						BYTE cbHongCardCount = 0;
						BYTE cbHeiCardCount = 0;
						// ���Ӽ�������Ϣ
						addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

						tagCountInfo CountInfo;
						memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
						CountInfo.cbCardCount = cbCardIndexCount;
						CountInfo.cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_27A_HU_XI : TYPE_X_27A_HU_XI;
						CountInfo.cbHongCardCount = cbHongCardCount;
						CountInfo.cbHeiCardCount = cbHeiCardCount;
						CountInfo.cbHuXingCount = cbHuXingCount;
						m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
					}
					else if (cbValue == 0x0A)
					{
						// ��һ����������
						cbTempCardList[1] = cbTempCardList[0] - 0x08;
						// �ڶ�����������
						cbTempCardList[2] = cbTempCardList[0] - 0x03;

						BYTE cbHuXingCount = 0;
						BYTE cbHongCardCount = 0;
						BYTE cbHeiCardCount = 0;
						// ���Ӽ�������Ϣ
						addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

						tagCountInfo CountInfo;
						memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
						CountInfo.cbCardCount = cbCardIndexCount;
						CountInfo.cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_27A_HU_XI : TYPE_X_27A_HU_XI;
						CountInfo.cbHongCardCount = cbHongCardCount;
						CountInfo.cbHeiCardCount = cbHeiCardCount;
						CountInfo.cbHuXingCount = cbHuXingCount;
						m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
					}
				}

				//˳���ж�
				BYTE cbResultCard[7][3];
				memset(&cbResultCard, 0, sizeof(cbResultCard));
				BYTE cbResultCardIndex = 0;
				BYTE cbExcursion[3] = { 0,1,2 };
				for (BYTE i = 0; i<3; i++)
				{
					int cbSpaceValue = cbValue - cbExcursion[i];
					if (cbSpaceValue <= 0 || cbSpaceValue > 8)
					{
						continue;
					}

					//���ý��
					if (i == 0)
					{
						cbResultCard[cbResultCardIndex][0] = cbValue;
						cbResultCard[cbResultCardIndex][1] = cbValue + 1;
						cbResultCard[cbResultCardIndex][2] = cbValue + 2;
					}
					else if (i == 1)
					{
						cbResultCard[cbResultCardIndex][0] = cbValue;
						cbResultCard[cbResultCardIndex][1] = cbValue - 1;
						cbResultCard[cbResultCardIndex][2] = cbValue + 1;
					}
					else if (i == 2)
					{
						cbResultCard[cbResultCardIndex][0] = cbValue;
						cbResultCard[cbResultCardIndex][1] = cbValue - 2;
						cbResultCard[cbResultCardIndex][2] = cbValue - 1;
					}

					++cbResultCardIndex;
				}
				for (BYTE i = 0; i<cbResultCardIndex; ++i)
				{
					// ��һ����������
					cbTempCardList[1] = cbCard + cbResultCard[i][1] - cbResultCard[i][0];
					// �ڶ�����������
					cbTempCardList[2] = cbCard + cbResultCard[i][2] - cbResultCard[i][0];

					BYTE cbHuXi = 0;
					if (cbValue == 0x01)
					{
						cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_123_HU_XI : TYPE_X_123_HU_XI;
					}

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// ���Ӽ�������Ϣ
					addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

					tagCountInfo CountInfo;
					memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
					CountInfo.cbCardCount = cbCardIndexCount;
					CountInfo.cbHuXi = cbHuXi;
					CountInfo.cbHongCardCount = cbHongCardCount;
					CountInfo.cbHeiCardCount = cbHeiCardCount;
					CountInfo.cbHuXingCount = cbHuXingCount;
					m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
				}

				// С���(XXD)
				{
					// ��һ����������
					cbTempCardList[1] = cbTempCardList[0];
					// �ڶ�����������
					cbTempCardList[2] = (cbTempCardList[0] + 0x10) % 0x20;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// ���Ӽ�������Ϣ
					addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

					tagCountInfo CountInfo;
					memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
					CountInfo.cbCardCount = cbCardIndexCount;
					CountInfo.cbHuXi = 0;
					CountInfo.cbHongCardCount = cbHongCardCount;
					CountInfo.cbHeiCardCount = cbHeiCardCount;
					CountInfo.cbHuXingCount = cbHuXingCount;
					m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
				}

				// С���(XDD)
				{
					// ��һ����������
					// �ڶ�����������
					BYTE cbCardData = (cbTempCardList[0] + 0x10) % 0x20;
					cbTempCardList[1] = cbCardData;
					cbTempCardList[2] = cbCardData;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// ���Ӽ�������Ϣ
					addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

					tagCountInfo CountInfo;
					memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
					CountInfo.cbCardCount = cbCardIndexCount;
					CountInfo.cbHuXi = 0;
					CountInfo.cbHongCardCount = cbHongCardCount;
					CountInfo.cbHeiCardCount = cbHeiCardCount;
					CountInfo.cbHuXingCount = cbHuXingCount;
					m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
				}

				m_cbCountInfoIndexCount++;
			}
		}
		else if (cbTempWangCardCount == 3)
		{
			if (cbCardIndexCount == 3)
			{
				BYTE cbCardValue[7] = { 0 };
				BYTE cbCardValueIndex = 0;

				// �������ó�����
				cbCardValue[cbCardValueIndex++] = m_TempHuFinish.cbXingCard;
				// ���Ʋ�����������Ĭ������
				if (m_TempHuFinish.cbXingCard != KING_VALUE)
				{
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// �������óɴ�1(��)
						cbCardValue[cbCardValueIndex++] = 0x11;
					}
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// �������óɴ�2(��)
						cbCardValue[cbCardValueIndex++] = 0x12;
					}
				}

				for (BYTE nIndex = 0; nIndex<cbCardValueIndex; ++nIndex)
				{
					BYTE cbCard = cbCardValue[nIndex];

					// �������ó�������
					cbTempCardList[0] = cbCard;
					cbTempCardList[1] = cbCard;
					cbTempCardList[2] = cbCard;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// ���Ӽ�������Ϣ
					addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

					tagCountInfo CountInfo;
					memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
					CountInfo.cbCardCount = cbCardIndexCount;
					CountInfo.cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;
					CountInfo.cbHongCardCount = cbHongCardCount;
					CountInfo.cbHeiCardCount = cbHeiCardCount;
					CountInfo.cbHuXingCount = cbHuXingCount;
					m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
				}
				m_cbCountInfoIndexCount++;
			}
			else if (cbCardIndexCount == 4)
			{
				vector<BYTE> vctTemp;
				vctTemp.push_back(cbTempCardList[0]);
				vctTemp.push_back(cbTempCardList[1]);
				vctTemp.push_back(cbTempCardList[2]);
				vctTemp.push_back(cbTempCardList[3]);
				sort(vctTemp.begin(), vctTemp.end());

				cbTempCardList[0] = vctTemp[0];
				cbTempCardList[1] = vctTemp[0];
				cbTempCardList[2] = vctTemp[0];
				cbTempCardList[3] = vctTemp[0];

				// ���Ӽ�������Ϣ
				addCardCountInfo(cbTempCardList, cbCardIndexCount);

				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].nWeaveKind = ACK_TI;
				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardCount = cbCardIndexCount;
				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCenterCard = cbTempCardList[0];
				memcpy(&m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList, cbTempCardList, sizeof(m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList));
				++m_TempHuFinish.cbWeaveCount;

				return 1;
			}
		}
		else if (cbTempWangCardCount == 4)
		{
			BYTE cbCardValue[7] = { 0 };
			BYTE cbCardValueIndex = 0;

			// �������ó�����
			cbCardValue[cbCardValueIndex++] = m_TempHuFinish.cbXingCard;
			// ���Ʋ�����������Ĭ������
			if (m_TempHuFinish.cbXingCard != KING_VALUE)
			{
				if (m_TempHuFinish.cbXingCard != 0x11)
				{
					// �������óɴ�1(��)
					cbCardValue[cbCardValueIndex++] = 0x11;
				}
				if (m_TempHuFinish.cbXingCard != 0x11)
				{
					// �������óɴ�2(��)
					cbCardValue[cbCardValueIndex++] = 0x12;
				}
			}

			for (BYTE nIndex = 0; nIndex<cbCardValueIndex; ++nIndex)
			{
				BYTE cbCard = cbCardValue[nIndex];

				// �������ó�������
				cbTempCardList[0] = cbCard;
				cbTempCardList[1] = cbCard;
				cbTempCardList[2] = cbCard;
				cbTempCardList[3] = cbCard;

				BYTE cbHuXingCount = 0;
				BYTE cbHongCardCount = 0;
				BYTE cbHeiCardCount = 0;
				// ���Ӽ�������Ϣ
				addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

				tagCountInfo CountInfo;
				memcpy(&CountInfo.cbCardList, cbTempCardList, sizeof(CountInfo.cbCardList));
				CountInfo.cbCardCount = cbCardIndexCount;
				CountInfo.cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_TI_HU_XI : TYPE_X_TI_HU_XI;
				CountInfo.cbHongCardCount = cbHongCardCount;
				CountInfo.cbHeiCardCount = cbHeiCardCount;
				CountInfo.cbHuXingCount = cbHuXingCount;
				m_CountInfo[m_cbCountInfoIndexCount][m_cbCountInfoIndex[m_cbCountInfoIndexCount]++] = CountInfo;
			}
			m_cbCountInfoIndexCount++;
		}

		return 0;
	}

	void analysisMaxScore()
	{
		if (m_cbCountInfoIndexCount <= 0)
		{
			// ������Ϣ
			if (m_cbHuXi + m_InitialHuFinish.cbEyeCardHuXi < MIN_HU_XI)
			{
				m_nHuScore = 0;
				return;
			}

			// ��������
			if (m_TempHuFinish.cbXingCard == KING_VALUE)
			{
				BYTE cbCardIndex[MAX_INDEX] = { 0 };
				// ����������ͳ��
				for (BYTE j = 0; j<m_TempHuFinish.cbWeaveCount; ++j)
				{
					tagWeaveItem &WeaveItem = m_TempHuFinish.WeaveItemArray[j];
					for (BYTE k = 0; k<WeaveItem.cbCardCount; ++k)
					{
						BYTE cbIndex = SwitchToCardIndex(WeaveItem.cbCardList[k]);
						if (cbIndex < MAX_INDEX)
						{
							++cbCardIndex[cbIndex];
						}
					}
				}

				BYTE cbMaxIndexCount = 0;
				for (BYTE j = 0; j<MAX_INDEX; ++j)
				{
					if (cbCardIndex[j] > cbMaxIndexCount)
					{
						cbMaxIndexCount = cbCardIndex[j];
					}
				}
				m_TempHuFinish.cbHuXingCount += cbMaxIndexCount;
			}

			BYTE cbHuHongHeiBase = 1;
			if (m_TempHuFinish.cbHongCardCount == 0)			// ȫ��
			{
				cbHuHongHeiBase *= 4;
			}
			else if (m_TempHuFinish.cbHongCardCount == 1)		// ����
			{
				cbHuHongHeiBase *= 3;
			}
			else if (m_TempHuFinish.cbHongCardCount >= 10)	// ���
			{
				cbHuHongHeiBase *= 2;
			}

			int nHuScore = ((1 + m_InitialHuFinish.cbAddTunCount + (m_cbHuXi + m_InitialHuFinish.cbEyeCardHuXi - MIN_HU_XI) / 3) + m_TempHuFinish.cbHuXingCount * m_TempHuFinish.cbXingTunCount) * m_TempHuFinish.cbHuBase * cbHuHongHeiBase;
			if (nHuScore <= m_nHuScore)
			{
				return;
			}

			// ��������ʱ��ԭ�򣬱���ǰ������һ��6����ϵģ�������5����ϣ���6������в���ֵ
			BYTE cbWeaveCount = m_TempHuFinish.cbWeaveCount;
			tagWeaveItem WeaveItemArray[MAX_WEAVE];
			for (BYTE j = 0; j<cbWeaveCount; ++j)
			{
				memcpy(&WeaveItemArray[j], &m_TempHuFinish.WeaveItemArray[j], sizeof(WeaveItemArray[j]));
			}

			// ������,�����۷���
			if (m_TempHuFinish.cbEyeCardCount > 0 && cbWeaveCount < MAX_WEAVE)
			{
				int nWeaveKind = ACK_NULL;
				if (m_TempHuFinish.cbEyeCardCount == 3)
				{
					if (m_TempHuFinish.cbEyeCard[0] == m_TempHuFinish.cbEyeCard[1] && m_TempHuFinish.cbEyeCard[1] == m_TempHuFinish.cbEyeCard[2])
					{
						nWeaveKind = ACK_WEI;
					}
					else
					{
						nWeaveKind = ACK_CHI;
					}
				}
				else if (m_TempHuFinish.cbEyeCardCount == 4)
				{
					nWeaveKind = ACK_TI;
				}

				WeaveItemArray[cbWeaveCount].nWeaveKind = nWeaveKind;
				WeaveItemArray[cbWeaveCount].cbCardCount = m_TempHuFinish.cbEyeCardCount;
				WeaveItemArray[cbWeaveCount].cbCenterCard = m_TempHuFinish.cbEyeCard[0];
				memcpy(&WeaveItemArray[cbWeaveCount].cbCardList, m_TempHuFinish.cbEyeCard, sizeof(WeaveItemArray[cbWeaveCount].cbCardList));
				cbWeaveCount++;
			}

			m_nHuScore = nHuScore;

			m_HuFinish.nHuScore = nHuScore;
			m_HuFinish.cbXingCard = m_TempHuFinish.cbXingCard;
			m_HuFinish.cbHuHongHeiBase = cbHuHongHeiBase;
			m_HuFinish.cbHuXi = m_cbHuXi + m_InitialHuFinish.cbEyeCardHuXi;
			m_HuFinish.cbHongCardCount = m_TempHuFinish.cbHongCardCount;
			m_HuFinish.cbHeiCardCount = m_TempHuFinish.cbHeiCardCount;
			m_HuFinish.cbHuXingCount = m_TempHuFinish.cbHuXingCount;
			m_HuFinish.cbWeaveCount = cbWeaveCount;
			memcpy(m_HuFinish.WeaveItemArray, WeaveItemArray, sizeof(m_HuFinish.WeaveItemArray));

			// ��������������
			memcpy(&m_cbFinishIndex, m_cbTempFinishIndex, sizeof(m_cbFinishIndex));
			memcpy(&m_cbFinishIndexCount, m_cbTempFinishIndexCount, sizeof(m_cbFinishIndexCount));
			m_cbFinishAllIndexCount = m_cbTempFinishAllIndexCount;
			return;
		}

		// �������
		vector<stResult> resultVector;
		if (m_cbCountInfoIndexCount == 4)
		{
			for (BYTE nIndex0 = 0; nIndex0<m_cbCountInfoIndex[0]; ++nIndex0)
			{
				for (BYTE nIndex1 = 0; nIndex1<m_cbCountInfoIndex[1]; ++nIndex1)
				{
					for (BYTE nIndex2 = 0; nIndex2<m_cbCountInfoIndex[2]; ++nIndex2)
					{
						for (BYTE nIndex3 = 0; nIndex3<m_cbCountInfoIndex[3]; ++nIndex3)
						{
							stResult result;
							result.cbIndex[0] = nIndex0;
							result.cbIndex[1] = nIndex1;
							result.cbIndex[2] = nIndex2;
							result.cbIndex[3] = nIndex3;
							result.cbIndexCount = 4;
							resultVector.push_back(result);
						}
					}
				}
			}
		}
		else if (m_cbCountInfoIndexCount == 3)
		{
			for (BYTE nIndex0 = 0; nIndex0<m_cbCountInfoIndex[0]; ++nIndex0)
			{
				for (BYTE nIndex1 = 0; nIndex1<m_cbCountInfoIndex[1]; ++nIndex1)
				{
					for (BYTE nIndex2 = 0; nIndex2<m_cbCountInfoIndex[2]; ++nIndex2)
					{
						stResult result;
						result.cbIndex[0] = nIndex0;
						result.cbIndex[1] = nIndex1;
						result.cbIndex[2] = nIndex2;
						result.cbIndexCount = 3;
						resultVector.push_back(result);
					}
				}
			}
		}
		else if (m_cbCountInfoIndexCount == 2)
		{
			for (BYTE nIndex0 = 0; nIndex0<m_cbCountInfoIndex[0]; ++nIndex0)
			{
				for (BYTE nIndex1 = 0; nIndex1<m_cbCountInfoIndex[1]; ++nIndex1)
				{
					stResult result;
					result.cbIndex[0] = nIndex0;
					result.cbIndex[1] = nIndex1;
					result.cbIndexCount = 2;
					resultVector.push_back(result);
				}
			}
		}
		else if (m_cbCountInfoIndexCount == 1)
		{
			for (BYTE nIndex0 = 0; nIndex0<m_cbCountInfoIndex[0]; ++nIndex0)
			{
				stResult result;
				result.cbIndex[0] = nIndex0;
				result.cbIndexCount = 1;
				resultVector.push_back(result);
			}
		}

		// �����
		size_t nResultCount = resultVector.size();
		if (nResultCount <= 0)
		{
			return;
		}

		/*	3������ͬʱ,��Ӧ������������,����Ϊ����,�����Ե���,���ﲻ�ô���ȫ���˴���
		��Ϊ�����,��ı�ǰ��ĺ�Ϣ������������Ҫ���¼���һ��ǰ����Ϻ�Ϣ
		*/
		BYTE cbHuXi = m_TempHuFinish.cbHuXi;
		BYTE cbTempWeaveCount = m_TempHuFinish.cbWeaveCount;
		for (BYTE i = 0; i<cbTempWeaveCount; ++i)
		{
			tagWeaveItem &WeaveItem = m_TempHuFinish.WeaveItemArray[i];
			cbHuXi += GetWeaveHuXi(WeaveItem);
		}

		for (size_t i = 0; i<nResultCount; ++i)
		{
			BYTE cbTempHongCardCount = m_TempHuFinish.cbHongCardCount;
			BYTE cbTempHeiCardCount = m_TempHuFinish.cbHeiCardCount;
			BYTE cbTempHuXingCount = m_TempHuFinish.cbHuXingCount;
			BYTE cbXingCard = m_TempHuFinish.cbXingCard;
			BYTE cbTempHuXi = 0;

			BYTE cbCardIndex[MAX_INDEX] = { 0 };
			// ����������ͳ��
			for (BYTE j = 0; j<m_TempHuFinish.cbWeaveCount; ++j)
			{
				tagWeaveItem &WeaveItem = m_TempHuFinish.WeaveItemArray[j];
				for (BYTE k = 0; k<WeaveItem.cbCardCount; ++k)
				{
					BYTE cbIndex = SwitchToCardIndex(WeaveItem.cbCardList[k]);
					if (cbIndex < MAX_INDEX)
					{
						++cbCardIndex[cbIndex];
					}
				}
			}

			// ������涼������������
			BYTE cbAllOnlyKingCount = 0;
			stResult &result = resultVector[i];
			for (BYTE j = 0; j<result.cbIndexCount; ++j)
			{
				tagCountInfo &CountInfo = m_CountInfo[j][result.cbIndex[j]];
				// ������涼������������
				BYTE cbKingCount = 0;
				for (BYTE k = 0; k<CountInfo.cbCardCount; ++k)
				{
					// ������
					if (CountInfo.cbCardList[k] != KING_VALUE)
					{
						cbKingCount = 0;
						break;
					}
					++cbKingCount;
				}
				// ������,���洦��
				if (cbKingCount > 0)
				{
					cbAllOnlyKingCount += cbKingCount;
					continue;
				}

				// �Ѳ��������������ӽ���
				for (BYTE k = 0; k<CountInfo.cbCardCount; ++k)
				{
					BYTE cbIndex = SwitchToCardIndex(CountInfo.cbCardList[k]);
					if (cbIndex < MAX_INDEX)
					{
						++cbCardIndex[cbIndex];
					}
				}

				cbTempHongCardCount += CountInfo.cbHongCardCount;
				cbTempHeiCardCount += CountInfo.cbHeiCardCount;
				cbTempHuXingCount += CountInfo.cbHuXingCount;
				cbTempHuXi += CountInfo.cbHuXi;
			}

			// ��������(��Ҫ��϶��ǵ���������)
			if (m_TempHuFinish.cbXingCard == KING_VALUE)
			{
				if (cbAllOnlyKingCount <= 0)
				{
					BYTE cbMaxIndexCount = 0;
					for (BYTE j = 0; j<MAX_INDEX; ++j)
					{
						if (j == KING_VALUE)
						{
							continue;
						}

						if (cbCardIndex[j] > cbMaxIndexCount)
						{
							cbMaxIndexCount = cbCardIndex[j];

							// ��������
							if (j <= 9)
							{
								cbXingCard = j % 10 + 0x01;
							}
							else
							{
								cbXingCard = j % 10 + 0x11;
							}
						}
					}

					// ���Ƽ������������
					cbTempHuXingCount += cbMaxIndexCount;
				}
				else
				{
					// ����ϵ�
					for (BYTE j = 0; j<result.cbIndexCount; ++j)
					{
						tagCountInfo &CountInfo = m_CountInfo[j][result.cbIndex[j]];
						for (BYTE k = 0; k<CountInfo.cbCardCount; ++k)
						{
							BYTE cbIndex = SwitchToCardIndex(CountInfo.cbCardList[k]);
							// ���Ʋ�����
							if (cbIndex == KING_INDEX)
							{
								continue;
							}
							if (cbIndex < MAX_INDEX)
							{
								++cbCardIndex[cbIndex];
							}
						}
					}

					// 0,1������,2,3������
					BYTE cbMaxIndexCount[4] = { 0 };
					BYTE cbMaxCard[4] = { 0 };
					for (BYTE j = 0; j<MAX_INDEX; ++j)
					{
						// ���Ʋ�����
						if (j == KING_INDEX)
						{
							continue;
						}

						// 0x01,0x07,0x0A
						if (j == 1 || j == 6 || j == 9)
						{
							if (cbCardIndex[j] > cbMaxIndexCount[0])
							{
								cbMaxIndexCount[0] = cbCardIndex[j];
								cbMaxCard[0] = j % 10 + 0x01;
							}
						}
						// 0x11,0x17,0x1A
						else if (j == 11 || j == 16 || j == 19)
						{
							if (cbCardIndex[j] > cbMaxIndexCount[1])
							{
								cbMaxIndexCount[1] = cbCardIndex[j];
								cbMaxCard[1] = j % 10 + 0x11;
							}
						}
						else if (j <= 9)
						{
							if (cbCardIndex[j] > cbMaxIndexCount[2])
							{
								cbMaxIndexCount[2] = cbCardIndex[j];
								cbMaxCard[2] = j % 10 + 0x01;
							}
						}
						else
						{
							if (cbCardIndex[j] > cbMaxIndexCount[3])
							{
								cbMaxIndexCount[3] = cbCardIndex[j];
								cbMaxCard[3] = j % 10 + 0x11;
							}
						}
					}

					for (BYTE j = 0; j<4; ++j)
					{
						BYTE cbKingCount = cbAllOnlyKingCount;
						BYTE cbKingHongCardCount = 0;
						BYTE cbKingHeiCardCount = 0;

						BYTE cbKingCardData = cbMaxCard[j];
						if (cbKingCardData == 0)
						{
							if (j == 0)
							{
								cbKingCardData = 0x02;
							}
							else if (j == 1)
							{
								cbKingCardData = 0x12;
							}
							else if (j == 2)
							{
								cbKingCardData = 0x01;
							}
							else
							{
								cbKingCardData = 0x11;
							}
						}

						BYTE cbValue = cbKingCardData&MASK_VALUE;
						if (cbValue == 0x02 || cbValue == 0x07 || cbValue == 0x0A)
						{
							cbKingHongCardCount = cbKingCount;
						}
						else
						{
							cbKingHeiCardCount = cbKingCount;
						}

						BYTE cbKingHuXi = 0;
						if (cbKingCount == 3)
						{
							cbKingHuXi = (cbKingCardData&MASK_COLOR) > 0 ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;
						}
						else if (cbKingCount == 4)
						{
							cbKingHuXi = (cbKingCardData&MASK_COLOR) > 0 ? TYPE_D_TI_HU_XI : TYPE_X_TI_HU_XI;
						}

						// С����С��Ϣ,������
						if (cbHuXi + cbKingHuXi < MIN_HU_XI)
						{
							continue;
						}

						BYTE cbHuHongHeiBase = 1;
						if (cbTempHongCardCount + cbKingHongCardCount == 0)			// ȫ��
						{
							cbHuHongHeiBase = 4;
						}
						else if (cbTempHongCardCount + cbKingHongCardCount == 1)		// ����
						{
							cbHuHongHeiBase = 3;
						}
						else if (cbTempHongCardCount + cbKingHongCardCount >= 10)		// ���
						{
							cbHuHongHeiBase = 2;
						}

						BYTE cbAllHuXingCount = cbTempHuXingCount + cbKingCount + cbCardIndex[SwitchToCardIndex(cbKingCardData)];
						int nHuScore = ((1 + m_InitialHuFinish.cbAddTunCount + (cbHuXi + cbKingHuXi - MIN_HU_XI) / 3) + cbAllHuXingCount * m_TempHuFinish.cbXingTunCount) * m_TempHuFinish.cbHuBase * cbHuHongHeiBase;
						if (nHuScore <= m_nHuScore)
						{
							continue;
						}

						// ��������
						cbXingCard = cbKingCardData;

						// ��������ʱ��ԭ�򣬱���ǰ������һ��6����ϵģ�������5����ϣ���6������в���ֵ
						BYTE cbWeaveCount = m_TempHuFinish.cbWeaveCount;
						tagWeaveItem WeaveItemArray[MAX_WEAVE];
						for (BYTE j = 0; j<m_TempHuFinish.cbWeaveCount; ++j)
						{
							memcpy(&WeaveItemArray[j], &m_TempHuFinish.WeaveItemArray[j], sizeof(WeaveItemArray[j]));
						}
						// ����������
						for (BYTE j = 0; j<result.cbIndexCount; ++j)
						{
							if (cbWeaveCount >= MAX_WEAVE)
							{
								break;
							}

							tagCountInfo &CountInfo = m_CountInfo[j][result.cbIndex[j]];
							// ������涼������������
							BYTE cbKingCount = 0;
							for (BYTE k = 0; k<CountInfo.cbCardCount; ++k)
							{
								// ������
								if (CountInfo.cbCardList[k] != KING_VALUE)
								{
									cbKingCount = 0;
									break;
								}
								++cbKingCount;
							}
							// ������,���洦��
							if (cbKingCount > 0)
							{
								continue;
							}
							int nWeaveKind = ACK_NULL;
							if (CountInfo.cbCardCount == 3)
							{
								if (CountInfo.cbCardList[0] == CountInfo.cbCardList[1] && CountInfo.cbCardList[1] == CountInfo.cbCardList[2])
								{
									nWeaveKind = ACK_WEI;
								}
								else
								{
									nWeaveKind = ACK_CHI;
								}
							}
							else if (CountInfo.cbCardCount == 4)
							{
								nWeaveKind = ACK_TI;
							}

							WeaveItemArray[cbWeaveCount].nWeaveKind = nWeaveKind;
							WeaveItemArray[cbWeaveCount].cbCardCount = CountInfo.cbCardCount;
							WeaveItemArray[cbWeaveCount].cbCenterCard = CountInfo.cbCardList[0];
							memcpy(&WeaveItemArray[cbWeaveCount].cbCardList, CountInfo.cbCardList, sizeof(WeaveItemArray[cbWeaveCount].cbCardList));
							cbWeaveCount++;
						}

						// ������ȫ���������
						if (cbWeaveCount < MAX_WEAVE)
						{
							int nWeaveKind = ACK_NULL;
							if (cbKingCount == 3)
							{
								nWeaveKind = ACK_WEI;
							}
							else if (cbKingCount == 4)
							{
								nWeaveKind = ACK_TI;
							}

							WeaveItemArray[cbWeaveCount].nWeaveKind = nWeaveKind;
							WeaveItemArray[cbWeaveCount].cbCardCount = cbKingCount;
							WeaveItemArray[cbWeaveCount].cbCenterCard = cbKingCardData;
							for (BYTE k = 0; k<cbKingCount; ++k)
							{
								WeaveItemArray[cbWeaveCount].cbCardList[k] = cbKingCardData;
							}
							cbWeaveCount++;
						}

						m_nHuScore = nHuScore;

						// ���Ƶ����ս������
						m_HuFinish.nHuScore = nHuScore;
						m_HuFinish.cbXingCard = cbXingCard;
						m_HuFinish.cbHuHongHeiBase = cbHuHongHeiBase;
						m_HuFinish.cbHuXi = cbHuXi + cbKingHuXi;
						m_HuFinish.cbHongCardCount = cbTempHongCardCount + cbKingHongCardCount;
						m_HuFinish.cbHeiCardCount = cbTempHeiCardCount + cbKingHeiCardCount;
						m_HuFinish.cbHuXingCount = cbAllHuXingCount;
						m_HuFinish.cbWeaveCount = cbWeaveCount;
						memcpy(m_HuFinish.WeaveItemArray, WeaveItemArray, sizeof(m_TempHuFinish.WeaveItemArray));

						// ��������������
						memcpy(&m_cbFinishIndex, m_cbTempFinishIndex, sizeof(m_cbFinishIndex));
						memcpy(&m_cbFinishIndexCount, m_cbTempFinishIndexCount, sizeof(m_cbFinishIndexCount));
						m_cbFinishAllIndexCount = m_cbTempFinishAllIndexCount;
					}

					return;
				}
			}

			// С����С��Ϣ,������
			if (cbHuXi + cbTempHuXi < MIN_HU_XI)
			{
				continue;
			}

			BYTE cbHuHongHeiBase = 1;
			if (cbTempHongCardCount == 0)			// ȫ��
			{
				cbHuHongHeiBase = 4;
			}
			else if (cbTempHongCardCount == 1)		// ����
			{
				cbHuHongHeiBase = 3;
			}
			else if (cbTempHongCardCount >= 10)		// ���
			{
				cbHuHongHeiBase = 2;
			}

			int nHuScore = ((1 + m_InitialHuFinish.cbAddTunCount + (cbHuXi + cbTempHuXi - MIN_HU_XI) / 3) + cbTempHuXingCount * m_TempHuFinish.cbXingTunCount) * m_TempHuFinish.cbHuBase * cbHuHongHeiBase;
			if (nHuScore <= m_nHuScore)
			{
				continue;
			}

			// ��������ʱ��ԭ�򣬱���ǰ������һ��6����ϵģ�������5����ϣ���6������в���ֵ
			BYTE cbWeaveCount = m_TempHuFinish.cbWeaveCount;
			tagWeaveItem WeaveItemArray[MAX_WEAVE];
			for (BYTE j = 0; j<cbWeaveCount; ++j)
			{
				memcpy(&WeaveItemArray[j], &m_TempHuFinish.WeaveItemArray[j], sizeof(WeaveItemArray[j]));
			}
			// ����������
			for (BYTE j = 0; j<result.cbIndexCount; ++j)
			{
				if (cbWeaveCount >= MAX_WEAVE)
				{
					break;
				}

				tagCountInfo &CountInfo = m_CountInfo[j][result.cbIndex[j]];
				int nWeaveKind = ACK_NULL;
				if (CountInfo.cbCardCount == 3)
				{
					if (CountInfo.cbCardList[0] == CountInfo.cbCardList[1] && CountInfo.cbCardList[1] == CountInfo.cbCardList[2])
					{
						nWeaveKind = ACK_WEI;
					}
					else
					{
						nWeaveKind = ACK_CHI;
					}
				}
				else if (CountInfo.cbCardCount == 4)
				{
					nWeaveKind = ACK_TI;
				}

				WeaveItemArray[cbWeaveCount].nWeaveKind = nWeaveKind;
				WeaveItemArray[cbWeaveCount].cbCardCount = CountInfo.cbCardCount;
				WeaveItemArray[cbWeaveCount].cbCenterCard = CountInfo.cbCardList[0];
				memcpy(&WeaveItemArray[cbWeaveCount].cbCardList, CountInfo.cbCardList, sizeof(WeaveItemArray[cbWeaveCount].cbCardList));
				cbWeaveCount++;
			}

			// ������,�����۷���
			if (m_TempHuFinish.cbEyeCardCount > 0 && cbWeaveCount < MAX_WEAVE)
			{
				int nWeaveKind = ACK_NULL;
				if (m_TempHuFinish.cbEyeCardCount == 3)
				{
					if (m_TempHuFinish.cbEyeCard[0] == m_TempHuFinish.cbEyeCard[1] && m_TempHuFinish.cbEyeCard[1] == m_TempHuFinish.cbEyeCard[2])
					{
						nWeaveKind = ACK_WEI;
					}
					else
					{
						nWeaveKind = ACK_CHI;
					}
				}
				else if (m_TempHuFinish.cbEyeCardCount == 4)
				{
					nWeaveKind = ACK_TI;
				}

				WeaveItemArray[cbWeaveCount].nWeaveKind = nWeaveKind;
				WeaveItemArray[cbWeaveCount].cbCardCount = m_TempHuFinish.cbEyeCardCount;
				WeaveItemArray[cbWeaveCount].cbCenterCard = m_TempHuFinish.cbEyeCard[0];
				memcpy(&WeaveItemArray[cbWeaveCount].cbCardList, m_TempHuFinish.cbEyeCard, sizeof(WeaveItemArray[cbWeaveCount].cbCardList));
				cbWeaveCount++;
			}

			m_nHuScore = nHuScore;

			// ���Ƶ����ս������
			m_HuFinish.nHuScore = nHuScore;
			m_HuFinish.cbXingCard = cbXingCard;
			m_HuFinish.cbHuHongHeiBase = cbHuHongHeiBase;
			m_HuFinish.cbHuXi = cbHuXi + cbTempHuXi;
			m_HuFinish.cbHongCardCount = cbTempHongCardCount;
			m_HuFinish.cbHeiCardCount = cbTempHeiCardCount;
			m_HuFinish.cbHuXingCount = cbTempHuXingCount;
			m_HuFinish.cbWeaveCount = cbWeaveCount;
			memcpy(m_HuFinish.WeaveItemArray, WeaveItemArray, sizeof(m_HuFinish.WeaveItemArray));

			// ��������������
			memcpy(&m_cbFinishIndex, m_cbTempFinishIndex, sizeof(m_cbFinishIndex));
			memcpy(&m_cbFinishIndexCount, m_cbTempFinishIndexCount, sizeof(m_cbFinishIndexCount));
			m_cbFinishAllIndexCount = m_cbTempFinishAllIndexCount;
		}
	}

	//��ȡ��Ϣ
	BYTE GetWeaveHuXi(const tagWeaveItem &WeaveItem)
	{
		//�����Ϣ
		switch (WeaveItem.nWeaveKind)
		{
		case ACK_TI:	//��
		{
			return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 12 : 9;
		}
		break;
		case ACK_PAO:	//��
		{
			return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 9 : 6;
		}
		break;
		case ACK_WEI:	//��
		{
			return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 6 : 3;
		}
		break;
		case ACK_PENG:	//��
		{
			return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 3 : 1;
		}
		break;
		case ACK_CHI:	//��
		{
			//��ȡ��ֵ
			BYTE cbValue1 = WeaveItem.cbCardList[0] & MASK_VALUE;
			BYTE cbValue2 = WeaveItem.cbCardList[1] & MASK_VALUE;
			BYTE cbValue3 = WeaveItem.cbCardList[2] & MASK_VALUE;

			BYTE cbCardIndex[MAX_INDEX] = { 0 };
			// ��ת����С������
			cbCardIndex[cbValue1 - 1]++;
			cbCardIndex[cbValue2 - 1]++;
			cbCardIndex[cbValue3 - 1]++;

			//һ������
			if (cbCardIndex[0] == 1 && cbCardIndex[1] == 1 && cbCardIndex[2] == 1)
			{
				return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 6 : 3;
			}

			//����ʮ��
			if (cbCardIndex[1] == 1 && cbCardIndex[6] == 1 && cbCardIndex[9] == 1)
			{
				return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 6 : 3;
			}

			return 0;
		}
		break;
		default:
		{
			return 0;
		}
		break;
		}

		return 0;
	}

	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData)
	{
		if (cbCardData == KING_VALUE)
		{
			return true;
		}

		BYTE cbValue = (cbCardData&MASK_VALUE);
		BYTE cbColor = (cbCardData&MASK_COLOR) >> 4;
		return (cbValue >= 1 && cbValue <= 10 && cbColor <= 1);
	}

	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData)
	{
		if (cbCardData == KING_VALUE)
		{
			return KING_INDEX;
		}

		if (!IsValidCard(cbCardData))
		{
			return MAX_INDEX;
		}
		return ((cbCardData&MASK_COLOR) >> 4) * 10 + (cbCardData&MASK_VALUE) - 1;
	}

private:
	// ���ƽ�����Ϣ
	tagHuFinishInfo		m_HuFinish;								// ���ս�����Ϣ
	tagHuFinishInfo		m_InitialHuFinish;						// ��ʼ������Ϣ(��ԭ����ʱ��Ϣ��)
	tagHuFinishInfo		m_TempHuFinish;							// ��ʱ������Ϣ(���������ʹ��)

	stHuKeyData			m_HuKeyData[MAX_WEAVE];
	BYTE				m_cbHuKeyDataCount;

	tagCountInfo		m_CountInfo[MAX_WEAVE][7];
	BYTE				m_cbCountInfoIndex[MAX_WEAVE];
	BYTE				m_cbCountInfoIndexCount;

	BYTE				m_cbFinishIndex[MAX_WEAVE][MAX_WEAVE_CARD_COUNT];		// ���ʹ���������
	BYTE				m_cbFinishIndexCount[MAX_WEAVE];						// ���ʹ�������������
	BYTE				m_cbFinishAllIndexCount;								// ���ʹ�������������
	BYTE				m_cbTempFinishIndex[MAX_WEAVE][MAX_WEAVE_CARD_COUNT];	// ��ʱʹ���������(���������ʹ��)
	BYTE				m_cbTempFinishIndexCount[MAX_WEAVE];					// ���ʹ�������������
	BYTE				m_cbTempFinishAllIndexCount;							// ��ʱʹ���������(���������ʹ��)

	BYTE				m_cbHuXi;
	int					m_nHuScore;
};

//////////////////////////////////////////////////////////////////////////
// �����߼���
class CHuPaiLogic
{
public:
	CHuPaiLogic();
	~CHuPaiLogic();

public:
	// ��ʼ���ƺ�Ϣ
	void initCardHuXi();
	// ȡ�ú�Ϣ
	int getCardHuXiVal(int nKey);
	// ȡ�ú�Ϣ
	int getCardHuXiVal(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4 = 0x00);

	// �Ƿ�ӵ�й���
	bool isHavePlayType(enPlayType nType);
	// ��Ϣ��ֵ
	int getKey(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4);
	// ���Ӻ�Ϣ��
	void addMapVal(int nKey, BYTE nVal);

	// ȡ�����б�
	void getTiList();
	// ȡ�����б�
	void getWeiList();
	// ȡ��27A�б�
	void get27AList();
	// ȡ��˳���б�
	void getShunZhiList();

	// ��ȡ���ƽ��
	void getCardHuResult(stCardData stData, stAnswer &answerData, stAnswer &resultMax);
	// ��ʼ������
	bool initCardHu(vector<BYTE> &vctCardIn, stAnswer &resultAnswer, tagHuCardInfo &HuCardInfo);

private:
	// �ƺ�Ϣ��(key��ֵ)
	typedef map<int, BYTE>					MAP_LIST;
	MAP_LIST								m_mapKey;

	// �Ƿ�ֻ�жϺ�
	bool						m_bOnlyHu;
	tagHuCardInfo				m_HuCardInfo;
};

//////////////////////////////////////////////////////////////////////////

#endif	// __HUPAI_LOGIC_H__