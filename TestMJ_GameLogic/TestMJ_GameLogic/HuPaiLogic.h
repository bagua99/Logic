#ifndef HUPAI_LOGIC_HEAD_FILE
#define HUPAI_LOGIC_HEAD_FILE

#include "GameMessage.h"
#include <unordered_map>
#include <map>
#include <algorithm>
#include <functional>
using namespace std;

// ����Ȩֵ
#define KING_DIAO_KEY	1
// ����(��������˳��)
#define FENG_PAI_KEY	1
// ����(�з���˳��)
#define JIAN_PAI_KEY	1

/////////////////////////////////////////////////////		CardKey			/////////////////////////////////////////////////////
// ��Ȩֵ��Ϣ
class CardKey
{
public:
	// ���캯��
	CardKey();

	// ��������
	~CardKey();

	// ���ع��캯��
	CardKey(BYTE byValue, BYTE byCardIndex0, BYTE byCardIndex1, BYTE byCardIndex2=MAX_INDEX, BYTE byCardIndex3=MAX_INDEX);

public:
	// ȡ������������
	BYTE GetCardIndexCount();

	// ȡ��������
	BYTE GetCardIndex(BYTE cbIndex);

	// ȡ��Ȩֵ
	BYTE GetKey();

private:
	// ������
	void Clear();

private:
	BYTE			m_byCardIndex[MAX_WEAVE_CARD_COUNT];	// ������
	BYTE			m_byCardIndexCount;						// ������
	BYTE			m_byKey;								// Ȩֵ(�ܺ��ӿ��Ե�����Ϣ����)
};
/////////////////////////////////////////////////////		CardKey			/////////////////////////////////////////////////////

/////////////////////////////////////////////////////		CardData		/////////////////////////////////////////////////////
// ��������Ϣ
class CardData
{
public:
	// ���캯��
	CardData();

	// ��������
	~CardData();

public:
	// ������
	void AddCard(BYTE byCardValue);

	// ȡ��ʣ��������
	BYTE *GetLeftIndex();

	// ȡ��ʣ������������
	BYTE GetLeftIndexCount();

	// ȡ��������
	BYTE GetCardIndex(BYTE byCardValue);

	// ȡ����������Ӧ��ֵ
	BYTE GetCardValue(BYTE byCardIndex);

	// ���ñ�־
	void SetFlag(BYTE byIndex, bool bFlag);

	// ȡ������������
	BYTE *GetCardIndexArray(BYTE byCardValue);

	// ȡ��������������ֵ
	BYTE GetCardIndexArrayValue(BYTE byCardValue, BYTE byCardCountIndex);

	// ȡ����������������
	BYTE GetCardIndexArrayCount(BYTE byCardValue);

	// ȡ������������(ͨ������)
	BYTE *GetCardIndexArrayInIndex(BYTE byCardIndex);

	// ȡ��������������ֵ(ͨ������)
	BYTE GetCardIndexArrayValueInIndex(BYTE byCardIndex, BYTE byCardCountIndex);

	// ȡ����������������(ͨ������)
	BYTE GetCardIndexArrayCountInIndex(BYTE byCardIndex);

private:
	// ������
	void Clear();

	// ȡ�ñ�־
	bool GetFlag(BYTE byIndex);

	// ȡ���Ƿ����
	bool Finish();

private:
	int				m_nFlag;												// ��־ֵ
	BYTE			m_byCard[MAX_COUNT];									// ������
	BYTE			m_byCardCount;											// ������
	BYTE			m_byCardIndex[MAX_INDEX][MAX_WEAVE_CARD_COUNT];			// ������
	BYTE			m_byCardIndexCount[MAX_INDEX];							// ����������
	BYTE			m_byLeftIndex[MAX_COUNT];								// ʣ��������(���)
	BYTE			m_byLeftIndexCount;										// ʣ������������(���)
	BYTE			m_byLeftCount;											// ʣ������
};
/////////////////////////////////////////////////////		CardData		/////////////////////////////////////////////////////

/////////////////////////////////////////////////////		CardAnswer		/////////////////////////////////////////////////////
// �ƴ���Ϣ
class CardAnswer
{
public:
	// ���캯��
	CardAnswer();

	// ��������
	~CardAnswer();

public:
	// ������
	void Clear();

	// ����������
	void AddCardData(CardKey *pKeyData, CardData *pCardData);

	// �Ƴ�������
	void RemoveCardData(CardData *pCardData);

	// ȡ����Ȩֵ��Ϣ
	CardKey *GetCardKey(BYTE nCardKeyIndex);

	// ȡ����Ȩֵ����
	BYTE GetCardKeyCount();

private:
	CardKey				m_CardKey[MAX_ANSWER_WEAVE];	// �����
	BYTE				m_byCardKeyCount;				// ���������
	BYTE				m_byKey;						// ��Ȩֵ

};
/////////////////////////////////////////////////////		CardAnswer		/////////////////////////////////////////////////////

/////////////////////////////////////////////////////		HuPaiLogic		/////////////////////////////////////////////////////
// �����߼�
class HuPaiLogic
{
public:
	// ���캯��
	HuPaiLogic();

	// ��������
	~HuPaiLogic();

public:
	// ��ʼ����Ȩֵ
	void InitCard();

	// ����ϼ�ֵ
	DWORD GetCardValue(BYTE byCard0, BYTE byCard1, BYTE byCard2, BYTE byCard3);

	// ��ȡ�����Ȩֵ
	int GetCardValueKey(BYTE byCard0, BYTE byCard1, BYTE byCard2, BYTE byCard3);

	// ��ȡ�����Ȩֵ
	int GetCardValueKey(DWORD dwKey);

	// ��ʼ������
	bool InitHuCard(CardData *pCardData, CardAnswer *pAnswer, BYTE byKingCard, BYTE byKingIndex, BYTE byJiangCount);

	// ��ú��ƽ��
	bool GetHuCardResult(CardData *pCardData, CardAnswer *pAnswer);

private:
	// ������
	void Clear();

	// ����Ȩֵ��(�����ֵΪkey)
	void AddKeyMap(int nCardValue, BYTE byKey);

	// ��ȡ��ͬ�����
	void GetThreeSameCard();

	// ��ȡ˳�������
	void GetStraightCard();

	// ��ȡ���������
	void GetOtherCard();

	// �˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex);

private:
	// ��Ȩֵ��
	typedef unordered_map<DWORD, BYTE>			MAP_KEY;
	MAP_KEY										m_mapKey;

	// ������
	BYTE										m_byJiangCount;
	// ����ֵ
	BYTE										m_byKingCard;
	// ��������
	BYTE										m_byKingIndex;
};
/////////////////////////////////////////////////////		HuPaiLogic		/////////////////////////////////////////////////////

#endif	// HUPAI_LOGIC_HEAD_FILE