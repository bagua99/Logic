#ifndef __HUPAI_LOGIC_H__
#define __HUPAI_LOGIC_H__

#pragma once

#include <wtypes.h>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

// 小提胡息
#define TYPE_X_TI_HU_XI			9
// 大提胡息
#define TYPE_D_TI_HU_XI			12
// 小偎胡息
#define TYPE_X_WEI_HU_XI		3
// 大偎胡息
#define TYPE_D_WEI_HU_XI		6
// 小碰胡息
#define TYPE_X_PENG_HU_XI		1
// 大碰胡息
#define TYPE_D_PENG_HU_XI		3
// 小27A胡息
#define TYPE_X_27A_HU_XI		3
// 大27A胡息
#define TYPE_D_27A_HU_XI		6
// 小123胡息
#define TYPE_X_123_HU_XI		3
// 大123胡息
#define TYPE_D_123_HU_XI		6

// 规则类型
enum enPlayType
{
	enPlayType_NORMAL = 0,			// 一般
	enPlayType_XDD,					// 小大搭
	enPlayType_27A,					// 二七十
	enPlayType_123,					// 一二三
	enPlayType_Max,
};

// 设置规则
#define PLAY_TYPE	(1 + (1<<enPlayType_XDD) + (1<<enPlayType_27A) + (1<<enPlayType_123))

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
	vector<char>		vctCardIndex[21];	//21种牌
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

// 胡牌逻辑类
class CHuPaiLogic
{
public:
	CHuPaiLogic();
	~CHuPaiLogic();

public:
	// 初始化牌胡息
	void initCardHuXi();
	// 取得胡息
	int getCardHuXiVal(int nKey);
	// 取得胡息
	int getCardHuXiVal(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4 = 0x00);

	// 是否拥有规则
	bool isHavePlayType(enPlayType nType);
	// 胡息键值
	int getKey(BYTE cbCard1, BYTE cbCard2, BYTE cbCard3, BYTE cbCard4);
	// 增加胡息表
	void addMapVal(int nKey, BYTE nVal);

	// 取得提列表
	void getTiList();
	// 取得偎列表
	void getWeiList();
	// 取得27A列表
	void get27AList();
	// 取得顺子列表
	void getShunZhiList();

	// 获取胡牌结果
	void getCardHuResult(stCardData stData, stAnswer &answerData, stAnswer &resultMax);
	// 初始化胡牌
	bool initCardHu(vector<BYTE> &vctCardIn, stAnswer &resultAnswer, BYTE cbInsertCard, bool bSelfAddCard);

private:
	// 牌胡息表(key键值)
	typedef map<int, BYTE>		MAP_LIST;
	MAP_LIST					m_mapKey;

	// 是否胡牌错误
	bool						m_bHuError;
	// 自己摸牌
	bool						m_bSelfAddCard;
	// 插入牌
	BYTE						m_cbInsertCard;
};

//////////////////////////////////////////////////////////////////////////

#endif	// __HUPAI_LOGIC_H__