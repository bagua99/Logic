#include "HuPaiLogic.h"

CHuPaiLogic::CHuPaiLogic()
{
	m_mapKey.clear();
	m_bOnlyHu = false;
	::memset(&m_HuCardInfo, 0, sizeof(m_HuCardInfo));
}

CHuPaiLogic::~CHuPaiLogic()
{
}

// 初始化牌胡息
void CHuPaiLogic::initCardHuXi()
{
	//四张牌时  只有一种情况 
	//	1.四张相同的或带王	
	//三张牌时 
	//	1.三张一样的或带王
	//  2.二七十
	//  3.大小搭
	//	4.连牌
	if (m_mapKey.empty())
	{
		getTiList();
		getWeiList();
		get27AList();
		getShunZhiList();
	}
}

// 取得胡息
int CHuPaiLogic::getCardHuXiVal(int nKey)
{
	//四张牌时  只有一种情况 
	//	1.四张相同的或带王	
	//三张牌时 
	//	1.三张一样的或带王
	//  2.二七十
	//  3.大小搭
	//	4.连牌
	if (m_mapKey.empty())
	{
		getTiList();
		getWeiList();
		get27AList();
		getShunZhiList();
	}

	MAP_LIST::iterator iter = m_mapKey.find(nKey);
	if (iter != m_mapKey.end())
	{
		return int(iter->second);
	}

	return -1;
}

// 取得胡息
int CHuPaiLogic::getCardHuXiVal(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4)
{
	// 牌键值
	int nKey = getKey(cbCard1, cbCard2, cbCard3, cbCard4);

	return getCardHuXiVal(nKey);
}

// 是否拥有规则
bool CHuPaiLogic::isHavePlayType(enPlayType nType)
{
	if (nType >= enPlayType_Max)
	{
		return false;
	}

	return (PLAY_TYPE & (1 << nType)) != 0;
}

// 胡息键值
int CHuPaiLogic::getKey(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4)
{
	vector<BYTE> vctTemp;
	vctTemp.push_back(cbCard1);
	vctTemp.push_back(cbCard2);
	vctTemp.push_back(cbCard3);
	vctTemp.push_back(cbCard4);
	sort(vctTemp.begin(), vctTemp.end());

	return ((vctTemp[3] << 24) + (vctTemp[2] << 16) + (vctTemp[1] << 8) + vctTemp[0]);
}

// 增加胡息表
void CHuPaiLogic::addMapVal(int nKey, BYTE cbVal)
{
	MAP_LIST::iterator iter = m_mapKey.find(nKey);
	if (iter == m_mapKey.end())
	{
		m_mapKey[nKey] = cbVal;
	}
	else if (iter->second < cbVal)
	{
		m_mapKey[nKey] = cbVal;
	}
}

// 取得提列表
void CHuPaiLogic::getTiList()
{
	for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
	{
		BYTE cbBigVal = cbVal + 0x10;
		addMapVal(getKey(cbVal, cbVal, cbVal, cbVal), TYPE_X_TI_HU_XI);
		addMapVal(getKey(cbBigVal, cbBigVal, cbBigVal, cbBigVal), TYPE_D_TI_HU_XI);

		// 跟王补成小提
		addMapVal(getKey(KING_VALUE, KING_VALUE, KING_VALUE, cbVal), TYPE_X_TI_HU_XI);
		addMapVal(getKey(KING_VALUE, KING_VALUE, cbVal, cbVal), TYPE_X_TI_HU_XI);
		addMapVal(getKey(KING_VALUE, cbVal, cbVal, cbVal), TYPE_X_TI_HU_XI);

		// 跟王补成大提
		addMapVal(getKey(KING_VALUE, KING_VALUE, KING_VALUE, cbBigVal), TYPE_D_TI_HU_XI);
		addMapVal(getKey(KING_VALUE, KING_VALUE, cbBigVal, cbBigVal), TYPE_D_TI_HU_XI);
		addMapVal(getKey(KING_VALUE, cbBigVal, cbBigVal, cbBigVal), TYPE_D_TI_HU_XI);
	}

	// 四王默认组大提
	addMapVal(getKey(KING_VALUE, KING_VALUE, KING_VALUE, KING_VALUE), TYPE_D_TI_HU_XI);
}

