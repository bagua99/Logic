#ifndef __GAME_MESSAGE_H__
#define __GAME_MESSAGE_H__

#include <string.h>

typedef BYTE* PBYTE;

// �ļ����ֶ���
#define GAMENAME						TEXT("���������")
#define GAME_ID							136
#define NAME_ID							30000136

// �汾����
#define GAME_MAX_VER					1									// ������߰汾
#define GAME_LESS_VER					1									// ������Ͱ汾
#define GAME_CHANGE_VER					0									// �޸İ汾

// ֧�����Ͷ���
#define SUP_NORMAL_GAME			        0x01								// ��ͨ��Ϸ
#define SUP_MATCH_GAME			        0x02								// ������Ϸ
#define SUP_MONEY_GAME			        0x04								// �����Ϸ

// ������汾
#define DEV_LIB_VER				       1									// ������汾

// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

// ��Ϸ��Ϣ
#define PLAY_COUNT						3									// ��Ϸ����
#define INVALID_CHAIR					(WORD(-1))

// ��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				1									// �ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					0									// �ڲ������Ͱ汾��

// ��Ϸ״̬����
#define GS_WAIT_AGREE					0									// �ȴ�ͬ������
#define GS_PLAY_GAME					21									// ��Ϸ��״̬

#define ASS_GM_GAME_STATION				2									// ��Ϸ״̬
#define ASS_MAIN_GAME					180							        // ��Ϸר����Ϣ��ID

// ��ֵ����
#define MAX_WEAVE					7										// ������
#define MAX_INDEX					20										// �������
#define MAX_COUNT					20										// �����Ŀ
#define MAX_REPERTORY				80										// �����
#define MAX_LEFT					22										// ���ʣ����
#define MIN_HU_XI					7										// ������С��Ϣ
#define MAX_WEAVE_HU_PAI			12										// ���������

#define	MASK_COLOR					0xF0									// ��ɫ����
#define	MASK_VALUE					0x0F									// ��ֵ����

// ��������
#define ACK_NULL					0x00									// ��
#define ACK_LIU						0x01									// ��
#define ACK_WAI						0x02									// ��
#define ACK_KAN						0x04									// ��
#define ACK_CHI						0x08									// ��
#define ACK_CHI_EX					0x10									// ��
#define ACK_PENG					0x20									// ��
#define ACK_CHIHU					0x40									// ��

// ��������
#define CK_NULL						0x00									// ��Ч����
#define CK_EQS						0x01									// ����ʮ��
#define CK_LEFT						0x02									// �������
#define CK_CENTER					0x04									// ���ж���
#define CK_RIGHT					0x08									// ���Ҷ���

// ��������
enum HuPai_Type
{
	HuPai_TianHu = 0,			// ���
	HuPai_DiHu,					// �غ�
	HuPai_PingHu,				// ƽ��
	HuPai_TingHu,				// ����
	HuPai_ShiSanHong,			// ʮ����
	HuPai_HeiHu,				// �ں���ȫ�ڣ�
	HuPai_YiDianHongHu,			// һ���
	HuPai_QuanDaHu,				// ȫ��
	HuPai_QuanXiaoHu,			// ȫС
	HuPai_WuDui,				// �޶�
	HuPai_ShiDui,				// ʮ��
	HuPai_YiDui,				// һ��
	HuPai_JiuDui,				// �Ŷ�
	HuPai_HangHangXi,			// ����Ϣ
	HuPai_DuiZiXi,				// ����Ϣ
	HuPai_HaiLao,				// ����
	HuPai_QuanQiuRen,			// ȫ����
	HuPai_MaxHu,				// ����
};

// ���Ʊ���
const BYTE nHuPaiBase[HuPai_MaxHu] =
{
	100,
	100,
	1,
	60,
	80,
	80,
	60,
	100,
	100,
	120,
	120,
	100,
	100,
	60,
	100,
	60,
	60
};

enum YYWHZ_OPTION1_WAIPAI
{
	YYWHZ_OPTION1_NULL = 0,
	YYWHZ_OPTION1_WAIPAI_ZI_DONG = 1,			// �Զ�����(������)
	YYWHZ_OPTION1_WAIPAI_SHOU_DONG = 2,			// �ֶ�����(��ѡ����)
};

// ��������
enum HAO_PAI_TYPE
{
	HAO_PAI_OTHER_WAI_HAO = 10,						// ���(��,�����˵����γɵĺ�)
	HAO_PAI_SELF_WAI_HAO = 20,						// ���(�����Ժ��Լ�����һ���γɵĺ�)
	HAO_PAI_NEI_HAO = 30,							// �ں�(����4���γɵĺ�)
};

#pragma pack(1)

