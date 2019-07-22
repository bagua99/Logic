#ifndef __GAME_MESSAGE_H__
#define __GAME_MESSAGE_H__

#include <string.h>

typedef BYTE* PBYTE;

// 文件名字定义
#define GAMENAME						TEXT("长沙跑胡子")
#define GAME_ID							134
#define NAME_ID							30000134

// 版本定义
#define GAME_MAX_VER					1									// 现在最高版本
#define GAME_LESS_VER					1									// 现在最低版本
#define GAME_CHANGE_VER					0									// 修改版本

// 支持类型定义
#define SUP_NORMAL_GAME			        0x01								// 普通游戏
#define SUP_MATCH_GAME			        0x02								// 比赛游戏
#define SUP_MONEY_GAME			        0x04								// 金币游戏

// 开发库版本
#define DEV_LIB_VER				       1									// 开发库版本

// 支持类型
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

// 游戏信息
#define PLAY_COUNT						3									// 游戏人数
#define INVALID_CHAIR					(WORD(-1))

// 游戏内部开发版本号
#define DEV_HEIGHT_VERSION				1									// 内部开发高版本号(每修正一次LOW_VERSIONG+1)
#define DEV_LOW_VERSION					0									// 内部开发低版本号

// 游戏状态定义
#define GS_WAIT_AGREE					0									// 等待同意设置
#define GS_PLAY_GAME					21									// 游戏中状态

#define ASS_GM_GAME_STATION				2									// 游戏状态
#define ASS_MAIN_GAME					180							        // 游戏专用消息主ID

// 数值定义
#define MAX_WEAVE					7										// 最大组合
#define MAX_INDEX					20										// 最大索引
#define MAX_COUNT					21										// 最大数目
#define MAX_REPERTORY				80										// 最大库存
#define MAX_LEFT					19										// 最大剩余牌
#define MIN_HU_XI					15										// 胡牌最小胡息

#define	MASK_COLOR					0xF0									// 花色掩码
#define	MASK_VALUE					0x0F									// 数值掩码

// 动作定义
#define ACK_NULL					0x00									// 空
#define ACK_TI						0x01									// 提
#define ACK_PAO						0x02									// 跑
#define ACK_WEI						0x04									// 偎
#define ACK_CHI						0x08									// 吃
#define ACK_CHI_EX					0x10									// 吃
#define ACK_PENG					0x20									// 碰
#define ACK_CHIHU					0x40									// 胡

// 吃牌类型
#define CK_NULL						0x00									// 无效类型
#define CK_XXD						0x01									// 小小大搭
#define CK_XDD						0x02									// 小大大搭
#define CK_EQS						0x04									// 二七十吃
#define CK_LEFT						0x10									// 靠左对齐
#define CK_CENTER					0x20									// 居中对齐
#define CK_RIGHT					0x40									// 靠右对齐

// 胡牌类型
enum HuPai_Type
{
	HuPai_TianHu = 0,				// 天胡
	HuPai_DiHu,						// 地胡
	HuPai_PengPengHu,				// 碰碰胡
	HuPai_HeiHu,					// 黑胡
	HuPai_ShiHong,					// 十红胡
	HuPai_YiDianHongHu,				// 一点红胡
	HuPai_ShiBaDaHu,				// 十八大胡
	HuPai_ShiBaXiaoHu,				// 十八小胡
	HuPai_ErBiHu,					// 二比胡
	HuPai_SanBiHu,					// 三比胡
	HuPai_SiBiHu,					// 四比胡
	HuPai_ShuangPiaoHu,				// 双飘胡
	HuPai_MaxHu,					// 最大胡
};

// 胡牌倍率
const BYTE nHuPaiBase[HuPai_MaxHu] =
{
	5,
	5,
	5,
	5,
	2,
	4,
	5,
	5,
	2,
	3,
	4,
	2
};

enum CSPHZ_OPTION1_HUPAI
{
	CSPHZ_OPTION1_NULL = 0,
	CSPHZ_OPTION1_HU_BASE_LIMIT = 1,		// 胡牌上限
	CSPHZ_OPTION1_HU_BASE_NO_LIMIT = 2,		// 胡牌不上限
};

#pragma pack(1)

/********************************************************************************/
// 组合子项
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						// 组合类型
	BYTE							cbCardCount;						// 扑克数目
	BYTE							cbCenterCard;						// 中心扑克
	BYTE							cbCardList[4];						// 扑克列表

	tagWeaveItem()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 吃牌信息
struct tagChiCardInfo
{
	BYTE							cbChiKind;							// 吃牌类型
	BYTE							cbCenterCard;						// 中心扑克
	BYTE							cbResultCount;						// 结果数目
	BYTE							cbCardData[3][3];					// 吃牌组合

