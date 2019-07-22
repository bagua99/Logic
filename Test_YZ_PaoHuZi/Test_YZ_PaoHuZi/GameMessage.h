#ifndef __GAME_MESSAGE_H__
#define __GAME_MESSAGE_H__

#include <string.h>

typedef BYTE* PBYTE;

// �ļ����ֶ���
#define GAMENAME						TEXT("�����ܺ���")
#define GAME_ID							130
#define NAME_ID							30000130

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
#define MAX_WEAVE_CARD_COUNT		4										// ������������
#define MAX_INDEX					21										// �������
#define MAX_COUNT					21										// �����Ŀ
#define MAX_REPERTORY				80										// �����
#define MAX_LEFT					23										// ���ʣ����(���4��ʣ��23��)
#define MIN_HU_XI					15										// ������С��Ϣ
#define COMMOND_CARD_COUNT			20										// ����������

#define	MASK_COLOR					0xF0									// ��ɫ����
#define	MASK_VALUE					0x0F									// ��ֵ����

#define KING_VALUE					0xFF									// ����ֵ
#define KING_INDEX					MAX_INDEX-1								// ��������
#define KING_SCORE					5										// ��ׯһ����Ҫ��ķ���

// ��������
#define ACK_NULL					0x00									// ��
#define ACK_TI						0x01									// ��
#define ACK_PAO						0x02									// ��
#define ACK_WEI						0x04									// ��
#define ACK_WANG_DIAO				0x10									// ����
#define ACK_WANG_CHUANG				0x20									// ����
#define ACK_WANG_ZHA				0x40									// ��ը
#define ACK_CHI						0x100									// ��
#define ACK_CHI_EX					0x200									// ��
#define ACK_PENG					0x400									// ��
#define ACK_CHIHU					0x800									// ��
#define ACK_GUOPAI					0x1000									// ����

// ��������
#define CK_NULL						0x00									// ��Ч����
#define CK_XXD						0x01									// СС���
#define CK_XDD						0x02									// С����
#define CK_EQS						0x04									// ����ʮ��
#define CK_LEFT						0x10									// �������
#define CK_CENTER					0x20									// ���ж���
#define CK_RIGHT					0x40									// ���Ҷ���

// С���Ϣ
#define TYPE_X_TI_HU_XI			9
// �����Ϣ
#define TYPE_D_TI_HU_XI			12
// С�˺�Ϣ
#define TYPE_X_WEI_HU_XI		3
// ���˺�Ϣ
#define TYPE_D_WEI_HU_XI		6
// С����Ϣ
#define TYPE_X_PENG_HU_XI		1
// ������Ϣ
#define TYPE_D_PENG_HU_XI		3
// С27A��Ϣ
#define TYPE_X_27A_HU_XI		3
// ��27A��Ϣ
#define TYPE_D_27A_HU_XI		6
// С123��Ϣ
#define TYPE_X_123_HU_XI		3
// ��123��Ϣ
#define TYPE_D_123_HU_XI		6

// �����Ʊ�������
enum CheckHuBase_Type
{
	CheckHuBase_Nomal = 0x01,			// �����ͨ��
	CheckHuBase_WangDiao = 0x02,			// �������
	CheckHuBase_WangChuang = 0x04,			// �������
	CheckHuBase_WangZha = 0x08,			// �����ը
	CheckHuBase_Max = 0xFF,			// �����
};

// ��������
enum HuPai_Type
{
	HuPai_HongHu = 0,			// ���
	HuPai_YiDianHong,			// һ����
	HuPai_HeiHu,				// �ں�
	HuPai_ZiMo,					// ����
	HuPai_WangDiao,				// ����
	HuPai_WangDiaoWang,			// ������
	HuPai_WangChuang,			// ����
	HuPai_WangChuangWang,		// ������
	HuPai_WangZha,				// ��ը
	HuPai_WangZhaWang,			// ��ը��
	HuPai_MaxHu,				// ����
};

