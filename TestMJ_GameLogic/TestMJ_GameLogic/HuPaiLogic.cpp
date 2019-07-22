#include "HuPaiLogic.h"

/////////////////////////////////////////////////////		CardKey		/////////////////////////////////////////////////////
// 构造函数
CardKey::CardKey()
{
	Clear();
}

// 析构函数
CardKey::~CardKey()
{

}

// 重载构造函数
CardKey::CardKey(BYTE byKey, BYTE byCardIndex0, BYTE byCardIndex1, BYTE byCardIndex2, BYTE byCardIndex3)
{
	Clear();

	m_byKey = byKey;
	m_byCardIndex[m_byCardIndexCount++] = byCardIndex0;
	m_byCardIndex[m_byCardIndexCount++] = byCardIndex1;
	if (byCardIndex2 < MAX_INDEX)
	{
		m_byCardIndex[m_byCardIndexCount++] = byCardIndex2;
	}
	if (byCardIndex3 < MAX_INDEX)
	{
		m_byCardIndex[m_byCardIndexCount++] = byCardIndex3;
	}
}

// 取得牌索引数量
BYTE CardKey::GetCardIndexCount()
{
	return m_byCardIndexCount;
}

// 取得牌索引
BYTE CardKey::GetCardIndex(BYTE byIndex)
{
	if (byIndex > m_byCardIndexCount)
	{
		return MAX_INDEX;
	}

	return m_byCardIndex[byIndex];
}

// 取得权值
BYTE CardKey::GetKey()
{
	return m_byKey;
}

// 清理函数
void CardKey::Clear()
{
	memset(&m_byCardIndex, 0, sizeof(m_byCardIndex));
	m_byCardIndexCount = 0;
	m_byKey = 0;
}
/////////////////////////////////////////////////////		CardKey		/////////////////////////////////////////////////////


/////////////////////////////////////////////////////		CardData		/////////////////////////////////////////////////////
// 构造函数
CardData::CardData()
{
	Clear();
}

// 析构函数
CardData::~CardData()
{

}

// 增加牌
void CardData::AddCard(BYTE byCardValue)
{
	BYTE byCardIndex = GetCardIndex(byCardValue);
	if (byCardIndex >= MAX_INDEX)
	{
		return;
	}

	if (m_byCardCount >= MAX_COUNT)
	{
		return;
	}

	if (m_byCardIndexCount[byCardIndex] >= MAX_WEAVE_CARD_COUNT)
	{
		return;
	}

	// 记录位置
	m_byCardIndex[byCardIndex][m_byCardIndexCount[byCardIndex]++] = m_byCardCount;
	// 插入牌
	m_byCard[m_byCardCount++] = byCardValue;
	// 增加剩余数量
	m_byLeftCount++;
}

// 取得剩余牌索引
BYTE *CardData::GetLeftIndex()
{
	memset(&m_byLeftIndex, 0, sizeof(m_byLeftIndex));
	m_byLeftIndexCount = 0;
	for (BYTE i = 0; i < m_byCardCount; i++)
	{
		if (!GetFlag(i))
		{
			m_byLeftIndex[m_byLeftIndexCount++] = i;
		}
	}

	return (BYTE *)&m_byLeftIndex;
}

// 取得剩余牌索引数量
BYTE CardData::GetLeftIndexCount()
{
	return m_byLeftIndexCount;
}

// 取得牌索引
BYTE CardData::GetCardIndex(BYTE byCardValue)
{
	BYTE byValue = (byCardValue&MASK_VALUE);
	BYTE byColor = (byCardValue&MASK_COLOR) >> 4;
	if (!((byValue >= 1 && byValue <= 9 && byColor <= 2) || (byValue >= 1 && byValue <= 7 && byColor == 3)))
	{
		return MAX_INDEX;
	}

	return (byColor * 9 + byValue - 1);
}

// 取得牌索引对应牌值
BYTE CardData::GetCardValue(BYTE byCardIndex)
{
	if (byCardIndex >= m_byCardCount)
	{
		return 0;
	}

	return m_byCard[byCardIndex];
}

