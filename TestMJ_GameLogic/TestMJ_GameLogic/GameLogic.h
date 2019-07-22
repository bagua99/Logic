#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "GameMessage.h"
#include "HuPaiLogic.h"

// 游戏逻辑类
class CGameLogic
{
	// 函数定义
public:
	// 构造函数
	CGameLogic();
	// 析构函数
	virtual ~CGameLogic();

	//控制函数
public:
	// 混乱扑克
	void RandCardData(BYTE byCardData[], BYTE byMaxCount);
	// 删除扑克
	bool RemoveCard(BYTE byCardIndex[MAX_INDEX], BYTE byRemoveCard);
	//删除扑克
	bool RemoveCard(BYTE byCardIndex[MAX_INDEX], BYTE byRemoveCard[], BYTE bRemoveCount);
	// 删除扑克
	bool RemoveCard(BYTE byCardData[], BYTE bCardCount, BYTE byRemoveCard[], BYTE bRemoveCount);

	// 辅助函数
public:
	// 有效判断
	bool IsValidCommonCard(BYTE byCardData);
	// 判断王牌
	void SetKingCardData(BYTE byCardData);
	// 获取王牌
	BYTE GetKingCardData();
	// 获取王牌
	BYTE GetKingCardIndex();
	// 判断王牌
	bool IsKingCardData(BYTE byCardData);
	// 判断王索引
	bool IsKingCardIndex(BYTE byCardIndex);
	// 扑克数目
	BYTE GetCardCount(BYTE byCardIndex[MAX_INDEX]);
	// 获取组合
	BYTE GetWeaveCardData(BYTE byWeaveKind, BYTE byCenterCard, BYTE byCardBuffer[MAX_WEAVE_CARD_COUNT]);
	
	// 被动函数
public:
	// 吃牌类型
	BYTE GetEatCardKind(BYTE byCardIndex[MAX_INDEX], BYTE byCurrentCard);
	// 碰牌类型
	BYTE GetPengCardKind(BYTE byCardIndex[MAX_INDEX], BYTE byCurrentCard);
	// 杠牌类型
	BYTE GetGangCardKind(BYTE byCardIndex[MAX_INDEX], BYTE byCurrentCard);
	// 胡牌信息
	bool GetHuInfo(tagHuInput *pInput);

	// 主动函数
public:
	// 听牌状态
	bool IsTingPaiStatus(BYTE byCardIndex[MAX_INDEX]);
	// 杠牌信息
	BYTE GetGangCardInfo(BYTE byCardIndex[MAX_INDEX], tagWeaveItem *pWeaveItem, BYTE byItemCount, BYTE byGangCard[MAX_WEAVE]);

	// 转换函数
public:
	// 扑克转换
	BYTE SwitchToCardData(BYTE byCardIndex);
	// 扑克转换
	BYTE SwitchToCardIndex(BYTE byCardData);
	// 扑克转换
	BYTE SwitchToCardData(BYTE byCardIndex[MAX_INDEX], BYTE byCardData[], BYTE bMaxCount);
	// 扑克转换
	BYTE SwitchToCardIndex(BYTE byCardData[], BYTE bCardCount, BYTE byCardIndex[MAX_INDEX]);

private:
	// 7对胡
	bool _GetHuInfo7Dui(tagHuInput* pInput, bool &bKingWaitHu);

	// 变量定义
private:
	static const BYTE				m_byCardDataArray[MAX_REPERTORY];					// 扑克数据
	BYTE							m_byKingCardValue;
	BYTE							m_byKingCardIndex;

	HuPaiLogic						m_HuPaiLogic;										// 胡牌逻辑
};

#endif