	tagChiCardInfo()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 胡牌信息
struct tagHuCardInfo
{
	bool							bSelfAddCard;						// 自己摸牌
	BYTE							cbWeaveCount;						// 组合数目
	tagWeaveItem					WeaveItemArray[MAX_WEAVE];			// 组合扑克

	tagHuCardInfo()
	{
		memset(this, 0, sizeof(*this));
	}
};

//////////////////////////////////////////////////////////////////////////
// 服务器命令结构

#define SUB_S_READY					100									// 准备消息
#define SUB_S_GAME_START			101									// 游戏开始
#define SUB_S_USER_TI_CARD			102									// 用户提牌
#define SUB_S_USER_PAO_CARD			103									// 用户跑牌
#define SUB_S_USER_WEI_CARD			104									// 用户偎牌
#define SUB_S_USER_PENG_CARD		105									// 用户碰牌
#define SUB_S_USER_CHI_CARD			106									// 用户吃牌
#define SUB_S_OPERATE_NOTIFY		107									// 操作提示
#define SUB_S_OUT_CARD_NOTIFY		108									// 出牌提示
#define SUB_S_OUT_CARD				109									// 用户出牌
#define SUB_S_SEND_CARD				110									// 发牌命令
#define SUB_S_GAME_END				111									// 游戏结束
#define SUB_S_USER_ADD_CARD			112									// 玩家补牌
#define SUB_S_USER_OFFLINE			113									// 玩家离线消息

// 准备命令			SUB_S_READY
struct CMD_S_UserReady
{
	BYTE							cbUserReady[PLAY_COUNT];			// 是否准备

	CMD_S_UserReady()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 游戏状态			// nGameStation==GS_WAIT_AGREE
struct CMD_S_StatusFree
{
	BYTE							cbGameStation;						// 游戏状态
	int								nCellScore;							// 基础积分
	int								nBankerUser;						// 庄家用户
	BYTE							cbUserReady[PLAY_COUNT];			// 是否准备

	CMD_S_StatusFree()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 游戏状态			// nGameStation==GS_PLAY_GAME
struct CMD_S_StatusPlay
{
	// 游戏信息
	BYTE							cbGameStation;						// 游戏状态
	int								nCellScore;							// 基础积分
	int								nBankerUser;						// 庄家用户
	int								nCurrentUser;						// 当前用户

																		// 出牌信息
	BYTE							bOutCard;							// 出牌标志(0不出牌,1出牌)
	int								nOutCardUser;						// 出牌用户
	BYTE							cbOutCardData;						// 出牌扑克

																		// 扑克信息
	BYTE							cbLeftCardCount;					// 剩余数目
	BYTE							cbCardData[MAX_COUNT];				// 用户扑克
	BYTE							cbOutCard[PLAY_COUNT][MAX_COUNT];	// 出牌扑克

																		// 组合信息
	BYTE							cbWeaveItemCount[PLAY_COUNT];				// 组合数目
	tagWeaveItem					WeaveItemArray[PLAY_COUNT][MAX_WEAVE];		// 组合扑克

																				// 动作信息
	BYTE							bResponse;							// 响应标志(是否已操作,0未操作,1已操作)
	BYTE							cbUserAction;						// 用户动作(玩家可操作类型)

	CMD_S_StatusPlay()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 游戏开始			SUB_S_GAME_START
struct CMD_S_GameStart
{
	int								nBankerUser;						// 庄家用户
	int								nCurrentUser;						// 当前用户
	BYTE							cbCardData[MAX_COUNT];				// 扑克列表

	CMD_S_GameStart()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 提牌命令			SUB_S_USER_TI_CARD
struct CMD_S_UserTiCard
{
	int								nActionUser;						// 动作用户
	BYTE							cbActionCard;						// 操作扑克
	BYTE							cbRemoveCount;						// 删除数目
	BYTE							cbHuXiCount;						// 胡息数目
	BYTE							cbWeiToTi;							// 是否偎变提,0提;不为0偎变提

	CMD_S_UserTiCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 跑牌命令			SUB_S_USER_PAO_CARD
struct CMD_S_UserPaoCard
{
	int								nActionUser;						// 动作用户
	BYTE							cbActionCard;						// 操作扑克
	BYTE							cbRemoveCount;						// 删除数目
	BYTE							cbHuXiCount;						// 胡息数目
	BYTE							cbWeiToPao;							// 是否偎变跑,0跑;不为0偎变跑

	CMD_S_UserPaoCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 偎牌命令			SUB_S_USER_WEI_CARD
struct CMD_S_UserWeiCard
{
	int								nActionUser;						// 动作用户
	BYTE							cbActionCard;						// 操作扑克
	BYTE							cbHuXiCount;						// 胡息数目

