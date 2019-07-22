#include "GameLogic.h"
#include <stdlib.h>
#include <assert.h>
#include <algorithm>

//////////////////////////////////////////////////////////////////////////

//扑克数据
BYTE const CGameLogic::m_cbCardDataArray[MAX_REPERTORY] =
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
	m_HuPaiLogic.initCardHuXi();
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE bMaxCount)
{
	//混乱准备
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)] = { 0 };
	memcpy(cbCardDataTemp, m_cbCardDataArray, sizeof(m_cbCardDataArray));

	//混乱扑克
	BYTE bRandCount = 0, bPosition = 0;
	do
	{
		bPosition = rand() % (bMaxCount - bRandCount);
		cbCardData[bRandCount++] = cbCardDataTemp[bPosition];
		cbCardDataTemp[bPosition] = cbCardDataTemp[bMaxCount - bRandCount];
	} while (bRandCount < bMaxCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//效验扑克
	if (!IsValidCard(cbRemoveCard))
	{
		return false;
	}
	if (cbCardIndex[SwitchToCardIndex(cbRemoveCard)] <= 0)
	{
		return false;
	}

	//删除扑克
	BYTE cbRemoveIndex = SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex] > 0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	return false;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE bRemoveCount)
{
	//删除扑克
	for (BYTE i = 0; i<bRemoveCount; i++)
	{
		//效验扑克
		if (!IsValidCard(cbRemoveCard[i]))
		{
			continue;
		}

		BYTE cbRemoveIndex = SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex] <= 0)
		{
			continue;
		}

		//删除扑克
		if (cbCardIndex[cbRemoveIndex] == 0)
		{
			//还原删除
			for (BYTE j = 0; j<i; j++)
			{
				if (!IsValidCard(cbRemoveCard[j]))
				{
					continue;
				}
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else
		{
			//删除扑克
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE bCardCount, BYTE cbRemoveCard[], BYTE bRemoveCount)
{
	//检验数据
	if (bCardCount > MAX_COUNT)
	{
		return false;
	}
	if (bRemoveCount > bCardCount)
	{
		return false;
	}

	//定义变量
	BYTE bDeleteCount = 0;
	BYTE cbTempCardData[MAX_COUNT] = { 0 };
	if (bCardCount > CountArray(cbTempCardData))
	{
		return false;
	}
	memcpy(cbTempCardData, cbCardData, bCardCount * sizeof(cbCardData[0]));

	//置零扑克
	for (BYTE i = 0; i<bRemoveCount; i++)
	{
		for (BYTE j = 0; j<bCardCount; j++)
		{
			if (cbRemoveCard[i] == cbTempCardData[j])
			{
				bDeleteCount++;
				cbTempCardData[j] = 0;
				break;
			}
		}
	}

	//成功判断
	if (bDeleteCount != bRemoveCount)
	{
		return false;
	}

	//清理扑克
	BYTE bCardPos = 0;
	for (BYTE i = 0; i<bCardCount; i++)
	{
		if (cbTempCardData[i] != 0)
		{
			cbCardData[bCardPos++] = cbTempCardData[i];
		}
	}

	return true;
}

//溜牌判断
BYTE CGameLogic::GetAcitonTiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbTiCardIndex[5])
{
	//溜牌搜索
	BYTE cbTiCardCount = 0;
	for (BYTE i = 0; i<MAX_INDEX; i++)
	{
		if (cbCardIndex[i] == 4)
		{
			cbTiCardIndex[cbTiCardCount++] = i;
		}
	}

	return cbTiCardCount;
}

//畏牌判断
BYTE CGameLogic::GetActionWeiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbWeiCardIndex[7])
{
	//畏牌搜索
	BYTE cbWeiCardCount = 0;
	for (BYTE i = 0; i<MAX_INDEX; i++)
	{
		if (cbCardIndex[i] == 3)
		{
			cbWeiCardIndex[cbWeiCardCount++] = i;
		}
	}

	return cbWeiCardCount;
}

