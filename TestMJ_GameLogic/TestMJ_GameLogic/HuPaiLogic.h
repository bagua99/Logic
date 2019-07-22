#ifndef HUPAI_LOGIC_HEAD_FILE
#define HUPAI_LOGIC_HEAD_FILE

#include "GameMessage.h"
#include <unordered_map>
#include <map>
#include <algorithm>
#include <functional>
using namespace std;

// 王吊权值
#define KING_DIAO_KEY	1
// 风牌(东南西北顺子)
#define FENG_PAI_KEY	1
// 箭牌(中发白顺子)
#define JIAN_PAI_KEY	1

/////////////////////////////////////////////////////		CardKey			/////////////////////////////////////////////////////
// 牌权值信息
class CardKey
{
public:
	// 构造函数
	CardKey();

	// 析构函数
	~CardKey();

	// 重载构造函数
	CardKey(BYTE byValue, BYTE byCardIndex0, BYTE byCardIndex1, BYTE byCardIndex2=MAX_INDEX, BYTE byCardIndex3=MAX_INDEX);

public:
	// 取得牌索引数量
	BYTE GetCardIndexCount();

	// 取得牌索引
	BYTE GetCardIndex(BYTE cbIndex);

	// 取得权值
	BYTE GetKey();

private:
	// 清理函数
	void Clear();

private:
	BYTE			m_byCardIndex[MAX_WEAVE_CARD_COUNT];	// 牌索引
	BYTE			m_byCardIndexCount;						// 牌数量
	BYTE			m_byKey;								// 权值(跑胡子可以当作胡息来用)
};
/////////////////////////////////////////////////////		CardKey			/////////////////////////////////////////////////////

/////////////////////////////////////////////////////		CardData		/////////////////////////////////////////////////////
// 牌数据信息
class CardData
{
public:
	// 构造函数
	CardData();

	// 析构函数
	~CardData();

public:
	// 增加牌
	void AddCard(BYTE byCardValue);

	// 取得剩余牌索引
	BYTE *GetLeftIndex();

	// 取得剩余牌索引数量
	BYTE GetLeftIndexCount();

	// 取得牌索引
	BYTE GetCardIndex(BYTE byCardValue);

	// 取得牌索引对应牌值
	BYTE GetCardValue(BYTE byCardIndex);

	// 设置标志
	void SetFlag(BYTE byIndex, bool bFlag);

	// 取得牌索引数组
	BYTE *GetCardIndexArray(BYTE byCardValue);

	// 取得牌索引数组牌值
	BYTE GetCardIndexArrayValue(BYTE byCardValue, BYTE byCardCountIndex);

	// 取得牌索引数组数量
	BYTE GetCardIndexArrayCount(BYTE byCardValue);

	// 取得牌索引数组(通过索引)
	BYTE *GetCardIndexArrayInIndex(BYTE byCardIndex);

	// 取得牌索引数组牌值(通过索引)
	BYTE GetCardIndexArrayValueInIndex(BYTE byCardIndex, BYTE byCardCountIndex);

	// 取得牌索引数组数量(通过索引)
	BYTE GetCardIndexArrayCountInIndex(BYTE byCardIndex);

private:
	// 清理函数
	void Clear();

	// 取得标志
	bool GetFlag(BYTE byIndex);

	// 取得是否完成
	bool Finish();

private:
	int				m_nFlag;												// 标志值
	BYTE			m_byCard[MAX_COUNT];									// 牌数组
	BYTE			m_byCardCount;											// 牌数量
	BYTE			m_byCardIndex[MAX_INDEX][MAX_WEAVE_CARD_COUNT];			// 牌索引
	BYTE			m_byCardIndexCount[MAX_INDEX];							// 牌索引数量
	BYTE			m_byLeftIndex[MAX_COUNT];								// 剩余牌索引(输出)
	BYTE			m_byLeftIndexCount;										// 剩余牌索引数量(输出)
	BYTE			m_byLeftCount;											// 剩余数量
};
/////////////////////////////////////////////////////		CardData		/////////////////////////////////////////////////////

/////////////////////////////////////////////////////		CardAnswer		/////////////////////////////////////////////////////
// 牌答案信息
class CardAnswer
{
public:
	// 构造函数
	CardAnswer();

	// 析构函数
	~CardAnswer();

public:
	// 清理函数
	void Clear();

	// 增加牌数据
	void AddCardData(CardKey *pKeyData, CardData *pCardData);

	// 移除牌数据
	void RemoveCardData(CardData *pCardData);

	// 取得牌权值信息
	CardKey *GetCardKey(BYTE nCardKeyIndex);

	// 取得牌权值数量
	BYTE GetCardKeyCount();

private:
	CardKey				m_CardKey[MAX_ANSWER_WEAVE];	// 答案组合
	BYTE				m_byCardKeyCount;				// 答案组合数量
	BYTE				m_byKey;						// 答案权值

};
/////////////////////////////////////////////////////		CardAnswer		/////////////////////////////////////////////////////

/////////////////////////////////////////////////////		HuPaiLogic		/////////////////////////////////////////////////////
// 胡牌逻辑
class HuPaiLogic
{
public:
	// 构造函数
	HuPaiLogic();

	// 析构函数
	~HuPaiLogic();

public:
	// 初始化牌权值
	void InitCard();

	// 牌组合键值
	DWORD GetCardValue(BYTE byCard0, BYTE byCard1, BYTE byCard2, BYTE byCard3);

	// 获取牌组合权值
	int GetCardValueKey(BYTE byCard0, BYTE byCard1, BYTE byCard2, BYTE byCard3);

	// 获取牌组合权值
	int GetCardValueKey(DWORD dwKey);

	// 初始化胡牌
	bool InitHuCard(CardData *pCardData, CardAnswer *pAnswer, BYTE byKingCard, BYTE byKingIndex, BYTE byJiangCount);

	// 获得胡牌结果
	bool GetHuCardResult(CardData *pCardData, CardAnswer *pAnswer);

private:
	// 清理函数
	void Clear();

	// 增加权值表(牌组合值为key)
	void AddKeyMap(int nCardValue, BYTE byKey);

	// 获取三同牌组合
	void GetThreeSameCard();

	// 获取顺子牌组合
	void GetStraightCard();

	// 获取其他牌组合
	void GetOtherCard();

	// 扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex);

private:
	// 牌权值表
	typedef unordered_map<DWORD, BYTE>			MAP_KEY;
	MAP_KEY										m_mapKey;

	// 将数量
	BYTE										m_byJiangCount;
	// 王牌值
	BYTE										m_byKingCard;
	// 王牌索引
	BYTE										m_byKingIndex;
};
/////////////////////////////////////////////////////		HuPaiLogic		/////////////////////////////////////////////////////

#endif	// HUPAI_LOGIC_HEAD_FILE