// 设置标志
void CardData::SetFlag(BYTE byIndex, bool bFlag)
{
	if (byIndex >= m_byCardCount || byIndex >= MAX_COUNT)
	{
		return;
	}

	BYTE byCardIndex = GetCardIndex(m_byCard[byIndex]);
	if (byCardIndex >= MAX_INDEX)
	{
		return;
	}

	if (bFlag)
	{
		if (GetFlag(byIndex))
		{
			return;
		}

		for (BYTE i = 0; i < MAX_WEAVE_CARD_COUNT; i++)
		{
			if (m_byCardIndex[byCardIndex][i] == byIndex)
			{
				if (i + 1 < MAX_WEAVE_CARD_COUNT)
				{
					memmove(&m_byCardIndex[byCardIndex][i], &m_byCardIndex[byCardIndex][i + 1], (MAX_WEAVE_CARD_COUNT - i - 1) * sizeof(m_byCardIndex[byCardIndex][i]));
				}
				--m_byCardIndexCount[byCardIndex];
				memset(&m_byCardIndex[byCardIndex][m_byCardIndexCount[byCardIndex]], 0, sizeof(m_byCardIndex[byCardIndex][m_byCardIndexCount[byCardIndex]]));

				break;
			}
		}

		--m_byLeftCount;
		m_nFlag |= (1 << byIndex);
	}
	else
	{
		if (!GetFlag(byIndex))
		{
			return;
		}

		if (m_byCardIndexCount[byCardIndex] >= MAX_WEAVE_CARD_COUNT)
		{
			return;
		}

		m_byCardIndex[byCardIndex][m_byCardIndexCount[byCardIndex]] = byIndex;
		++m_byCardIndexCount[byCardIndex];

		++m_byLeftCount;
		m_nFlag &= ~(1 << byIndex);
	}
}

// 取得牌索引数组
BYTE *CardData::GetCardIndexArray(BYTE byCardValue)
{
	BYTE byCardIndex = GetCardIndex(byCardValue);
	if (byCardIndex >= MAX_INDEX)
	{
		return NULL;
	}

	return (BYTE *)m_byCardIndex[byCardIndex];
}

// 取得牌索引数组牌值
BYTE CardData::GetCardIndexArrayValue(BYTE byCardValue, BYTE byCardCountIndex)
{
	BYTE byCardIndex = GetCardIndex(byCardValue);
	if (byCardIndex >= MAX_INDEX)
	{
		return 0;
	}

	if (byCardCountIndex >= MAX_WEAVE_CARD_COUNT)
	{
		return 0;
	}

	return m_byCardIndex[byCardIndex][byCardCountIndex];
}

// 取得牌索引数组数量
BYTE CardData::GetCardIndexArrayCount(BYTE byCardValue)
{
	BYTE byCardIndex = GetCardIndex(byCardValue);
	if (byCardIndex >= MAX_INDEX)
	{
		return 0;
	}

	return m_byCardIndexCount[byCardIndex];
}

// 取得牌索引数组(通过索引)
BYTE *CardData::GetCardIndexArrayInIndex(BYTE byCardIndex)
{
	if (byCardIndex >= MAX_INDEX)
	{
		return NULL;
	}

	return (BYTE *)m_byCardIndex[byCardIndex];
}

// 取得牌索引数组牌值(通过索引)
BYTE CardData::GetCardIndexArrayValueInIndex(BYTE byCardIndex, BYTE byCardCountIndex)
{
	if (byCardIndex >= MAX_INDEX)
	{
		return 0;
	}

	if (byCardCountIndex >= MAX_WEAVE_CARD_COUNT)
	{
		return 0;
	}

	return m_byCardIndex[byCardIndex][byCardCountIndex];
}

// 取得牌索引数组数量(通过索引)
BYTE CardData::GetCardIndexArrayCountInIndex(BYTE byCardIndex)
{
	if (byCardIndex >= MAX_INDEX)
	{
		return 0;
	}

	return m_byCardIndexCount[byCardIndex];
}