//吃牌判断
BYTE CGameLogic::GetActionChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, tagChiCardInfo ChiCardInfo[6])
{
	//效验扑克
	if (cbCurrentCard == 0)
	{
		return 0;
	}

	//变量定义
	BYTE cbChiCardCount = 0;
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//二七十吃
	BYTE bCardValue = cbCurrentCard&MASK_VALUE;
	if (bCardValue == 0x02 || bCardValue == 0x07 || bCardValue == 0x0A)
	{
		//变量定义
		BYTE cbExcursion[] = { 1, 6, 9 };
		BYTE cbInceptIndex = ((cbCurrentCard&MASK_COLOR) == 0x00) ? 0 : 10;

		//类型判断
		BYTE i = 0;
		for (i = 0; i<CountArray(cbExcursion); i++)
		{
			BYTE cbIndex = cbInceptIndex + cbExcursion[i];
			if (cbIndex != cbCurrentIndex && (cbCardIndex[cbIndex] == 0 || cbCardIndex[cbIndex] == 3))
			{
				break;
			}
		}

		//提取判断
		if (i == CountArray(cbExcursion))
		{
			//构造扑克
			BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

			//删除扑克
			for (BYTE j = 0; j<CountArray(cbExcursion); j++)
			{
				BYTE cbIndex = cbInceptIndex + cbExcursion[j];
				if (cbIndex != cbCurrentIndex)
				{
					cbCardIndexTemp[cbIndex]--;
				}
			}

			//提取判断
			BYTE cbResultCount = 1;
			while (cbCardIndexTemp[cbCurrentIndex] > 0)
			{
				PBYTE pcbResult = ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
				if (TakeOutChiCard(cbCardIndexTemp, cbCurrentCard, pcbResult) != CK_NULL)
				{
					cbResultCount++;
				}
				else
				{
					break;
				}
			}

			//设置结果
			if (cbCardIndexTemp[cbCurrentIndex] == 0)
			{
				ChiCardInfo[cbChiCardCount].cbChiKind = CK_EQS;
				ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
				ChiCardInfo[cbChiCardCount].cbResultCount = cbResultCount;
				ChiCardInfo[cbChiCardCount].cbCardData[0][0] = SwitchToCardData(cbInceptIndex + cbExcursion[0]);
				ChiCardInfo[cbChiCardCount].cbCardData[0][1] = SwitchToCardData(cbInceptIndex + cbExcursion[1]);
				ChiCardInfo[cbChiCardCount++].cbCardData[0][2] = SwitchToCardData(cbInceptIndex + cbExcursion[2]);
			}
		}
	}

	//顺子类型
	BYTE cbExcursion[3] = { 0, 1, 2 };
	for (BYTE i = 0; i<CountArray(cbExcursion); i++)
	{
		BYTE cbValueIndex = cbCurrentIndex % 10;
		if (cbValueIndex >= cbExcursion[i] && (cbValueIndex - cbExcursion[i]) <= 7)
		{
			//索引定义
			BYTE cbFirstIndex = cbCurrentIndex - cbExcursion[i];

			//吃牌判断
			BYTE j = 0;
			for (; j<3; j++)
			{
				BYTE cbIndex = cbFirstIndex + j;
				if (cbIndex != cbCurrentIndex && (cbCardIndex[cbIndex] == 0 || cbCardIndex[cbIndex] == 3))
				{
					break;
				}
			}

			//提取判断
			if (j == CountArray(cbExcursion))
			{
				//构造扑克
				BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
				memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

				//删除扑克
				for (BYTE j = 0; j<3; j++)
				{
					BYTE cbIndex = cbFirstIndex + j;
					if (cbIndex != cbCurrentIndex)
					{
						cbCardIndexTemp[cbIndex]--;
					}
				}

				//提取判断
				BYTE cbResultCount = 1;
				while (cbCardIndexTemp[cbCurrentIndex] > 0)
				{
					PBYTE pcbResult = ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
					if (TakeOutChiCard(cbCardIndexTemp, cbCurrentCard, pcbResult) != CK_NULL)
					{
						cbResultCount++;
					}
					else
					{
						break;
					}
				}

				//设置结果
				if (cbCardIndexTemp[cbCurrentIndex] == 0)
				{
					BYTE cbChiKind[3] = { CK_LEFT, CK_CENTER, CK_RIGHT };
					ChiCardInfo[cbChiCardCount].cbChiKind = cbChiKind[i];
					ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
					ChiCardInfo[cbChiCardCount].cbResultCount = cbResultCount;
					ChiCardInfo[cbChiCardCount].cbCardData[0][0] = SwitchToCardData(cbFirstIndex);
					ChiCardInfo[cbChiCardCount].cbCardData[0][1] = SwitchToCardData(cbFirstIndex + 1);
					ChiCardInfo[cbChiCardCount++].cbCardData[0][2] = SwitchToCardData(cbFirstIndex + 2);
				}
			}
		}
	}

	return cbChiCardCount;
}

