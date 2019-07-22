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

//提牌判断
BYTE CGameLogic::GetAcitonTiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbTiCardIndex[5])
{
	//提牌搜索
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

	//三牌判断
	if (cbCardIndex[cbCurrentIndex] >= 3)
	{
		return cbChiCardCount;
	}

	//大小搭吃
	BYTE cbReverseIndex = (cbCurrentIndex + 10) % COMMOND_CARD_COUNT;
	if (cbCardIndex[cbCurrentIndex] >= 1 && cbCardIndex[cbReverseIndex] >= 1 && cbCardIndex[cbReverseIndex] != 3)
	{
		//构造扑克
		BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
		memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

		//删除扑克
		cbCardIndexTemp[cbCurrentIndex]--;
		cbCardIndexTemp[cbReverseIndex]--;

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
			ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbResultCount = cbResultCount;
			ChiCardInfo[cbChiCardCount].cbCardData[0][0] = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][1] = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][2] = SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount++].cbChiKind = ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XXD : CK_XDD;
		}
	}

	//大小搭吃
	if (cbCardIndex[cbReverseIndex] == 2)
	{
		//构造扑克
		BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
		memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

		//删除扑克
		cbCardIndexTemp[cbReverseIndex] -= 2;

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
			ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbResultCount = cbResultCount;
			ChiCardInfo[cbChiCardCount].cbCardData[0][0] = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][1] = SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount].cbCardData[0][2] = SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount++].cbChiKind = ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XDD : CK_XXD;
		}
	}

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
bool CGameLogic::IsChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE &cbChiCardCount)
{
	if (cbCurrentCard == KING_VALUE)
	{
		return false;
	}
	//效验扑克
	if (!IsValidCard(cbCurrentCard))
	{
		return false;
	}

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//插入扑克
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//提取判断
	while (cbCardIndexTemp[cbCurrentIndex] > 0)
	{
		BYTE cbResult[3] = { 0 };
		if (TakeOutChiCard(cbCardIndexTemp, cbCurrentCard, cbResult) == CK_NULL)
		{
			break;
		}

		cbChiCardCount += 3;
	}

	return (cbCardIndexTemp[cbCurrentIndex] == 0);
}

//是否提跑
bool CGameLogic::IsTiPaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	if (cbCurrentCard == KING_VALUE)
	{
		return false;
	}
	//效验扑克
	if (!IsValidCard(cbCurrentCard))
	{
		return false;
	}

	//转换索引
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//提跑判断
	return (cbCardIndex[cbCurrentIndex] == 3) ? true : false;
}

//是否偎碰
bool CGameLogic::IsWeiPengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	if (cbCurrentCard == KING_VALUE)
	{
		return false;
	}
	//效验扑克
	if (!IsValidCard(cbCurrentCard))
	{
		return false;
	}

	//转换索引
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//偎碰判断
	return (cbCardIndex[cbCurrentIndex] == 2) ? true : false;
}

