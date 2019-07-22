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

	//�����ж�
	if (cbCardIndex[cbCurrentIndex] >= 3)
	{
		return cbChiCardCount;
	}

	//��С���
	BYTE cbReverseIndex = (cbCurrentIndex + 10) % COMMOND_CARD_COUNT;
	if (cbCardIndex[cbCurrentIndex] >= 1 && cbCardIndex[cbReverseIndex] >= 1 && cbCardIndex[cbReverseIndex] != 3)
	{
		//�����˿�
		BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
		memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

		//ɾ���˿�
		cbCardIndexTemp[cbCurrentIndex]--;
		cbCardIndexTemp[cbReverseIndex]--;

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
			ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbResultCount = cbResultCount;
			ChiCardInfo[cbChiCardCount].cbCardData[0][0] = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][1] = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][2] = SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount++].cbChiKind = ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XXD : CK_XDD;
		}
	}

	//��С���
	if (cbCardIndex[cbReverseIndex] == 2)
	{
		//�����˿�
		BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
		memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

		//ɾ���˿�
		cbCardIndexTemp[cbReverseIndex] -= 2;

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
			ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbResultCount = cbResultCount;
			ChiCardInfo[cbChiCardCount].cbCardData[0][0] = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][1] = SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount].cbCardData[0][2] = SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount++].cbChiKind = ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XDD : CK_XXD;
		}
	}

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
bool CGameLogic::IsChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE &cbChiCardCount)
{
	if (cbCurrentCard == KING_VALUE)
	{
		return false;
	}
	//Ч���˿�
	if (!IsValidCard(cbCurrentCard))
	{
		return false;
	}

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//�����˿�
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//��ȡ�ж�
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

//�Ƿ�����
bool CGameLogic::IsTiPaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	if (cbCurrentCard == KING_VALUE)
	{
		return false;
	}
	//Ч���˿�
	if (!IsValidCard(cbCurrentCard))
	{
		return false;
	}

	//ת������
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//�����ж�
	return (cbCardIndex[cbCurrentIndex] == 3) ? true : false;
}

//�Ƿ�����
bool CGameLogic::IsWeiPengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	if (cbCurrentCard == KING_VALUE)
	{
		return false;
	}
	//Ч���˿�
	if (!IsValidCard(cbCurrentCard))
	{
		return false;
	}

	//ת������
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//�����ж�
	return (cbCardIndex[cbCurrentIndex] == 2) ? true : false;
}