// 清理函数
void CardData::Clear()
{
	m_nFlag = 0;
	memset(&m_byCard, 0, sizeof(m_byCard));
	m_byCardCount = 0;
	memset(&m_byCardIndex, 0, sizeof(m_byCardIndex));
	memset(&m_byCardIndexCount, 0, sizeof(m_byCardIndexCount));
	memset(&m_byLeftIndex, 0, sizeof(m_byLeftIndex));
	m_byLeftIndexCount = 0;
	m_byLeftCount = 0;
}

// 取得标志
bool CardData::GetFlag(BYTE byIndex)
{
	return (m_nFlag & (1 << byIndex)) != 0;
}

// 取得是否完成
bool CardData::Finish()
{
	int nAllFlag = (1 << m_byCardCount) - 1;
	return (nAllFlag & m_nFlag) == nAllFlag;
}
/////////////////////////////////////////////////////		CardData		/////////////////////////////////////////////////////

/////////////////////////////////////////////////////		CardAnswer		/////////////////////////////////////////////////////
// 构造函数
CardAnswer::CardAnswer()
{
	Clear();
}

// 析构函数
CardAnswer::~CardAnswer()
{

}

// 清理函数
void CardAnswer::Clear()
{
	memset(&m_CardKey, 0, sizeof(m_CardKey));
	m_byCardKeyCount = 0;
	m_byKey = 0;
}

// 增加牌数据
void CardAnswer::AddCardData(CardKey *pKeyData, CardData *pCardData)
{
	if (pKeyData == NULL || pCardData == NULL)
	{
		return;
	}

	if (m_byCardKeyCount >= MAX_ANSWER_WEAVE)
	{
		return;
	}

	m_byKey += pKeyData->GetKey();
	memcpy(&m_CardKey[m_byCardKeyCount], pKeyData, sizeof(m_CardKey[m_byCardKeyCount]));
	++m_byCardKeyCount;

	BYTE byCardIndexCount = pKeyData->GetCardIndexCount();
	for (BYTE i = 0; i < byCardIndexCount; ++i)
	{
		BYTE byCardIndex = pKeyData->GetCardIndex(i);
		if (byCardIndex >= MAX_INDEX)
		{
			continue;
		}

		pCardData->SetFlag(byCardIndex, true);
	}
}

// 移除牌数据
void CardAnswer::RemoveCardData(CardData *pCardData)
{
	if (pCardData == NULL)
	{
		return;
	}

	if (m_byCardKeyCount <= 0)
	{
		return;
	}

	BYTE byCurrentCardKeyCount = m_byCardKeyCount - 1;
	BYTE byCardIndexCount = m_CardKey[byCurrentCardKeyCount].GetCardIndexCount();
	for (BYTE i = 0; i < byCardIndexCount; ++i)
	{
		BYTE byCardIndex = m_CardKey[byCurrentCardKeyCount].GetCardIndex(i);
		if (byCardIndex >= MAX_INDEX)
		{
			continue;
		}

		pCardData->SetFlag(byCardIndex, false);
	}

	m_byKey -= m_CardKey[byCurrentCardKeyCount].GetKey();
	memset(&m_CardKey[byCurrentCardKeyCount], 0, sizeof(m_CardKey[byCurrentCardKeyCount]));
	--m_byCardKeyCount;
}

// 取得牌权值信息
CardKey *CardAnswer::GetCardKey(BYTE nCardKeyIndex)
{
	if (nCardKeyIndex >= m_byCardKeyCount)
	{
		return NULL;
	}

	return &m_CardKey[nCardKeyIndex];
}

// 取得牌权值数量
BYTE CardAnswer::GetCardKeyCount()
{
	return m_byCardKeyCount;
}
/////////////////////////////////////////////////////		CardAnswer		/////////////////////////////////////////////////////


/////////////////////////////////////////////////////		HuPaiLogic		/////////////////////////////////////////////////////
// 构造函数
HuPaiLogic::HuPaiLogic()
{
	Clear();
}
// 析构函数
HuPaiLogic::~HuPaiLogic()
{

}

// 初始化牌权值
void HuPaiLogic::InitCard()
{
	if (!m_mapKey.empty())
	{
		return;
	}

	// 获取三同牌组合
	GetThreeSameCard();
	// 获取顺子牌组合
	GetStraightCard();
	// 获取其他牌组合
	GetOtherCard();
}

