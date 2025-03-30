#pragma once

#include <Windows.h>
#include <iostream>

#include <algorithm>
#include <random>
#include <vector>
#include <list>
#include <map>

using namespace std;

const int MAX_NODE_COUNT = 1000;
const int MAX_NODE_INDEX = 6;

const double MAX_NODE_SCORE = 1000000000000;
const double MIN_NODE_SCORE = 0;

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

    bool ChoiceInit(int nListCount, int& rCheckCount)
    {
        rCheckCount = rCheckCount * 10;

        if (nListCount % rCheckCount != 0)
        {
            return false;
        }

        return true;
    }

    bool ChoiceInsert()
    {
        if ((rand() + 1) % 2 == 0)
        {
            return false;
        }

        return true;
    }   

    bool Test(list<CSkipNode>& rSortList)   // 외부에서 내림차순 정렬 된 데이터로 들어와야 한다..
    {       
        rSortList.push_front(CSkipNode(MAX_NODE_SCORE, -1) );
        rSortList.push_back(CSkipNode(MIN_NODE_SCORE, -2));

        int nListCount = 0;
        
        for (auto iterList : rSortList)
        {        
            if (iterList.m_dScore <= MIN_NODE_SCORE && MAX_NODE_SCORE <= iterList.m_dScore)
            {
                printf("Error,MIN_NODE_SCORE && MAX_NODE_SCORE,%f,%lld,\n", iterList.m_dScore, iterList.m_nValue);

                continue;
            }

            CSkipNode* pNode = new CSkipNode(iterList.m_dScore, iterList.m_nValue);

            m_lstNode.push_back(pNode);

            auto iterNode = --m_lstNode.end();

            m_lstIter[0].push_back(iterNode);

            (*iterNode)->m_vecNext.push_back(--m_lstIter[0].end());

            int nCheckCount = 1;

            for (int nIndex = 1; nIndex < MAX_NODE_INDEX; ++nIndex)
            {
                if (nListCount != 0 && ChoiceInit(nListCount, nCheckCount) == false)
                {
                    break;
                }

                m_lstIter[nIndex].push_back(iterNode);

                (*iterNode)->m_vecNext.push_back(--m_lstIter[nIndex].end());
            }

            ++nListCount;
        }

        return true;
    }

    void Init()
    {       
        for (int nCount = 1; nCount < 3; ++nCount)  // Max, Min Insert
        {
            double dScore = MAX_NODE_SCORE;

            if (nCount != 1)
            {
                dScore = MIN_NODE_SCORE;
            }

            CSkipNode* pNode = new CSkipNode(dScore, -nCount);

            m_lstNode.push_back(pNode);

            auto iterNode = --m_lstNode.end();

            m_lstIter[0].push_back(iterNode);

            (*iterNode)->m_vecNext.push_back(--m_lstIter[0].end());

            for (int nIndex = 1; nIndex < MAX_NODE_INDEX; ++nIndex)
            {
                m_lstIter[nIndex].push_back(iterNode);

                (*iterNode)->m_vecNext.push_back(--m_lstIter[nIndex].end());
            }
        }
    }

    bool Update(CSkipNode* pNode, int& rSerchCount)
    {
        if (pNode == nullptr)
        {
            printf("pNode == nullptr,\n");

            return false;
        }

        if (pNode->m_dScore <= MIN_NODE_SCORE && MAX_NODE_SCORE <= pNode->m_dScore)
        {
            printf("Error,MIN_NODE_SCORE && MAX_NODE_SCORE,%f,%lld,\n", pNode->m_dScore, pNode->m_nValue);

            return false;
        }

        // 삭제 추가 필요

        return Search(pNode, rSerchCount, true);
    }

    bool Search(CSkipNode* pNode, int& rSerchCount, bool bInsert = false, int nIndex = (MAX_NODE_INDEX - 1), list<list<CSkipNode*>::iterator>::iterator* pIterPoint = nullptr)
    {
        if (pNode == nullptr)
        {
            printf("pNode == nullptr,\n");

            return false;
        }

        double dScore = pNode->m_dScore;
        __int64 nValue = pNode->m_nValue;

        if (dScore <= MIN_NODE_SCORE || MAX_NODE_SCORE <= dScore)
        {
            printf("MIN_NODE_SCORE || MAX_NODE_SCORE,\n");
            //dScore = rand() % 10; // 임시 테스트..
            return false;
        }

        if (nValue <= 0)
        {
            printf("nValue <= 0,\n");

            return false;
        }

        if (nIndex < 0)
        {
            printf("nIndex < 0,\n");

            return false;
        }

        list<list<CSkipNode*>::iterator>::iterator iterPoint;

        if (pIterPoint == nullptr)
        {
            iterPoint = m_lstIter[nIndex].begin();

            rSerchCount = 0;
        }
        else
        {
            iterPoint = *pIterPoint;
        }

        for (; iterPoint != m_lstIter[nIndex].end(); ++iterPoint)
        {
            //printf("Search Index,%d,%d,\n", nIndex, rSerchCount);

            ++rSerchCount;
            
            CSkipNode* pPoint = (*(*iterPoint));

            if (pPoint == nullptr)
            {
                printf("Error,pPoint == nullptr,\n");

                return false;
            }

            ++iterPoint;

            auto iterNext = iterPoint;

            --iterPoint;

            double dNextScore = 0;

            if (iterNext != m_lstIter[nIndex].end())
            {
                dNextScore = (*(*iterNext))->m_dScore;
            }
   
            if (pPoint->m_dScore >= dScore && dScore >= dNextScore)
            {
                if (0 < nIndex)
                {
                    return Search(pNode, rSerchCount, bInsert, nIndex - 1, &pPoint->m_vecNext[nIndex - 1]);
                }

                if (pPoint->m_dScore == dScore && pPoint->m_nValue == nValue)
                {
                    printf("Find Score,%f,Value,%lld,Index,%d,Count,%d,\n", dScore, nValue, nIndex, rSerchCount);

                    if (100 < rSerchCount)
                    {
                        printf("War,100 < rSerchCount,Score,%f,Value,%lld,Index,%d,Count,%d,\n", dScore, nValue, nIndex, rSerchCount);
                    }

                    return true;
                }

                if (bInsert == true && dScore > dNextScore)
                {
                    auto iterInsert = m_lstNode.insert((*iterNext), pNode);

                    m_lstIter[nIndex].insert(iterNext, iterInsert);

                    return true;
                }

                continue;
            }
        }

        printf("Error,return false,\n");

        return false;
    }
};

