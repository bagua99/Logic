#ifndef GAME_MESSAGE_FILE
#define GAME_MESSAGE_FILE

#include <windows.h>

#pragma pack(push)	// 保存当前的字节对齐数目
#pragma pack(1)		// 字节对齐

#define GAME_ID						20004200							// 游戏ID
#define GAME_PLAYER					4									// 游戏人数

#define MAX_WEAVE					4									// 最大组合
#define MAX_WEAVE_CARD_COUNT		4									// 最大组合牌数量
#define MAX_ANSWER_WEAVE			5									// 最大答案组合
#define MAX_INDEX					35									// 最大索引
#define MAX_COUNT					14									// 最大数目
#define MAX_REPERTORY				136									// 最大库存

//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//无效数值
#define INVALID_BYTE				((BYTE)0xFF)						// 无效数值
#define INVALID_WORD				((WORD)0xFFF)						// 无效数值
#define INVALID_DWORD				((DWORD)0xFFFFFFFF)					// 无效数值

//无效数值
#define INVALID_TABLE				INVALID_WORD						// 无效桌子
#define INVALID_CHAIR				INVALID_WORD						// 无效椅子

//////////////////////////////////////////////////////////////////////////

//数值定义
#define	MASK_COLOR					0xF0								// 花色掩码
#define	MASK_VALUE					0x0F								// 数值掩码

//组合类型
#define ACK_NULL					0x00								// 没有类型
#define ACK_LEFT					0x01								// 左吃类型
#define ACK_CENTER					0x02								// 中吃类型
#define ACK_RIGHT					0x04								// 右吃类型
#define ACK_PENG					0x08								// 碰牌类型
#define ACK_GANG					0x10								// 杠牌类型
#define ACKLISTEN					0x20								// 吃牌类型
#define ACK_CHIHU					0x40								// 吃胡类型
#define ACK_ZI_MO					0x80								// 自摸

//////////////////////////////////////////////////////////////////////////

//组合子项
struct CMD_WeaveItem
{
	BYTE							byWeaveKind;						// 组合类型
	BYTE							byCenterCard;						// 中心扑克
	WORD							wProvideUser;						// 供应用户

	CMD_WeaveItem()
	{
		memset(this, 0, sizeof(*this));
	}
};

//////////////////////////////////////////////////////////////////////////

//类型子项
struct tagKindItem
{
	BYTE							byWeaveKind;						// 组合类型
	BYTE							byCenterCard;						// 中心扑克
	BYTE							byCardIndex[3];						// 扑克索引

	tagKindItem()
	{
		memset(this, 0, sizeof(*this));
	}
};

//组合子项
struct tagWeaveItem
{
	BYTE							byWeaveKind;						// 组合类型
	BYTE							byCenterCard;						// 中心扑克
	WORD							wProvideUser;						// 供应用户

	tagWeaveItem()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 组合牌
struct tagWeaveCardItem
{
	BYTE							byCard[MAX_WEAVE_CARD_COUNT];		// 扑克

	tagWeaveCardItem()
	{
		memset(this, 0, sizeof(*this));
	}
};

//杠牌信息
struct tagGangInfo
{
	BYTE							byGangCardData;						// 杠牌
	WORD							wPerformUser;						// 杠牌者
	WORD							wProvideUser;						// 提供者
	BYTE							bySendData;							// 补牌
	BYTE							byCurrentOutCount;					// 当时出牌数目

	tagGangInfo()
	{
		Reset();
	}
	void Reset()
	{
		byGangCardData = 0;
		wPerformUser = INVALID_CHAIR;
		wProvideUser = INVALID_CHAIR;
		bySendData = 0;
		byCurrentOutCount = 0;
	}
};

// 胡牌输入
struct tagHuInput
{
	BYTE*							pCardIndexSeq;						// 牌列
	bool							bInsertCurrentCard;					// 是否插入判断
	bool							bJudgeKingWait;						// 判断抛花麻
	tagWeaveCardItem*				pWeaveCardItemSeq;					// 组合牌
	BYTE							byWeaveCardItemCount;				// 组合牌数目
	BYTE							byCurrentCard;						// 当前牌

	tagHuInput()
	{
		pCardIndexSeq = NULL;
		bJudgeKingWait = false;
		bInsertCurrentCard = false;
		pWeaveCardItemSeq = NULL;
		byWeaveCardItemCount = 0;
		byCurrentCard = 0;
	}
};

#pragma pack(pop)

#endif	// GAME_MESSAGE_FILE