// 取得偎列表
void CHuPaiLogic::getWeiList()
{
	for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
	{
		BYTE cbBigVal = cbVal + 0x10;
		addMapVal(getKey(cbVal, cbVal, cbVal, 0), TYPE_X_WEI_HU_XI);
		addMapVal(getKey(cbBigVal, cbBigVal, cbBigVal, 0), TYPE_D_WEI_HU_XI);

		// 跟王补成小偎
		addMapVal(getKey(KING_VALUE, cbVal, cbVal, 0), TYPE_X_WEI_HU_XI);
		addMapVal(getKey(KING_VALUE, KING_VALUE, cbVal, 0), TYPE_X_WEI_HU_XI);

		// 跟王补成大偎
		addMapVal(getKey(KING_VALUE, cbBigVal, cbBigVal, 0), TYPE_D_WEI_HU_XI);
		addMapVal(getKey(KING_VALUE, KING_VALUE, cbBigVal, 0), TYPE_D_WEI_HU_XI);

		if (isHavePlayType(enPlayType_XDD))
		{
			addMapVal(getKey(cbBigVal, cbVal, cbVal, 0), 0);
			addMapVal(getKey(cbBigVal, cbBigVal, cbVal, 0), 0);

			// 跟王补成小大搭
			addMapVal(getKey(KING_VALUE, cbBigVal, cbVal, 0), 0);
		}
	}

	// 三王默认组大偎
	addMapVal(getKey(KING_VALUE, KING_VALUE, KING_VALUE, 0), TYPE_D_WEI_HU_XI);
}

// 取得27A列表
void CHuPaiLogic::get27AList()
{
	if (isHavePlayType(enPlayType_27A))
	{
		addMapVal(getKey(0x0A, 0x07, 0x02, 0x00), TYPE_X_27A_HU_XI);
		addMapVal(getKey(0x1A, 0x17, 0x12, 0x00), TYPE_D_27A_HU_XI);

		//跟王补成小27A
		addMapVal(getKey(KING_VALUE, 0x02, 0x07, 0x00), TYPE_X_27A_HU_XI);
		addMapVal(getKey(KING_VALUE, 0x07, 0x0A, 0x00), TYPE_X_27A_HU_XI);
		addMapVal(getKey(KING_VALUE, 0x02, 0x0A, 0x00), TYPE_X_27A_HU_XI);

		//跟王补成大27A
		addMapVal(getKey(KING_VALUE, 0x12, 0x17, 0x00), TYPE_D_27A_HU_XI);
		addMapVal(getKey(KING_VALUE, 0x17, 0x1A, 0x00), TYPE_D_27A_HU_XI);
		addMapVal(getKey(KING_VALUE, 0x12, 0x1A, 0x00), TYPE_D_27A_HU_XI);
	}
}

// 取得顺子列表
void CHuPaiLogic::getShunZhiList()
{
	for (BYTE cbVal = 1; cbVal <= 8; ++cbVal)
	{
		BYTE cbBigVal = cbVal + 0x10;

		BYTE nHuXi = 0;
		if (isHavePlayType(enPlayType_123))
			nHuXi = (cbVal == 1) ? 1 : 0;

		addMapVal(getKey(cbVal, cbVal + 1, cbVal + 2, 0x00), nHuXi * TYPE_X_123_HU_XI);
		addMapVal(getKey(0x10 + cbVal, 0x10 + cbVal + 1, 0x10 + cbVal + 2, 0x00), nHuXi * TYPE_D_123_HU_XI);

		// 跟王补成小顺子
		addMapVal(getKey(KING_VALUE, cbVal, cbVal + 1, 0x00), nHuXi * TYPE_X_123_HU_XI);
		addMapVal(getKey(KING_VALUE, cbVal, cbVal + 2, 0x00), nHuXi * TYPE_X_123_HU_XI);
		addMapVal(getKey(KING_VALUE, cbVal + 1, cbVal + 2, 0x00), nHuXi * TYPE_X_123_HU_XI);

		// 跟王补成大顺子
		addMapVal(getKey(KING_VALUE, cbBigVal, cbBigVal + 1, 0x00), nHuXi * TYPE_D_123_HU_XI);
		addMapVal(getKey(KING_VALUE, cbBigVal, cbBigVal + 2, 0x00), nHuXi * TYPE_D_123_HU_XI);
		addMapVal(getKey(KING_VALUE, cbBigVal + 1, cbBigVal + 2, 0x00), nHuXi * TYPE_D_123_HU_XI);
	}
}