//是否吃牌
bool CGameLogic::IsChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//效验扑克
	if (cbCurrentCard == 0)
	{
		return false;
	}

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//插入扑克
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	BYTE cbResult[3] = { 0 };
	return TakeOutChiCard(cbCardIndexTemp, cbCurrentCard, cbResult) != CK_NULL;
}

//是否提跑
bool CGameLogic::IsTiPaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//效验扑克
	if (!IsValidCard(cbCurrentCard))
	{
		return false;
	}

	//转换索引
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//提跑判断
	return (cbCardIndex[cbCurrentIndex] >= 3) ? true : false;
}

//是否歪碰
bool CGameLogic::IsWeiPengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//效验扑克
	if (!IsValidCard(cbCurrentCard))
	{
		return false;
	}

	//转换索引
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//跑歪判断
	return (cbCardIndex[cbCurrentIndex] >= 2) ? true : false;
}

//胡牌结果
bool CGameLogic::GetHuCardInfo(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbHuXiWeave, tagHuCardInfo &HuCardInfo)
{
	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	vector<BYTE> vctTest;
	for (int i = 0; i<MAX_INDEX; ++i)
	{
		for (int j = 0; j<cbCardIndex[i]; ++j)
		{
			vctTest.push_back(SwitchToCardData(i));
		}
	}

	if (IsValidCard(cbCurrentCard))
	{
		vctTest.push_back(cbCurrentCard);
	}

	// 需要判断特殊胡(庄家0对,1对,9对,10对;闲家0对,1对,9对),直接判断cbCardIndex对数和单牌数量,cbCurrentCard肯定为0,不然就错误了
	if (HuCardInfo.bSpecialHu)
	{
		// 单牌容器
		vector<BYTE> vDanPai;
		// 对牌容器
		vector<BYTE> vDuiPai;
		// 总牌数
		int nCardPaiCount = 0;

		for (int i = 0; i<MAX_INDEX; ++i)
		{
			if (cbCardIndex[i] == 0)
			{
				continue;
			}

			// 转换牌
			BYTE cbCardData = SwitchToCardData(i);

			nCardPaiCount += cbCardIndex[i];

			if (cbCardIndex[i] == 1)
			{
				vDanPai.push_back(cbCardData);
			}
			else if (cbCardIndex[i] == 3)
			{
				vDanPai.push_back(cbCardData);
				vDuiPai.push_back(cbCardData);
			}
			else if (cbCardIndex[i] % 2 == 0)
			{
				int nCount = cbCardIndex[i] / 2;
				for (int j = 0; j<nCount; ++j)
				{
					vDuiPai.push_back(cbCardData);
				}
			}
		}

		// 单牌数量
		int nDanPaiCount = vDanPai.size();
		// 对牌数量
		int nDuiPaiCount = vDuiPai.size();
		// 无对
		if (nDanPaiCount == nCardPaiCount)
		{
			// 摆5个组合，每个组合4张牌
			HuCardInfo.cbWeaveCount = 5;
			HuCardInfo.WeaveItemArray[0].cbCardCount = 4;
			HuCardInfo.WeaveItemArray[1].cbCardCount = 4;
			HuCardInfo.WeaveItemArray[2].cbCardCount = 4;
			HuCardInfo.WeaveItemArray[3].cbCardCount = 4;
			HuCardInfo.WeaveItemArray[4].cbCardCount = 4;

			for (int i = 0; i<nDanPaiCount; ++i)
			{
				HuCardInfo.WeaveItemArray[i / 4].cbCardList[i % 4] = vDanPai[i];
			}

			return true;
		}

		// 十对
		if (nDuiPaiCount == 10)
		{
			// 摆10个组合，每个组合2张牌
			HuCardInfo.cbWeaveCount = 10;
			HuCardInfo.WeaveItemArray[0].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[1].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[2].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[3].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[4].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[5].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[6].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[7].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[8].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[9].cbCardCount = 2;

			for (int i = 0; i<nDuiPaiCount; ++i)
			{
				BYTE cbCardData = vDuiPai[i];
				HuCardInfo.WeaveItemArray[i].cbCardList[0] = cbCardData;
				HuCardInfo.WeaveItemArray[i].cbCardList[1] = cbCardData;
			}

			return true;
		}

		// 一对
		if (nDuiPaiCount == 1)
		{
			// 摆6个组合，第1个组合2张牌，4个4张，1个2张
			HuCardInfo.cbWeaveCount = 6;
			HuCardInfo.WeaveItemArray[0].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[1].cbCardCount = 4;
			HuCardInfo.WeaveItemArray[2].cbCardCount = 4;
			HuCardInfo.WeaveItemArray[3].cbCardCount = 4;
			HuCardInfo.WeaveItemArray[4].cbCardCount = 4;
			HuCardInfo.WeaveItemArray[5].cbCardCount = 2;

			for (int i = 0; i<nDuiPaiCount; ++i)
			{
				BYTE cbCardData = vDuiPai[i];
				HuCardInfo.WeaveItemArray[i].cbCardList[0] = cbCardData;
				HuCardInfo.WeaveItemArray[i].cbCardList[1] = cbCardData;
			}

			for (int i = 0; i<nDanPaiCount; ++i)
			{
				HuCardInfo.WeaveItemArray[1 + i / 4].cbCardList[i % 4] = vDanPai[i];
			}

			return true;
		}

		// 九对
		if (nDuiPaiCount == 9)
		{
			// 摆10个组合，每个组合2张牌
			HuCardInfo.cbWeaveCount = 10;
			HuCardInfo.WeaveItemArray[0].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[1].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[2].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[3].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[4].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[5].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[6].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[7].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[8].cbCardCount = 2;
			HuCardInfo.WeaveItemArray[9].cbCardCount = 2;

			for (int i = 0; i<nDuiPaiCount; ++i)
			{
				BYTE cbCardData = vDuiPai[i];
				HuCardInfo.WeaveItemArray[i].cbCardList[0] = cbCardData;
				HuCardInfo.WeaveItemArray[i].cbCardList[1] = cbCardData;
			}

			for (int i = 0; i<nDanPaiCount; ++i)
			{
				HuCardInfo.WeaveItemArray[9 + i / 4].cbCardList[i % 4] = vDanPai[i];
			}

			return true;
		}
	}

	// 只判断特殊胡
	if (HuCardInfo.bOnlySpecialHu)
	{
		return false;
	}

	stAnswer sResult;
	sResult.clear();
	if (!m_HuPaiLogic.initCardHu(vctTest, sResult, cbCurrentCard, HuCardInfo.bSelfAddCard))
	{
		return false;
	}

	// 胡息不够
	if (sResult.getHuPer() + cbHuXiWeave < MIN_HU_XI)
	{
		return false;
	}

	int nWeaveIndex = 0;
	for (size_t i = 0; i<sResult.vctData.size(); ++i)
	{
		if (nWeaveIndex >= MAX_WEAVE)
		{
			break;
		}

		stHuKeyData &stTemp = sResult.vctData[i];

		size_t nVctIndexCount = stTemp.vctIndex.size();
		for (size_t j = 0; j<nVctIndexCount; ++j)
		{
			if (stTemp.vctIndex[j] >= 0 && stTemp.vctIndex[j] < (int)vctTest.size())
			{
				HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[j] = vctTest[stTemp.vctIndex[j]];
			}
		}
		HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardCount = nVctIndexCount;
		HuCardInfo.WeaveItemArray[nWeaveIndex].cbCenterCard = HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[0];

		if (nVctIndexCount == 4)
		{
			HuCardInfo.WeaveItemArray[nWeaveIndex].cbWeaveKind = ACK_LIU;
		}
		else if (nVctIndexCount == 3)
		{
			if (HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[0] == HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[1] && HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[1] == HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[2])
			{
				// 不是3胡，设置成碰
				if (stTemp.nHuKey != 3)
				{
					HuCardInfo.WeaveItemArray[nWeaveIndex].cbWeaveKind = ACK_PENG;
				}
				else
				{
					HuCardInfo.WeaveItemArray[nWeaveIndex].cbWeaveKind = ACK_KAN;
				}
			}
			else
			{
				HuCardInfo.WeaveItemArray[nWeaveIndex].cbWeaveKind = ACK_CHI;
			}
		}

		// 增加组合索引
		++nWeaveIndex;
	}
	HuCardInfo.cbWeaveCount = nWeaveIndex;

	return true;
}