/********************************************************************************/
// �������
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						// �������
	BYTE							cbCardCount;						// �˿���Ŀ
	BYTE							cbCenterCard;						// �����˿�
	BYTE							cbCardList[4];						// �˿��б�

	tagWeaveItem()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ������Ϣ
struct tagChiCardInfo
{
	BYTE							cbChiKind;							// ��������
	BYTE							cbCenterCard;						// �����˿�
	BYTE							cbResultCount;						// �����Ŀ
	BYTE							cbCardData[3][3];					// �������

	tagChiCardInfo()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ������Ϣ
struct tagHuCardInfo
{
	bool							bSelfAddCard;						// �Լ�����
	bool							bSpecialHu;							// �Ƿ��ж������
	bool							bOnlySpecialHu;						// �Ƿ�ֻ�ж������
	BYTE							cbWeaveCount;						// �����Ŀ
	tagWeaveItem					WeaveItemArray[MAX_WEAVE_HU_PAI];	// ����˿�

	tagHuCardInfo()
	{
		memset(this, 0, sizeof(*this));
	}
};

//////////////////////////////////////////////////////////////////////////
// ����������ṹ

#define SUB_S_READY					100									// ׼����Ϣ
#define SUB_S_USER_OFFLINE			101									// ���������Ϣ
#define SUB_S_GAME_START			110									// ��Ϸ��ʼ
#define SUB_S_USER_LIU_CARD			111									// �û�����
#define SUB_S_USER_WAI_CARD			112									// �û�����
#define SUB_S_USER_PENG_CARD		113									// �û�����
#define SUB_S_USER_CHI_CARD			114									// �û�����
#define SUB_S_OPERATE_NOTIFY		115									// ������ʾ
#define SUB_S_OUT_CARD_NOTIFY		116									// ������ʾ
#define SUB_S_OUT_CARD				117									// �û�����
#define SUB_S_SEND_CARD				118									// ��������
#define SUB_S_GAME_END				119									// ��Ϸ����

// ׼������			SUB_S_READY
struct CMD_S_UserReady
{
	BYTE							cbUserReady[PLAY_COUNT];			// �Ƿ�׼��

	CMD_S_UserReady()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ���������Ϣ		SUB_S_USER_OFFLINE
struct CMD_S_User_Offline
{
	BYTE							cbOffLine[PLAY_COUNT];				// ����(0����,��Ϊ0����)

	CMD_S_User_Offline()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��Ϸ״̬			// nGameStation==GS_WAIT_AGREE
struct CMD_S_StatusFree
{
	BYTE							cbGameStation;						// ��Ϸ״̬
	int								nCellScore;							// ��������
	int								nBankerUser;						// ׯ���û�
	BYTE							cbUserReady[PLAY_COUNT];			// �Ƿ�׼��

	CMD_S_StatusFree()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��Ϸ״̬			// nGameStation==GS_PLAY_GAME
struct CMD_S_StatusPlay
{
	// ��Ϸ��Ϣ
	BYTE							cbGameStation;						// ��Ϸ״̬
	int								nCellScore;							// ��������
	int								nBankerUser;						// ׯ���û�
	int								nCurrentUser;						// ��ǰ�û�

																		// ������Ϣ
	BYTE							bOutCard;							// ���Ʊ�־(0������,1����)
	int								nOutCardUser;						// �����û�
	BYTE							cbOutCardData;						// �����˿�

																		// �˿���Ϣ
	BYTE							cbLeftCardCount;					// ʣ����Ŀ
	BYTE							cbCardData[MAX_COUNT];				// �û��˿�
	BYTE							cbOutCard[PLAY_COUNT][MAX_COUNT];	// �����˿�

																		// �����Ϣ
	BYTE							cbWeaveItemCount[PLAY_COUNT];				// �����Ŀ
	tagWeaveItem					WeaveItemArray[PLAY_COUNT][MAX_WEAVE];		// ����˿�

																				// ������Ϣ
	BYTE							bResponse;							// ��Ӧ��־(�Ƿ��Ѳ���,0δ����,1�Ѳ���)
	BYTE							cbUserAction;						// �û�����(��ҿɲ�������)
	BYTE							cbDispatch;							// �Ƿ���(0����,1����)
	BYTE							cbLockCard[MAX_COUNT];				// �����˿�

	CMD_S_StatusPlay()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��Ϸ��ʼ			SUB_S_GAME_START
struct CMD_S_GameStart
{
	int								nBankerUser;						// ׯ���û�
	int								nCurrentUser;						// ��ǰ�û�
	BYTE							cbCardData[MAX_COUNT];				// �˿��б�

