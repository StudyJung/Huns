#pragma once

#include <Windows.h>
#include <iostream>

#include <algorithm>
#include <random>
#include <vector>
#include <list>
#include <map>

using namespace std;

const int MAX_NODE_COUNT    = 1000000;
const int MAX_NODE_INDEX    = 17;
const int WAR_SEARCH_COUNT  = MAX_NODE_INDEX * 10;

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

    map<__int64, CSkipNode*>            m_mapNode;
    list<CSkipNode*>                    m_lstNode;
    list<list<CSkipNode*>::iterator>    m_lstIter[MAX_NODE_INDEX];

    CSkipList()
    {
        srand(GetTickCount());
    }

    ~CSkipList()
    {
    }

    bool RandomNext()
    {
        if ((rand() + 1) % 2 == 0)
        {
            return false;
        }

        return true;
    }   

    void Init()
    {
        for (int nCount = 1; nCount < 3; ++nCount)
        {
            double dScore = MAX_NODE_SCORE;

            if (nCount != 1)
            {
                dScore = MIN_NODE_SCORE;
            }

            CSkipNode* pNode = new CSkipNode(dScore, -nCount);

            m_mapNode.insert(make_pair(pNode->m_nValue, pNode));

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

    bool Init(list<CSkipNode>& rSortList)   // 외부에서 내림차순 정렬 된 데이터로 들어와야 한다..
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

            m_mapNode.insert(make_pair(pNode->m_nValue, pNode));

            m_lstNode.push_back(pNode);

            auto iterNode = --m_lstNode.end();

            m_lstIter[0].push_back(iterNode);

            (*iterNode)->m_vecNext.push_back(--m_lstIter[0].end());

            int nCheckCount = 1;

            for (int nIndex = 1; nIndex < MAX_NODE_INDEX; ++nIndex)
            {
                if (0 < pNode->m_nValue && RandomNext() == false)
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

    bool Delete(CSkipNode* pNode, int& rSerchCount)
    {
        if (pNode == nullptr)
        {
            printf("Error,pNode == nullptr,\n");

            return false;
        }

        ++rSerchCount;

        int nIndex = 0;

        for (auto iterNext : pNode->m_vecNext)
        {
            if (nIndex == 0)
            {
                m_lstNode.erase(*iterNext);
            }

            m_lstIter[nIndex].erase(iterNext);

            ++nIndex;
        }

        pNode->m_vecNext.clear();

        return true;
    }

    bool Select(CSkipNode* pNode, int& rSerchCount, bool bInsert = false, int nIndex = (MAX_NODE_INDEX - 1),
        list<list<CSkipNode*>::iterator>::iterator* pIterPoint = nullptr, list<list<list<CSkipNode*>::iterator>::iterator>* pIterPointList = nullptr)
    {
        if (pNode == nullptr)
        {
            printf("Error,pNode == nullptr,\n");

            return false;
        }

        double dScore = pNode->m_dScore;
        __int64 nValue = pNode->m_nValue;

        if (dScore <= MIN_NODE_SCORE || MAX_NODE_SCORE <= dScore)
        {
            printf("Error,MIN_NODE_SCORE || MAX_NODE_SCORE,\n");
            
            return false;
        }

        if (nValue <= 0)
        {
            printf("Error,nValue <= 0,\n");

            return false;
        }

        if (nIndex < 0)
        {
            printf("Error,nIndex < 0,\n");

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
        
        if (bInsert == true && pIterPointList == nullptr)
        {
            pIterPointList = new list<list<list<CSkipNode*>::iterator>::iterator>;
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
                    if (bInsert == true)
                    {
                        pIterPointList->push_front(iterPoint);
                    }

                    return Select(pNode, rSerchCount, bInsert, nIndex - 1, &pPoint->m_vecNext[nIndex - 1], pIterPointList);
                }

                if (pPoint->m_dScore == dScore && pPoint->m_nValue == nValue)
                {
                    //printf("Find Score,%f,Value,%lld,Index,%d,Count,%d,\n", dScore, nValue, nIndex, rSerchCount);

                    if (nIndex == 0 && WAR_SEARCH_COUNT < rSerchCount)
                    {
                        printf("War,WAR_SEARCH_COUNT < rSerchCount,Score,%f,Value,%lld,Index,%d,Count,%d,\n", dScore, nValue, nIndex, rSerchCount);
                    }

                    return true;
                }

                if (bInsert == true && dScore > dNextScore)
                {           
                    auto iterInsertNode = m_lstNode.insert((*iterNext), pNode);
                 
                    auto iterInsertPoint = m_lstIter[nIndex].insert(iterNext, iterInsertNode);

                    pNode->m_vecNext.push_back(iterInsertPoint);

                    for (auto iterInsertNext : *pIterPointList)
                    {
                        if (RandomNext() == false)
                        {
                            break;
                        }

                        auto iterInsertPoint = m_lstIter[++nIndex].insert(++iterInsertNext, iterInsertNode);

                        pNode->m_vecNext.push_back(iterInsertPoint);
                    }

                    if (pIterPointList != nullptr)
                    {
                        delete pIterPointList;

                        pIterPointList = nullptr;
                    }

                    if (nIndex == 0 && WAR_SEARCH_COUNT < rSerchCount)
                    {
                        printf("War,WAR_SEARCH_COUNT < rSerchCount,Score,%f,Value,%lld,Index,%d,Count,%d,\n", dScore, nValue, nIndex, rSerchCount);
                    }

                    return true;
                }

                continue;
            }
        }

        printf("Error,return false,\n");

        return false;
    }

    bool Update(CSkipNode& rNode, int& rSerchCount)
    {
        if (rNode.m_dScore <= MIN_NODE_SCORE && MAX_NODE_SCORE <= rNode.m_dScore)
        {
            printf("Error,MIN_NODE_SCORE && MAX_NODE_SCORE,%f,%lld,\n", rNode.m_dScore, rNode.m_nValue);

            return false;
        }

        if (rNode.m_nValue <= 0)
        {
            printf("Error,m_nValue <= 0,\n");

            return false;
        }

        CSkipNode* pNode = nullptr;

        auto iterNode = m_mapNode.find(rNode.m_nValue);

        if (iterNode != m_mapNode.end())
        {
            Delete(iterNode->second, rSerchCount);

            iterNode->second->m_dScore = rNode.m_dScore;

            pNode = iterNode->second;
        }
        else
        {
            CSkipNode* pNode = new CSkipNode(rNode.m_dScore, rNode.m_nValue);

            m_mapNode.insert(make_pair(pNode->m_nValue, pNode));
        }

        if (Select(pNode, rSerchCount, true) == false)
        {
            m_mapNode.erase(pNode->m_nValue);

            delete pNode;

            pNode = nullptr;

            printf("Error,Select == false,%f,%lld,\n", pNode->m_dScore, pNode->m_nValue);

            return false;
        }

        return true;
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

    int nSearchCount = 0;
    
    list<CSkipNode> kNodeList;

    for (int nCount = MAX_NODE_COUNT; nCount > 0; --nCount)
    {
        kNodeList.push_back(CSkipNode(nCount, nCount));
    }

    kNodeList.sort(compareCSkipNode);

    kList.Init(kNodeList);

    CSkipNode kNode11(88, 990001);
    kList.Update(kNode11, nSearchCount);
    CSkipNode kNode22(88, 66);
    kList.Update(kNode22, nSearchCount);
    CSkipNode kNode33(55, 44);
    kList.Update(kNode33, nSearchCount);

    printf("List Size,%lld\n", kList.m_lstNode.size());

    CSkipNode kNode1(88, 990001);
    kList.Update(kNode1, nSearchCount);
    CSkipNode kNode2(88, 88);
    kList.Update(kNode2, nSearchCount);
    CSkipNode kNode3(88, 66);
    kList.Update(kNode3, nSearchCount);
    CSkipNode kNode4(55, 55);
    kList.Update(kNode4, nSearchCount);
    CSkipNode kNode5(55, 44);
    kList.Update(kNode5, nSearchCount);
    CSkipNode kNode6(1, 1);
    kList.Update(kNode6, nSearchCount);
    CSkipNode kNode7(MAX_NODE_COUNT, MAX_NODE_COUNT);
    kList.Update(kNode7, nSearchCount);

    nSearchCount = 0;

    while (true)
    {
        srand(GetTickCount());

        unsigned __int64 uTick = GetTickCount64();

        int nSerchCount = 0;
        int nIndexCount = 0;
        int nMaxCount = 0;
        int nMinCount = 100000000;

        CSkipNode kNode(0, 0);

        int nCount = 0;

        for (int nIndex = 0; nIndex < 20000; ++nIndex)
        {                
            {
                nCount = 0;
                kNode.m_dScore = MAX_NODE_COUNT - (rand() % 1000);
                kNode.m_nValue = (__int64)kNode.m_dScore;

                if (kList.Update(kNode, nCount) == false)
                {
                    printf("Error,Update == false,\n");
                }

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

            {
                nCount = 0;
                kNode.m_dScore = MAX_NODE_COUNT - (rand() % 100000);
                kNode.m_nValue = (__int64)kNode.m_dScore;

                if (kList.Update(kNode, nCount) == false)
                {
                    printf("Error,Update == false,\n");
                }

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

            {
                nCount = 0;
                kNode.m_dScore = rand() % 1000000 + 1;
                kNode.m_nValue = (__int64)kNode.m_dScore;

                if (kList.Update(kNode, nCount) == false)
                {
                    printf("Error,Update == false,\n");
                }

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

            {
                nCount = 0;
                kNode.m_dScore = rand() % 100 + 1;
                kNode.m_nValue = (__int64)kNode.m_dScore;

                if (kList.Update(kNode, nCount) == false)
                {
                    printf("Error,Update == false,\n");
                }

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

            {
                nCount = 0;
                kNode.m_dScore = rand() % MAX_NODE_COUNT + 1;
                kNode.m_nValue = rand() % MAX_NODE_COUNT + 1;

                if (kList.Update(kNode, nCount) == false)
                {
                    printf("Error,Update == false,\n");
                }

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
        }

        uTick = GetTickCount64() - uTick;

        if (600 < uTick || 150 < nMaxCount)
        {
            printf("Check,Count,%d,Update,%d,Avg,%d,Min,%d,Max,%d,Tick,%lld\n", nIndexCount, nSerchCount, nSerchCount / nIndexCount, nMinCount, nMaxCount, uTick);
        }

        Sleep(10);
    }

    return 0;
}
