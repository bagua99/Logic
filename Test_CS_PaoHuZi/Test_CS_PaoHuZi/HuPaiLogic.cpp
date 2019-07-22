#include "HuPaiLogic.h"

CHuPaiLogic::CHuPaiLogic()
{
	m_mapKey.clear();
	m_bHuError = false;
	m_bSelfAddCard = false;
	m_cbInsertCard = 0;
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

// ȡ�ú�Ϣ
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

// ȡ�ú�Ϣ
int CHuPaiLogic::getCardHuXiVal(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4)
{
	// �Ƽ�ֵ
	int nKey = getKey(cbCard1, cbCard2, cbCard3, cbCard4);

	return getCardHuXiVal(nKey);
}

// �Ƿ�ӵ�й���
bool CHuPaiLogic::isHavePlayType(enPlayType nType)
{
	if (nType >= enPlayType_Max)
	{
		return false;
	}

	return (PLAY_TYPE & (1 << nType)) != 0;
}

// ��Ϣ��ֵ
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

// ���Ӻ�Ϣ��
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

// ȡ�����б�
void CHuPaiLogic::getTiList()
{
	for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
	{
		BYTE cbBigVal = cbVal + 0x10;
		addMapVal(getKey(cbVal, cbVal, cbVal, cbVal), TYPE_X_TI_HU_XI);
		addMapVal(getKey(cbBigVal, cbBigVal, cbBigVal, cbBigVal), TYPE_D_TI_HU_XI);
	}
}

// ȡ�����б�
void CHuPaiLogic::getWeiList()
{
	for (BYTE cbVal = 1; cbVal <= 10; ++cbVal)
	{
		BYTE cbBigVal = cbVal + 0x10;
		addMapVal(getKey(cbVal, cbVal, cbVal, 0), TYPE_X_WEI_HU_XI);
		addMapVal(getKey(cbBigVal, cbBigVal, cbBigVal, 0), TYPE_D_WEI_HU_XI);
		if (isHavePlayType(enPlayType_XDD))
		{
			addMapVal(getKey(cbBigVal, cbVal, cbVal, 0), 0);
			addMapVal(getKey(cbBigVal, cbBigVal, cbVal, 0), 0);
		}
	}
}

// ȡ��27A�б�
void CHuPaiLogic::get27AList()
{
	if (isHavePlayType(enPlayType_27A))
	{
		addMapVal(getKey(0x0A, 0x07, 0x02, 0x00), TYPE_X_27A_HU_XI);
		addMapVal(getKey(0x1A, 0x17, 0x12, 0x00), TYPE_D_27A_HU_XI);
	}
}

// ȡ��˳���б�
void CHuPaiLogic::getShunZhiList()
{
	for (BYTE cbVal = 1; cbVal <= 8; ++cbVal)
	{
		BYTE nHuXi = 0;
		if (isHavePlayType(enPlayType_123))
			nHuXi = (cbVal == 1) ? 1 : 0;

		addMapVal(getKey(cbVal, cbVal + 1, cbVal + 2, 0x00), nHuXi * TYPE_X_123_HU_XI);
		addMapVal(getKey(0x10 + cbVal, 0x10 + cbVal + 1, 0x10 + cbVal + 2, 0x00), nHuXi * TYPE_D_123_HU_XI);
	}
}

// ��ȡ���ƽ��
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
		m_bHuError = true;
	}
	else if (nLeft == 3)
	{
		int nHuKey = getCardHuXiVal(stData.getVal(vctLeftIndex[0]), stData.getVal(vctLeftIndex[1]), stData.getVal(vctLeftIndex[2]));
		if (nHuKey >= 0)
		{
			stAnswer answerTemp = answerData;
			answerTemp.push(stHuKeyData(nHuKey, vctLeftIndex[0], vctLeftIndex[1], vctLeftIndex[2]));
			int nMaxHuPer = resultMax.getHuPer();
			int nAnswerHuPer = answerData.getHuPer();
			if (nMaxHuPer == 0 || nMaxHuPer < nAnswerHuPer)
			{
				resultMax = answerTemp;
			}
		}
	}
	else if (nLeft == 2)
	{
		BYTE cbCard1 = stData.getVal(vctLeftIndex[0]);
		BYTE cbCard2 = stData.getVal(vctLeftIndex[1]);
		// ���Ժ�
		if (cbCard1 == cbCard2 || cbCard1 == 0xFF || cbCard2 == 0xFF)
		{
			int nMaxHuPer = resultMax.getHuPer();
			int nAnswerHuPer = answerData.getHuPer();
			if (nMaxHuPer == 0 || nMaxHuPer < nAnswerHuPer)
			{
				resultMax = answerData;
				resultMax.push(stHuKeyData(0, vctLeftIndex[0], vctLeftIndex[1]));
			}
		}
	}
	else	// ���ж��
	{
		//��2.1�� �ж��������Ƿ��������
		{
			// �����Լ����ƣ�Ҫ�ж��£�������Ƿ���3������3��Ҫ�����������
			if (!m_bSelfAddCard && m_cbInsertCard != 0)
			{
				vector<char> vctTemp1 = stData.getcardsNumByVal(m_cbInsertCard);
				if (vctTemp1.size() >= 3)
				{
					BYTE cbHuxi = (m_cbInsertCard & 0xF0) > 0 ? TYPE_D_PENG_HU_XI : TYPE_X_PENG_HU_XI;
					stHuKeyData sKeyData(cbHuxi, vctTemp1[0], vctTemp1[1], vctTemp1[2]);
					answerData.push(sKeyData, &stData);
					getCardHuResult(stData, answerData, resultMax);
					answerData.pop(&stData);
				}
			}
		}

		//��2.2�� һ����
		{
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x11);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x12);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x13);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				stHuKeyData sKeyData(6, vctTemp1[0], vctTemp2[0], vctTemp3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}
		}

		//��2.3�� 1 2 3
		{
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x01);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x02);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x03);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				stHuKeyData sKeyData(3, vctTemp1[0], vctTemp2[0], vctTemp3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}
		}

		//��2.4�� ����ʮ
		{
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x12);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x17);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x1A);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				stHuKeyData sKeyData(6, vctTemp1[0], vctTemp2[0], vctTemp3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
			}
		}

		//��2.5�� 2 7 10
		{
			vector<char> vctTemp1 = stData.getcardsNumByVal(0x02);
			vector<char> vctTemp2 = stData.getcardsNumByVal(0x07);
			vector<char> vctTemp3 = stData.getcardsNumByVal(0x0A);
			if (vctTemp1.size() > 0 && vctTemp2.size() > 0 && vctTemp3.size() > 0)
			{
				stHuKeyData sKeyData(3, vctTemp1[0], vctTemp2[0], vctTemp3[0]);
				answerData.push(sKeyData, &stData);
				getCardHuResult(stData, answerData, resultMax);
				answerData.pop(&stData);
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
		int nHuVal = resultMax.getHuPer();
		for (; iter != mapHuKeyVal0.end(); ++iter)
		{
			answerData.push(iter->second, &stData);
			getCardHuResult(stData, answerData, resultMax);
			answerData.pop(&stData);
			if (resultMax.getHuPer() > nHuVal)
			{
				break;
			}
			if (m_bHuError)
			{
				break;
			}
		}
	}
}