//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue = (cbCardData&MASK_VALUE);
	BYTE cbColor = (cbCardData&MASK_COLOR) >> 4;
	return (cbValue >= 1 && cbValue <= 10 && cbColor <= 1);
}

//扑克数目
BYTE CGameLogic::GetCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//数目统计
	BYTE cbCount = 0;
	for (BYTE i = 0; i<MAX_INDEX; i++)
	{
		cbCount += cbCardIndex[i];
	}

	return cbCount;
}

//获取胡息
BYTE CGameLogic::GetWeaveHuXi(const tagWeaveItem &WeaveItem)
{
	//计算胡息
	switch (WeaveItem.cbWeaveKind)
	{
	case ACK_LIU:	// 溜
	{
		return 5;
	}
	case ACK_WAI:	// 歪
	{
		return 4;
	}
	case ACK_KAN:	// 坎
	{
		return 3;
	}
	case ACK_PENG:	// 碰
	{
		return 1;
	}
	case ACK_CHI:	// 吃
	{
		// 获取数值
		BYTE cbValue1 = WeaveItem.cbCardList[0] & MASK_VALUE;
		BYTE cbValue2 = WeaveItem.cbCardList[1] & MASK_VALUE;
		BYTE cbValue3 = WeaveItem.cbCardList[2] & MASK_VALUE;

		BYTE cbCardIndex[MAX_INDEX] = { 0 };
		// 都转换成小的索引
		cbCardIndex[cbValue1 - 1]++;
		cbCardIndex[cbValue2 - 1]++;
		cbCardIndex[cbValue3 - 1]++;

		//二七十吃
		if (cbCardIndex[1] == 1 && cbCardIndex[6] == 1 && cbCardIndex[9] == 1)
		{
			return 1;
		}

		return 0;
	}
	case ACK_NULL:
	{
		// 2张牌,一对黑(1胡息),一对红(1胡息),红字胡红字(1胡息)
		BYTE cbHuXi = 0;

		if (WeaveItem.cbCardCount == 2)
		{
			// 获取数值
			BYTE cbValue1 = WeaveItem.cbCardList[0] & MASK_VALUE;
			BYTE cbValue2 = WeaveItem.cbCardList[1] & MASK_VALUE;

			// 对子再加1息
			if (WeaveItem.cbCardList[0] == WeaveItem.cbCardList[1])
			{
				cbHuXi = 1;
			}
			else
			{
				// 2个红字加1息
				if (cbValue1 == 0x02 || cbValue1 == 0x07 || cbValue1 == 0x0A)
				{
					if (cbValue2 == 0x02 || cbValue2 == 0x07 || cbValue2 == 0x0A)
					{
						cbHuXi = 1;
					}
				}
			}
		}
		return cbHuXi;
	}
	default:
	{
		return 0;
	}
	}

	return 0;
}