//胡牌结果
bool CGameLogic::GetHuCardInfo(BYTE cbCardIndex[MAX_INDEX], tagHuCardInfo &HuCardInfo)
{
	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	vector<BYTE> vctTest;
	stAnswer sResult;
	// 设置胡牌类型
	BYTE cbTempHuBaseType = 0;

	switch (HuCardInfo.cbCheckHuBaseType)
	{
	case CheckHuBase_Nomal:
	{
		vctTest.clear();
		for (int i = 0; i<MAX_INDEX; ++i)
		{
			for (int j = 0; j<cbCardIndexTemp[i]; ++j)
			{
				vctTest.push_back(SwitchToCardData(i));
			}
		}

		// 设置倍率
		HuCardInfo.HuFinish.cbHuBase = 1;

		sResult.clear();
		if (!m_HuPaiLogic.initCardHu(vctTest, sResult, HuCardInfo))
		{
			return false;
		}

		int nHuScore = sResult.getHuScore();
		if (nHuScore == 0)
		{
			return false;
		}

		// 设置普通胡
		cbTempHuBaseType |= CheckHuBase_Nomal;
	}
	break;
	case CheckHuBase_WangDiao:
	{
		// 王数量不够
		if (cbCardIndexTemp[KING_INDEX] < 1)
		{
			return false;
		}
		// 没插入牌，不可以王钓
		if (HuCardInfo.cbInsertCard == 0)
		{
			return false;
		}

		HuCardInfo.HuFinish.cbInitialEyeCard[0] = HuCardInfo.cbInsertCard;
		HuCardInfo.HuFinish.cbInitialEyeCard[1] = KING_VALUE;
		HuCardInfo.HuFinish.cbEyeCard[0] = HuCardInfo.cbInsertCard;
		if (HuCardInfo.cbInsertCard != KING_VALUE)
		{
			HuCardInfo.HuFinish.cbEyeCard[1] = HuCardInfo.cbInsertCard;
		}
		else
		{
			HuCardInfo.HuFinish.cbEyeCard[1] = KING_VALUE;
		}
		HuCardInfo.HuFinish.cbEyeCardCount = 2;
		HuCardInfo.HuFinish.cbEyeCardHuXi = 0;

		// 设置倍率
		HuCardInfo.HuFinish.cbHuBase = 2;
		// 王吊王
		if (HuCardInfo.cbInsertCard == KING_VALUE)
		{
			HuCardInfo.HuFinish.cbHuBase *= 2;
		}
		// 去除插入牌
		HuCardInfo.cbInsertCard = 0;
		// 减除王牌
		cbCardIndexTemp[KING_INDEX]--;

		vctTest.clear();
		for (int i = 0; i<MAX_INDEX; ++i)
		{
			for (int j = 0; j<cbCardIndexTemp[i]; ++j)
			{
				vctTest.push_back(SwitchToCardData(i));
			}
		}

		sResult.clear();
		if (!m_HuPaiLogic.initCardHu(vctTest, sResult, HuCardInfo))
		{
			return false;
		}

		int nHuScore = sResult.getHuScore();
		if (nHuScore == 0)
		{
			return false;
		}

		// 设置王钓
		cbTempHuBaseType |= CheckHuBase_WangDiao;
	}
	break;
	case CheckHuBase_WangChuang:
	{
		// 王数量不够
		if (cbCardIndexTemp[KING_INDEX] < 2)
		{
			return false;
		}
		// 没插入牌，不可以王闯
		if (HuCardInfo.cbInsertCard == 0)
		{
			return false;
		}

		// 结果里面再加这个组合胡息
		HuCardInfo.HuFinish.cbInitialEyeCard[0] = HuCardInfo.cbInsertCard;
		HuCardInfo.HuFinish.cbInitialEyeCard[1] = KING_VALUE;
		HuCardInfo.HuFinish.cbInitialEyeCard[2] = KING_VALUE;
		HuCardInfo.HuFinish.cbEyeCard[0] = HuCardInfo.cbInsertCard;
		if (HuCardInfo.cbInsertCard != KING_VALUE)
		{
			HuCardInfo.HuFinish.cbEyeCard[1] = HuCardInfo.cbInsertCard;
			HuCardInfo.HuFinish.cbEyeCard[2] = HuCardInfo.cbInsertCard;
		}
		else
		{
			HuCardInfo.HuFinish.cbEyeCard[1] = KING_VALUE;
			HuCardInfo.HuFinish.cbEyeCard[2] = KING_VALUE;
		}
		HuCardInfo.HuFinish.cbEyeCardCount = 3;
		HuCardInfo.HuFinish.cbEyeCardHuXi = (HuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;		// 是王的时候设置最大胡息,算法里面截枝用

																																// 设置倍率
		HuCardInfo.HuFinish.cbHuBase = 4;
		// 王闯王
		if (HuCardInfo.cbInsertCard == KING_VALUE)
		{
			HuCardInfo.HuFinish.cbHuBase *= 2;
		}
		// 去除插入牌
		HuCardInfo.cbInsertCard = 0;
		// 减除王牌
		cbCardIndexTemp[KING_INDEX] -= 2;

		vctTest.clear();
		for (int i = 0; i<MAX_INDEX; ++i)
		{
			for (int j = 0; j<cbCardIndexTemp[i]; ++j)
			{
				vctTest.push_back(SwitchToCardData(i));
			}
		}

		sResult.clear();
		if (!m_HuPaiLogic.initCardHu(vctTest, sResult, HuCardInfo))
		{
			return false;
		}

		int nHuScore = sResult.getHuScore();
		if (nHuScore == 0)
		{
			return false;
		}

		// 设置王闯
		cbTempHuBaseType |= CheckHuBase_WangChuang;
	}
	break;
	case CheckHuBase_WangZha:
	{
		// 王数量不够
		if (cbCardIndexTemp[KING_INDEX] < 3)
		{
			return false;
		}
		// 没插入牌，不可以王闯
		if (HuCardInfo.cbInsertCard == 0)
		{
			return false;
		}

		// 结果里面再加这个组合胡息
		HuCardInfo.HuFinish.cbInitialEyeCard[0] = HuCardInfo.cbInsertCard;
		HuCardInfo.HuFinish.cbInitialEyeCard[1] = KING_VALUE;
		HuCardInfo.HuFinish.cbInitialEyeCard[2] = KING_VALUE;
		HuCardInfo.HuFinish.cbInitialEyeCard[3] = KING_VALUE;
		HuCardInfo.HuFinish.cbEyeCard[0] = HuCardInfo.cbInsertCard;
		if (HuCardInfo.cbInsertCard != KING_VALUE)
		{
			HuCardInfo.HuFinish.cbEyeCard[1] = HuCardInfo.cbInsertCard;
			HuCardInfo.HuFinish.cbEyeCard[2] = HuCardInfo.cbInsertCard;
			HuCardInfo.HuFinish.cbEyeCard[3] = HuCardInfo.cbInsertCard;
		}
		else
		{
			HuCardInfo.HuFinish.cbEyeCard[1] = KING_VALUE;
			HuCardInfo.HuFinish.cbEyeCard[2] = KING_VALUE;
			HuCardInfo.HuFinish.cbEyeCard[3] = KING_VALUE;
		}
		HuCardInfo.HuFinish.cbEyeCardCount = 4;
		HuCardInfo.HuFinish.cbEyeCardHuXi = (HuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_TI_HU_XI : TYPE_X_TI_HU_XI;		// 是王的时候设置最大胡息,算法里面截枝用

																															// 设置倍率
		HuCardInfo.HuFinish.cbHuBase = 8;
		// 王炸王
		if (HuCardInfo.cbInsertCard == KING_VALUE)
		{
			HuCardInfo.HuFinish.cbHuBase *= 2;
		}
		// 去除插入牌
		HuCardInfo.cbInsertCard = 0;
		// 减除王牌
		cbCardIndexTemp[KING_INDEX] -= 3;

		vctTest.clear();
		for (int i = 0; i<MAX_INDEX; ++i)
		{
			for (int j = 0; j<cbCardIndexTemp[i]; ++j)
			{
				vctTest.push_back(SwitchToCardData(i));
			}
		}

		sResult.clear();
		if (!m_HuPaiLogic.initCardHu(vctTest, sResult, HuCardInfo))
		{
			return false;
		}

		int nHuScore = sResult.getHuScore();
		if (nHuScore == 0)
		{
			return false;
		}

		// 设置王炸
		cbTempHuBaseType |= CheckHuBase_WangZha;
	}
	break;
	case (CheckHuBase_WangDiao | CheckHuBase_WangChuang | CheckHuBase_WangZha):			// 王炸,王闯，王吊
	{
		// 王炸
		do
		{
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// 胡牌信息
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// 王数量不够
			if (cbCardIndexTemp[KING_INDEX] < 3)
			{
				break;
			}
			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// 没插入牌，不可以王钓
			if (cbInsertCard == 0)
			{
				break;
			}

			// 结果里面再加这个组合胡息
			tempHuCardInfo.HuFinish.cbInitialEyeCard[0] = tempHuCardInfo.cbInsertCard;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[1] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[2] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[3] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbEyeCard[0] = tempHuCardInfo.cbInsertCard;
			if (tempHuCardInfo.cbInsertCard != KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = HuCardInfo.cbInsertCard;
				tempHuCardInfo.HuFinish.cbEyeCard[2] = HuCardInfo.cbInsertCard;
				tempHuCardInfo.HuFinish.cbEyeCard[3] = HuCardInfo.cbInsertCard;
			}
			else
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = KING_VALUE;
				tempHuCardInfo.HuFinish.cbEyeCard[2] = KING_VALUE;
				tempHuCardInfo.HuFinish.cbEyeCard[3] = KING_VALUE;
			}
			tempHuCardInfo.HuFinish.cbEyeCardCount = 4;
			tempHuCardInfo.HuFinish.cbEyeCardHuXi = (tempHuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_TI_HU_XI : TYPE_X_TI_HU_XI;		// 是王的时候设置最大胡息,算法里面截枝用

																																		// 设置倍率
			tempHuCardInfo.HuFinish.cbHuBase = 8;
			// 王炸王
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// 去除插入牌
			tempHuCardInfo.cbInsertCard = 0;
			// 减除王牌
			cbCardIndexTemp[KING_INDEX] -= 3;

			vctTest.clear();
			for (int i = 0; i<MAX_INDEX; ++i)
			{
				for (int j = 0; j<cbCardIndexTemp[i]; ++j)
				{
					vctTest.push_back(SwitchToCardData(i));
				}
			}

			// 复制回来
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// 设置王炸
			cbTempHuBaseType |= CheckHuBase_WangZha;
		} while (false);

		// 王闯
		do
		{
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// 胡牌信息
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// 王数量不够
			if (cbCardIndexTemp[KING_INDEX] < 2)
			{
				break;
			}
			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// 没插入牌，不可以王钓
			if (cbInsertCard == 0)
			{
				break;
			}

			// 结果里面再加这个组合胡息
			tempHuCardInfo.HuFinish.cbInitialEyeCard[0] = tempHuCardInfo.cbInsertCard;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[1] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[2] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbEyeCard[0] = tempHuCardInfo.cbInsertCard;
			if (tempHuCardInfo.cbInsertCard != KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = tempHuCardInfo.cbInsertCard;
				tempHuCardInfo.HuFinish.cbEyeCard[2] = tempHuCardInfo.cbInsertCard;
			}
			else
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = KING_VALUE;
				tempHuCardInfo.HuFinish.cbEyeCard[2] = KING_VALUE;
			}
			tempHuCardInfo.HuFinish.cbEyeCardCount = 3;
			tempHuCardInfo.HuFinish.cbEyeCardHuXi = (tempHuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;		// 是王的时候设置最大胡息,算法里面截枝用

																																			// 设置倍率
			tempHuCardInfo.HuFinish.cbHuBase = 4;
			// 王闯王
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// 去除插入牌
			tempHuCardInfo.cbInsertCard = 0;
			// 减除王牌
			cbCardIndexTemp[KING_INDEX] -= 2;

			vctTest.clear();
			for (int i = 0; i<MAX_INDEX; ++i)
			{
				for (int j = 0; j<cbCardIndexTemp[i]; ++j)
				{
					vctTest.push_back(SwitchToCardData(i));
				}
			}

			sResult.clear();
			if (!m_HuPaiLogic.initCardHu(vctTest, sResult, tempHuCardInfo))
			{
				break;
			}

			// 复制回来
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// 设置王闯
			cbTempHuBaseType |= CheckHuBase_WangChuang;
		} while (false);

		// 王钓
		do
		{
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// 胡牌信息
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// 王数量不够
			if (cbCardIndexTemp[KING_INDEX] < 1)
			{
				break;
			}

			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// 没插入牌，不可以王钓
			if (cbInsertCard == 0)
			{
				break;
			}

			tempHuCardInfo.HuFinish.cbInitialEyeCard[0] = tempHuCardInfo.cbInsertCard;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[1] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbEyeCard[0] = tempHuCardInfo.cbInsertCard;
			if (tempHuCardInfo.cbInsertCard != KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = tempHuCardInfo.cbInsertCard;
			}
			else
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = KING_VALUE;
			}
			tempHuCardInfo.HuFinish.cbEyeCardCount = 2;
			tempHuCardInfo.HuFinish.cbEyeCardHuXi = 0;

			// 设置倍率
			tempHuCardInfo.HuFinish.cbHuBase = 2;
			// 王吊王
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// 去除插入牌
			tempHuCardInfo.cbInsertCard = 0;
			// 减除王牌
			cbCardIndexTemp[KING_INDEX]--;

			vctTest.clear();
			for (int i = 0; i<MAX_INDEX; ++i)
			{
				for (int j = 0; j<cbCardIndexTemp[i]; ++j)
				{
					vctTest.push_back(SwitchToCardData(i));
				}
			}

			sResult.clear();
			if (!m_HuPaiLogic.initCardHu(vctTest, sResult, tempHuCardInfo))
			{
				break;
			}

			// 复制回来
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// 设置王钓
			cbTempHuBaseType |= CheckHuBase_WangDiao;
		} while (false);
	}
	break;
	case CheckHuBase_Max:
	{
		// 最后胡牌分数
		int nLastHuScore = 0;
		// 王炸
		do
		{
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// 胡牌信息
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// 王数量不够
			if (cbCardIndexTemp[KING_INDEX] < 3)
			{
				break;
			}
			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// 没插入牌，不可以王钓
			if (cbInsertCard == 0)
			{
				break;
			}

			// 结果里面再加这个组合胡息
			tempHuCardInfo.HuFinish.cbInitialEyeCard[0] = tempHuCardInfo.cbInsertCard;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[1] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[2] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[3] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbEyeCard[0] = tempHuCardInfo.cbInsertCard;
			if (tempHuCardInfo.cbInsertCard != KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = tempHuCardInfo.cbInsertCard;
				tempHuCardInfo.HuFinish.cbEyeCard[2] = tempHuCardInfo.cbInsertCard;
				tempHuCardInfo.HuFinish.cbEyeCard[3] = tempHuCardInfo.cbInsertCard;
			}
			else
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = KING_VALUE;
				tempHuCardInfo.HuFinish.cbEyeCard[2] = KING_VALUE;
				tempHuCardInfo.HuFinish.cbEyeCard[3] = KING_VALUE;
			}
			tempHuCardInfo.HuFinish.cbEyeCardCount = 4;
			tempHuCardInfo.HuFinish.cbEyeCardHuXi = (tempHuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_TI_HU_XI : TYPE_X_TI_HU_XI;		// 是王的时候设置最大胡息,算法里面截枝用

																																		// 设置倍率
			tempHuCardInfo.HuFinish.cbHuBase = 8;
			// 王炸王
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// 去除插入牌
			tempHuCardInfo.cbInsertCard = 0;
			// 减除王牌
			cbCardIndexTemp[KING_INDEX] -= 3;

			vctTest.clear();
			for (int i = 0; i<MAX_INDEX; ++i)
			{
				for (int j = 0; j<cbCardIndexTemp[i]; ++j)
				{
					vctTest.push_back(SwitchToCardData(i));
				}
			}

			sResult.clear();
			if (!m_HuPaiLogic.initCardHu(vctTest, sResult, tempHuCardInfo))
			{
				break;
			}

			int nHuScore = sResult.getHuScore();
			if (nHuScore == 0 || nHuScore < nLastHuScore)
			{
				break;
			}
			nLastHuScore = nHuScore;

			// 复制回来
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// 设置王炸
			cbTempHuBaseType |= CheckHuBase_WangZha;
		} while (false);

		// 王闯
		do
		{
			if (nLastHuScore > 0)
			{
				break;
			}

			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// 胡牌信息
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// 王数量不够
			if (cbCardIndexTemp[KING_INDEX] < 2)
			{
				break;
			}
			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// 没插入牌，不可以王钓
			if (cbInsertCard == 0)
			{
				break;
			}

			// 结果里面再加这个组合胡息
			tempHuCardInfo.HuFinish.cbInitialEyeCard[0] = tempHuCardInfo.cbInsertCard;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[1] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[2] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbEyeCard[0] = tempHuCardInfo.cbInsertCard;
			if (tempHuCardInfo.cbInsertCard != KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = tempHuCardInfo.cbInsertCard;
				tempHuCardInfo.HuFinish.cbEyeCard[2] = tempHuCardInfo.cbInsertCard;
			}
			else
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = KING_VALUE;
				tempHuCardInfo.HuFinish.cbEyeCard[2] = KING_VALUE;
			}
			tempHuCardInfo.HuFinish.cbEyeCardCount = 3;
			tempHuCardInfo.HuFinish.cbEyeCardHuXi = (tempHuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;		// 是王的时候设置最大胡息,算法里面截枝用

																																			// 设置倍率
			tempHuCardInfo.HuFinish.cbHuBase = 4;
			// 王闯王
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// 去除插入牌
			tempHuCardInfo.cbInsertCard = 0;
			// 减除王牌
			cbCardIndexTemp[KING_INDEX] -= 2;

			vctTest.clear();
			for (int i = 0; i<MAX_INDEX; ++i)
			{
				for (int j = 0; j<cbCardIndexTemp[i]; ++j)
				{
					vctTest.push_back(SwitchToCardData(i));
				}
			}

			sResult.clear();
			if (!m_HuPaiLogic.initCardHu(vctTest, sResult, tempHuCardInfo))
			{
				break;
			}

			int nHuScore = sResult.getHuScore();
			if (nHuScore == 0 || nHuScore < nLastHuScore)
			{
				break;
			}
			nLastHuScore = nHuScore;

			// 复制回来
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// 设置王闯
			cbTempHuBaseType |= CheckHuBase_WangChuang;
		} while (false);

		// 王钓
		do
		{
			if (nLastHuScore > 0)
			{
				break;
			}

			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// 胡牌信息
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// 王数量不够
			if (cbCardIndexTemp[KING_INDEX] < 1)
			{
				break;
			}

			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// 没插入牌，不可以王钓
			if (cbInsertCard == 0)
			{
				break;
			}

			tempHuCardInfo.HuFinish.cbInitialEyeCard[0] = tempHuCardInfo.cbInsertCard;
			tempHuCardInfo.HuFinish.cbInitialEyeCard[1] = KING_VALUE;
			tempHuCardInfo.HuFinish.cbEyeCard[0] = tempHuCardInfo.cbInsertCard;
			if (tempHuCardInfo.cbInsertCard != KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = tempHuCardInfo.cbInsertCard;
			}
			else
			{
				tempHuCardInfo.HuFinish.cbEyeCard[1] = KING_VALUE;
			}
			tempHuCardInfo.HuFinish.cbEyeCardCount = 2;
			tempHuCardInfo.HuFinish.cbEyeCardHuXi = 0;

			// 设置倍率
			tempHuCardInfo.HuFinish.cbHuBase = 2;
			// 王吊王
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// 去除插入牌
			tempHuCardInfo.cbInsertCard = 0;
			// 减除王牌
			cbCardIndexTemp[KING_INDEX]--;

			vctTest.clear();
			for (int i = 0; i<MAX_INDEX; ++i)
			{
				for (int j = 0; j<cbCardIndexTemp[i]; ++j)
				{
					vctTest.push_back(SwitchToCardData(i));
				}
			}

			sResult.clear();
			if (!m_HuPaiLogic.initCardHu(vctTest, sResult, tempHuCardInfo))
			{
				break;
			}

			int nHuScore = sResult.getHuScore();
			if (nHuScore == 0 || nHuScore < nLastHuScore)
			{
				break;
			}
			nLastHuScore = nHuScore;

			// 复制回来
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// 设置王钓
			cbTempHuBaseType |= CheckHuBase_WangDiao;
		} while (false);

		// 普通胡
		do
		{
			if (nLastHuScore > 0)
			{
				break;
			}

			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

			// 胡牌信息
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));

			vctTest.clear();
			for (int i = 0; i<MAX_INDEX; ++i)
			{
				for (int j = 0; j<cbCardIndexTemp[i]; ++j)
				{
					vctTest.push_back(SwitchToCardData(i));
				}
			}

			// 设置倍率
			tempHuCardInfo.HuFinish.cbHuBase = 1;

			sResult.clear();
			if (!m_HuPaiLogic.initCardHu(vctTest, sResult, tempHuCardInfo))
			{
				break;
			}

			int nHuScore = sResult.getHuScore();
			if (nHuScore == 0 || nHuScore < nLastHuScore)
			{
				break;
			}
			nLastHuScore = nHuScore;

			// 复制回来
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));

			// 设置普通胡
			cbTempHuBaseType |= CheckHuBase_Nomal;
		} while (false);

		if (nLastHuScore <= 0)
		{
			return false;
		}
	}
	break;
	default:
	{
		return false;
	}
	break;
	}

	// 设置最终胡牌类型
	HuCardInfo.cbCheckHuBaseType = cbTempHuBaseType;

	int nWeaveIndex = 0;
	BYTE cbFinishIndexCount = sResult.getFinishAllIndexCount();
	for (size_t i = 0; i<cbFinishIndexCount; ++i)
	{
		if (nWeaveIndex >= MAX_WEAVE)
		{
			break;
		}

		BYTE cbCardIndexCount = sResult.getFinishIndexCount(i);
		if (cbCardIndexCount == 0)
		{
			continue;
		}

		vector<BYTE> vCard;
		for (size_t j = 0; j<cbCardIndexCount; ++j)
		{
			BYTE cbCardIndex = sResult.getFinishIndex(i, j);
			if (cbCardIndex >= MAX_INDEX)
			{
				continue;
			}

			if (cbCardIndex >= 0 && cbCardIndex < (int)vctTest.size())
			{
				vCard.push_back(vctTest[cbCardIndex]);
			}
		}
		BYTE cbCardCount = (BYTE)vCard.size();
		if (cbCardCount == 0)
		{
			continue;
		}

		sort(vCard.begin(), vCard.end());
		HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardCount = cbCardCount;
		HuCardInfo.WeaveItemArray[nWeaveIndex].cbCenterCard = vCard[0];
		for (size_t j = 0; j<cbCardIndexCount; ++j)
		{
			HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[j] = vCard[j];
		}

		if (cbCardIndexCount == 4)
		{
			HuCardInfo.WeaveItemArray[nWeaveIndex].nWeaveKind = ACK_TI;
		}
		else if (cbCardIndexCount == 3)
		{
			if (HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[0] == HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[1] && HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[1] == HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[2])
			{
				// 非自己摸牌，且是插入牌设置成碰
				if (!HuCardInfo.bSelfAddCard && HuCardInfo.cbInsertCard == HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[0])
				{
					HuCardInfo.WeaveItemArray[nWeaveIndex].nWeaveKind = ACK_PENG;
				}
				else
				{
					HuCardInfo.WeaveItemArray[nWeaveIndex].nWeaveKind = ACK_WEI;
				}
			}
			else
			{
				HuCardInfo.WeaveItemArray[nWeaveIndex].nWeaveKind = ACK_CHI;
			}
		}

		// 增加组合索引
		++nWeaveIndex;
	}

	// 有牌眼
	BYTE cbEyeCardCount = HuCardInfo.HuFinish.cbEyeCardCount;
	if (cbEyeCardCount > 0)
	{
		HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardCount = cbEyeCardCount;
		HuCardInfo.WeaveItemArray[nWeaveIndex].cbCenterCard = HuCardInfo.HuFinish.cbInitialEyeCard[0];
		for (int i = 0; i<cbEyeCardCount; ++i)
		{
			HuCardInfo.WeaveItemArray[nWeaveIndex].cbCardList[i] = HuCardInfo.HuFinish.cbInitialEyeCard[i];
		}
		if (cbEyeCardCount == 2)
		{
			HuCardInfo.WeaveItemArray[nWeaveIndex].nWeaveKind = ACK_NULL;
		}
		else if (cbEyeCardCount == 3)
		{
			HuCardInfo.WeaveItemArray[nWeaveIndex].nWeaveKind = ACK_WEI;
		}
		else if (cbEyeCardCount == 4)
		{
			HuCardInfo.WeaveItemArray[nWeaveIndex].nWeaveKind = ACK_TI;
		}

		// 增加组合索引
		++nWeaveIndex;
	}
	HuCardInfo.cbWeaveCount = nWeaveIndex;
	tagHuFinishInfo *pHuFinsh = sResult.getHuFinshInfo();
	if (pHuFinsh != NULL)
	{
		memcpy(&HuCardInfo.HuFinish, pHuFinsh, sizeof(HuCardInfo.HuFinish));
	}

	return true;
}