// ���Ʊ���
const BYTE nHuPaiBase[HuPai_MaxHu] =
{
	2,
	3,
	4,
	2,
	4,
	8,
	8,
	16,
	16,
	32
};

// ���Ʒ�������
enum YZPHZ_OPTION1_HU
{
	YZPHZ_OPTION1_HU_BASE_NULL = 0,
	YZPHZ_OPTION1_HU_BASE_LIMIT = 1,		// ��������
	YZPHZ_OPTION1_HU_BASE_NO_LIMIT = 2,		// ���Ʋ�����
};

#define HU_BASE_LIMIT_NUM	100				// �������޷���

// ������
enum YZPHZ_OPTION2_XING
{
	YZPHZ_OPTION2_XING_NULL = 0,			// Ĭ����
	YZPHZ_OPTION2_XING_FANG_XING = 1,		// ����
	YZPHZ_OPTION2_XING_GENG_XING = 2,		// ����
};

// ���ƼӶ�
enum YZPHZ_OPTION3_ADD_TUN
{
	YZPHZ_OPTION3_ADD_TUN_NULL = 0,			// Ĭ����
	YZPHZ_OPTION3_ADD_TUN_NORMAL = 1,		// ���Ӷ�(15Ϣһ����)
	YZPHZ_OPTION3_ADD_TUN_ONE = 2,			// ��1��(15Ϣ������)
};

// ������
enum YZPHZ_OPTION4_WANG_COUNT
{
	YZPHZ_OPTION4_WANG_COUNT_NULL = 0,		// ������(0��)
	YZPHZ_OPTION4_WANG_COUNT_2 = 1,			// ������(2��)
	YZPHZ_OPTION4_WANG_COUNT_4 = 2,			// ������(4��)
};

#pragma pack(1)

/********************************************************************************/
// �������
struct tagWeaveItem
{
	int								nWeaveKind;							// �������
	BYTE							cbCardCount;						// �˿���Ŀ
	BYTE							cbCenterCard;						// �����˿�
	BYTE							cbCardList[MAX_WEAVE_CARD_COUNT];	// �˿��б�

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

struct tagHuFinishInfo
{
	int								nHuScore;							// ���պ��Ʒ���
	BYTE							cbXingCard;							// ����ֵ
	BYTE							cbXingTunCount;						// һ������
	BYTE							cbHuBase;							// �������ձ���(����1��,����2��,������4��,����8��,������16��,��ը16��,��ը��32��)
	BYTE							cbHuHongHeiBase;					// ���ƺ�����ձ���
	BYTE							cbHuXi;								// �������պ�Ϣ
	BYTE							cbHuXingCount;						// ����������������
	BYTE							cbHongCardCount;					// �����������
	BYTE							cbHeiCardCount;						// �����������
	BYTE							cbAddTunCount;						// �������Ӷ���(15Ϣ��1��,0����,1����15Ϣ��2������)
	BYTE							cbInitialEyeCard[MAX_WEAVE_CARD_COUNT];	// ���۳�ʼ�б�
	BYTE							cbEyeCard[MAX_WEAVE_CARD_COUNT];	// �����б�
	BYTE							cbEyeCardCount;						// ��������
	BYTE							cbEyeCardHuXi;						// ���ۺ�Ϣ
	BYTE							cbWeaveCount;						// �����Ŀ
	tagWeaveItem					WeaveItemArray[MAX_WEAVE];			// ����˿�

