#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#pragma once

#include <wtypes.h>
#include <windows.h>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#include "GameMessage.h"
#include "HuPaiLogic.h"
//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//控制函数
public:
	//混乱扑克
	void RandCardData(BYTE cbCardData[], BYTE bMaxCount);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE bRemoveCount);
	//删除扑克
	bool RemoveCard(BYTE cbCardData[], BYTE bCardCount, BYTE cbRemoveCard[], BYTE bRemoveCount);

	//逻辑函数
public:
	//提牌判断
	BYTE GetAcitonTiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbTiCardIndex[5]);
	//畏牌判断
	BYTE GetActionWeiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbWeiCardIndex[7]);
	//吃牌判断
	BYTE GetActionChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, tagChiCardInfo ChiCardInfo[6]);

	//判断函数
public:
	//是否吃牌
	bool IsChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE &cbChiCardCount);
	//是否提跑
	bool IsTiPaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//是否偎碰
	bool IsWeiPengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//胡牌结果
	bool GetHuCardInfo(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbHuXiWeave, tagHuCardInfo &HuCardInfo);

	//辅助函数
public:
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//扑克数目
	BYTE GetCardCount(BYTE cbCardIndex[MAX_INDEX]);
	//获取胡息
	BYTE GetWeaveHuXi(const tagWeaveItem &WeaveItem);

	//转换函数
public:
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[], BYTE bMaxCount);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData[], BYTE bCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//内部函数
private:
	//提取吃牌
	BYTE TakeOutChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbResultCard[3]);

	//变量定义
protected:
	static const BYTE	m_cbCardDataArray[MAX_REPERTORY];				// 扑克数据
	CHuPaiLogic			m_HuPaiLogic;									// 胡牌逻辑
};

//////////////////////////////////////////////////////////////////////////

#endif	// __GAME_LOGIC_H__