#pragma once

#include <Windows.h>
#include <iostream>

#include <algorithm>
#include <random>
#include <vector>
#include <list>
#include <map>

using namespace std;

const int MAX_NODE_COUNT = 1000000; // 10000 이면 MAX_NODE_INDEX = 4
const int MAX_NODE_INDEX = 6;

class CSkipNode
{
public:

    double      m_dScore;
    __int64     m_nValue;

    vector<list<list<CSkipNode*>::iterator>::iterator>  m_vecNext;

    CSkipNode()
    {
        m_dScore = 0;
        m_nValue = 0;
    }

    CSkipNode(double dScore = 0, __int64 nValue = 0)
    {
        m_dScore = dScore;
        m_nValue = nValue;
    }

    ~CSkipNode()
    {
    }
};

class CSkipList
{
public:

    list<CSkipNode*> m_lstNode;

    list<list<CSkipNode*>::iterator> m_lstIter[MAX_NODE_INDEX];

    CSkipList()
    {
    }

    ~CSkipList()
    {
    }

    bool Choice1()
    {
        if ((rand() + 1) % 2 == 0)
        {
            return false;
        }

        return true;
    }

    bool Choice2(int nListCount, int& rCheckCount)
    {
        rCheckCount = rCheckCount * 10;

        if (nListCount % rCheckCount != 0)
        {
            return false;
        }

        return true;
    }

    bool Init(list<CSkipNode>& rSortList)   // 외부에서 내림차순 정렬 된 데이터로 들어와야 한다..
    {   
        if (MAX_NODE_COUNT < rSortList.size())
        {
            return false;
        }

        srand((unsigned int)time(0));

        int nListCount = 0;
        
        for (auto iterList : rSortList)
        {        
            CSkipNode* pNode = new CSkipNode(iterList.m_dScore, iterList.m_nValue);

            m_lstNode.push_back(pNode);

            auto iterNode = --m_lstNode.end();

            m_lstIter[0].push_back(iterNode);

            (*iterNode)->m_vecNext.push_back(--m_lstIter[0].end());

            int nCheckCount = 1;

            for (int nIndex = 1; nIndex < MAX_NODE_INDEX; ++nIndex)
            {
                if (MAX_NODE_INDEX > 10)
                {
                    if (nListCount != 0 && Choice1() == false)
                    {
                        break;
                    }
                }
                else
                {
                    if (nListCount != 0 && Choice2(nListCount, nCheckCount) == false)
                    {
                        break;
                    }
                }

                m_lstIter[nIndex].push_back(iterNode);

                (*iterNode)->m_vecNext.push_back(--m_lstIter[nIndex].end());
            }

            ++nListCount;
        }

        return true;
    }

    int Search(double dScore, int& rCount, int nIndex = (MAX_NODE_INDEX - 1), list<list<CSkipNode*>::iterator>::iterator* pIter = nullptr)
    {
        int nRanking = 0;

        list<list<CSkipNode*>::iterator>::iterator iterPoint;

        if (pIter == nullptr)
        {
            iterPoint = m_lstIter[nIndex].begin();

            rCount = 0;
        }
        else
        {
            iterPoint = *pIter;
        }

        for (; iterPoint != m_lstIter[nIndex].end(); ++iterPoint)
        {
            //printf("Search Index,%d,%d,\n", nIndex, rCount);

            ++rCount;
            
            CSkipNode* pNode = (*(*iterPoint));

            if (pNode == nullptr)
            {
                return 0;
            }

            if (pNode->m_dScore == dScore)
            {
                printf("Find Score,%f,Index,%d,%d,\n", dScore, nIndex, rCount);

                return nRanking;
            }
    
            if (nIndex == 0)
            {
                continue;
            }

            ++iterPoint;

            auto iterNext = iterPoint;

            --iterPoint;

            double dNextScore = 0;

            if (iterNext != m_lstIter[nIndex].end())
            {
                dNextScore = (*(*iterNext))->m_dScore;
            }

            if (pNode->m_dScore >= dScore && dScore >= dNextScore)
            {
                int nSearch = Search(dScore, rCount, nIndex - 1, &pNode->m_vecNext[nIndex - 1]);

                if (nSearch == 0)
                {
                    return 0;
                }

                return nRanking + nSearch;
            }
        }

        return 0;
    }
};