bool compareCSkipNode(const CSkipNode& a, const CSkipNode& b) 
{
    return a.m_dScore > b.m_dScore; // 내림차순 정렬
}

int Test()
{
    srand(GetTickCount());

    CSkipList kList;

    kList.Init();

    int nRand = MAX_NODE_COUNT - ( rand() % 100 );
    int nSearchCount = 0;
    
    for (int nCount = nRand; nCount > 0; --nCount)
    {
        kList.Update(new CSkipNode(nCount, nCount), nSearchCount);
    }

    kList.Update(new CSkipNode(88, 77), nSearchCount);
    kList.Update(new CSkipNode(88, 66), nSearchCount);
    kList.Update(new CSkipNode(55, 44), nSearchCount);

    printf("List Size,%lld\n", kList.m_lstNode.size());

    CSkipNode kNode(88, 77);
    kList.Search(&kNode, nSearchCount);

    nSearchCount = 0;

    /*
    list<CSkipNode> kNodeList;

    for (int nCount = nRand; nCount > 0; --nCount)
    {
        kNodeList.push_back(CSkipNode(nCount, nCount));
    }

    kNodeList.push_back(CSkipNode(88, 77));
    kNodeList.push_back(CSkipNode(88, 66));
    kNodeList.push_back(CSkipNode(55, 44));

    kNodeList.sort(compareCSkipNode);
  
    kList.Test(kNodeList);

    printf("List Size,%lld\n", kList.m_lstNode.size());

    kList.Search(88, 77, nSearchCount);
    kList.Search(88, 88, nSearchCount);
    kList.Search(88, 66, nSearchCount);
    kList.Search(55, 55, nSearchCount);
    kList.Search(55, 44, nSearchCount);

    kList.Search(1, 1, nSearchCount);   
    kList.Search(nRand, nRand, nSearchCount);

    nSearchCount = 0;

    while (true)
    {
        srand(GetTickCount());

        unsigned __int64 uTick = GetTickCount64();

        int nSerchCount = 0;
        int nIndexCount = 0;
        int nMaxCount = 0;
        int nMinCount = 100000000;

        for (int nIndex = 0; nIndex < 2000; ++nIndex)
        {          
            nCount = 0;

            kList.Search((rand() + nRand) % nRand, nCount);

            if (nMaxCount < nCount)
            {
                nMaxCount = nCount;
            }

            if (nMinCount > nCount)
            {
                nMinCount = nCount;
            }

            ++nIndexCount;

            nSerchCount += nCount;

            nCount = 0;

            kList.Search((rand() + nRand) % 100000, nCount);

            if (nMaxCount < nCount)
            {
                nMaxCount = nCount;
            }

            if (nMinCount > nCount)
            {
                nMinCount = nCount;
            }

            ++nIndexCount;

            nSerchCount += nCount;

            nCount = 0;

            kList.Search((rand() + nRand) % 10000, nCount);

            if (nMaxCount < nCount)
            {
                nMaxCount = nCount;
            }

            if (nMinCount > nCount)
            {
                nMinCount = nCount;
            }

            ++nIndexCount;

            nSerchCount += nCount;

            nCount = 0;

            kList.Search((rand() + nRand) % 1000, nCount);

            if (nMaxCount < nCount)
            {
                nMaxCount = nCount;
            }

            if (nMinCount > nCount)
            {
                nMinCount = nCount;
            }

            ++nIndexCount;

            nSerchCount += nCount;

            nCount = 0;

            kList.Search((rand() + nRand) % 100, nCount);

            if (nMaxCount < nCount)
            {
                nMaxCount = nCount;
            }

            if (nMinCount > nCount)
            {
                nMinCount = nCount;
            }

            ++nIndexCount;

            nSerchCount += nCount;
        }

        printf("Check,Count,%d,Search,%d,Avg,%d,Min,%d,Max,%d,Tick,%lld\n", nIndexCount, nSerchCount, nSerchCount / nIndexCount, nMinCount, nMaxCount, GetTickCount64() - uTick);

        Sleep(1000);
    }
*/
    return 0;
}