	tagHuFinishInfo()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ������Ϣ
struct tagHuCardInfo
{
	bool							bSelfAddCard;						// �Լ�����
	bool							bOnlyHu;							// ֻ�жϺ�(��������˳�)
	BYTE							cbCheckHuBaseType;					// ����������
	BYTE							cbInsertCard;						// �������
	tagHuFinishInfo					HuFinish;							// ���ƽ�����Ϣ
	BYTE							cbWeaveCount;						// �����Ŀ
	tagWeaveItem					WeaveItemArray[MAX_WEAVE];			// ����˿�

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
#define SUB_S_USER_TI_CARD			111									// �û�����
#define SUB_S_USER_PAO_CARD			112									// �û�����
#define SUB_S_USER_WEI_CARD			113									// �û�����
#define SUB_S_USER_PENG_CARD		114									// �û�����
#define SUB_S_USER_CHI_CARD			115									// �û�����
#define SUB_S_OPERATE_NOTIFY		116									// ������ʾ
#define SUB_S_OUT_CARD_NOTIFY		117									// ������ʾ
#define SUB_S_OUT_CARD				118									// �û�����
#define SUB_S_SEND_CARD				119									// ��������
#define SUB_S_GAME_END				120									// ��Ϸ����
#define SUB_S_WANG_OPERATE			121									// ����,����,��ը�������
#define SUB_S_USER_PASS_CARD		122									// ��ҹ���
#define SUB_S_USER_UNABLE_HU		123									// ��Ҳ��ܺ�(����)

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
	int								nUserAction;						// �û�����(��ҿɲ�������)

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

// ��������			SUB_S_USER_TI_CARD
struct CMD_S_UserTiCard
{
	int								nActionUser;						// �����û�
	BYTE							cbActionCard;						// �����˿�
	BYTE							cbRemoveCount;						// ɾ����Ŀ
	BYTE							cbHuXiCount;						// ��Ϣ��Ŀ
	BYTE							cbWeiToTi;							// �Ƿ��˱���,0��;��Ϊ0�˱���

	CMD_S_UserTiCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��������			SUB_S_USER_PAO_CARD
struct CMD_S_UserPaoCard
{
	int								nActionUser;						// �����û�
	BYTE							cbActionCard;						// �����˿�
	BYTE							cbRemoveCount;						// ɾ����Ŀ
	BYTE							cbHuXiCount;						// ��Ϣ��Ŀ
	BYTE							cbWeiToPao;							// �Ƿ��˱���,0��;��Ϊ0�˱���

	CMD_S_UserPaoCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��������			SUB_S_USER_WEI_CARD
struct CMD_S_UserWeiCard
{
	int								nActionUser;						// �����û�
	BYTE							cbActionCard;						// �����˿�
	BYTE							cbHuXiCount;						// ��Ϣ��Ŀ
	BYTE							cbChouWei;							// �Ƿ����(0��,��Ϊ0�ǳ���)

	CMD_S_UserWeiCard()
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
	int								nOperateCode;						// ��������

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
	BYTE							cbXingCardData;						// ������

	tagWeaveItem					tagWinnerCards[MAX_WEAVE];			// Ӯ����չʾ��
	BYTE							cbWeaveItemCount;					// ����������������(�����������)

	BYTE							cbHuPaiTypeCount;					// ������������
	BYTE							cbHuPaiType[HuPai_MaxHu];			// ��������
	BYTE							cbHuPaiBase[HuPai_MaxHu];			// ���Ʊ���

	BYTE							cbKingChangeCard[MAX_WEAVE_CARD_COUNT];				// ��ת������ֵ(��Ϊ0����)
	BYTE							cbXingCount;						// ��������

	BYTE							cbHuXi[MAX_WEAVE];					// ��Ӧ��Ϣ

	CMD_S_GameEnd()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ����,����,��ը�������			SUB_S_WANG_OPERATE
struct CMD_S_WangOperate
{
	int								nCurrentUser;						// ��ǰ�û�
	int								nTargetAction;						// ��������
	BYTE							cbCardData;							// �����˿�

	CMD_S_WangOperate()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��ҹ���			SUB_S_USER_PASS_CARD
struct CMD_S_UserPassCard
{
	int								nCurrentUser;						// ��ǰ�û�

	CMD_S_UserPassCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��Ҳ��ܺ�(����)			SUB_S_USER_UNABLE_HU
struct CMD_S_UnableHu
{
	int								nCurrentUser;						// ��ǰ�û�

	CMD_S_UnableHu()
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
	int								nOperateCode;						// ��������

	CMD_C_OperateCard()
	{
		memset(this, 0, sizeof(*this));
	}
};

// һ���ֽڶ���
#pragma pack()

#endif		//__GAME_MESSAGE_H__