// 获取胡牌结果
void CHuPaiLogic::getCardHuResult(stCardData stData, stAnswer &answerData, stAnswer &resultMax)
{
	// 设置只判断胡
	if (m_bOnlyHu)
	{
		return;
	}

	// 1.预处理	
	//vector<stHuKeyData> &vctHuKeyData = answerData.vctData;	

	// 2.
	BYTE *pLeftIndex = stData.getLeftIndex();
	int nLeftIndexCount = stData.getLeftIndexCount();
	if (nLeftIndexCount == 0)
	{
		int nMaxHuPer = resultMax.getHuScore();
		int nAnswerHuPer = answerData.getHuScore(&stData);
		if (nMaxHuPer == 0 || (nAnswerHuPer > 0 && nMaxHuPer < nAnswerHuPer))
		{
			resultMax = answerData;

			// 设置只判断胡
			m_bOnlyHu = m_HuCardInfo.bOnlyHu;
		}
	}
	else if (nLeftIndexCount <= 1)
	{
		// 1个不处理,继续等待后面运算
	}
	else if (nLeftIndexCount == 2)
	{
		// 牌眼是对，还来对，不能胡
		if (answerData.getHuEyeCardCount() != 2)
		{
			BYTE cbCard1 = stData.getCardValue(pLeftIndex[0]);
			BYTE cbCard2 = stData.getCardValue(pLeftIndex[1]);
			// 可以胡
			if (cbCard1 == cbCard2 || cbCard1 == KING_VALUE || cbCard2 == KING_VALUE)
			{
				answerData.push(stHuKeyData(0, pLeftIndex[0], pLeftIndex[1]), &stData);
				int nMaxHuPer = resultMax.getHuScore();
				int nAnswerHuPer = answerData.getHuScore(&stData);
				if (nMaxHuPer == 0 || (nAnswerHuPer > 0 && nMaxHuPer < nAnswerHuPer))
				{
					resultMax = answerData;

					// 设置只判断胡
					m_bOnlyHu = m_HuCardInfo.bOnlyHu;
				}
				answerData.pop(&stData);
			}
		}
	}
	else if (nLeftIndexCount == 3)
	{
		int nHuKey = getCardHuXiVal(stData.getCardValue(pLeftIndex[0]), stData.getCardValue(pLeftIndex[1]), stData.getCardValue(pLeftIndex[2]));
		if (nHuKey >= 0)
		{
			answerData.push(stHuKeyData(nHuKey, pLeftIndex[0], pLeftIndex[1], pLeftIndex[2]), &stData);
			int nMaxHuPer = resultMax.getHuScore();
			int nAnswerHuPer = answerData.getHuScore(&stData);
			if (nMaxHuPer == 0 || (nAnswerHuPer > 0 && nMaxHuPer < nAnswerHuPer))
			{
				resultMax = answerData;

				// 设置只判断胡
				m_bOnlyHu = m_HuCardInfo.bOnlyHu;
			}
			answerData.pop(&stData);
		}
	}
	else	// 还有多个
	{
		// 王组提
		for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
		{
			// 取得王牌信息
			BYTE cbWangCardIndex = stData.getCardIndex(KING_VALUE);
			BYTE cbWangCardCount = stData.getCardIndexCountInIndex(cbWangCardIndex);
			BYTE *pWangCardIndexArray = stData.getCardIndexArrayInIndex(cbWangCardIndex);
			if (cbWangCardCount >= 1)
			{
				// 取得牌信息
				BYTE cbCardIndex_1 = stData.getCardIndex(cbVal);
				BYTE cbCardCount_1 = stData.getCardIndexCountInIndex(cbCardIndex_1);
				BYTE *pCardIndexArray_1 = stData.getCardIndexArrayInIndex(cbCardIndex_1);
				// 有4张，不是自己摸的，并且是插入的这张牌，可以尝试跟王组个提
				if (cbCardCount_1 == 4 && !m_HuCardInfo.bSelfAddCard && m_HuCardInfo.cbInsertCard == cbVal)
				{
					stHuKeyData sKeyData(TYPE_X_TI_HU_XI, pCardIndexArray_1[0], pCardIndexArray_1[1], pCardIndexArray_1[2], pWangCardIndexArray[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
				else if (cbCardCount_1 == 3)
				{
					stHuKeyData sKeyData(TYPE_X_TI_HU_XI, pCardIndexArray_1[0], pCardIndexArray_1[1], pCardIndexArray_1[2], pWangCardIndexArray[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
				else if (cbCardCount_1 == 2 && cbWangCardCount >= 2)
				{
					stHuKeyData sKeyData(TYPE_X_TI_HU_XI, pCardIndexArray_1[0], pCardIndexArray_1[1], pWangCardIndexArray[0], pWangCardIndexArray[1]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
				else if (cbCardCount_1 == 1 && cbWangCardCount >= 3)
				{
					stHuKeyData sKeyData(TYPE_X_TI_HU_XI, pCardIndexArray_1[0], pWangCardIndexArray[0], pWangCardIndexArray[1], pWangCardIndexArray[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				BYTE cbBigVal = cbVal + 0x10;
				// 取得牌信息
				BYTE cbCardIndex_2 = stData.getCardIndex(cbBigVal);
				BYTE cbCardCount_2 = stData.getCardIndexCountInIndex(cbCardIndex_2);
				BYTE *pCardIndexArray_2 = stData.getCardIndexArrayInIndex(cbCardIndex_2);
				// 有4张，不是自己摸的，并且是插入的这张牌，可以尝试跟王组个提
				if (cbCardCount_2 == 4 && !m_HuCardInfo.bSelfAddCard && m_HuCardInfo.cbInsertCard == cbBigVal)
				{
					stHuKeyData sKeyData(TYPE_D_TI_HU_XI, pCardIndexArray_2[0], pCardIndexArray_2[1], pCardIndexArray_2[2], pWangCardIndexArray[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
				else if (cbCardCount_2 >= 3)
				{
					stHuKeyData sKeyData(TYPE_D_TI_HU_XI, pCardIndexArray_2[0], pCardIndexArray_2[1], pCardIndexArray_2[2], pWangCardIndexArray[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
				else if (cbCardCount_2 == 2 && cbWangCardCount >= 2)
				{
					stHuKeyData sKeyData(TYPE_D_TI_HU_XI, pCardIndexArray_2[0], pCardIndexArray_2[1], pWangCardIndexArray[0], pWangCardIndexArray[1]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
				else if (cbCardCount_2 == 1 && cbWangCardCount >= 3)
				{
					stHuKeyData sKeyData(TYPE_D_TI_HU_XI, pCardIndexArray_2[0], pWangCardIndexArray[0], pWangCardIndexArray[1], pWangCardIndexArray[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
			else
			{
				break;
			}
		}

		// 王组偎
		for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
		{
			// 取得王牌信息
			BYTE cbWangCardIndex = stData.getCardIndex(KING_VALUE);
			BYTE cbWangCardCount = stData.getCardIndexCountInIndex(cbWangCardIndex);
			BYTE *pWangCardIndexArray = stData.getCardIndexArrayInIndex(cbWangCardIndex);
			if (cbWangCardCount >= 1)
			{
				// 取得牌信息
				BYTE cbCardIndex_1 = stData.getCardIndex(cbVal);
				BYTE cbCardCount_1 = stData.getCardIndexCountInIndex(cbCardIndex_1);
				BYTE *pCardIndexArray_1 = stData.getCardIndexArrayInIndex(cbCardIndex_1);
				// 有3张，不是自己摸的，并且是插入的这张牌，可以尝试跟王组个偎
				if (cbCardCount_1 == 3 && !m_HuCardInfo.bSelfAddCard && m_HuCardInfo.cbInsertCard == cbVal)
				{
					stHuKeyData sKeyData(TYPE_X_WEI_HU_XI, pCardIndexArray_1[0], pCardIndexArray_1[1], pWangCardIndexArray[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
				else if (cbCardCount_1 == 2 && cbWangCardCount >= 1)
				{
					stHuKeyData sKeyData(TYPE_X_WEI_HU_XI, pCardIndexArray_1[0], pCardIndexArray_1[1], pWangCardIndexArray[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
				else if (cbCardCount_1 == 1 && cbWangCardCount >= 2)
				{
					stHuKeyData sKeyData(TYPE_X_WEI_HU_XI, pCardIndexArray_1[0], pWangCardIndexArray[0], pWangCardIndexArray[1]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				BYTE cbBigVal = cbVal + 0x10;
				// 取得牌信息
				BYTE cbCardIndex_2 = stData.getCardIndex(cbBigVal);
				BYTE cbCardCount_2 = stData.getCardIndexCountInIndex(cbCardIndex_2);
				BYTE *pCardIndexArray_2 = stData.getCardIndexArrayInIndex(cbCardIndex_2);
				// 有3张，不是自己摸的，并且是插入的这张牌，可以尝试跟王组个偎
				if (cbCardCount_2 == 3 && !m_HuCardInfo.bSelfAddCard && m_HuCardInfo.cbInsertCard == cbBigVal)
				{
					stHuKeyData sKeyData(TYPE_D_WEI_HU_XI, pCardIndexArray_2[0], pCardIndexArray_2[1], pWangCardIndexArray[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
				else if (cbCardCount_2 == 2 && cbWangCardCount >= 1)
				{
					stHuKeyData sKeyData(TYPE_D_WEI_HU_XI, pCardIndexArray_2[0], pCardIndexArray_2[1], pWangCardIndexArray[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
				else if (cbCardCount_2 == 1 && cbWangCardCount >= 2)
				{
					stHuKeyData sKeyData(TYPE_D_WEI_HU_XI, pCardIndexArray_2[0], pWangCardIndexArray[0], pWangCardIndexArray[1]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
			else
			{
				break;
			}
		}

		// 4个王组提
		{
			// 取得王牌信息
			BYTE cbWangCardIndex = stData.getCardIndex(KING_VALUE);
			BYTE cbWangCardCount = stData.getCardIndexCountInIndex(cbWangCardIndex);
			BYTE *pWangCardIndexArray = stData.getCardIndexArrayInIndex(cbWangCardIndex);
			if (cbWangCardCount >= 4)
			{
				stHuKeyData sKeyData(TYPE_D_TI_HU_XI, pWangCardIndexArray[0], pWangCardIndexArray[1], pWangCardIndexArray[2], pWangCardIndexArray[3]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}
		}

		// 3王组偎
		{
			// 取得王牌信息
			BYTE cbWangCardIndex = stData.getCardIndex(KING_VALUE);
			BYTE cbWangCardCount = stData.getCardIndexCountInIndex(cbWangCardIndex);
			BYTE *pWangCardIndexArray = stData.getCardIndexArrayInIndex(cbWangCardIndex);
			if (cbWangCardCount >= 3)
			{
				stHuKeyData sKeyData(TYPE_D_WEI_HU_XI, pWangCardIndexArray[0], pWangCardIndexArray[1], pWangCardIndexArray[2]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}
		}

		// 组偎
		for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
		{
			// 取得牌信息
			BYTE cbCardIndex_1 = stData.getCardIndex(cbVal);
			BYTE cbCardCount_1 = stData.getCardIndexCountInIndex(cbCardIndex_1);
			BYTE *pCardIndexArray_1 = stData.getCardIndexArrayInIndex(cbCardIndex_1);

			// 不是自己摸的，并且是插入的这张牌
			if (!m_HuCardInfo.bSelfAddCard && m_HuCardInfo.cbInsertCard == cbVal)
			{
				// 有4张，可以尝试组个偎
				if (cbCardCount_1 == 4)
				{
					stHuKeyData sKeyData(TYPE_X_WEI_HU_XI, pCardIndexArray_1[0], pCardIndexArray_1[1], pCardIndexArray_1[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				// 有3张，可以尝试组个碰
				if (cbCardCount_1 == 3)
				{
					stHuKeyData sKeyData(TYPE_X_PENG_HU_XI, pCardIndexArray_1[0], pCardIndexArray_1[1], pCardIndexArray_1[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
			else
			{
				// 有3张，可以尝试组个偎
				if (cbCardCount_1 == 3)
				{
					stHuKeyData sKeyData(TYPE_X_WEI_HU_XI, pCardIndexArray_1[0], pCardIndexArray_1[1], pCardIndexArray_1[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}

			BYTE cbBigVal = cbVal + 0x10;
			// 取得牌信息
			BYTE cbCardIndex_2 = stData.getCardIndex(cbBigVal);
			BYTE cbCardCount_2 = stData.getCardIndexCountInIndex(cbCardIndex_2);
			BYTE *pCardIndexArray_2 = stData.getCardIndexArrayInIndex(cbCardIndex_2);
			// 不是自己摸的，并且是插入的这张牌
			if (!m_HuCardInfo.bSelfAddCard && m_HuCardInfo.cbInsertCard == cbBigVal)
			{
				// 有4张，可以尝试组个偎
				if (cbCardCount_2 == 4)
				{
					stHuKeyData sKeyData(TYPE_D_WEI_HU_XI, pCardIndexArray_2[0], pCardIndexArray_2[1], pCardIndexArray_2[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (cbCardCount_2 == 3)
				{
					stHuKeyData sKeyData(TYPE_D_PENG_HU_XI, pCardIndexArray_2[0], pCardIndexArray_2[1], pCardIndexArray_2[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
			else
			{
				if (cbCardCount_2 == 3)
				{
					stHuKeyData sKeyData(TYPE_D_WEI_HU_XI, pCardIndexArray_2[0], pCardIndexArray_2[1], pCardIndexArray_2[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
		}

		//【2.1】 判断摸的牌是否可以组碰
		{
			// 不是自己摸牌，要判断下，这个牌是否有3个，有3个要尝试下组合碰
			if (!m_HuCardInfo.bSelfAddCard && m_HuCardInfo.cbInsertCard != 0)
			{
				// 取得牌信息
				BYTE cbCardIndex_1 = stData.getCardIndex(m_HuCardInfo.cbInsertCard);
				BYTE cbCardCount_1 = stData.getCardIndexCountInIndex(cbCardIndex_1);
				BYTE *pCardIndexArray_1 = stData.getCardIndexArrayInIndex(cbCardIndex_1);
				if (cbCardCount_1 >= 3)
				{
					BYTE cbHuxi = (m_HuCardInfo.cbInsertCard & 0xF0) > 0 ? TYPE_D_PENG_HU_XI : TYPE_X_PENG_HU_XI;
					stHuKeyData sKeyData(cbHuxi, pCardIndexArray_1[0], pCardIndexArray_1[1], pCardIndexArray_1[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
		}

		//【2.2】 一二三
		{
			// 取得牌信息
			BYTE cbCardIndex_1 = stData.getCardIndex(0x11);
			BYTE cbCardCount_1 = stData.getCardIndexCountInIndex(cbCardIndex_1);
			BYTE *pCardIndexArray_1 = stData.getCardIndexArrayInIndex(cbCardIndex_1);
			// 取得牌信息
			BYTE cbCardIndex_2 = stData.getCardIndex(0x12);
			BYTE cbCardCount_2 = stData.getCardIndexCountInIndex(cbCardIndex_2);
			BYTE *pCardIndexArray_2 = stData.getCardIndexArrayInIndex(cbCardIndex_2);
			// 取得牌信息
			BYTE cbCardIndex_3 = stData.getCardIndex(0x13);
			BYTE cbCardCount_3 = stData.getCardIndexCountInIndex(cbCardIndex_3);
			BYTE *pCardIndexArray_3 = stData.getCardIndexArrayInIndex(cbCardIndex_3);
			if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_2 > 0 && cbCardCount_2 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
			{
				stHuKeyData sKeyData(6, pCardIndexArray_1[0], pCardIndexArray_2[0], pCardIndexArray_3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}

			// 取得王牌信息
			BYTE cbWangCardIndex = stData.getCardIndex(KING_VALUE);
			BYTE cbWangCardCount = stData.getCardIndexCountInIndex(cbWangCardIndex);
			BYTE *pWangCardIndexArray = stData.getCardIndexArrayInIndex(cbWangCardIndex);
			if (cbWangCardCount >= 1)
			{
				if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_2 > 0 && cbCardCount_2 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_1[0], pCardIndexArray_2[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_1[0], pCardIndexArray_3[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (cbCardCount_2 > 0 && cbCardCount_2 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_2[0], pCardIndexArray_3[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
		}

		//【2.3】 1 2 3
		{
			// 取得牌信息
			BYTE cbCardIndex_1 = stData.getCardIndex(0x01);
			BYTE cbCardCount_1 = stData.getCardIndexCountInIndex(cbCardIndex_1);
			BYTE *pCardIndexArray_1 = stData.getCardIndexArrayInIndex(cbCardIndex_1);
			// 取得牌信息
			BYTE cbCardIndex_2 = stData.getCardIndex(0x02);
			BYTE cbCardCount_2 = stData.getCardIndexCountInIndex(cbCardIndex_2);
			BYTE *pCardIndexArray_2 = stData.getCardIndexArrayInIndex(cbCardIndex_2);
			// 取得牌信息
			BYTE cbCardIndex_3 = stData.getCardIndex(0x03);
			BYTE cbCardCount_3 = stData.getCardIndexCountInIndex(cbCardIndex_3);
			BYTE *pCardIndexArray_3 = stData.getCardIndexArrayInIndex(cbCardIndex_3);
			if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_2 > 0 && cbCardCount_2 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
			{
				stHuKeyData sKeyData(3, pCardIndexArray_1[0], pCardIndexArray_2[0], pCardIndexArray_3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}

			// 取得王牌信息
			BYTE cbWangCardIndex = stData.getCardIndex(KING_VALUE);
			BYTE cbWangCardCount = stData.getCardIndexCountInIndex(cbWangCardIndex);
			BYTE *pWangCardIndexArray = stData.getCardIndexArrayInIndex(cbWangCardIndex);
			if (cbWangCardCount >= 1)
			{
				if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_2 > 0 && cbCardCount_2 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_1[0], pCardIndexArray_2[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_1[0], pCardIndexArray_3[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (cbCardCount_2 > 0 && cbCardCount_2 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_2[0], pCardIndexArray_3[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
		}

		//【2.4】 二七十
		{
			// 取得牌信息
			BYTE cbCardIndex_1 = stData.getCardIndex(0x12);
			BYTE cbCardCount_1 = stData.getCardIndexCountInIndex(cbCardIndex_1);
			BYTE *pCardIndexArray_1 = stData.getCardIndexArrayInIndex(cbCardIndex_1);
			// 取得牌信息
			BYTE cbCardIndex_2 = stData.getCardIndex(0x17);
			BYTE cbCardCount_2 = stData.getCardIndexCountInIndex(cbCardIndex_2);
			BYTE *pCardIndexArray_2 = stData.getCardIndexArrayInIndex(cbCardIndex_2);
			// 取得牌信息
			BYTE cbCardIndex_3 = stData.getCardIndex(0x1A);
			BYTE cbCardCount_3 = stData.getCardIndexCountInIndex(cbCardIndex_3);
			BYTE *pCardIndexArray_3 = stData.getCardIndexArrayInIndex(cbCardIndex_3);
			if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_2 > 0 && cbCardCount_2 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
			{
				stHuKeyData sKeyData(6, pCardIndexArray_1[0], pCardIndexArray_2[0], pCardIndexArray_3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}

			// 取得王牌信息
			BYTE cbWangCardIndex = stData.getCardIndex(KING_VALUE);
			BYTE cbWangCardCount = stData.getCardIndexCountInIndex(cbWangCardIndex);
			BYTE *pWangCardIndexArray = stData.getCardIndexArrayInIndex(cbWangCardIndex);
			if (cbWangCardCount >= 1)
			{
				if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_2 > 0 && cbCardCount_2 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_1[0], pCardIndexArray_2[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_1[0], pCardIndexArray_3[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (cbCardCount_2 > 0 && cbCardCount_2 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_2[0], pCardIndexArray_3[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
		}

		//【2.5】 2 7 10
		{
			// 取得牌信息
			BYTE cbCardIndex_1 = stData.getCardIndex(0x02);
			BYTE cbCardCount_1 = stData.getCardIndexCountInIndex(cbCardIndex_1);
			BYTE *pCardIndexArray_1 = stData.getCardIndexArrayInIndex(cbCardIndex_1);
			// 取得牌信息
			BYTE cbCardIndex_2 = stData.getCardIndex(0x07);
			BYTE cbCardCount_2 = stData.getCardIndexCountInIndex(cbCardIndex_2);
			BYTE *pCardIndexArray_2 = stData.getCardIndexArrayInIndex(cbCardIndex_2);
			// 取得牌信息
			BYTE cbCardIndex_3 = stData.getCardIndex(0x0A);
			BYTE cbCardCount_3 = stData.getCardIndexCountInIndex(cbCardIndex_3);
			BYTE *pCardIndexArray_3 = stData.getCardIndexArrayInIndex(cbCardIndex_3);
			if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_2 > 0 && cbCardCount_2 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
			{
				stHuKeyData sKeyData(3, pCardIndexArray_1[0], pCardIndexArray_2[0], pCardIndexArray_3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}

			// 取得王牌信息
			BYTE cbWangCardIndex = stData.getCardIndex(KING_VALUE);
			BYTE cbWangCardCount = stData.getCardIndexCountInIndex(cbWangCardIndex);
			BYTE *pWangCardIndexArray = stData.getCardIndexArrayInIndex(cbWangCardIndex);
			if (cbWangCardCount >= 1)
			{
				if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_2 > 0 && cbCardCount_2 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_1[0], pCardIndexArray_2[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (cbCardCount_1 > 0 && cbCardCount_1 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_1[0], pCardIndexArray_3[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (cbCardCount_2 > 0 && cbCardCount_2 < 3 && cbCardCount_3 > 0 && cbCardCount_3 < 3)
				{
					stHuKeyData sKeyData(6, pWangCardIndexArray[0], pCardIndexArray_2[0], pCardIndexArray_3[0]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
		}

		int nMaxHuPer = resultMax.getHuScore();
		int nAnswerHuPer = answerData.getHuScore(&stData);
		// 小于0是胡息不够的清空，0是够胡息牌还要继续组合
		if (nAnswerHuPer < 0)
		{
			return;
		}
		if (nMaxHuPer != 0 && nAnswerHuPer <= nMaxHuPer)
		{
			return;
		}

		map<int, stHuKeyData> mapHuKeyVal;
		map<int, stHuKeyData> mapHuKeyVal0;
		int nAll = nLeftIndexCount;

		for (int i = 0; i < nAll; ++i)
		{
			for (int j = i + 1; j < nAll; ++j)
			{
				for (int k = j + 1; k < nAll; ++k)
				{
					// 取得第一张牌信息
					BYTE cbCardValue_0 = stData.getCardValue(pLeftIndex[i]);
					BYTE cbCardIndex_0 = stData.getCardIndex(cbCardValue_0);
					BYTE cbCardCount_0 = stData.getCardIndexCountInIndex(cbCardIndex_0);
					if (cbCardCount_0 >= 3)
					{
						continue;
					}

					// 取得第二张牌信息
					BYTE cbCardValue_1 = stData.getCardValue(pLeftIndex[j]);
					BYTE cbCardIndex_1 = stData.getCardIndex(cbCardValue_1);
					BYTE cbCardCount_1 = stData.getCardIndexCountInIndex(cbCardIndex_1);
					if (cbCardCount_1 >= 3)
					{
						continue;
					}

					// 取得第三张牌信息
					BYTE cbCardValue_2 = stData.getCardValue(pLeftIndex[k]);
					BYTE cbCardIndex_2 = stData.getCardIndex(cbCardValue_2);
					BYTE cbCardCount_2 = stData.getCardIndexCountInIndex(cbCardIndex_2);
					if (cbCardCount_2 >= 3)
					{
						continue;
					}

					int nKey = getKey(cbCardValue_0, cbCardValue_1, cbCardValue_2, 0x00);
					int nHuKey = getCardHuXiVal(nKey);
					if (nHuKey > 0)
					{
						mapHuKeyVal[nKey] = stHuKeyData(nHuKey, pLeftIndex[i], pLeftIndex[j], pLeftIndex[k]);
					}
					else if (nHuKey == 0)
					{
						mapHuKeyVal0[nKey] = stHuKeyData(0, pLeftIndex[i], pLeftIndex[j], pLeftIndex[k]);
					}
				}
			}
		}

		map<int, stHuKeyData>::iterator iter = mapHuKeyVal0.begin();
		int nMaxHuVal = resultMax.getHuScore();
		for (; iter != mapHuKeyVal0.end(); ++iter)
		{
			answerData.push(iter->second, &stData);
			getCardHuResult(stData, answerData, resultMax);
			answerData.pop(&stData);
			if (answerData.getHuScore() > nMaxHuVal)
			{
				break;
			}
		}
	}
}

// 初始化胡牌
bool CHuPaiLogic::initCardHu(vector<BYTE> &vctCardIn, stAnswer &resultAnswer, tagHuCardInfo &HuCardInfo)
{
	size_t nSizeCard = vctCardIn.size();

	memcpy(&m_HuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));

	stCardData sCardData;
	stAnswer sAnswer;
	sAnswer.setHuFinshInfo(HuCardInfo.HuFinish);
	map<BYTE, vector<BYTE> >	mapKeyCount;

	// 统计牌
	for (size_t i = 0; i<nSizeCard; ++i)
	{
		BYTE cbCardData = vctCardIn[i];
		sCardData.push(cbCardData);
		mapKeyCount[cbCardData].push_back(i);
	}

	// 优先拿出4个的(不拿出3个的，3个可以与王组合)
	map<BYTE, vector<BYTE> >::iterator iter = mapKeyCount.begin();
	for (; iter != mapKeyCount.end(); ++iter)
	{
		vector<BYTE> &vctIndex = iter->second;
		if (vctIndex.size() == 4)
		{
			int nHuKey = getCardHuXiVal(sCardData.getCardValue(vctIndex[0]), sCardData.getCardValue(vctIndex[1]), sCardData.getCardValue(vctIndex[2]), sCardData.getCardValue(vctIndex[3]));
			sAnswer.push(stHuKeyData(nHuKey, vctIndex[0], vctIndex[1], vctIndex[2], vctIndex[3]), &sCardData);
		}
	}

	// 最后摸的，如果别人摸的可以组顺子，自己摸可能会王牌组钓，闯，炸，所以后面再加入
	if (m_HuCardInfo.cbInsertCard != 0)
	{
		vctCardIn.push_back(m_HuCardInfo.cbInsertCard);
		sCardData.push(m_HuCardInfo.cbInsertCard);
		mapKeyCount[m_HuCardInfo.cbInsertCard].push_back(nSizeCard);
	}

	m_bOnlyHu = false;
	getCardHuResult(sCardData, sAnswer, resultAnswer);
	// 拷贝回去
	memcpy(&HuCardInfo, &m_HuCardInfo, sizeof(tagHuCardInfo));

	return resultAnswer.getHuScore() > 0;
}