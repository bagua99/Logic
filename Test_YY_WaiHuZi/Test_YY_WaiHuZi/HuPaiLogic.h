#ifndef __HUPAI_LOGIC_H__
#define __HUPAI_LOGIC_H__
#include <wtypes.h>
#include <vector>
#include <map>
#include <algorithm>
#include "GameMessage.h"
using namespace std;

//��������
enum enPlayType
{
	enPlayType_NORMAL = 0,			// һ��
	enPlayType_XDD,					// С���
	enPlayType_27A,					// ����ʮ
	enPlayType_123,					// һ����
	enPlayType_Max,
};

#define TY_MY	(1 + (1<<enPlayType_27A))

struct stHuKeyData
{
	bool bKey;
	int nHuKey;
	vector<char> vctIndex;

	stHuKeyData()
	{
		nHuKey = 0;
		bKey = false;
	}
	stHuKeyData(int key, char index1, char index2, char index3 = -1, char index4 = -1, bool bK = false)
	{
		nHuKey = key;
		bKey = bK;
		vctIndex.push_back(index1);
		vctIndex.push_back(index2);
		if (index3 >= 0)
			vctIndex.push_back(index3);
		if (index4 >= 0)
			vctIndex.push_back(index4);
	}
};


struct stCardData
{
	stCardData() : nFlag(0), nLeft(0)
	{
	}

	void push(char val)
	{
		char n = getCardNumIndex(val);
		if (n >= 0 && n < 21)
		{
			vctCardIndex[n].push_back(vctCard.size());
		}
		vctCard.push_back(val);
		nLeft++;
	}

	BYTE getVal(BYTE index)
	{
		return vctCard[index];
	}

	size_t size()
	{
		return vctCard.size();
	}

	char getLeft()
	{
		return nLeft;
	}

	vector<char> getLeftIndex()
	{
		vector<char> vctLeftIndex;
		size_t nSize = vctCard.size();
		for (size_t i = 0; i < nSize; ++i)
		{
			if (!getFlag(i))
			{
				vctLeftIndex.push_back(i);
			}
		}
		return vctLeftIndex;
	}

	void setFlag(char index, bool bFlag)
	{
		if (index < (char)vctCard.size())
		{
			if (bFlag)
			{
				if (getFlag(index) == false)
				{
					char n = getCardNumIndex(vctCard[index]);
					if (n >= 0 && n < 21)
					{
						vector<char>::iterator pos = find(vctCardIndex[n].begin(), vctCardIndex[n].end(), index);
						if (pos != vctCardIndex[n].end())
							vctCardIndex[n].erase(pos);
					}
					--nLeft;
					nFlag |= (1 << index);
				}
			}
			else
			{
				if (getFlag(index))
				{
					char n = getCardNumIndex(vctCard[index]);
					if (n >= 0 && n < 21)
					{
						vctCardIndex[n].push_back(index);
					}
					++nLeft;
					nFlag &= ~(1 << index);
				}
			}
		}
	}

	bool getFlag(char index)
	{
		return (nFlag &(1 << index)) != 0;
	}

	bool isFinised()
	{
		int nSize = vctCard.size();
		int nFlagAll = ((1 << (nSize)) - 1);
		return (nFlagAll & nFlag) == nFlagAll;
	}
	vector<char>& getcardsNumByVal(char val)
	{
		return vctCardIndex[getCardNumIndex(val)];
	}

private:
	char getCardNumIndex(char val)
	{
		if (val <= 0)
			return 20;
		else if (val <= 0x0A)
			return val - 1;
		else if (val >= 0x10 && val <= 0x1A)
			return 10 + val - 0x11;

		return -1;
	}

private:
	vector<char>		vctCard;
	int					nFlag;
	char				nLeft;
	vector<char>		vctCardIndex[21];	//21����
};

struct stAnswer
{
	char				nHuVal;
	vector<stHuKeyData> vctData;

	void push(stHuKeyData stKey, stCardData *pVctCard = NULL)
	{
		nHuVal += stKey.nHuKey;
		vctData.push_back(stKey);
		if (pVctCard != NULL)
		{
			vector<char> &vIndex = stKey.vctIndex;
			for (size_t i = 0; i < vIndex.size(); ++i)
			{
				(*pVctCard).setFlag(vIndex[i], true);
			}
		}
	}

	void pop(stCardData *pVctCard = NULL)
	{
		int nSize = vctData.size();
		if (pVctCard != NULL && nSize > 0)
		{
			vector<char> &vIndex = vctData[nSize - 1].vctIndex;
			for (size_t i = 0; i < vIndex.size(); ++i)
			{
				(*pVctCard).setFlag(vIndex[i], false);
			}
		}
		nHuVal -= vctData[vctData.size() - 1].nHuKey;
		vctData.pop_back();
	}

	stAnswer()
	{
		nHuVal = 0;
	}

	void clear()
	{
		nHuVal = 0;
		vctData.clear();
	}

	char getHuPer()
	{
		return nHuVal;
	}
};

//////////////////////////////////////////////////////////////////////////

// �����߼���
class CHuPaiLogic
{
public:
	CHuPaiLogic();
	~CHuPaiLogic();

public:
	// ��ʼ���ƺ�Ϣ
	void initCardHuXi();
	// ȡ���ƺ�Ϣ
	int getCardHuXiVal(int nKey);
	// ȡ���ƺ�Ϣ
	int getCardHuXiVal(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4 = 0x00);

	// �Ƿ��������
	bool isHavePlayType(enPlayType nType);
	// ��ֵ
	int getKey(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4);
	// map��
	void addMapVal(int nKey, BYTE nVal);

	// ��
	void getTiList();
	// ����
	void getWeiList();
	// ����ʮ
	void get27AList();
	// ˳��
	void getShunZhiList();

	// ��ʼ������
	bool initCardHu(vector<BYTE> &vctCardIn, stAnswer &resultAnswer, BYTE cbInsertCard, bool bSelfAddCard);
	// ��ȡ����
	void getCardHuResult(stCardData stData, stAnswer &answerData, stAnswer &resultMax);

private:
	// �ƺ�Ϣ��(key��ֵ)
	typedef map<int, BYTE>		MAP_LIST;
	MAP_LIST					m_mapKey;

	// �Լ�����
	bool						m_bSelfAddCard;
	// ������
	BYTE						m_cbInsertCard;
	// ������
	BYTE						m_cbMenCount;
};

//////////////////////////////////////////////////////////////////////////
#endif	// __HUPAI_LOGIC_H__