//提取吃牌
BYTE CGameLogic::TakeOutChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbResultCard[3])
{
	//效验扑克
	if (cbCurrentCard == 0)
	{
		return 0;
	}

	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//二七十吃
	BYTE bCardValue = cbCurrentCard&MASK_VALUE;
	if (bCardValue == 0x02 || bCardValue == 0x07 || bCardValue == 0x0A)
	{
		//变量定义
		BYTE cbExcursion[] = { 1,6,9 };
		BYTE cbInceptIndex = ((cbCurrentCard&MASK_COLOR) == 0x00) ? 0 : 10;

		//类型判断
		BYTE i = 0;
		for (i = 0; i<CountArray(cbExcursion); i++)
		{
			BYTE cbIndex = cbInceptIndex + cbExcursion[i];
			if (cbCardIndex[cbIndex] == 0)
			{
				break;
			}
		}

		//成功判断
		if (i == CountArray(cbExcursion))
		{
			//删除扑克
			cbCardIndex[cbInceptIndex + cbExcursion[0]]--;
			cbCardIndex[cbInceptIndex + cbExcursion[1]]--;
			cbCardIndex[cbInceptIndex + cbExcursion[2]]--;

			//设置结果
			cbResultCard[0] = SwitchToCardData(cbInceptIndex + cbExcursion[0]);
			cbResultCard[1] = SwitchToCardData(cbInceptIndex + cbExcursion[1]);
			cbResultCard[2] = SwitchToCardData(cbInceptIndex + cbExcursion[2]);

			return CK_EQS;
		}
	}

	BYTE cbFirstIndex = 0;
	//顺子判断
	BYTE cbExcursion[3] = { 0,1,2 };
	for (BYTE i = 0; i<CountArray(cbExcursion); i++)
	{
		BYTE cbValueIndex = cbCurrentIndex % 10;
		if (cbValueIndex >= cbExcursion[i] && (cbValueIndex - cbExcursion[i]) <= 7)
		{
			//吃牌判断
			cbFirstIndex = cbCurrentIndex - cbExcursion[i];
			if (cbCardIndex[cbFirstIndex] == 0)
			{
				continue;
			}
			if (cbCardIndex[cbFirstIndex + 1] == 0)
			{
				continue;
			}
			if (cbCardIndex[cbFirstIndex + 2] == 0)
			{
				continue;
			}

			//删除扑克
			cbCardIndex[cbFirstIndex]--;
			cbCardIndex[cbFirstIndex + 1]--;
			cbCardIndex[cbFirstIndex + 2]--;

			//设置结果
			cbResultCard[0] = SwitchToCardData(cbFirstIndex);
			cbResultCard[1] = SwitchToCardData(cbFirstIndex + 1);
			cbResultCard[2] = SwitchToCardData(cbFirstIndex + 2);

			BYTE cbChiKind[3] = { CK_LEFT, CK_CENTER, CK_RIGHT };
			return cbChiKind[i];
		}
	}

	return CK_NULL;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	if (cbCardIndex >= MAX_INDEX)
	{
		return 0;
	}
	return ((cbCardIndex / 10) << 4) | (cbCardIndex % 10 + 1);
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	if (!IsValidCard(cbCardData))
	{
		return MAX_INDEX;
	}
	return ((cbCardData&MASK_COLOR) >> 4) * 10 + (cbCardData&MASK_VALUE) - 1;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[], BYTE bMaxCount)
{
	//转换扑克
	BYTE bPosition = 0;
	for (BYTE i = 0; i<MAX_INDEX; i++)
	{
		BYTE cbIndex = (i % 2) * 10 + i / 2;
		if (cbCardIndex[cbIndex] != 0)
		{
			for (BYTE j = 0; j<cbCardIndex[cbIndex]; j++)
			{
				if (bPosition >= bMaxCount)
				{
					continue;
				}
				cbCardData[bPosition++] = SwitchToCardData(cbIndex);
			}
		}
	}

	return bPosition;
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE bCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//设置变量
	ZeroMemory(cbCardIndex, sizeof(cbCardIndex));

	//转换扑克
	for (BYTE i = 0; i<bCardCount; i++)
	{
		if (!IsValidCard(cbCardData[i]))
		{
			continue;
		}

		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return bCardCount;
}

//////////////////////////////////////////////////////////////////////////