//���ƽ��
bool CGameLogic::GetHuCardInfo(BYTE cbCardIndex[MAX_INDEX], tagHuCardInfo &HuCardInfo)
{
	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	vector<BYTE> vctTest;
	stAnswer sResult;
	// ���ú�������
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

		// ���ñ���
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

		// ������ͨ��
		cbTempHuBaseType |= CheckHuBase_Nomal;
	}
	break;
	case CheckHuBase_WangDiao:
	{
		// ����������
		if (cbCardIndexTemp[KING_INDEX] < 1)
		{
			return false;
		}
		// û�����ƣ�����������
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

		// ���ñ���
		HuCardInfo.HuFinish.cbHuBase = 2;
		// ������
		if (HuCardInfo.cbInsertCard == KING_VALUE)
		{
			HuCardInfo.HuFinish.cbHuBase *= 2;
		}
		// ȥ��������
		HuCardInfo.cbInsertCard = 0;
		// ��������
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

		// ��������
		cbTempHuBaseType |= CheckHuBase_WangDiao;
	}
	break;
	case CheckHuBase_WangChuang:
	{
		// ����������
		if (cbCardIndexTemp[KING_INDEX] < 2)
		{
			return false;
		}
		// û�����ƣ�����������
		if (HuCardInfo.cbInsertCard == 0)
		{
			return false;
		}

		// ��������ټ������Ϻ�Ϣ
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
		HuCardInfo.HuFinish.cbEyeCardHuXi = (HuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;		// ������ʱ����������Ϣ,�㷨�����֦��

																																// ���ñ���
		HuCardInfo.HuFinish.cbHuBase = 4;
		// ������
		if (HuCardInfo.cbInsertCard == KING_VALUE)
		{
			HuCardInfo.HuFinish.cbHuBase *= 2;
		}
		// ȥ��������
		HuCardInfo.cbInsertCard = 0;
		// ��������
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

		// ��������
		cbTempHuBaseType |= CheckHuBase_WangChuang;
	}
	break;
	case CheckHuBase_WangZha:
	{
		// ����������
		if (cbCardIndexTemp[KING_INDEX] < 3)
		{
			return false;
		}
		// û�����ƣ�����������
		if (HuCardInfo.cbInsertCard == 0)
		{
			return false;
		}

		// ��������ټ������Ϻ�Ϣ
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
		HuCardInfo.HuFinish.cbEyeCardHuXi = (HuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_TI_HU_XI : TYPE_X_TI_HU_XI;		// ������ʱ����������Ϣ,�㷨�����֦��

																															// ���ñ���
		HuCardInfo.HuFinish.cbHuBase = 8;
		// ��ը��
		if (HuCardInfo.cbInsertCard == KING_VALUE)
		{
			HuCardInfo.HuFinish.cbHuBase *= 2;
		}
		// ȥ��������
		HuCardInfo.cbInsertCard = 0;
		// ��������
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

		// ������ը
		cbTempHuBaseType |= CheckHuBase_WangZha;
	}
	break;
	case (CheckHuBase_WangDiao | CheckHuBase_WangChuang | CheckHuBase_WangZha):			// ��ը,����������
	{
		// ��ը
		do
		{
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// ������Ϣ
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// ����������
			if (cbCardIndexTemp[KING_INDEX] < 3)
			{
				break;
			}
			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// û�����ƣ�����������
			if (cbInsertCard == 0)
			{
				break;
			}

			// ��������ټ������Ϻ�Ϣ
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
			tempHuCardInfo.HuFinish.cbEyeCardHuXi = (tempHuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_TI_HU_XI : TYPE_X_TI_HU_XI;		// ������ʱ����������Ϣ,�㷨�����֦��

																																		// ���ñ���
			tempHuCardInfo.HuFinish.cbHuBase = 8;
			// ��ը��
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// ȥ��������
			tempHuCardInfo.cbInsertCard = 0;
			// ��������
			cbCardIndexTemp[KING_INDEX] -= 3;

			vctTest.clear();
			for (int i = 0; i<MAX_INDEX; ++i)
			{
				for (int j = 0; j<cbCardIndexTemp[i]; ++j)
				{
					vctTest.push_back(SwitchToCardData(i));
				}
			}

			// ���ƻ���
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// ������ը
			cbTempHuBaseType |= CheckHuBase_WangZha;
		} while (false);

		// ����
		do
		{
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// ������Ϣ
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// ����������
			if (cbCardIndexTemp[KING_INDEX] < 2)
			{
				break;
			}
			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// û�����ƣ�����������
			if (cbInsertCard == 0)
			{
				break;
			}

			// ��������ټ������Ϻ�Ϣ
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
			tempHuCardInfo.HuFinish.cbEyeCardHuXi = (tempHuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;		// ������ʱ����������Ϣ,�㷨�����֦��

																																			// ���ñ���
			tempHuCardInfo.HuFinish.cbHuBase = 4;
			// ������
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// ȥ��������
			tempHuCardInfo.cbInsertCard = 0;
			// ��������
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

			// ���ƻ���
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// ��������
			cbTempHuBaseType |= CheckHuBase_WangChuang;
		} while (false);

		// ����
		do
		{
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// ������Ϣ
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// ����������
			if (cbCardIndexTemp[KING_INDEX] < 1)
			{
				break;
			}

			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// û�����ƣ�����������
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

			// ���ñ���
			tempHuCardInfo.HuFinish.cbHuBase = 2;
			// ������
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// ȥ��������
			tempHuCardInfo.cbInsertCard = 0;
			// ��������
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

			// ���ƻ���
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// ��������
			cbTempHuBaseType |= CheckHuBase_WangDiao;
		} while (false);
	}
	break;
	case CheckHuBase_Max:
	{
		// �����Ʒ���
		int nLastHuScore = 0;
		// ��ը
		do
		{
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// ������Ϣ
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// ����������
			if (cbCardIndexTemp[KING_INDEX] < 3)
			{
				break;
			}
			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// û�����ƣ�����������
			if (cbInsertCard == 0)
			{
				break;
			}

			// ��������ټ������Ϻ�Ϣ
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
			tempHuCardInfo.HuFinish.cbEyeCardHuXi = (tempHuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_TI_HU_XI : TYPE_X_TI_HU_XI;		// ������ʱ����������Ϣ,�㷨�����֦��

																																		// ���ñ���
			tempHuCardInfo.HuFinish.cbHuBase = 8;
			// ��ը��
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// ȥ��������
			tempHuCardInfo.cbInsertCard = 0;
			// ��������
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

			// ���ƻ���
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// ������ը
			cbTempHuBaseType |= CheckHuBase_WangZha;
		} while (false);

		// ����
		do
		{
			if (nLastHuScore > 0)
			{
				break;
			}

			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// ������Ϣ
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// ����������
			if (cbCardIndexTemp[KING_INDEX] < 2)
			{
				break;
			}
			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// û�����ƣ�����������
			if (cbInsertCard == 0)
			{
				break;
			}

			// ��������ټ������Ϻ�Ϣ
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
			tempHuCardInfo.HuFinish.cbEyeCardHuXi = (tempHuCardInfo.cbInsertCard&MASK_COLOR)>0 ? TYPE_D_WEI_HU_XI : TYPE_X_WEI_HU_XI;		// ������ʱ����������Ϣ,�㷨�����֦��

																																			// ���ñ���
			tempHuCardInfo.HuFinish.cbHuBase = 4;
			// ������
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// ȥ��������
			tempHuCardInfo.cbInsertCard = 0;
			// ��������
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

			// ���ƻ���
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// ��������
			cbTempHuBaseType |= CheckHuBase_WangChuang;
		} while (false);

		// ����
		do
		{
			if (nLastHuScore > 0)
			{
				break;
			}

			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			// ������Ϣ
			tagHuCardInfo tempHuCardInfo;
			memcpy(&tempHuCardInfo, &HuCardInfo, sizeof(tagHuCardInfo));
			// ����������
			if (cbCardIndexTemp[KING_INDEX] < 1)
			{
				break;
			}

			BYTE cbInsertCard = tempHuCardInfo.cbInsertCard;
			// û�����ƣ�����������
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

			// ���ñ���
			tempHuCardInfo.HuFinish.cbHuBase = 2;
			// ������
			if (tempHuCardInfo.cbInsertCard == KING_VALUE)
			{
				tempHuCardInfo.HuFinish.cbHuBase *= 2;
			}
			// ȥ��������
			tempHuCardInfo.cbInsertCard = 0;
			// ��������
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

			// ���ƻ���
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));
			HuCardInfo.cbInsertCard = cbInsertCard;

			// ��������
			cbTempHuBaseType |= CheckHuBase_WangDiao;
		} while (false);

		// ��ͨ��
		do
		{
			if (nLastHuScore > 0)
			{
				break;
			}

			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

			// ������Ϣ
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

			// ���ñ���
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

			// ���ƻ���
			memcpy(&HuCardInfo, &tempHuCardInfo, sizeof(tagHuCardInfo));

			// ������ͨ��
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

	// �������պ�������
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
				// ���Լ����ƣ����ǲ��������ó���
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

		// �����������
		++nWeaveIndex;
	}

	// ������
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

		// �����������
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


//��Ч�ж�
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

//��ȡ����
BYTE CGameLogic::TakeOutChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbResultCard[3])
{
	//Ч���˿�
	if (cbCurrentCard == 0)
	{
		return 0;
	}

	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	// ������Լ�����2��+Ҫ�Ե�1�������3��
	if (cbCardIndex[cbCurrentIndex] >= 4)
	{
		return 0;
	}

	//��С���
	BYTE cbReverseIndex = (cbCurrentIndex + 10) % COMMOND_CARD_COUNT;
	if (cbCardIndex[cbCurrentIndex] >= 2 && cbCardIndex[cbReverseIndex] >= 1 && cbCardIndex[cbReverseIndex] <= 2)
	{
		//ɾ���˿�
		cbCardIndex[cbCurrentIndex] -= 2;
		cbCardIndex[cbReverseIndex]--;

		//���ý��
		cbResultCard[0] = cbCurrentCard;
		cbResultCard[1] = cbCurrentCard;
		cbResultCard[2] = SwitchToCardData(cbReverseIndex);

		return ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XXD : CK_XDD;
	}

	//��С���
	if (cbCardIndex[cbReverseIndex] == 2 && cbCardIndex[cbCurrentIndex] >= 1 && cbCardIndex[cbCurrentIndex] <= 2)
	{
		//ɾ���˿�
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbReverseIndex] -= 2;

		//���ý��
		cbResultCard[0] = cbCurrentCard;
		cbResultCard[1] = SwitchToCardData(cbReverseIndex);
		cbResultCard[2] = SwitchToCardData(cbReverseIndex);

		return ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XDD : CK_XXD;
	}

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
			if (cbCardIndex[cbIndex] == 0 || (cbCurrentIndex != cbIndex && cbCardIndex[cbIndex] >= 3))
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

//�˿�ת��
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

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[], BYTE bMaxCount)
{
	//ת���˿�
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