#ifndef __GAME_MESSAGE_H__
#define __GAME_MESSAGE_H__

#include <string.h>

typedef BYTE* PBYTE;

// �ļ����ֶ���
#define GAMENAME						TEXT("��ɳ�ܺ���")
#define GAME_ID							134
#define NAME_ID							30000134

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
#define MAX_COUNT					21										// �����Ŀ
#define MAX_REPERTORY				80										// �����
#define MAX_LEFT					19										// ���ʣ����
#define MIN_HU_XI					15										// ������С��Ϣ

#define	MASK_COLOR					0xF0									// ��ɫ����
#define	MASK_VALUE					0x0F									// ��ֵ����

// ��������
#define ACK_NULL					0x00									// ��
#define ACK_TI						0x01									// ��
#define ACK_PAO						0x02									// ��
#define ACK_WEI						0x04									// ��
#define ACK_CHI						0x08									// ��
#define ACK_CHI_EX					0x10									// ��
#define ACK_PENG					0x20									// ��
#define ACK_CHIHU					0x40									// ��

// ��������
#define CK_NULL						0x00									// ��Ч����
#define CK_XXD						0x01									// СС���
#define CK_XDD						0x02									// С����
#define CK_EQS						0x04									// ����ʮ��
#define CK_LEFT						0x10									// �������
#define CK_CENTER					0x20									// ���ж���
#define CK_RIGHT					0x40									// ���Ҷ���

// ��������
enum HuPai_Type
{
	HuPai_TianHu = 0,				// ���
	HuPai_DiHu,						// �غ�
	HuPai_PengPengHu,				// ������
	HuPai_HeiHu,					// �ں�
	HuPai_ShiHong,					// ʮ���
	HuPai_YiDianHongHu,				// һ����
	HuPai_ShiBaDaHu,				// ʮ�˴��
	HuPai_ShiBaXiaoHu,				// ʮ��С��
	HuPai_ErBiHu,					// ���Ⱥ�
	HuPai_SanBiHu,					// ���Ⱥ�
	HuPai_SiBiHu,					// �ıȺ�
	HuPai_ShuangPiaoHu,				// ˫Ʈ��
	HuPai_MaxHu,					// ����
};

// ���Ʊ���
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
	CSPHZ_OPTION1_HU_BASE_LIMIT = 1,		// ��������
	CSPHZ_OPTION1_HU_BASE_NO_LIMIT = 2,		// ���Ʋ�����
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
#define SUB_S_GAME_START			101									// ��Ϸ��ʼ
#define SUB_S_USER_TI_CARD			102									// �û�����
#define SUB_S_USER_PAO_CARD			103									// �û�����
#define SUB_S_USER_WEI_CARD			104									// �û�����
#define SUB_S_USER_PENG_CARD		105									// �û�����
#define SUB_S_USER_CHI_CARD			106									// �û�����
#define SUB_S_OPERATE_NOTIFY		107									// ������ʾ
#define SUB_S_OUT_CARD_NOTIFY		108									// ������ʾ
#define SUB_S_OUT_CARD				109									// �û�����
#define SUB_S_SEND_CARD				110									// ��������
#define SUB_S_GAME_END				111									// ��Ϸ����
#define SUB_S_USER_ADD_CARD			112									// ��Ҳ���
#define SUB_S_USER_OFFLINE			113									// ���������Ϣ

// ׼������			SUB_S_READY
struct CMD_S_UserReady
{
	BYTE							cbUserReady[PLAY_COUNT];			// �Ƿ�׼��

	CMD_S_UserReady()
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

	tagWeaveItem					tagWinnerCards[MAX_WEAVE];			// Ӯ����չʾ��
	BYTE							cbWeaveItemCount;					// ����������������(�����������)

	BYTE							cbHuPaiTypeCount;					// ������������
	BYTE							cbHuPaiType[HuPai_MaxHu];			// ��������
	BYTE							cbHuPaiBase[HuPai_MaxHu];			// ���Ʊ���

	int								nBase;								// ����

	CMD_S_GameEnd()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��Ҳ���			SUB_S_USER_ADD_CARD
struct CMD_S_UserAddCard
{
	int								nAddUser;							// �����û�
	BYTE							cbCardData;							// �����˿�
	BYTE							cbLeftCardCount;					// ʣ��������

	CMD_S_UserAddCard()
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