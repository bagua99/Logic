#include "GameLogic.h"
#include <iostream>

int main()
{
	CGameLogic gameLogic;

	///////////////////可胡///////////////////
	/*BYTE cbCardData[MAX_COUNT] =
	{
	0x01,0x01,0x01,0x01,
	0x02,0x02,0x02,0x02,
	0x03,0x03,0x03,0x03,
	0x04,0x04,0x04,0x04,
	0x05,0x05,0x05,
	0x06,0x06
	};*/

	/*char cbCardData[MAX_COUNT] =
	{
	0x11,0x11,0x11,
	0x12,0x12,0x12,
	0x13,0x13,0x13,
	0x07,0x07,0x07,
	0x08,0x08,0x08,
	0x1A,0x1A,0x1A,
	0x09,0x09,0x09
	};*/

	// 12胡可胡，9胡不可胡
	//BYTE cbCardData[] =
	//{
	//	0x12,0x13,0x14,
	//	0x02,0x07,0x0A
	//};

	// 0胡自己摸可胡，3胡别人摸可胡
	/*BYTE cbCardData[] =
	{
	0x12,0x12,0x12,
	0x03,0x03,
	0x02,0x07,0x0A,
	0x15,0x16,0x17,
	0x1A,0x1A,0x1A
	};*/

	// 9胡别人摸可胡，自己摸不可胡
	/*BYTE cbCardData[] =
	{
	0x13,0x13,0x17,0x07,0x17,0x12,0x1A,0x07,0x08,0x06,0x07
	};*/
	///////////////////可胡///////////////////


	///////////////////不可胡///////////////////
	/*BYTE cbCardData[MAX_COUNT] =
	{
	0x01,0x01,0x01,0x07,
	0x02,0x02,0x02,0x02,
	0x03,0x03,0x03,0x0A,
	0x04,0x04,0x04,0x04,
	0x05,0x05,0x05,
	0x06,0x06
	};*/

	/*BYTE cbCardData[MAX_COUNT] =
	{
	0x11,0x11,0x11,
	0x12,0x12,0x02,
	0x13,0x13,0x0A,
	0x14,0x14,0x04,
	0x05,0x05,0x05,
	0x07,0x07,0x06,
	0x07,0x07,0x0A
	};*/

	/*BYTE cbCardData[] =
	{
	0x11,0x11,0x11,
	0x12,0x12,0x12,
	0x13,0x13,0x13,
	0x14,0x14,0x14,
	0x05,0x05,0x05,
	0x07,0x07,0x07,
	0x17,0x19,0x1A
	};*/
	///////////////////不可胡///////////////////

	/*BYTE cbCardData[] =
	{
	0xff,0x04,0x03,0xff,0x15,0x16,0xff,0x05,0x06
	};*/
	//BYTE cbCardData[] =
	//{
	//	0x01,0x02,0x03,
	//	0x04,0x05,0x06,
	//	0x07,0x07,0x0a,
	//	0x12,0x12,0x13,
	//	0x14,0x15,0x17,
	//	0x17,0x18,0x1a,
	//	0x1a,0xFF,0x15
	//};

	//ULONGLONG dwTick = GetTickCount64();
	//BYTE cbCardIndex[MAX_INDEX] = {0};	// 用户扑克
	//for (int i=0; i<sizeof(cbCardData)-1; ++i)
	//{
	//	cbCardIndex[gameLogic.SwitchToCardIndex(cbCardData[i])]++;
	//}

	//int nTick = GetTickCount();
	//tagHuCardInfo HuCardInfo;
	//HuCardInfo.bSelfAddCard = true;
	//HuCardInfo.bOnlyHu = true;
	//HuCardInfo.cbCheckHuBaseType = CheckHuBase_Nomal;
	//HuCardInfo.cbInsertCard = cbCardData[sizeof(cbCardData)-1];
	//
	//HuCardInfo.HuFinish.cbXingCard = 0x11;
	//HuCardInfo.HuFinish.cbXingTunCount = 1;
	//HuCardInfo.HuFinish.cbHuBase = 1;
	//HuCardInfo.HuFinish.cbHuXi = 6;
	//HuCardInfo.HuFinish.cbHuXingCount = 0;
	//HuCardInfo.HuFinish.cbHongCardCount = 0;
	//HuCardInfo.HuFinish.cbHeiCardCount = 0;

	BYTE cbCardData[] =
	{
		0x11,0x11,0x11,
		0x12,0x12,0x12,
		0x05,0x05,
		0x07,0x0a,
		0x02
	};

	ULONGLONG dwTick = GetTickCount64();
	BYTE cbCardIndex[MAX_INDEX] = { 0 };	// 用户扑克
	for (int i = 0; i<sizeof(cbCardData) - 1; ++i)
	{
		cbCardIndex[gameLogic.SwitchToCardIndex(cbCardData[i])]++;
	}

	int nTick = GetTickCount();
	tagHuCardInfo HuCardInfo;
	HuCardInfo.bSelfAddCard = false;
	HuCardInfo.bOnlyHu = true;
	HuCardInfo.cbCheckHuBaseType = CheckHuBase_Nomal;
	HuCardInfo.cbInsertCard = cbCardData[sizeof(cbCardData) - 1];

	HuCardInfo.HuFinish.cbXingCard = 0x13;
	HuCardInfo.HuFinish.cbXingTunCount = 1;
	HuCardInfo.HuFinish.cbHuBase = 1;
	HuCardInfo.HuFinish.cbHuXi = 0;
	HuCardInfo.HuFinish.cbHuXingCount = 0;
	HuCardInfo.HuFinish.cbHongCardCount = 1;
	HuCardInfo.HuFinish.cbHeiCardCount = 9;
	HuCardInfo.HuFinish.cbAddTunCount = 0;
	if (gameLogic.GetHuCardInfo(cbCardIndex, HuCardInfo))
	{
		cout << "Find Hu ms = " << GetTickCount() - nTick << endl;

		printf("HuCardInfo.cbCheckHuBaseType=0x%02x\n", HuCardInfo.cbCheckHuBaseType);

		printf("*****************************************************************************\n");
		for (int i = 0; i<HuCardInfo.cbWeaveCount; ++i)
		{
			tagWeaveItem &WeaveItem = HuCardInfo.WeaveItemArray[i];
			int nWeaveHuXi = gameLogic.GetWeaveHuXi(WeaveItem);
			printf("nWeaveKind=%d,cbCardCount=%d,nWeaveHuXi=%d\ncbCardList[0]=0x%02x,cbCardList[1]=0x%02x,cbCardList[2]=0x%02x,cbCardList[3]=0x%02x\n", WeaveItem.nWeaveKind, WeaveItem.cbCardCount, nWeaveHuXi, WeaveItem.cbCardList[0], WeaveItem.cbCardList[1], WeaveItem.cbCardList[2], WeaveItem.cbCardList[3]);
		}
		printf("*****************************************************************************\n");

		for (int i = 0; i<HuCardInfo.HuFinish.cbWeaveCount; ++i)
		{
			tagWeaveItem &WeaveItem = HuCardInfo.HuFinish.WeaveItemArray[i];
			int nWeaveHuXi = gameLogic.GetWeaveHuXi(WeaveItem);
			printf("nWeaveKind=%d,cbCardCount=%d,nWeaveHuXi=%d\ncbCardList[0]=0x%02x,cbCardList[1]=0x%02x,cbCardList[2]=0x%02x,cbCardList[3]=0x%02x\n", WeaveItem.nWeaveKind, WeaveItem.cbCardCount, nWeaveHuXi, WeaveItem.cbCardList[0], WeaveItem.cbCardList[1], WeaveItem.cbCardList[2], WeaveItem.cbCardList[3]);
		}
		printf("*****************************************************************************\n");

		printf("cbHuXi=%d,cbHuBase=%d,cbHuHongHeiBase=%d,nHuScore=%d\n", HuCardInfo.HuFinish.cbHuXi, HuCardInfo.HuFinish.cbHuBase, HuCardInfo.HuFinish.cbHuHongHeiBase, HuCardInfo.HuFinish.nHuScore);
		printf("cbXingCard=0x%02x,cbHuXingCount=%d,cbHongCardCount=%d,cbHeiCardCount=%d\n", HuCardInfo.HuFinish.cbXingCard, HuCardInfo.HuFinish.cbHuXingCount, HuCardInfo.HuFinish.cbHongCardCount, HuCardInfo.HuFinish.cbHeiCardCount);
	}
	else
	{
		cout << "not Hu ms = " << GetTickCount() - nTick << endl;
	}

	return 0;
}