// 牌组合键值
DWORD HuPaiLogic::GetCardValue(BYTE byCard0, BYTE byCard1, BYTE byCard2, BYTE byCard3)
{
	BYTE byCard[MAX_WEAVE_CARD_COUNT] = { 0 };
	BYTE byCardCount = 0;

	byCard[byCardCount++] = byCard0;
	byCard[byCardCount++] = byCard1;
	byCard[byCardCount++] = byCard2;
	byCard[byCardCount++] = byCard3;

	/*
		#include <algorithm>	->	sort();
		#include <functional>	->	less,greater
		从小到大排序，比较函数设置为：less<数据类型>
		sort(byCard, byCard + byCardCount, less<BYTE>());
		从大到小排序，比较函数设置为：greater<数据类型>
		sort(byCard, byCard + byCardCount, greater<BYTE>());
		默认从小到大排序
		sort(byCard, byCard + byCardCount);
	*/
	sort(byCard, byCard + byCardCount, less<BYTE>());

	return ((byCard[3] << 24) + (byCard[2] << 16) + (byCard[1] << 8) + byCard[0]);
}

// 获取牌组合权值
int HuPaiLogic::GetCardValueKey(BYTE byCard0, BYTE byCard1, BYTE byCard2, BYTE byCard3)
{
	return GetCardValueKey(GetCardValue(byCard0, byCard1, byCard2, byCard3));
}

// 获取牌组合权值
int HuPaiLogic::GetCardValueKey(DWORD dwKey)
{
	MAP_KEY::iterator Iter = m_mapKey.find(dwKey);
	if (Iter == m_mapKey.end())
	{
		return -1;
	}

	return Iter->second;
}

// 初始化胡牌
bool HuPaiLogic::InitHuCard(CardData *pCardData, CardAnswer *pAnswer, BYTE byKingCard, BYTE byKingIndex, BYTE byJiangCount)
{
	if (pCardData == NULL || pAnswer == NULL)
	{
		return false;
	}

	m_byKingCard = byKingCard;
	m_byKingIndex = byKingIndex;
	m_byJiangCount = byJiangCount;

	return GetHuCardResult(pCardData, pAnswer);
}

