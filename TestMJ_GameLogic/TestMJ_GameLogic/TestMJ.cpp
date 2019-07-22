#include "GameLogic.h"
#include <iostream>
using namespace std;

void main()
{
	CGameLogic Logic;

	/*BYTE byCard[] = 
	{ 
		0x11,0x12,0x13,
		0x12,0x13,0x14,
		0x13,0x14,0x15,
		0x04,0x04,0x04,
		0x02,0x02,
	};*/

	/*BYTE byCard[] =
	{
		0x11,0x12,0x13,
		0x12,0x14,
		0x13,0x15,
		0x04,0x04,0x04,
		0x02,0x02,
		0x05,0x05,
	};*/

	/*BYTE byCard[] =
	{
		0x11,0x12,0x13,
		0x31,0x32,0x33,
		0x35,0x36,0x37,
		0x02,0x02,0x02,
		0x34,0x05,
	};*/

	BYTE byCard[] =
	{
		0x01,0x02,0x03,
		0x04,0x05,0x06,
		0x07,0x08,0x09,
		0x15,0x19,
	};

	BYTE byKingCard = 0x15;
	Logic.SetKingCardData(byKingCard);

	int nTick = GetTickCount();

	BYTE byCardIndex[MAX_INDEX] = { 0 };
	for (BYTE i = 0; i < sizeof(byCard) - 1; i++)
	{
		byCardIndex[Logic.SwitchToCardIndex(byCard[i])]++;
	}

	tagWeaveCardItem WeaveCardItem[MAX_ANSWER_WEAVE];

	tagHuInput Input;
	Input.pCardIndexSeq = byCardIndex;
	Input.bInsertCurrentCard = true;
	Input.byCurrentCard = byCard[sizeof(byCard) - 1];
	Input.pWeaveCardItemSeq = &WeaveCardItem[0];
	if (Logic.GetHuInfo(&Input))
	{
		cout << "Find Hu ms=" << GetTickCount() - nTick << endl;

		for (BYTE i = 0; i < Input.byWeaveCardItemCount; i++)
		{
			char szCard[256] = { 0 };
			for (BYTE j = 0; j < MAX_WEAVE_CARD_COUNT; j++)
			{
				sprintf_s(szCard, sizeof(szCard), "%s card[%d]=0x%02x", szCard, j, Input.pWeaveCardItemSeq[i].byCard[j]);
			}
			printf("%s\n", szCard);
		}
	}
	else
	{
		cout << "not Find Hu ms=" << GetTickCount() - nTick << endl;
	}
}