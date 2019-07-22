#include "GameLogic.h"
#include <stdlib.h>
#include <assert.h>
#include <algorithm>

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE const CGameLogic::m_cbCardDataArray[MAX_REPERTORY] =
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//Сд
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//Сд
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//Сд
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//Сд
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//��д
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//��д
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//��д
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//��д
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	m_HuPaiLogic.initCardHuXi();
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE bMaxCount)
{
	//����׼��
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)] = { 0 };
	memcpy(cbCardDataTemp, m_cbCardDataArray, sizeof(m_cbCardDataArray));

	//�����˿�
	BYTE bRandCount = 0, bPosition = 0;
	do
	{
		bPosition = rand() % (bMaxCount - bRandCount);
		cbCardData[bRandCount++] = cbCardDataTemp[bPosition];
		cbCardDataTemp[bPosition] = cbCardDataTemp[bMaxCount - bRandCount];
	} while (bRandCount < bMaxCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//Ч���˿�
	if (!IsValidCard(cbRemoveCard))
	{
		return false;
	}
	if (cbCardIndex[SwitchToCardIndex(cbRemoveCard)] <= 0)
	{
		return false;
	}

	//ɾ���˿�
	BYTE cbRemoveIndex = SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex] > 0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	return false;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE bRemoveCount)
{
	//ɾ���˿�
	for (BYTE i = 0; i<bRemoveCount; i++)
	{
		//Ч���˿�
		if (!IsValidCard(cbRemoveCard[i]))
		{
			continue;
		}

		BYTE cbRemoveIndex = SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex] <= 0)
		{
			continue;
		}

		//ɾ���˿�
		if (cbCardIndex[cbRemoveIndex] == 0)
		{
			//��ԭɾ��
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
			//ɾ���˿�
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE bCardCount, BYTE cbRemoveCard[], BYTE bRemoveCount)
{
	//��������
	if (bCardCount > MAX_COUNT)
	{
		return false;
	}
	if (bRemoveCount > bCardCount)
	{
		return false;
	}

	//�������
	BYTE bDeleteCount = 0;
	BYTE cbTempCardData[MAX_COUNT] = { 0 };
	if (bCardCount > CountArray(cbTempCardData))
	{
		return false;
	}
	memcpy(cbTempCardData, cbCardData, bCardCount * sizeof(cbCardData[0]));

	//�����˿�
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

	//�ɹ��ж�
	if (bDeleteCount != bRemoveCount)
	{
		return false;
	}

	//�����˿�
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

//�����ж�
BYTE CGameLogic::GetAcitonTiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbTiCardIndex[5])
{
	//��������
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

//η���ж�
BYTE CGameLogic::GetActionWeiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbWeiCardIndex[7])
{
	//η������
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

//�����ж�
BYTE CGameLogic::GetActionChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, tagChiCardInfo ChiCardInfo[6])
{
	//Ч���˿�
	if (cbCurrentCard == 0)
	{
		return 0;
	}

	//��������
	BYTE cbChiCardCount = 0;
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//����ʮ��
	BYTE bCardValue = cbCurrentCard&MASK_VALUE;
	if (bCardValue == 0x02 || bCardValue == 0x07 || bCardValue == 0x0A)
	{
		//��������
		BYTE cbExcursion[] = { 1, 6, 9 };
		BYTE cbInceptIndex = ((cbCurrentCard&MASK_COLOR) == 0x00) ? 0 : 10;

		//�����ж�
		BYTE i = 0;
		for (i = 0; i<CountArray(cbExcursion); i++)
		{
			BYTE cbIndex = cbInceptIndex + cbExcursion[i];
			if (cbIndex != cbCurrentIndex && (cbCardIndex[cbIndex] == 0 || cbCardIndex[cbIndex] == 3))
			{
				break;
			}
		}

		//��ȡ�ж�
		if (i == CountArray(cbExcursion))
		{
			//�����˿�
			BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

			//ɾ���˿�
			for (BYTE j = 0; j<CountArray(cbExcursion); j++)
			{
				BYTE cbIndex = cbInceptIndex + cbExcursion[j];
				if (cbIndex != cbCurrentIndex)
				{
					cbCardIndexTemp[cbIndex]--;
				}
			}

			//��ȡ�ж�
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

			//���ý��
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

	//˳������
	BYTE cbExcursion[3] = { 0, 1, 2 };
	for (BYTE i = 0; i<CountArray(cbExcursion); i++)
	{
		BYTE cbValueIndex = cbCurrentIndex % 10;
		if (cbValueIndex >= cbExcursion[i] && (cbValueIndex - cbExcursion[i]) <= 7)
		{
			//��������
			BYTE cbFirstIndex = cbCurrentIndex - cbExcursion[i];

			//�����ж�
			BYTE j = 0;
			for (; j<3; j++)
			{
				BYTE cbIndex = cbFirstIndex + j;
				if (cbIndex != cbCurrentIndex && (cbCardIndex[cbIndex] == 0 || cbCardIndex[cbIndex] == 3))
				{
					break;
				}
			}

			//��ȡ�ж�
			if (j == CountArray(cbExcursion))
			{
				//�����˿�
				BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
				memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

				//ɾ���˿�
				for (BYTE j = 0; j<3; j++)
				{
					BYTE cbIndex = cbFirstIndex + j;
					if (cbIndex != cbCurrentIndex)
					{
						cbCardIndexTemp[cbIndex]--;
					}
				}

				//��ȡ�ж�
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

				//���ý��
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

//�Ƿ����
bool CGameLogic::IsChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//Ч���˿�
	if (cbCurrentCard == 0)
	{
		return false;
	}

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//�����˿�
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	BYTE cbResult[3] = { 0 };
	return TakeOutChiCard(cbCardIndexTemp, cbCurrentCard, cbResult) != CK_NULL;
}

//�Ƿ�����
bool CGameLogic::IsTiPaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//Ч���˿�
	if (!IsValidCard(cbCurrentCard))
	{
		return false;
	}

	//ת������
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//�����ж�
	return (cbCardIndex[cbCurrentIndex] >= 3) ? true : false;
}

//�Ƿ�����
bool CGameLogic::IsWeiPengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//Ч���˿�
	if (!IsValidCard(cbCurrentCard))
	{
		return false;
	}

	//ת������
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//�����ж�
	return (cbCardIndex[cbCurrentIndex] >= 2) ? true : false;
}

//���ƽ��
bool CGameLogic::GetHuCardInfo(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbHuXiWeave, tagHuCardInfo &HuCardInfo)
{
	//�����˿�
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

	// ��Ҫ�ж������(ׯ��0��,1��,9��,10��;�м�0��,1��,9��),ֱ���ж�cbCardIndex�����͵�������,cbCurrentCard�϶�Ϊ0,��Ȼ�ʹ�����
	if (HuCardInfo.bSpecialHu)
	{
		// ��������
		vector<BYTE> vDanPai;
		// ��������
		vector<BYTE> vDuiPai;
		// ������
		int nCardPaiCount = 0;

		for (int i = 0; i<MAX_INDEX; ++i)
		{
			if (cbCardIndex[i] == 0)
			{
				continue;
			}

			// ת����
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

		// ��������
		int nDanPaiCount = vDanPai.size();
		// ��������
		int nDuiPaiCount = vDuiPai.size();
		// �޶�
		if (nDanPaiCount == nCardPaiCount)
		{
			// ��5����ϣ�ÿ�����4����
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

		// ʮ��
		if (nDuiPaiCount == 10)
		{
			// ��10����ϣ�ÿ�����2����
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

		// һ��
		if (nDuiPaiCount == 1)
		{
			// ��6����ϣ���1�����2���ƣ�4��4�ţ�1��2��
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

		// �Ŷ�
		if (nDuiPaiCount == 9)
		{
			// ��10����ϣ�ÿ�����2����
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

	// ֻ�ж������
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

	// ��Ϣ����
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
				// ����3�������ó���
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

		// �����������
		++nWeaveIndex;
	}
	HuCardInfo.cbWeaveCount = nWeaveIndex;

	return true;
}


//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue = (cbCardData&MASK_VALUE);
	BYTE cbColor = (cbCardData&MASK_COLOR) >> 4;
	return (cbValue >= 1 && cbValue <= 10 && cbColor <= 1);
}

//�˿���Ŀ
BYTE CGameLogic::GetCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//��Ŀͳ��
	BYTE cbCount = 0;
	for (BYTE i = 0; i<MAX_INDEX; i++)
	{
		cbCount += cbCardIndex[i];
	}

	return cbCount;
}

//��ȡ��Ϣ
BYTE CGameLogic::GetWeaveHuXi(const tagWeaveItem &WeaveItem)
{
	//�����Ϣ
	switch (WeaveItem.cbWeaveKind)
	{
	case ACK_LIU:	// ��
	{
		return 5;
	}
	case ACK_WAI:	// ��
	{
		return 4;
	}
	case ACK_KAN:	// ��
	{
		return 3;
	}
	case ACK_PENG:	// ��
	{
		return 1;
	}
	case ACK_CHI:	// ��
	{
		// ��ȡ��ֵ
		BYTE cbValue1 = WeaveItem.cbCardList[0] & MASK_VALUE;
		BYTE cbValue2 = WeaveItem.cbCardList[1] & MASK_VALUE;
		BYTE cbValue3 = WeaveItem.cbCardList[2] & MASK_VALUE;

		BYTE cbCardIndex[MAX_INDEX] = { 0 };
		// ��ת����С������
		cbCardIndex[cbValue1 - 1]++;
		cbCardIndex[cbValue2 - 1]++;
		cbCardIndex[cbValue3 - 1]++;

		//����ʮ��
		if (cbCardIndex[1] == 1 && cbCardIndex[6] == 1 && cbCardIndex[9] == 1)
		{
			return 1;
		}

		return 0;
	}
	case ACK_NULL:
	{
		// 2����,һ�Ժ�(1��Ϣ),һ�Ժ�(1��Ϣ),���ֺ�����(1��Ϣ)
		BYTE cbHuXi = 0;

		if (WeaveItem.cbCardCount == 2)
		{
			// ��ȡ��ֵ
			BYTE cbValue1 = WeaveItem.cbCardList[0] & MASK_VALUE;
			BYTE cbValue2 = WeaveItem.cbCardList[1] & MASK_VALUE;

			// �����ټ�1Ϣ
			if (WeaveItem.cbCardList[0] == WeaveItem.cbCardList[1])
			{
				cbHuXi = 1;
			}
			else
			{
				// 2�����ּ�1Ϣ
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

//��ȡ����
BYTE CGameLogic::TakeOutChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbResultCard[3])
{
	//Ч���˿�
	if (cbCurrentCard == 0)
	{
		return 0;
	}

	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//����ʮ��
	BYTE bCardValue = cbCurrentCard&MASK_VALUE;
	if (bCardValue == 0x02 || bCardValue == 0x07 || bCardValue == 0x0A)
	{
		//��������
		BYTE cbExcursion[] = { 1,6,9 };
		BYTE cbInceptIndex = ((cbCurrentCard&MASK_COLOR) == 0x00) ? 0 : 10;

		//�����ж�
		BYTE i = 0;
		for (i = 0; i<CountArray(cbExcursion); i++)
		{
			BYTE cbIndex = cbInceptIndex + cbExcursion[i];
			if (cbCardIndex[cbIndex] == 0)
			{
				break;
			}
		}

		//�ɹ��ж�
		if (i == CountArray(cbExcursion))
		{
			//ɾ���˿�
			cbCardIndex[cbInceptIndex + cbExcursion[0]]--;
			cbCardIndex[cbInceptIndex + cbExcursion[1]]--;
			cbCardIndex[cbInceptIndex + cbExcursion[2]]--;

			//���ý��
			cbResultCard[0] = SwitchToCardData(cbInceptIndex + cbExcursion[0]);
			cbResultCard[1] = SwitchToCardData(cbInceptIndex + cbExcursion[1]);
			cbResultCard[2] = SwitchToCardData(cbInceptIndex + cbExcursion[2]);

			return CK_EQS;
		}
	}

	BYTE cbFirstIndex = 0;
	//˳���ж�
	BYTE cbExcursion[3] = { 0,1,2 };
	for (BYTE i = 0; i<CountArray(cbExcursion); i++)
	{
		BYTE cbValueIndex = cbCurrentIndex % 10;
		if (cbValueIndex >= cbExcursion[i] && (cbValueIndex - cbExcursion[i]) <= 7)
		{
			//�����ж�
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

			//ɾ���˿�
			cbCardIndex[cbFirstIndex]--;
			cbCardIndex[cbFirstIndex + 1]--;
			cbCardIndex[cbFirstIndex + 2]--;

			//���ý��
			cbResultCard[0] = SwitchToCardData(cbFirstIndex);
			cbResultCard[1] = SwitchToCardData(cbFirstIndex + 1);
			cbResultCard[2] = SwitchToCardData(cbFirstIndex + 2);

			BYTE cbChiKind[3] = { CK_LEFT, CK_CENTER, CK_RIGHT };
			return cbChiKind[i];
		}
	}

	return CK_NULL;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	if (cbCardIndex >= MAX_INDEX)
	{
		return 0;
	}
	return ((cbCardIndex / 10) << 4) | (cbCardIndex % 10 + 1);
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	if (!IsValidCard(cbCardData))
	{
		return MAX_INDEX;
	}
	return ((cbCardData&MASK_COLOR) >> 4) * 10 + (cbCardData&MASK_VALUE) - 1;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[], BYTE bMaxCount)
{
	//ת���˿�
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

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE bCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex, sizeof(cbCardIndex));

	//ת���˿�
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