//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	if (cbCardData == KING_VALUE)
	{
		return true;
	}

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

//提取吃牌
BYTE CGameLogic::TakeOutChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbResultCard[3])
{
	//效验扑克
	if (cbCurrentCard == 0)
	{
		return 0;
	}

	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	// 最多是自己手上2个+要吃的1个，最多3个
	if (cbCardIndex[cbCurrentIndex] >= 4)
	{
		return 0;
	}

	//大小搭吃
	BYTE cbReverseIndex = (cbCurrentIndex + 10) % COMMOND_CARD_COUNT;
	if (cbCardIndex[cbCurrentIndex] >= 2 && cbCardIndex[cbReverseIndex] >= 1 && cbCardIndex[cbReverseIndex] <= 2)
	{
		//删除扑克
		cbCardIndex[cbCurrentIndex] -= 2;
		cbCardIndex[cbReverseIndex]--;

		//设置结果
		cbResultCard[0] = cbCurrentCard;
		cbResultCard[1] = cbCurrentCard;
		cbResultCard[2] = SwitchToCardData(cbReverseIndex);

		return ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XXD : CK_XDD;
	}

	//大小搭吃
	if (cbCardIndex[cbReverseIndex] == 2 && cbCardIndex[cbCurrentIndex] >= 1 && cbCardIndex[cbCurrentIndex] <= 2)
	{
		//删除扑克
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbReverseIndex] -= 2;

		//设置结果
		cbResultCard[0] = cbCurrentCard;
		cbResultCard[1] = SwitchToCardData(cbReverseIndex);
		cbResultCard[2] = SwitchToCardData(cbReverseIndex);

		return ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XDD : CK_XXD;
	}

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
			if (cbCardIndex[cbIndex] == 0 || (cbCurrentIndex != cbIndex && cbCardIndex[cbIndex] >= 3))
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
			if (cbCardIndex[cbFirstIndex] == 0 || (cbCurrentIndex != cbFirstIndex && cbCardIndex[cbFirstIndex] == 3))
			{
				continue;
			}
			if (cbCardIndex[cbFirstIndex + 1] == 0 || (cbCurrentIndex != cbFirstIndex + 1 && cbCardIndex[cbFirstIndex + 1] == 3))
			{
				continue;
			}
			if (cbCardIndex[cbFirstIndex + 2] == 0 || (cbCurrentIndex != cbFirstIndex + 2 && cbCardIndex[cbFirstIndex + 2] == 3))
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
	if (cbCardIndex == KING_INDEX)
	{
		return KING_VALUE;
	}

	if (cbCardIndex >= MAX_INDEX)
	{
		return 0;
	}
	return ((cbCardIndex / 10) << 4) | (cbCardIndex % 10 + 1);
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
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

//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[], BYTE bMaxCount)
{
	//转换扑克
	BYTE bPosition = 0;
	for (BYTE i = 0; i<MAX_INDEX; i++)
	{
		if (cbCardIndex[i] != 0)
		{
			for (BYTE j = 0; j<cbCardIndex[i]; j++)
			{
				if (bPosition >= bMaxCount)
				{
					continue;
				}
				cbCardData[bPosition++] = SwitchToCardData(i);
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