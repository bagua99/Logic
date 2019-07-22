#ifndef __HUPAI_LOGIC_H__
#define __HUPAI_LOGIC_H__

#pragma once

#include <wtypes.h>
#include <vector>
#include <map>
#include <algorithm>
#include "GameMessage.h"
using namespace std;

// 规则类型
enum enPlayType
{
	enPlayType_NORMAL = 0,			// 一般
	enPlayType_XDD,					// 小大搭
	enPlayType_27A,					// 二七十
	enPlayType_123,					// 一二三
	enPlayType_Max,
};

// 设置规则
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
	// 牌索引
	BYTE m_cbCardIndex[MAX_WEAVE_CARD_COUNT];
	// 牌索引数量
	BYTE m_cbCardIndexCount;
	// 组合胡息
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

		// 记录位置
		m_cbCardIndex[cbIndex][m_cbCardIndexCount[cbIndex]++] = m_cbCardCount;
		// 插入牌
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
			// 已经是true了
			if (getFlag(cbIndex))
			{
				return;
			}

			// 找到这个索引
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
			// 已经是false了
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

	// 取得牌索引数组(通过传牌值取得)
	BYTE *getCardIndexArrayInValue(BYTE cbCardValue)
	{
		BYTE cbCardIndex = getCardIndex(cbCardValue);
		if (cbCardIndex >= MAX_INDEX)
		{
			return NULL;
		}

		return (BYTE *)&m_cbCardIndex[cbCardIndex];
	}

	// 取得牌索引值(通过传牌值取得)
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

	// 取得牌索引数量
	BYTE getCardIndexCountInValue(BYTE cbCardValue)
	{
		BYTE cbCardIndex = getCardIndex(cbCardValue);
		if (cbCardIndex >= MAX_INDEX)
		{
			return 0;
		}

		return m_cbCardIndexCount[cbCardIndex];
	}

	// 取得牌索引数组(通过传牌索引取得)
	BYTE *getCardIndexArrayInIndex(BYTE cbCardIndex)
	{
		if (cbCardIndex >= MAX_INDEX)
		{
			return NULL;
		}

		return (BYTE *)&m_cbCardIndex[cbCardIndex];
	}

	// 取得牌索引值(通过传牌索引取得)
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

	// 取得牌索引数量(通过传牌索引取得)
	BYTE getCardIndexCountInIndex(BYTE cbCardIndex)
	{
		if (cbCardIndex >= MAX_INDEX)
		{
			return 0;
		}

		return m_cbCardIndexCount[cbCardIndex];
	}

	// 取得牌值索引
	BYTE getCardIndex(BYTE cbValue)
	{
		// 是王牌
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

// 统计信息
struct tagCountInfo
{
	BYTE				cbCardList[MAX_WEAVE_CARD_COUNT];	// 牌列表
	BYTE				cbCardCount;						// 牌数量
	BYTE				cbHuXi;								// 牌胡息
	BYTE				cbHuXingCount;						// 醒牌数量
	BYTE				cbHongCardCount;					// 红牌数量
	BYTE				cbHeiCardCount;						// 黑牌数量

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

		// 加入
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

		// 操作当前信息
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

		// 清除相关
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

	// 只判断胡,不要判断牌数
	int getHuScore(stCardData *pVctCard)
	{
		if (pVctCard == NULL)
		{
			return -1;
		}

		// 截枝,不够胡息,不处理
		if (m_cbHuXi + m_InitialHuFinish.cbEyeCardHuXi < MIN_HU_XI)
		{
			return -1;
		}

		memcpy(&m_TempHuFinish, &m_InitialHuFinish, sizeof(m_TempHuFinish));

		// 最后统计索引
		memset(&m_cbTempFinishIndex, 0, sizeof(m_cbTempFinishIndex));
		memset(&m_cbTempFinishIndexCount, 0, sizeof(m_cbTempFinishIndexCount));
		m_cbTempFinishAllIndexCount = 0;
		BYTE cbTempFinishIndex[MAX_WEAVE][MAX_WEAVE_CARD_COUNT];
		memset(&cbTempFinishIndex, 0, sizeof(cbTempFinishIndex));
		BYTE cbTempFinishIndexCount[MAX_WEAVE];
		memset(&cbTempFinishIndexCount, 0, sizeof(cbTempFinishIndexCount));
		BYTE cbTempFinishAllIndexCount = 0;

		// 统计有王的组合
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

		// 所有组合牌数
		BYTE cbAllCardIndexCount = 0;
		for (BYTE i = 0; i<m_cbHuKeyDataCount; ++i)
		{
			BYTE cbTempWangCardCount = 0;
			BYTE cbTempCardList[MAX_WEAVE_CARD_COUNT] = { 0 };

			stHuKeyData &data = m_HuKeyData[i];
			// 取得组合牌索引数量
			BYTE cbCardIndexCount = data.getCardIndexCount();
			// 胡息
			BYTE cbTempHuXi = data.getHuXi();
			// 加上组合牌数
			cbAllCardIndexCount += cbCardIndexCount;
			for (BYTE j = 0; j<cbCardIndexCount; ++j)
			{
				// 取得牌索引
				BYTE cbCardIndex = data.getCardIndex(j);
				// 取得牌值
				BYTE cbCardData = pVctCard->getCardValue(cbCardIndex);
				if (cbCardData == 0)
				{
					continue;
				}
				// 设置牌列表
				cbTempCardList[j] = cbCardData;
				// 是王牌
				if (cbCardData == KING_VALUE)
				{
					++cbTempWangCardCount;
				}
			}

			// 分析扑克(返回1是直接塞入组合的,记录索引)
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

		// 截枝,不够牌,不处理
		if (m_InitialHuFinish.cbHongCardCount + m_InitialHuFinish.cbHeiCardCount + cbAllCardIndexCount + m_TempHuFinish.cbEyeCardCount < MAX_COUNT)
		{
			return 0;
		}

		// 把组合的加入进来
		for (BYTE i = 0; i<cbTempFinishAllIndexCount; ++i)
		{
			memcpy(&m_cbTempFinishIndex[m_cbTempFinishAllIndexCount], &cbTempFinishIndex[i], sizeof(m_cbTempFinishIndex[m_cbTempFinishAllIndexCount]));
			memcpy(&m_cbTempFinishIndexCount[m_cbTempFinishAllIndexCount], &cbTempFinishIndexCount[i], sizeof(m_cbTempFinishIndexCount[m_cbTempFinishAllIndexCount]));
			++m_cbTempFinishAllIndexCount;
		}

		// 加入牌眼分析
		if (m_TempHuFinish.cbEyeCardCount > 0)
		{
			BYTE cbTempWangCardCount = 0;
			BYTE cbTempCardList[MAX_WEAVE_CARD_COUNT] = { 0 };
			BYTE cbCardIndexCount = 0;
			// 牌眼有值,加入组合
			for (int i = 0; i<MAX_WEAVE_CARD_COUNT; ++i)
			{
				// 取得牌值
				BYTE cbCardData = m_TempHuFinish.cbEyeCard[i];
				if (cbCardData == 0)
				{
					continue;
				}
				// 设置牌列表
				cbTempCardList[cbCardIndexCount++] = cbCardData;
				// 是王牌
				if (cbCardData == KING_VALUE)
				{
					++cbTempWangCardCount;
				}
			}

			// 牌眼前面就转换成具体值,这里没有王,直接转换成醒,红,黑数量
			if (cbTempWangCardCount <= 0)
			{
				// 增加计算牌信息
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
				// 增加胡息
				m_TempHuFinish.cbHuXi += cbHuXi;
			}
			else
			{
				// 还有王,说明几个都是王,后面处理
				BYTE cbCardValue[7] = { 0 };
				BYTE cbCardValueIndex = 0;

				// 把王设置成醒牌
				cbCardValue[cbCardValueIndex++] = m_TempHuFinish.cbXingCard;
				// 醒牌不是王，可以默认设置
				if (m_TempHuFinish.cbXingCard != KING_VALUE)
				{
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// 把王设置成大1(黑)
						cbCardValue[cbCardValueIndex++] = 0x11;
					}
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// 把王设置成大2(红)
						cbCardValue[cbCardValueIndex++] = 0x12;
					}
				}

				for (BYTE nIndex = 0; nIndex<cbCardValueIndex; ++nIndex)
				{
					BYTE cbCard = cbCardValue[nIndex];

					for (BYTE i = 0; i<cbCardIndexCount; ++i)
					{
						// 把王设置成这张牌
						cbTempCardList[i] = cbCard;
					}

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// 增加计算牌信息
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

		// 分析最大分数
		analysisMaxScore();

		return m_nHuScore;
	}

	int getHuScore()
	{
		return m_nHuScore;
	}

	// 增加计算牌信息
	void addCardCountInfo(BYTE cbCardList[], BYTE cbCardCount)
	{
		BYTE cbHuXingCount = 0;
		BYTE cbHongCardCount = 0;
		BYTE cbHeiCardCount = 0;
		for (BYTE i = 0; i<cbCardCount; ++i)
		{
			BYTE cbCardData = cbCardList[i];
			// 如果是醒牌(增加王牌这个醒)
			if (cbCardData == m_TempHuFinish.cbXingCard)
			{
				cbHuXingCount++;
			}
			// 先转成小牌
			BYTE cbCardValue = cbCardData&MASK_VALUE;
			// 是2,7,A
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

	// 增加计算牌信息
	void addCardCountInfo(BYTE cbCardList[], BYTE cbCardCount, BYTE &cbHuXingCount, BYTE &cbHongCardCount, BYTE &cbHeiCardCount)
	{
		cbHuXingCount = 0;
		cbHongCardCount = 0;
		cbHeiCardCount = 0;
		for (BYTE i = 0; i<cbCardCount; ++i)
		{
			BYTE cbCardData = cbCardList[i];
			// 如果是醒牌(增加王牌这个醒)
			if (cbCardData == m_TempHuFinish.cbXingCard)
			{
				cbHuXingCount++;
			}
			// 先转成小牌
			BYTE cbCardValue = cbCardData&MASK_VALUE;
			// 是2,7,A
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
		// 没有王,直接放入结果里面
		if (cbTempWangCardCount == 0)
		{
			int nWeaveKind = ACK_NULL;
			if (cbCardIndexCount == 3)
			{
				if (cbTempCardList[0] == cbTempCardList[1] && cbTempCardList[1] == cbTempCardList[2])
				{
					// 大牌
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

			// 增加计算牌信息
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
			// 只有2张牌(直接塞入结果)
			if (cbCardIndexCount == 2)
			{
				vector<BYTE> vctTemp;
				vctTemp.push_back(cbTempCardList[0]);
				vctTemp.push_back(cbTempCardList[1]);
				sort(vctTemp.begin(), vctTemp.end());

				// 这个王必定跟等于这个牌，凑对
				cbTempCardList[0] = vctTemp[0];
				cbTempCardList[1] = vctTemp[0];

				// 增加计算牌信息
				addCardCountInfo(cbTempCardList, cbCardIndexCount);

				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].nWeaveKind = ACK_NULL;
				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardCount = cbCardIndexCount;
				m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCenterCard = cbTempCardList[0];
				memcpy(&m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList, cbTempCardList, sizeof(m_TempHuFinish.WeaveItemArray[m_TempHuFinish.cbWeaveCount].cbCardList));
				++m_TempHuFinish.cbWeaveCount;

				return 1;
			}
			else if (cbCardIndexCount == 4)	//(直接塞入结果)
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

				// 增加计算牌信息
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

				// 3个牌本质就是x+y=z已知2个求第3个数

				if (vctTemp[0] == vctTemp[1])
				{
					// 第1张跟第2张一样，王也带这张，或者他的大小搭张
					cbCardValue[cbCardValueIndex++] = vctTemp[0];
					cbCardValue[cbCardValueIndex++] = (vctTemp[0] + 0x10) % 0x20;
				}
				else	// 2张不一样的
				{
					// 差值超过2说明是大小搭,或者27A类型的
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
								// 把王设置成第1张牌
								cbCardValue[cbCardValueIndex++] = vctTemp[0];
								// 把王设置成第2张牌
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
								// 把王设置成第1张牌
								cbCardValue[cbCardValueIndex++] = vctTemp[0];
								// 把王设置成第2张牌
								cbCardValue[cbCardValueIndex++] = vctTemp[1];
							}
						}
						else	// 本应该判断0x0A,但因为上面排序了,所以不存在此种可能
						{
							// 把王设置成第1张牌
							cbCardValue[cbCardValueIndex++] = vctTemp[0];
							// 把王设置成第2张牌
							cbCardValue[cbCardValueIndex++] = vctTemp[1];
						}
					}
					else if (nValue == 2)	// 相差为2
					{
						cbCardValue[cbCardValueIndex++] = vctTemp[0] + 1;
					}
					else
					{
						// 附近值
						// 第1个是最小的值
						BYTE cbValue = vctTemp[0] & MASK_VALUE;
						// 最小的值是1
						if (cbValue <= 0x01)
						{
							// 王只能组个上面的值
							// 把王设置成第1张牌值+2
							cbCardValue[cbCardValueIndex++] = vctTemp[0] + 2;
						}
						else if (cbValue >= 0x09)	// 最小的值是9
						{
							// 只能往下组个
							// 把王设置成第1张牌值-1
							cbCardValue[cbCardValueIndex++] = vctTemp[0] - 1;
						}
						else
						{
							// 把王设置成第1张牌值+2
							cbCardValue[cbCardValueIndex++] = vctTemp[0] + 2;
							// 把王设置成第1张牌值-1
							cbCardValue[cbCardValueIndex++] = vctTemp[0] - 1;
						}
					}
				}

				for (BYTE nIndex = 0; nIndex<cbCardValueIndex; ++nIndex)
				{
					BYTE cbCard = cbCardValue[nIndex];

					// 把王设置成这张牌
					cbTempCardList[0] = vctTemp[0];
					cbTempCardList[1] = vctTemp[1];
					cbTempCardList[2] = cbCard;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// 增加计算牌信息
					addCardCountInfo(cbTempCardList, cbCardIndexCount, cbHuXingCount, cbHongCardCount, cbHeiCardCount);

					BYTE cbHuXi = 0;
					// 3张相同
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
						//获取数值
						BYTE cbValue1 = cbTempCardList[0] & MASK_VALUE;
						BYTE cbValue2 = cbTempCardList[1] & MASK_VALUE;
						BYTE cbValue3 = cbTempCardList[2] & MASK_VALUE;

						BYTE cbCardIndex[MAX_INDEX] = { 0 };
						// 都转换成小的索引
						cbCardIndex[cbValue1 - 1]++;
						cbCardIndex[cbValue2 - 1]++;
						cbCardIndex[cbValue3 - 1]++;

						//一二三
						if (cbCardIndex[0] == 1 && cbCardIndex[1] == 1 && cbCardIndex[2] == 1)
						{
							cbHuXi = ((cbTempCardList[0] & MASK_COLOR) > 0) ? TYPE_D_123_HU_XI : TYPE_X_123_HU_XI;
						}

						//二七十
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
			// 只有2张牌
			if (cbCardIndexCount == 2)
			{
				BYTE cbCardValue[7] = { 0 };
				BYTE cbCardValueIndex = 0;

				// 把王设置成醒牌
				cbCardValue[cbCardValueIndex++] = m_TempHuFinish.cbXingCard;
				// 醒牌不是王，可以默认设置
				if (m_TempHuFinish.cbXingCard != KING_VALUE)
				{
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// 把王设置成大1(黑)
						cbCardValue[cbCardValueIndex++] = 0x11;
					}
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// 把王设置成大2(红)
						cbCardValue[cbCardValueIndex++] = 0x12;
					}
				}

				for (BYTE nIndex = 0; nIndex<cbCardValueIndex; ++nIndex)
				{
					BYTE cbCard = cbCardValue[nIndex];

					// 把王设置成这张牌
					cbTempCardList[0] = cbCard;
					cbTempCardList[1] = cbCard;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// 增加计算牌信息
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

				// 增加计算牌信息
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

				// 3个牌本质就是x+y=z已知2个求第3个数
				BYTE cbCard = vctTemp[0];
				BYTE cbValue = cbCard&MASK_VALUE;

				// 组偎
				{
					// 第一个王代替牌
					// 第二个王代替牌
					BYTE cbCardData = cbTempCardList[0];
					cbTempCardList[1] = cbCardData;
					cbTempCardList[2] = cbCardData;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// 增加计算牌信息
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
					// 是2
					if (cbValue == 0x02)
					{
						// 第一个王代替牌
						cbTempCardList[1] = cbTempCardList[0] + 0x05;
						// 第二个王代替牌
						cbTempCardList[2] = cbTempCardList[0] + 0x08;

						BYTE cbHuXingCount = 0;
						BYTE cbHongCardCount = 0;
						BYTE cbHeiCardCount = 0;
						// 增加计算牌信息
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
						// 第一个王代替牌
						cbTempCardList[1] = cbTempCardList[0] - 0x05;
						// 第二个王代替牌
						cbTempCardList[2] = cbTempCardList[0] + 0x03;

						BYTE cbHuXingCount = 0;
						BYTE cbHongCardCount = 0;
						BYTE cbHeiCardCount = 0;
						// 增加计算牌信息
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
						// 第一个王代替牌
						cbTempCardList[1] = cbTempCardList[0] - 0x08;
						// 第二个王代替牌
						cbTempCardList[2] = cbTempCardList[0] - 0x03;

						BYTE cbHuXingCount = 0;
						BYTE cbHongCardCount = 0;
						BYTE cbHeiCardCount = 0;
						// 增加计算牌信息
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

				//顺子判断
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

					//设置结果
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
					// 第一个王代替牌
					cbTempCardList[1] = cbCard + cbResultCard[i][1] - cbResultCard[i][0];
					// 第二个王代替牌
					cbTempCardList[2] = cbCard + cbResultCard[i][2] - cbResultCard[i][0];

					BYTE cbHuXi = 0;
					if (cbValue == 0x01)
					{
						cbHuXi = ((cbCard&MASK_COLOR) > 0) ? TYPE_D_123_HU_XI : TYPE_X_123_HU_XI;
					}

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// 增加计算牌信息
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

				// 小大搭(XXD)
				{
					// 第一个王代替牌
					cbTempCardList[1] = cbTempCardList[0];
					// 第二个王代替牌
					cbTempCardList[2] = (cbTempCardList[0] + 0x10) % 0x20;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// 增加计算牌信息
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

				// 小大搭(XDD)
				{
					// 第一个王代替牌
					// 第二个王代替牌
					BYTE cbCardData = (cbTempCardList[0] + 0x10) % 0x20;
					cbTempCardList[1] = cbCardData;
					cbTempCardList[2] = cbCardData;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// 增加计算牌信息
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

				// 把王设置成醒牌
				cbCardValue[cbCardValueIndex++] = m_TempHuFinish.cbXingCard;
				// 醒牌不是王，可以默认设置
				if (m_TempHuFinish.cbXingCard != KING_VALUE)
				{
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// 把王设置成大1(黑)
						cbCardValue[cbCardValueIndex++] = 0x11;
					}
					if (m_TempHuFinish.cbXingCard != 0x11)
					{
						// 把王设置成大2(红)
						cbCardValue[cbCardValueIndex++] = 0x12;
					}
				}

				for (BYTE nIndex = 0; nIndex<cbCardValueIndex; ++nIndex)
				{
					BYTE cbCard = cbCardValue[nIndex];

					// 把王设置成这张牌
					cbTempCardList[0] = cbCard;
					cbTempCardList[1] = cbCard;
					cbTempCardList[2] = cbCard;

					BYTE cbHuXingCount = 0;
					BYTE cbHongCardCount = 0;
					BYTE cbHeiCardCount = 0;
					// 增加计算牌信息
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

				// 增加计算牌信息
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

			// 把王设置成醒牌
			cbCardValue[cbCardValueIndex++] = m_TempHuFinish.cbXingCard;
			// 醒牌不是王，可以默认设置
			if (m_TempHuFinish.cbXingCard != KING_VALUE)
			{
				if (m_TempHuFinish.cbXingCard != 0x11)
				{
					// 把王设置成大1(黑)
					cbCardValue[cbCardValueIndex++] = 0x11;
				}
				if (m_TempHuFinish.cbXingCard != 0x11)
				{
					// 把王设置成大2(红)
					cbCardValue[cbCardValueIndex++] = 0x12;
				}
			}

			for (BYTE nIndex = 0; nIndex<cbCardValueIndex; ++nIndex)
			{
				BYTE cbCard = cbCardValue[nIndex];

				// 把王设置成这张牌
				cbTempCardList[0] = cbCard;
				cbTempCardList[1] = cbCard;
				cbTempCardList[2] = cbCard;
				cbTempCardList[3] = cbCard;

				BYTE cbHuXingCount = 0;
				BYTE cbHongCardCount = 0;
				BYTE cbHeiCardCount = 0;
				// 增加计算牌信息
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
			// 不够胡息
			if (m_cbHuXi + m_InitialHuFinish.cbEyeCardHuXi < MIN_HU_XI)
			{
				m_nHuScore = 0;
				return;
			}

			// 醒牌是王
			if (m_TempHuFinish.cbXingCard == KING_VALUE)
			{
				BYTE cbCardIndex[MAX_INDEX] = { 0 };
				// 组合里面的牌统计
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
			if (m_TempHuFinish.cbHongCardCount == 0)			// 全黑
			{
				cbHuHongHeiBase *= 4;
			}
			else if (m_TempHuFinish.cbHongCardCount == 1)		// 点摸
			{
				cbHuHongHeiBase *= 3;
			}
			else if (m_TempHuFinish.cbHongCardCount >= 10)	// 红胡
			{
				cbHuHongHeiBase *= 2;
			}

			int nHuScore = ((1 + m_InitialHuFinish.cbAddTunCount + (m_cbHuXi + m_InitialHuFinish.cbEyeCardHuXi - MIN_HU_XI) / 3) + m_TempHuFinish.cbHuXingCount * m_TempHuFinish.cbXingTunCount) * m_TempHuFinish.cbHuBase * cbHuHongHeiBase;
			if (nHuScore <= m_nHuScore)
			{
				return;
			}

			// 这里用临时的原因，比如前面组了一个6个组合的，本次是5个组合，第6个组合有残留值
			BYTE cbWeaveCount = m_TempHuFinish.cbWeaveCount;
			tagWeaveItem WeaveItemArray[MAX_WEAVE];
			for (BYTE j = 0; j<cbWeaveCount; ++j)
			{
				memcpy(&WeaveItemArray[j], &m_TempHuFinish.WeaveItemArray[j], sizeof(WeaveItemArray[j]));
			}

			// 有牌眼,将牌眼放入
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

			// 复制最后索引相关
			memcpy(&m_cbFinishIndex, m_cbTempFinishIndex, sizeof(m_cbFinishIndex));
			memcpy(&m_cbFinishIndexCount, m_cbTempFinishIndexCount, sizeof(m_cbFinishIndexCount));
			m_cbFinishAllIndexCount = m_cbTempFinishAllIndexCount;
			return;
		}

		// 结果容器
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

		// 有组合
		size_t nResultCount = resultVector.size();
		if (nResultCount <= 0)
		{
			return;
		}

		/*	3张牌相同时,本应该区分碰和偎,但因为有王,不可以点炮,这里不用处理，全按偎处理
		因为新组合,会改变前面的胡息，所以这里需要重新计算一遍前面组合胡息
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
			// 组合里面的牌统计
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

			// 组合里面都是王的总数量
			BYTE cbAllOnlyKingCount = 0;
			stResult &result = resultVector[i];
			for (BYTE j = 0; j<result.cbIndexCount; ++j)
			{
				tagCountInfo &CountInfo = m_CountInfo[j][result.cbIndex[j]];
				// 组合里面都是王的总数量
				BYTE cbKingCount = 0;
				for (BYTE k = 0; k<CountInfo.cbCardCount; ++k)
				{
					// 不是王
					if (CountInfo.cbCardList[k] != KING_VALUE)
					{
						cbKingCount = 0;
						break;
					}
					++cbKingCount;
				}
				// 都是王,后面处理
				if (cbKingCount > 0)
				{
					cbAllOnlyKingCount += cbKingCount;
					continue;
				}

				// 把不是王的牌数量加进来
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

			// 醒牌是王(还要组合都是单出来的王)
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

							// 设置醒牌
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

					// 醒牌加上最大牌数量
					cbTempHuXingCount += cbMaxIndexCount;
				}
				else
				{
					// 王组合的
					for (BYTE j = 0; j<result.cbIndexCount; ++j)
					{
						tagCountInfo &CountInfo = m_CountInfo[j][result.cbIndex[j]];
						for (BYTE k = 0; k<CountInfo.cbCardCount; ++k)
						{
							BYTE cbIndex = SwitchToCardIndex(CountInfo.cbCardList[k]);
							// 王牌不处理
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

					// 0,1红索引,2,3黑索引
					BYTE cbMaxIndexCount[4] = { 0 };
					BYTE cbMaxCard[4] = { 0 };
					for (BYTE j = 0; j<MAX_INDEX; ++j)
					{
						// 王牌不处理
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

						// 小于最小胡息,不处理
						if (cbHuXi + cbKingHuXi < MIN_HU_XI)
						{
							continue;
						}

						BYTE cbHuHongHeiBase = 1;
						if (cbTempHongCardCount + cbKingHongCardCount == 0)			// 全黑
						{
							cbHuHongHeiBase = 4;
						}
						else if (cbTempHongCardCount + cbKingHongCardCount == 1)		// 点摸
						{
							cbHuHongHeiBase = 3;
						}
						else if (cbTempHongCardCount + cbKingHongCardCount >= 10)		// 红胡
						{
							cbHuHongHeiBase = 2;
						}

						BYTE cbAllHuXingCount = cbTempHuXingCount + cbKingCount + cbCardIndex[SwitchToCardIndex(cbKingCardData)];
						int nHuScore = ((1 + m_InitialHuFinish.cbAddTunCount + (cbHuXi + cbKingHuXi - MIN_HU_XI) / 3) + cbAllHuXingCount * m_TempHuFinish.cbXingTunCount) * m_TempHuFinish.cbHuBase * cbHuHongHeiBase;
						if (nHuScore <= m_nHuScore)
						{
							continue;
						}

						// 设置醒牌
						cbXingCard = cbKingCardData;

						// 这里用临时的原因，比如前面组了一个6个组合的，本次是5个组合，第6个组合有残留值
						BYTE cbWeaveCount = m_TempHuFinish.cbWeaveCount;
						tagWeaveItem WeaveItemArray[MAX_WEAVE];
						for (BYTE j = 0; j<m_TempHuFinish.cbWeaveCount; ++j)
						{
							memcpy(&WeaveItemArray[j], &m_TempHuFinish.WeaveItemArray[j], sizeof(WeaveItemArray[j]));
						}
						// 设置最后组合
						for (BYTE j = 0; j<result.cbIndexCount; ++j)
						{
							if (cbWeaveCount >= MAX_WEAVE)
							{
								break;
							}

							tagCountInfo &CountInfo = m_CountInfo[j][result.cbIndex[j]];
							// 组合里面都是王的总数量
							BYTE cbKingCount = 0;
							for (BYTE k = 0; k<CountInfo.cbCardCount; ++k)
							{
								// 不是王
								if (CountInfo.cbCardList[k] != KING_VALUE)
								{
									cbKingCount = 0;
									break;
								}
								++cbKingCount;
							}
							// 都是王,后面处理
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

						// 这里组全是王的组合
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

						// 复制到最终结果里面
						m_HuFinish.nHuScore = nHuScore;
						m_HuFinish.cbXingCard = cbXingCard;
						m_HuFinish.cbHuHongHeiBase = cbHuHongHeiBase;
						m_HuFinish.cbHuXi = cbHuXi + cbKingHuXi;
						m_HuFinish.cbHongCardCount = cbTempHongCardCount + cbKingHongCardCount;
						m_HuFinish.cbHeiCardCount = cbTempHeiCardCount + cbKingHeiCardCount;
						m_HuFinish.cbHuXingCount = cbAllHuXingCount;
						m_HuFinish.cbWeaveCount = cbWeaveCount;
						memcpy(m_HuFinish.WeaveItemArray, WeaveItemArray, sizeof(m_TempHuFinish.WeaveItemArray));

						// 复制最后索引相关
						memcpy(&m_cbFinishIndex, m_cbTempFinishIndex, sizeof(m_cbFinishIndex));
						memcpy(&m_cbFinishIndexCount, m_cbTempFinishIndexCount, sizeof(m_cbFinishIndexCount));
						m_cbFinishAllIndexCount = m_cbTempFinishAllIndexCount;
					}

					return;
				}
			}

			// 小于最小胡息,不处理
			if (cbHuXi + cbTempHuXi < MIN_HU_XI)
			{
				continue;
			}

			BYTE cbHuHongHeiBase = 1;
			if (cbTempHongCardCount == 0)			// 全黑
			{
				cbHuHongHeiBase = 4;
			}
			else if (cbTempHongCardCount == 1)		// 点摸
			{
				cbHuHongHeiBase = 3;
			}
			else if (cbTempHongCardCount >= 10)		// 红胡
			{
				cbHuHongHeiBase = 2;
			}

			int nHuScore = ((1 + m_InitialHuFinish.cbAddTunCount + (cbHuXi + cbTempHuXi - MIN_HU_XI) / 3) + cbTempHuXingCount * m_TempHuFinish.cbXingTunCount) * m_TempHuFinish.cbHuBase * cbHuHongHeiBase;
			if (nHuScore <= m_nHuScore)
			{
				continue;
			}

			// 这里用临时的原因，比如前面组了一个6个组合的，本次是5个组合，第6个组合有残留值
			BYTE cbWeaveCount = m_TempHuFinish.cbWeaveCount;
			tagWeaveItem WeaveItemArray[MAX_WEAVE];
			for (BYTE j = 0; j<cbWeaveCount; ++j)
			{
				memcpy(&WeaveItemArray[j], &m_TempHuFinish.WeaveItemArray[j], sizeof(WeaveItemArray[j]));
			}
			// 设置最后组合
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

			// 有牌眼,将牌眼放入
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

			// 复制到最终结果里面
			m_HuFinish.nHuScore = nHuScore;
			m_HuFinish.cbXingCard = cbXingCard;
			m_HuFinish.cbHuHongHeiBase = cbHuHongHeiBase;
			m_HuFinish.cbHuXi = cbHuXi + cbTempHuXi;
			m_HuFinish.cbHongCardCount = cbTempHongCardCount;
			m_HuFinish.cbHeiCardCount = cbTempHeiCardCount;
			m_HuFinish.cbHuXingCount = cbTempHuXingCount;
			m_HuFinish.cbWeaveCount = cbWeaveCount;
			memcpy(m_HuFinish.WeaveItemArray, WeaveItemArray, sizeof(m_HuFinish.WeaveItemArray));

			// 复制最后索引相关
			memcpy(&m_cbFinishIndex, m_cbTempFinishIndex, sizeof(m_cbFinishIndex));
			memcpy(&m_cbFinishIndexCount, m_cbTempFinishIndexCount, sizeof(m_cbFinishIndexCount));
			m_cbFinishAllIndexCount = m_cbTempFinishAllIndexCount;
		}
	}

	//获取胡息
	BYTE GetWeaveHuXi(const tagWeaveItem &WeaveItem)
	{
		//计算胡息
		switch (WeaveItem.nWeaveKind)
		{
		case ACK_TI:	//提
		{
			return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 12 : 9;
		}
		break;
		case ACK_PAO:	//跑
		{
			return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 9 : 6;
		}
		break;
		case ACK_WEI:	//偎
		{
			return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 6 : 3;
		}
		break;
		case ACK_PENG:	//碰
		{
			return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 3 : 1;
		}
		break;
		case ACK_CHI:	//吃
		{
			//获取数值
			BYTE cbValue1 = WeaveItem.cbCardList[0] & MASK_VALUE;
			BYTE cbValue2 = WeaveItem.cbCardList[1] & MASK_VALUE;
			BYTE cbValue3 = WeaveItem.cbCardList[2] & MASK_VALUE;

			BYTE cbCardIndex[MAX_INDEX] = { 0 };
			// 都转换成小的索引
			cbCardIndex[cbValue1 - 1]++;
			cbCardIndex[cbValue2 - 1]++;
			cbCardIndex[cbValue3 - 1]++;

			//一二三吃
			if (cbCardIndex[0] == 1 && cbCardIndex[1] == 1 && cbCardIndex[2] == 1)
			{
				return ((WeaveItem.cbCardList[0] & MASK_COLOR) > 0) ? 6 : 3;
			}

			//二七十吃
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

	//有效判断
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

	//扑克转换
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
	// 胡牌结算信息
	tagHuFinishInfo		m_HuFinish;								// 最终结束信息
	tagHuFinishInfo		m_InitialHuFinish;						// 初始结束信息(还原计算时信息用)
	tagHuFinishInfo		m_TempHuFinish;							// 临时结束信息(计算过程中使用)

	stHuKeyData			m_HuKeyData[MAX_WEAVE];
	BYTE				m_cbHuKeyDataCount;

	tagCountInfo		m_CountInfo[MAX_WEAVE][7];
	BYTE				m_cbCountInfoIndex[MAX_WEAVE];
	BYTE				m_cbCountInfoIndexCount;

	BYTE				m_cbFinishIndex[MAX_WEAVE][MAX_WEAVE_CARD_COUNT];		// 最后使用组合索引
	BYTE				m_cbFinishIndexCount[MAX_WEAVE];						// 最后使用组合索引数量
	BYTE				m_cbFinishAllIndexCount;								// 最后使用所有组合数量
	BYTE				m_cbTempFinishIndex[MAX_WEAVE][MAX_WEAVE_CARD_COUNT];	// 临时使用组合索引(计算过程中使用)
	BYTE				m_cbTempFinishIndexCount[MAX_WEAVE];					// 最后使用组合索引数量
	BYTE				m_cbTempFinishAllIndexCount;							// 临时使用组合数量(计算过程中使用)

	BYTE				m_cbHuXi;
	int					m_nHuScore;
};

//////////////////////////////////////////////////////////////////////////
// 胡牌逻辑类
class CHuPaiLogic
{
public:
	CHuPaiLogic();
	~CHuPaiLogic();

public:
	// 初始化牌胡息
	void initCardHuXi();
	// 取得胡息
	int getCardHuXiVal(int nKey);
	// 取得胡息
	int getCardHuXiVal(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4 = 0x00);

	// 是否拥有规则
	bool isHavePlayType(enPlayType nType);
	// 胡息键值
	int getKey(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4);
	// 增加胡息表
	void addMapVal(int nKey, BYTE nVal);

	// 取得提列表
	void getTiList();
	// 取得偎列表
	void getWeiList();
	// 取得27A列表
	void get27AList();
	// 取得顺子列表
	void getShunZhiList();

	// 获取胡牌结果
	void getCardHuResult(stCardData stData, stAnswer &answerData, stAnswer &resultMax);
	// 初始化胡牌
	bool initCardHu(vector<BYTE> &vctCardIn, stAnswer &resultAnswer, tagHuCardInfo &HuCardInfo);

private:
	// 牌胡息表(key键值)
	typedef map<int, BYTE>					MAP_LIST;
	MAP_LIST								m_mapKey;

	// 是否只判断胡
	bool						m_bOnlyHu;
	tagHuCardInfo				m_HuCardInfo;
};

//////////////////////////////////////////////////////////////////////////

#endif	// __HUPAI_LOGIC_H__