// 获得胡牌结果
bool HuPaiLogic::GetHuCardResult(CardData *pCardData, CardAnswer *pAnswer)
{
	if (pCardData == NULL || pAnswer == NULL)
	{
		return false;
	}

	BYTE *pLeftIndex = pCardData->GetLeftIndex();
	if (pLeftIndex == NULL)
	{
		return false;
	}

	BYTE byLeftIndexCount = pCardData->GetLeftIndexCount();
	if (byLeftIndexCount == 0)
	{
		return true;
	}
	else if (byLeftIndexCount < 2)
	{
		return false;
	}
	else if (byLeftIndexCount == 2)
	{
		BYTE byCard0 = pCardData->GetCardValue(pLeftIndex[0]);
		BYTE byCard1 = pCardData->GetCardValue(pLeftIndex[1]);

		bool bCheckHu = false;
		if (byCard0 == m_byKingCard || m_byKingCard == byCard1)
		{
			bCheckHu = true;
		}
		else if (byCard0 == byCard1)
		{
			bCheckHu = true;
		}

		// 检测胡
		if (bCheckHu)
		{
			if (m_byJiangCount != 0)
			{
				return false;
			}

			CardKey key(0, pLeftIndex[0], pLeftIndex[1]);
			pAnswer->AddCardData(&key, pCardData);
			return true;
		}
	}
	else if (byLeftIndexCount == 3)
	{
		int nCardValueKey = GetCardValueKey(pCardData->GetCardValue(pLeftIndex[0]), pCardData->GetCardValue(pLeftIndex[1]), pCardData->GetCardValue(pLeftIndex[2]), 0);
		if (nCardValueKey >= 0)
		{
			CardKey key(0, pLeftIndex[0], pLeftIndex[1], pLeftIndex[2]);
			pAnswer->AddCardData(&key, pCardData);
			return true;
		}
	}
	else
	{
		// 中发白顺子
		do
		{
			BYTE byStartCard = 0x35;
			BYTE byCard0 = byStartCard;
			BYTE byCard1 = byStartCard + 1;
			BYTE byCard2 = byStartCard + 2;

			BYTE byCardIndex_0 = pCardData->GetCardIndex(byCard0);
			BYTE byCardCount_0 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_0);
			BYTE *pCardIndexArray_0 = pCardData->GetCardIndexArrayInIndex(byCardIndex_0);
			if (byCardCount_0 <= 0)
			{
				break;
			}

			BYTE byCardIndex_1 = pCardData->GetCardIndex(byCard1);
			BYTE byCardCount_1 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_1);
			BYTE *pCardIndexArray_1 = pCardData->GetCardIndexArrayInIndex(byCardIndex_1);
			if (byCardCount_1 <= 0)
			{
				break;
			}

			BYTE byCardIndex_2 = pCardData->GetCardIndex(byCard2);
			BYTE byCardCount_2 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_2);
			BYTE *pCardIndexArray_2 = pCardData->GetCardIndexArrayInIndex(byCardIndex_2);
			if (byCardCount_2 <= 0)
			{
				break;
			}

			CardKey key(JIAN_PAI_KEY, pCardIndexArray_0[0], pCardIndexArray_1[0], pCardIndexArray_2[0]);
			pAnswer->AddCardData(&key, pCardData);
			if (GetHuCardResult(pCardData, pAnswer))
			{
				return true;
			}
			else
			{
				pAnswer->RemoveCardData(pCardData);
			}
		} while (false);

		// 东南西北顺子
		do
		{
			BYTE byCardGrop[4][3] =
			{
				{ 0, 1, 2 },
				{ 0, 1, 3 },
				{ 0, 2, 3 },
				{ 1, 2, 3 },
			};

			for (BYTE i = 0; i < 4; i++)
			{
				BYTE byStartCard = 0x31;
				BYTE byCard0 = byStartCard + byCardGrop[i][0];
				BYTE byCard1 = byStartCard + byCardGrop[i][1];
				BYTE byCard2 = byStartCard + byCardGrop[i][2];

				BYTE byCardIndex_0 = pCardData->GetCardIndex(byCard0);
				BYTE byCardCount_0 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_0);
				BYTE *pCardIndexArray_0 = pCardData->GetCardIndexArrayInIndex(byCardIndex_0);
				if (byCardCount_0 <= 0)
				{
					continue;
				}

				BYTE byCardIndex_1 = pCardData->GetCardIndex(byCard1);
				BYTE byCardCount_1 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_1);
				BYTE *pCardIndexArray_1 = pCardData->GetCardIndexArrayInIndex(byCardIndex_1);
				if (byCardCount_1 <= 0)
				{
					continue;
				}

				BYTE byCardIndex_2 = pCardData->GetCardIndex(byCard2);
				BYTE byCardCount_2 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_2);
				BYTE *pCardIndexArray_2 = pCardData->GetCardIndexArrayInIndex(byCardIndex_2);
				if (byCardCount_2 <= 0)
				{
					continue;
				}

				CardKey key(FENG_PAI_KEY, pCardIndexArray_0[0], pCardIndexArray_1[0], pCardIndexArray_2[0]);
				pAnswer->AddCardData(&key, pCardData);
				if (GetHuCardResult(pCardData, pAnswer))
				{
					return true;
				}
				else
				{
					pAnswer->RemoveCardData(pCardData);
				}
			}
			
		} while (false);

		// 顺子
		for (BYTE i = 0; i < 3; ++i)
		{
			for (BYTE j = 1; j <= 7; ++j)
			{
				BYTE byCard0 = (i << 4) + j;
				BYTE byCard1 = byCard0 + 1;
				BYTE byCard2 = byCard0 + 2;

				BYTE byCardIndex_0 = pCardData->GetCardIndex(byCard0);
				BYTE byCardCount_0 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_0);
				BYTE *pCardIndexArray_0 = pCardData->GetCardIndexArrayInIndex(byCardIndex_0);

				BYTE byCardIndex_1 = pCardData->GetCardIndex(byCard1);
				BYTE byCardCount_1 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_1);
				BYTE *pCardIndexArray_1 = pCardData->GetCardIndexArrayInIndex(byCardIndex_1);

				BYTE byCardIndex_2 = pCardData->GetCardIndex(byCard2);
				BYTE byCardCount_2 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_2);
				BYTE *pCardIndexArray_2 = pCardData->GetCardIndexArrayInIndex(byCardIndex_2);

				BYTE byCardCount_King = pCardData->GetCardIndexArrayCountInIndex(m_byKingIndex);
				BYTE *pCardIndexArray_King = pCardData->GetCardIndexArrayInIndex(m_byKingIndex);

				if (byCardCount_0 + byCardCount_1 + byCardCount_2 + byCardCount_King < 3)
				{
					continue;
				}

				BYTE byCardIndex[3] = { 0 };
				BYTE byCardIndexCount = 0;
				if (byCardCount_0 > 0)
				{
					byCardIndex[byCardIndexCount++] = pCardIndexArray_0[0];
				}

				if (byCardCount_1 > 0)
				{
					byCardIndex[byCardIndexCount++] = pCardIndexArray_1[0];
				}

				if (byCardCount_2 > 0)
				{
					byCardIndex[byCardIndexCount++] = pCardIndexArray_2[0];
				}

				if (byCardIndexCount < 3 && byCardCount_King > 0)
				{
					// 这3个都不是王的索引
					if (byCardIndex_0 != m_byKingIndex && byCardIndex_1 != m_byKingIndex && byCardIndex_2 != m_byKingIndex)
					{
						byCardIndex[byCardIndexCount++] = pCardIndexArray_King[0];
					}
				}

				if (byCardIndexCount < 3)
				{
					continue;
				}

				CardKey key(0, byCardIndex[0], byCardIndex[1], byCardIndex[2]);
				pAnswer->AddCardData(&key, pCardData);
				if (GetHuCardResult(pCardData, pAnswer))
				{
					return true;
				}
				else
				{
					pAnswer->RemoveCardData(pCardData);
				}
			}
		}

		// 刻子
		for (BYTE i = 0; i < MAX_INDEX; ++i)
		{
			// 先排除王
			if (i == m_byKingIndex)
			{
				continue;
			}

			BYTE byCard0 = SwitchToCardData(i);
			if (byCard0 == 0)
			{
				continue;
			}

			BYTE byCardIndex_0 = pCardData->GetCardIndex(byCard0);
			BYTE byCardCount_0 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_0);
			BYTE *pCardIndexArray_0 = pCardData->GetCardIndexArrayInIndex(byCardIndex_0);
			if (byCardCount_0 <= 0)
			{
				continue;
			}

			BYTE byCardCount_King = pCardData->GetCardIndexArrayCountInIndex(m_byKingIndex);
			BYTE *pCardIndexArray_King = pCardData->GetCardIndexArrayInIndex(m_byKingIndex);
			if (byCardCount_0 + byCardCount_King < 3)
			{
				continue;
			}

			BYTE byCardIndex[3] = { 0 };
			BYTE byCardIndexCount = 0;
			for (BYTE nCount = 0; nCount < byCardCount_0; nCount++)
			{
				if (byCardIndexCount >= 3)
				{
					break;
				}

				byCardIndex[byCardIndexCount++] = pCardIndexArray_0[nCount];
			}

			if (byCardIndexCount < 3 && byCardCount_King > 0)
			{
				// 不是王的索引
				if (byCardIndex_0 != m_byKingIndex)
				{
					for (BYTE nCount = 0; nCount < byCardCount_King; nCount++)
					{
						if (byCardIndexCount >= 3)
						{
							break;
						}

						byCardIndex[byCardIndexCount++] = pCardIndexArray_King[nCount];
					}
				}
			}

			if (byCardIndexCount < 3)
			{
				continue;
			}

			CardKey key(0, byCardIndex[0], byCardIndex[1], byCardIndex[2]);
			pAnswer->AddCardData(&key, pCardData);
			if (GetHuCardResult(pCardData, pAnswer))
			{
				return true;
			}
			else
			{
				pAnswer->RemoveCardData(pCardData);
			}
		}

		// 将
		for (BYTE i = 0; i < MAX_INDEX; ++i)
		{
			BYTE byCard0 = SwitchToCardData(i);
			if (byCard0 == 0)
			{
				continue;
			}

			BYTE byCardIndex_0 = pCardData->GetCardIndex(byCard0);
			BYTE byCardCount_0 = pCardData->GetCardIndexArrayCountInIndex(byCardIndex_0);
			BYTE *pCardIndexArray_0 = pCardData->GetCardIndexArrayInIndex(byCardIndex_0);
			if (byCardCount_0 <= 0)
			{
				continue;
			}

			BYTE byCardCount_King = pCardData->GetCardIndexArrayCountInIndex(m_byKingIndex);
			BYTE *pCardIndexArray_King = pCardData->GetCardIndexArrayInIndex(m_byKingIndex);
			if (byCardCount_0 + byCardCount_King < 3)
			{
				continue;
			}

			BYTE byCardIndex[2] = { 0 };
			BYTE byCardIndexCount = 0;
			for (BYTE nCount = 0; nCount < byCardCount_0; nCount++)
			{
				if (byCardIndexCount >= 2)
				{
					break;
				}

				byCardIndex[byCardIndexCount++] = pCardIndexArray_0[nCount];
			}

			if (byCardIndexCount < 2)
			{
				for (BYTE nCount = 0; nCount < byCardCount_King; nCount++)
				{
					if (byCardIndexCount >= 2)
					{
						break;
					}

					byCardIndex[byCardIndexCount++] = pCardIndexArray_King[nCount];
				}
			}

			if (byCardIndexCount < 2)
			{
				continue;
			}

			CardKey key(0, byCardIndex[0], byCardIndex[1]);
			pAnswer->AddCardData(&key, pCardData);
			++m_byJiangCount;
			if (GetHuCardResult(pCardData, pAnswer))
			{
				return true;
			}
			else
			{
				pAnswer->RemoveCardData(pCardData);
				--m_byJiangCount;
			}
		}
	}

	return false;
}