// ��ʼ������
bool CHuPaiLogic::initCardHu(vector<BYTE> &vctCardIn, stAnswer &resultAnswer, BYTE cbInsertCard, bool bSelfAddCard)
{
	size_t nSizeCard = vctCardIn.size();
	if (nSizeCard == 0)
	{
		return false;
	}

	stCardData sCardData;
	stAnswer sAnswer;
	map<char, vector<char> >	mapKeyCount;

	// ͳ����
	for (size_t i = 0; i<nSizeCard; ++i)
	{
		BYTE cbCardData = vctCardIn[i];
		sCardData.push(cbCardData);
		mapKeyCount[cbCardData].push_back(i);
	}

	// �Լ����ƣ�ֱ�Ӳ���
	if (bSelfAddCard && cbInsertCard != 0)
	{
		vctCardIn.push_back(cbInsertCard);
		sCardData.push(cbInsertCard);
		mapKeyCount[cbInsertCard].push_back(nSizeCard);
	}

	// �����ó�4,3����
	map<char, vector<char> >::iterator iter = mapKeyCount.begin();
	for (; iter != mapKeyCount.end(); ++iter)
	{
		vector<char> &vctIndex = iter->second;
		if (vctIndex.size() == 4)
		{
			int nHuKey = getCardHuXiVal(sCardData.getVal(vctIndex[0]), sCardData.getVal(vctIndex[1]), sCardData.getVal(vctIndex[2]), sCardData.getVal(vctIndex[3]));
			sAnswer.push(stHuKeyData(nHuKey, vctIndex[0], vctIndex[1], vctIndex[2], vctIndex[3]), &sCardData);
		}
		else if (vctIndex.size() == 3)
		{
			int nHuKey = getCardHuXiVal(sCardData.getVal(vctIndex[0]), sCardData.getVal(vctIndex[1]), sCardData.getVal(vctIndex[2]));
			sAnswer.push(stHuKeyData(nHuKey, vctIndex[0], vctIndex[1], vctIndex[2], -1, false), &sCardData);
		}
	}

	// �����Լ������ƣ������ٲ���(��Ϊ����ƿ��Բ��)
	if (!bSelfAddCard && cbInsertCard != 0)
	{
		vctCardIn.push_back(cbInsertCard);
		sCardData.push(cbInsertCard);
	}

	m_bHuError = false;
	m_bSelfAddCard = bSelfAddCard;
	m_cbInsertCard = cbInsertCard;
	getCardHuResult(sCardData, sAnswer, resultAnswer);

	return resultAnswer.vctData.size() > 0;
}