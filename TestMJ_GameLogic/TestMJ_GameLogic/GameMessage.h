#ifndef GAME_MESSAGE_FILE
#define GAME_MESSAGE_FILE

#include <windows.h>

#pragma pack(push)	// ���浱ǰ���ֽڶ�����Ŀ
#pragma pack(1)		// �ֽڶ���

#define GAME_ID						20004200							// ��ϷID
#define GAME_PLAYER					4									// ��Ϸ����

#define MAX_WEAVE					4									// ������
#define MAX_WEAVE_CARD_COUNT		4									// ������������
#define MAX_ANSWER_WEAVE			5									// �������
#define MAX_INDEX					35									// �������
#define MAX_COUNT					14									// �����Ŀ
#define MAX_REPERTORY				136									// �����

//����ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//��Ч��ֵ
#define INVALID_BYTE				((BYTE)0xFF)						// ��Ч��ֵ
#define INVALID_WORD				((WORD)0xFFF)						// ��Ч��ֵ
#define INVALID_DWORD				((DWORD)0xFFFFFFFF)					// ��Ч��ֵ

//��Ч��ֵ
#define INVALID_TABLE				INVALID_WORD						// ��Ч����
#define INVALID_CHAIR				INVALID_WORD						// ��Ч����

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	MASK_COLOR					0xF0								// ��ɫ����
#define	MASK_VALUE					0x0F								// ��ֵ����

//�������
#define ACK_NULL					0x00								// û������
#define ACK_LEFT					0x01								// �������
#define ACK_CENTER					0x02								// �г�����
#define ACK_RIGHT					0x04								// �ҳ�����
#define ACK_PENG					0x08								// ��������
#define ACK_GANG					0x10								// ��������
#define ACKLISTEN					0x20								// ��������
#define ACK_CHIHU					0x40								// �Ժ�����
#define ACK_ZI_MO					0x80								// ����

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							byWeaveKind;						// �������
	BYTE							byCenterCard;						// �����˿�
	WORD							wProvideUser;						// ��Ӧ�û�

	CMD_WeaveItem()
	{
		memset(this, 0, sizeof(*this));
	}
};

//////////////////////////////////////////////////////////////////////////

//��������
struct tagKindItem
{
	BYTE							byWeaveKind;						// �������
	BYTE							byCenterCard;						// �����˿�
	BYTE							byCardIndex[3];						// �˿�����

	tagKindItem()
	{
		memset(this, 0, sizeof(*this));
	}
};

//�������
struct tagWeaveItem
{
	BYTE							byWeaveKind;						// �������
	BYTE							byCenterCard;						// �����˿�
	WORD							wProvideUser;						// ��Ӧ�û�

	tagWeaveItem()
	{
		memset(this, 0, sizeof(*this));
	}
};

// �����
struct tagWeaveCardItem
{
	BYTE							byCard[MAX_WEAVE_CARD_COUNT];		// �˿�

	tagWeaveCardItem()
	{
		memset(this, 0, sizeof(*this));
	}
};

//������Ϣ
struct tagGangInfo
{
	BYTE							byGangCardData;						// ����
	WORD							wPerformUser;						// ������
	WORD							wProvideUser;						// �ṩ��
	BYTE							bySendData;							// ����
	BYTE							byCurrentOutCount;					// ��ʱ������Ŀ

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

// ��������
struct tagHuInput
{
	BYTE*							pCardIndexSeq;						// ����
	bool							bInsertCurrentCard;					// �Ƿ�����ж�
	bool							bJudgeKingWait;						// �ж��׻���
	tagWeaveCardItem*				pWeaveCardItemSeq;					// �����
	BYTE							byWeaveCardItemCount;				// �������Ŀ
	BYTE							byCurrentCard;						// ��ǰ��

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