// 清理函数
void HuPaiLogic::Clear()
{
	m_mapKey.clear();

	m_byJiangCount = 0;

	m_byKingIndex = 0;
}

// 增加权值表(牌组合值为key)
void HuPaiLogic::AddKeyMap(int nCardValue, BYTE byKey)
{
	m_mapKey[nCardValue] = byKey;
}

// 获取三同牌组合
void HuPaiLogic::GetThreeSameCard()
{
	for (BYTE i = 0; i < MAX_INDEX; ++i)
	{
		BYTE byCard = SwitchToCardData(i);
		if (byCard == 0)
		{
			continue;
		}

		AddKeyMap(GetCardValue(byCard, byCard, byCard, 0), 0);
	}
}

// 获取顺子牌组合
void HuPaiLogic::GetStraightCard()
{
	for (BYTE i = 0; i < 3; ++i)
	{
		for (BYTE j = 1; j <= 7; ++j)
		{
			BYTE byCard = (i << 4) + j;

			AddKeyMap(GetCardValue(byCard, byCard + 1, byCard + 2, 0), 0);
		}
	}
}

// 获取其他牌组合
void HuPaiLogic::GetOtherCard()
{
	// 东南西北组合
	AddKeyMap(GetCardValue(0x31, 0x32, 0x33, 0), FENG_PAI_KEY);
	AddKeyMap(GetCardValue(0x31, 0x32, 0x34, 0), FENG_PAI_KEY);
	AddKeyMap(GetCardValue(0x31, 0x33, 0x34, 0), FENG_PAI_KEY);
	AddKeyMap(GetCardValue(0x32, 0x33, 0x34, 0), FENG_PAI_KEY);

	// 中发白组合
	AddKeyMap(GetCardValue(0x35, 0x36, 0x37, 0), JIAN_PAI_KEY);
}

// 扑克转换
BYTE HuPaiLogic::SwitchToCardData(BYTE byCardIndex)
{
	if (byCardIndex >= MAX_INDEX)
	{
		return 0;
	}

	return ((byCardIndex / 9) << 4) | (byCardIndex % 9 + 1);
}
/////////////////////////////////////////////////////		HuPaiLogic		/////////////////////////////////////////////////////