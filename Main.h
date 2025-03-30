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
        srand(GetTickCount());
    }

    ~CSkipList()
    {
    }

    bool Choice1(int nListCount, int& rCheckCount)
    {
        rCheckCount = rCheckCount * 10;

        if (nListCount % rCheckCount != 0)
        {
            return false;
        }

        return true;
    }

    bool Choice2()
    {
        if ((rand() + 1) % 2 == 0)
        {
            return false;
        }

        return true;
    }   

    bool Init(list<CSkipNode>& rSortList)   // 외부에서 내림차순 정렬 된 데이터로 들어와야 한다..
    {   
        if (MAX_NODE_COUNT < rSortList.size())
        {
            printf("Error,MAX_NODE_COUNT < rSortList.size(),\n");

            return false;
        }
        
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
                if (MAX_NODE_INDEX < 10)
                {
                    if (nListCount != 0 && Choice1(nListCount, nCheckCount) == false)
                    {
                        break;
                    }                  
                }
                else
                {
                    if (nListCount != 0 && Choice2() == false)
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
        if (dScore <= 0)
        {
            return 0;
        }

        list<list<CSkipNode*>::iterator>::iterator iterPoint;

        if (pIter == nullptr)
        {
            iterPoint = m_lstIter[nIndex].begin();

            if ((*(*iterPoint))->m_dScore < dScore)
            {
                return 1;
            }

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
                printf("Error,pNode == nullptr,\n");

                return 0;
            }

            if (pNode->m_dScore == dScore)
            {
                //printf("Find Score,%f,Index,%d,%d,\n", dScore, nIndex, rCount);
                //printf("Find Score,%f,Index,%d,%d,\n", dScore, nIndex, rCount);

                if (55 < rCount)
                {
                    printf("War,55 < rCount, Score,%f,Index,%d,%d,\n", dScore, nIndex, rCount);
                }

                return 1;
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
                    printf("Error,nSearch == 0,\n");

                    return 0;
                }

                return nSearch;
            }
        }

        printf("Error,return 0,\n");

        return 0;
    }
};

int Test()
{
    srand(GetTickCount());

    int nRand = (rand() * 10000 + rand()) % MAX_NODE_COUNT;

    CSkipList kList;

    list<CSkipNode> kNodeList;

    for (int nCount = nRand; nCount > 0; --nCount)
    {
        kNodeList.push_back(CSkipNode(nCount, nCount));
    }

    kList.Init(kNodeList);

    int nCount = 0;

    kList.Search(0, nCount);
    kList.Search(1, nCount);
    kList.Search(nRand, nCount);
    kList.Search(nRand + 1, nCount);

    printf("Rand,%d\n", nRand);

    while (true)
    {
        unsigned __int64 uTick = GetTickCount64();

        for (int nIndex = 0; nIndex < 10; ++nIndex)
        {
            nCount = 0;

            kList.Search((rand() * 10000 + rand()) % nRand, nCount);

            nCount = 0;

            kList.Search((rand() * 1000 + rand()) % nRand, nCount);

            nCount = 0;

            kList.Search((rand() * 100 + rand()) % nRand, nCount);

            nCount = 0;

            kList.Search(rand() % 1000, nCount);

            kList.Search(rand() % 10, nCount);
        }

        if (30 < (GetTickCount64() - uTick))
        {
            printf("Tick,%lld\n", GetTickCount64() - uTick);
        }

        Sleep(10);
    }

    return 0;
}