	CMD_S_GameStart()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��������			SUB_S_USER_LIU_CARD
struct CMD_S_UserLiuCard
{
	int								nActionUser;						// �����û�
	BYTE							cbActionCard;						// �����˿�
	BYTE							cbRemoveCount;						// ɾ����Ŀ
	BYTE							cbHuXiCount;						// ��Ϣ��Ŀ

	CMD_S_UserLiuCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��������			SUB_S_USER_WAI_CARD
struct CMD_S_UserWaiCard
{
	int								nActionUser;						// �����û�
	BYTE							cbActionCard;						// �����˿�
	BYTE							cbHuXiCount;						// ��Ϣ��Ŀ

	CMD_S_UserWaiCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��������			SUB_S_USER_PENG_CARD
struct CMD_S_UserPengCard
{
	int								nActionUser;						// �����û�
	BYTE							cbActionCard;						// �����˿�
	BYTE							cbHuXiCount;						// ��Ϣ��Ŀ

	CMD_S_UserPengCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��������			SUB_S_USER_CHI_CARD
struct CMD_S_UserChiCard
{
	int								nActionUser;						// �����û�
	BYTE							cbActionCard;						// �����˿�
	BYTE							cbResultCount;						// �����Ŀ
	BYTE							cbCardData[3][3];					// �������
	BYTE							cbHuXiCount[3];						// ��Ϣ��Ŀ

	CMD_S_UserChiCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ������ʾ			SUB_S_OPERATE_NOTIFY
struct CMD_S_OperateNotify
{
	int								nResumeUser;						// ��ԭ�û�
	BYTE							cbActionCard;						// �����˿�
	BYTE							cbOperateCode;						// ��������

	CMD_S_OperateNotify()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ������ʾ			SUB_S_OUT_CARD_NOTIFY
struct CMD_S_OutCardNotify
{
	BYTE							bOutCard;							// ���Ʊ�־
	int								nCurrentUser;						// ��ǰ�û�

	CMD_S_OutCardNotify()
	{
		memset(this, 0, sizeof(*this));
	}
};

// �û�����			SUB_S_OUT_CARD
struct CMD_S_OutCard
{
	int								nOutCardUser;						// �����û�
	BYTE							cbOutCardData;						// �����˿�

	CMD_S_OutCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��������			SUB_S_SEND_CARD
struct CMD_S_SendCard
{
	BYTE							cbCardData;							// �����˿�
	int								nAttachUser;						// ���û�

	CMD_S_SendCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��Ϸ����			SUB_S_GAME_END
struct CMD_S_GameEnd
{
	// ������Ϣ
	BYTE							cbReason;							// ����ԭ��
	BYTE							cbHuCard;							// �����˿�
	int								nWinUser;							// ʤ���û�
	int								nProvideUser;						// �����û�

																		// �ɼ�����
	int								nGameScore[PLAY_COUNT];				// ������Ӯ��Ϸ����
	int								nGameTotalScore[PLAY_COUNT];		// ��Ϸ�ܻ���

																		//�˿˱���
	BYTE							cbCardCount[PLAY_COUNT];			// �˿���Ŀ
	BYTE							cbCardData[PLAY_COUNT][MAX_COUNT];	// �˿��б�

	BYTE							cbBottomCards[MAX_LEFT];			// ʣ�����

	tagWeaveItem					tagWinnerCards[MAX_WEAVE_HU_PAI];	// Ӯ����չʾ��
	BYTE							cbWeaveItemCount;					// ����������������(�����������)

	BYTE							cbHuPaiTypeCount;					// ������������
	BYTE							cbHuPaiType[HuPai_MaxHu];			// ��������
	BYTE							cbHuPaiBase[HuPai_MaxHu];			// ���Ʊ���

	int								nBase;								// ����
	BYTE							cbAddScore[MAX_WEAVE];				// ���ӷ���(��,��,��)

	CMD_S_GameEnd()
	{
		memset(this, 0, sizeof(*this));
	}
};

//////////////////////////////////////////////////////////////////////////
// �ͻ�������ṹ
#define SUB_C_READY					1									// ׼����Ϣ
#define SUB_C_OUT_CARD				2									// ��������
#define SUB_C_OPERATE_CARD			3									// �����˿�

// ��������			SUB_C_OUT_CARD
struct CMD_C_OutCard
{
	BYTE							cbCardData;							// �˿�����

	CMD_C_OutCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��������			SUB_C_OPERATE_CARD
struct CMD_C_OperateCard
{
	int								nChiKind;							// ��������(7FFFFFFF),int4���ֽ�ÿ���ֽڱ�ʾ1���Բ���
	BYTE							cbOperateCode;						// ��������

	CMD_C_OperateCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// һ���ֽڶ���
#pragma pack()

#endif		//__GAME_MESSAGE_H__