	CMD_S_UserWeiCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 碰牌命令			SUB_S_USER_PENG_CARD
struct CMD_S_UserPengCard
{
	int								nActionUser;						// 动作用户
	BYTE							cbActionCard;						// 操作扑克
	BYTE							cbHuXiCount;						// 胡息数目

	CMD_S_UserPengCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 吃牌命令			SUB_S_USER_CHI_CARD
struct CMD_S_UserChiCard
{
	int								nActionUser;						// 动作用户
	BYTE							cbActionCard;						// 操作扑克
	BYTE							cbResultCount;						// 结果数目
	BYTE							cbCardData[3][3];					// 吃牌组合
	BYTE							cbHuXiCount[3];						// 胡息数目

	CMD_S_UserChiCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 操作提示			SUB_S_OPERATE_NOTIFY
struct CMD_S_OperateNotify
{
	int								nResumeUser;						// 还原用户
	BYTE							cbActionCard;						// 操作扑克
	BYTE							cbOperateCode;						// 操作代码

	CMD_S_OperateNotify()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 出牌提示			SUB_S_OUT_CARD_NOTIFY
struct CMD_S_OutCardNotify
{
	BYTE							bOutCard;							// 出牌标志
	int								nCurrentUser;						// 当前用户

	CMD_S_OutCardNotify()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 用户出牌			SUB_S_OUT_CARD
struct CMD_S_OutCard
{
	int								nOutCardUser;						// 出牌用户
	BYTE							cbOutCardData;						// 出牌扑克

	CMD_S_OutCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 发牌命令			SUB_S_SEND_CARD
struct CMD_S_SendCard
{
	BYTE							cbCardData;							// 发牌扑克
	int								nAttachUser;						// 绑定用户

	CMD_S_SendCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 游戏结束			SUB_S_GAME_END
struct CMD_S_GameEnd
{
	// 结束信息
	BYTE							cbReason;							// 结束原因
	BYTE							cbHuCard;							// 胡牌扑克
	int								nWinUser;							// 胜利用户
	int								nProvideUser;						// 放炮用户

																		// 成绩变量
	int								nGameScore[PLAY_COUNT];				// 本局输赢游戏积分
	int								nGameTotalScore[PLAY_COUNT];		// 游戏总积分

																		//扑克变量
	BYTE							cbCardCount[PLAY_COUNT];			// 扑克数目
	BYTE							cbCardData[PLAY_COUNT][MAX_COUNT];	// 扑克列表

	BYTE							cbBottomCards[MAX_LEFT];			// 剩余底牌

	tagWeaveItem					tagWinnerCards[MAX_WEAVE];			// 赢家已展示牌
	BYTE							cbWeaveItemCount;					// 吃碰偎提等组合数量(手上组合数量)

	BYTE							cbHuPaiTypeCount;					// 胡牌类型数量
	BYTE							cbHuPaiType[HuPai_MaxHu];			// 胡牌类型
	BYTE							cbHuPaiBase[HuPai_MaxHu];			// 胡牌倍率

	int								nBase;								// 倍数

	CMD_S_GameEnd()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 玩家补牌			SUB_S_USER_ADD_CARD
struct CMD_S_UserAddCard
{
	int								nAddUser;							// 加牌用户
	BYTE							cbCardData;							// 发牌扑克
	BYTE							cbLeftCardCount;					// 剩余牌数量

	CMD_S_UserAddCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 玩家离线消息		SUB_S_USER_OFFLINE
struct CMD_S_User_Offline
{
	BYTE							cbOffLine[PLAY_COUNT];				// 离线(0在线,不为0离线)

	CMD_S_User_Offline()
	{
		memset(this, 0, sizeof(*this));
	}
};

//////////////////////////////////////////////////////////////////////////
// 客户端命令结构
#define SUB_C_READY					1									// 准备消息
#define SUB_C_OUT_CARD				2									// 出牌命令
#define SUB_C_OPERATE_CARD			3									// 操作扑克

// 出牌命令			SUB_C_OUT_CARD
struct CMD_C_OutCard
{
	BYTE							cbCardData;							// 扑克数据

	CMD_C_OutCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 操作命令			SUB_C_OPERATE_CARD
struct CMD_C_OperateCard
{
	int								nChiKind;							// 吃牌类型(7FFFFFFF),int4个字节每个字节表示1个吃操作
	BYTE							cbOperateCode;						// 操作代码

	CMD_C_OperateCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// 一个字节对齐
#pragma pack()

#endif		//__GAME_MESSAGE_H__