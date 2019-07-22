#include "HuPaiLogic.h"

CHuPaiLogic::CHuPaiLogic()
{
	m_mapKey.clear();
	m_bSelfAddCard = false;
	m_cbInsertCard = 0;
	m_cbMenCount = 0;
}

CHuPaiLogic::~CHuPaiLogic()
{
}

// ��ʼ���ƺ�Ϣ
void CHuPaiLogic::initCardHuXi()
{
	//������ʱ  ֻ��һ����� 
	//	1.������ͬ�Ļ����	
	//������ʱ 
	//	1.����һ���Ļ����
	//  2.����ʮ
	//  3.��С��
	//	4.����
	if (m_mapKey.empty())
	{
		getTiList();
		getWeiList();
		get27AList();
		getShunZhiList();
	}
}

int CHuPaiLogic::getCardHuXiVal(int nKey)
{
	//������ʱ  ֻ��һ����� 
	//	1.������ͬ�Ļ����	
	//������ʱ 
	//	1.����һ���Ļ����
	//  2.����ʮ
	//  3.��С��
	//	4.����
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

int CHuPaiLogic::getCardHuXiVal(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4)
{
	// �Ƽ�ֵ
	int nKey = getKey(cbCard1, cbCard2, cbCard3, cbCard4);

	return getCardHuXiVal(nKey);
}

bool CHuPaiLogic::isHavePlayType(enPlayType nType)
{
	if (nType >= enPlayType_Max)
	{
		return false;
	}

	return (TY_MY & (1 << nType)) != 0;
}

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

void CHuPaiLogic::getTiList()
{
	for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
	{
		BYTE cbBigVal = cbVal + 0x10;
		addMapVal(getKey(cbVal, cbVal, cbVal, cbVal), 5);
		addMapVal(getKey(cbBigVal, cbBigVal, cbBigVal, cbBigVal), 5);
	}
}

void CHuPaiLogic::getWeiList()
{
	for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
	{
		BYTE cbBigVal = cbVal + 0x10;
		addMapVal(getKey(cbVal, cbVal, cbVal, 0), 3);
		addMapVal(getKey(cbBigVal, cbBigVal, cbBigVal, 0), 3);
		if (isHavePlayType(enPlayType_XDD))
		{
			addMapVal(getKey(cbBigVal, cbVal, cbVal, 0), 0);
			addMapVal(getKey(cbBigVal, cbBigVal, cbVal, 0), 0);
		}
	}
}

void CHuPaiLogic::get27AList()
{
	if (isHavePlayType(enPlayType_27A))
	{
		addMapVal(getKey(0x0A, 0x07, 0x02, 0x00), 1);
		addMapVal(getKey(0x1A, 0x17, 0x12, 0x00), 1);
	}
}

void CHuPaiLogic::getShunZhiList()
{
	for (BYTE cbVal = 1; cbVal <= 8; ++cbVal)
	{
		BYTE nHuXi = 0;
		if (isHavePlayType(enPlayType_123))
			nHuXi = (cbVal == 1) ? 1 : 0;

		addMapVal(getKey(cbVal, cbVal + 1, cbVal + 2, 0x00), nHuXi);
		addMapVal(getKey(0x10 + cbVal, 0x10 + cbVal + 1, 0x10 + cbVal + 2, 0x00), nHuXi * 2);
	}
}

void CHuPaiLogic::getCardHuResult(stCardData stData, stAnswer &answerData, stAnswer &resultMax)
{
	// 1.Ԥ����	
	//vector<stHuKeyData> &vctHuKeyData = answerData.vctData;	

	// 2.
	vector<char> vctLeftIndex = stData.getLeftIndex();

	// 3.�߼�
	int nLeft = vctLeftIndex.size();
	if (nLeft == 0)
	{
		int nMaxHuPer = resultMax.getHuPer();
		int nAnswerHuPer = answerData.getHuPer();
		if (nMaxHuPer == 0 || nMaxHuPer < nAnswerHuPer)
		{
			resultMax = answerData;
		}
	}
	else if (nLeft <= 1)
	{
	}
	// ���ﲻ���������жϣ������Լ����һ�Դ�½���������ģ����������ɿ���������������߼�
	/*else if (nLeft == 3)
	{
	int nHuKey = getCardHuXiVal(stData.getVal(vctLeftIndex[0]), stData.getVal(vctLeftIndex[1]), stData.getVal(vctLeftIndex[2]));
	if (nHuKey >= 0)
	{
	answerData.push(stHuKeyData(nHuKey, vctLeftIndex[0], vctLeftIndex[1], vctLeftIndex[2]));
	int nMaxHuPer = resultMax.getHuPer();
	int nAnswerHuPer = answerData.getHuPer();
	if (nMaxHuPer == 0 || nMaxHuPer < nAnswerHuPer)
	{
	resultMax = answerData;
	}
	answerData.pop(&stData);
	}
	}*/
	else if (nLeft == 2)
	{
		// �Ѿ����Ż��߽���
		if (m_cbMenCount > 0)
		{
			return;
		}

		BYTE cbCard1 = stData.getVal(vctLeftIndex[0]);
		BYTE cbCard2 = stData.getVal(vctLeftIndex[1]);
		bool bFindHu = false;
		do
		{
			if (cbCard1 == cbCard2)
			{
				bFindHu = true;
				break;
			}

			// ��ͬ��ɫ
			if ((cbCard1&MASK_COLOR) != (cbCard2&MASK_COLOR))
			{
				break;
			}

			// ȡ����ֵ
			BYTE cbCardValue1 = cbCard1&MASK_VALUE;
			// ȡ����ֵ
			BYTE cbCardValue2 = cbCard2&MASK_VALUE;
			// ��2��7��10
			if (cbCardValue1 == 0x02 || cbCardValue1 == 0x07 || cbCardValue1 == 0x0A)
			{
				if (cbCardValue2 == 0x02 || cbCardValue2 == 0x07 || cbCardValue2 == 0x0A)
				{
					bFindHu = true;
					break;
				}
			}

			if (abs(cbCardValue1 - cbCardValue2) <= 2)
			{
				bFindHu = true;
				break;
			}
		} while (false);

		// ���Ժ�
		if (bFindHu)
		{
			// 2����,һ�Ժ�(1��Ϣ),һ�Ժ�(1��Ϣ),���ֺ�����(Ҳ��1Ϣ)
			BYTE cbHuXi = 0;

			// �����ټ�1Ϣ
			if (cbCard1 == cbCard2)
			{
				cbHuXi = 1;
			}
			else
			{
				// ��ȡ��ֵ
				BYTE cbValue1 = cbCard1&MASK_VALUE;
				BYTE cbValue2 = cbCard2&MASK_VALUE;

				// 2�����ּ�1Ϣ
				if (cbValue1 == 0x02 || cbValue1 == 0x07 || cbValue1 == 0x0A)
				{
					if (cbValue2 == 0x02 || cbValue2 == 0x07 || cbValue2 == 0x0A)
					{
						cbHuXi = 1;
					}
				}
			}
			answerData.push(stHuKeyData(cbHuXi, vctLeftIndex[0], vctLeftIndex[1]));
			int nMaxHuPer = resultMax.getHuPer();
			int nAnswerHuPer = answerData.getHuPer();
			if (nMaxHuPer == 0 || nMaxHuPer < nAnswerHuPer)
			{
				resultMax = answerData;
			}
			answerData.pop(&stData);
		}
	}
	else	// ���ж��
	{
		{	//��2.1�� ��
			for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
			{
				BYTE cbCard = cbVal;
				vector<char> vctTemp1 = stData.getcardsNumByVal(cbCard);
				// �Լ����ƣ����ǲ����ƣ���4�Ų�����
				if (vctTemp1.size() >= 4 && (m_bSelfAddCard || m_cbInsertCard != cbCard))
				{
					stHuKeyData sKeyData(5, vctTemp1[0], vctTemp1[1], vctTemp1[2], vctTemp1[3]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				cbCard = cbVal + 0x10;
				vector<char> vctTemp2 = stData.getcardsNumByVal(cbCard);
				// �Լ����ƣ����ǲ����ƣ���4�Ų�����
				if (vctTemp2.size() >= 4 && (m_bSelfAddCard || m_cbInsertCard != cbCard))
				{
					stHuKeyData sKeyData(5, vctTemp2[0], vctTemp2[1], vctTemp2[2], vctTemp2[3]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
		}

		{	//��2.2�� ��
			for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
			{
				BYTE cbCard = cbVal;
				vector<char> vctTemp1 = stData.getcardsNumByVal(cbCard);
				// ��4�ţ������Լ����ģ������ǲ���������ƣ����Գ��������
				if (vctTemp1.size() == 4 && !m_bSelfAddCard && m_cbInsertCard == cbCard)
				{
					stHuKeyData sKeyData(3, vctTemp1[0], vctTemp1[1], vctTemp1[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (vctTemp1.size() >= 3)
				{
					// ֻ��3�ţ������Լ����ģ������ǲ���������ƣ�ֻ�������������㿲
					if (vctTemp1.size() == 3 && !m_bSelfAddCard && m_cbInsertCard == cbCard)
					{
						stHuKeyData sKeyData(1, vctTemp1[0], vctTemp1[1], vctTemp1[2]);
						answerData.push(sKeyData, &stData);
						getCardHuResult(stData, answerData, resultMax);
						answerData.pop(&stData);
					}
					else
					{
						stHuKeyData sKeyData(3, vctTemp1[0], vctTemp1[1], vctTemp1[2]);
						answerData.push(sKeyData, &stData);
						getCardHuResult(stData, answerData, resultMax);
						answerData.pop(&stData);
					}
				}

				cbCard = cbVal + 0x10;
				vector<char> vctTemp2 = stData.getcardsNumByVal(cbCard);
				// ��4�ţ������Լ����ģ������ǲ���������ƣ����Գ��������
				if (vctTemp2.size() == 4 && !m_bSelfAddCard && m_cbInsertCard == cbCard)
				{
					stHuKeyData sKeyData(3, vctTemp2[0], vctTemp2[1], vctTemp2[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}

				if (vctTemp2.size() >= 3)
				{
					// ֻ��3�ţ������Լ����ģ������ǲ���������ƣ�ֻ�������������㿲
					if (vctTemp2.size() == 3 && !m_bSelfAddCard && m_cbInsertCard == cbCard)
					{
						stHuKeyData sKeyData(1, vctTemp2[0], vctTemp2[1], vctTemp2[2]);
						answerData.push(sKeyData, &stData);
						getCardHuResult(stData, answerData, resultMax);
						answerData.pop(&stData);
					}
					else
					{
						stHuKeyData sKeyData(3, vctTemp2[0], vctTemp2[1], vctTemp2[2]);
						answerData.push(sKeyData, &stData);
						getCardHuResult(stData, answerData, resultMax);
						answerData.pop(&stData);
					}
				}
			}
		}

		{	//��2.3�� ����ʮ
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x12);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x17);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x1A);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				stHuKeyData sKeyData(1, vctTemp1[0], vctTemp2[0], vctTemp3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}
		}

		{	//��2.4�� 2 7 10
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x02);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x07);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x0A);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				stHuKeyData sKeyData(1, vctTemp1[0], vctTemp2[0], vctTemp3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}
		}

		// û�н�����
		if (m_cbMenCount <= 0)
		{	//��2.5�� ��(1��Ϣ)
			for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
			{
				BYTE cbCard = cbVal;
				vector<char> vctTemp1 = stData.getcardsNumByVal(cbCard);
				if (vctTemp1.size() == 2)
				{
					++m_cbMenCount;
					stHuKeyData sKeyData(1, vctTemp1[0], vctTemp1[1]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
					--m_cbMenCount;
				}

				cbCard = cbVal + 0x10;
				vector<char> vctTemp2 = stData.getcardsNumByVal(cbCard);
				if (vctTemp2.size() == 2)
				{
					++m_cbMenCount;
					stHuKeyData sKeyData(1, vctTemp2[0], vctTemp2[1]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
					--m_cbMenCount;
				}
			}
		}

		// û�н�����
		if (m_cbMenCount <= 0)
		{	//��2.6����(2������1��Ϣ)
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x02);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x07);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x0A);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0)
			{
				++m_cbMenCount;
				stHuKeyData sKeyData(1, vctTemp1[0], vctTemp2[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
				--m_cbMenCount;
			}

			if (vctTemp1.size() > 0 && vctTemp3.size() > 0)
			{
				++m_cbMenCount;
				stHuKeyData sKeyData(1, vctTemp1[0], vctTemp3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
				--m_cbMenCount;
			}

			if (vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				++m_cbMenCount;
				stHuKeyData sKeyData(1, vctTemp2[0], vctTemp3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
				--m_cbMenCount;
			}

			vctTemp1 = stData.getcardsNumByVal(0x12);
			vctTemp2 = stData.getcardsNumByVal(0x17);
			vctTemp3 = stData.getcardsNumByVal(0x1A);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0)
			{
				++m_cbMenCount;
				stHuKeyData sKeyData(1, vctTemp1[0], vctTemp2[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
				--m_cbMenCount;
			}

			if (vctTemp1.size() > 0 && vctTemp3.size() > 0)
			{
				++m_cbMenCount;
				stHuKeyData sKeyData(1, vctTemp1[0], vctTemp3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
				--m_cbMenCount;
			}

			if (vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				++m_cbMenCount;
				stHuKeyData sKeyData(1, vctTemp2[0], vctTemp3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
				--m_cbMenCount;
			}
		}

		int nMaxHuPer = resultMax.getHuPer();
		int nAnswerHuPer = answerData.getHuPer();
		if (nMaxHuPer != 0 && nAnswerHuPer <= nMaxHuPer)
		{
			return;
		}

		map<int, stHuKeyData> mapHuKeyVal;
		map<int, stHuKeyData> mapHuKeyVal0;
		int nAll = vctLeftIndex.size();

		for (int i = 0; i < nAll; ++i)
		{
			for (int j = i + 1; j < nAll; ++j)
			{
				for (int k = j + 1; k < nAll; ++k)
				{
					int key = getKey(stData.getVal(vctLeftIndex[i]), stData.getVal(vctLeftIndex[j]), stData.getVal(vctLeftIndex[k]), 0x00);
					int nHuKey = getCardHuXiVal(key);
					if (nHuKey > 0)
					{
						mapHuKeyVal[key] = stHuKeyData(nHuKey, vctLeftIndex[i], vctLeftIndex[j], vctLeftIndex[k]);
					}
					else if (nHuKey == 0)
					{
						mapHuKeyVal0[key] = stHuKeyData(0, vctLeftIndex[i], vctLeftIndex[j], vctLeftIndex[k]);
					}
				}
			}
		}

		map<int, stHuKeyData>::iterator iter = mapHuKeyVal0.begin();
		int nMaxHuVal = resultMax.getHuPer();
		for (; iter != mapHuKeyVal0.end(); ++iter)
		{
			answerData.push(iter->second, &stData);
			getCardHuResult(stData, answerData, resultMax);
			answerData.pop(&stData);
			if (answerData.getHuPer() > nMaxHuVal)
			{
				break;
			}
		}
	}
}

bool CHuPaiLogic::initCardHu(vector<BYTE> &vctCardIn, stAnswer &resultAnswer, BYTE cbInsertCard, bool bSelfAddCard)
{
	size_t nSizeCard = vctCardIn.size();
	if (nSizeCard == 0)
	{
		return false;
	}

	stCardData sCardData;
	stAnswer sAnswer;

	// ͳ����
	for (size_t i = 0; i<nSizeCard; ++i)
	{
		sCardData.push(vctCardIn[i]);
	}

	m_bSelfAddCard = bSelfAddCard;
	m_cbInsertCard = cbInsertCard;
	m_cbMenCount = 0;
	getCardHuResult(sCardData, sAnswer, resultAnswer);

	return resultAnswer.vctData.size() > 0;
}