#pragma once

#include <Windows.h>
#include <iostream>
#include <functional>
#include <algorithm>
#include <random>
#include <vector>
#include <list>
#include <map>

using namespace std;

const int MAX_NODE_COUNT    = 100000;
const int MAX_NODE_INDEX    = 16;
const int WAR_SEARCH_COUNT  = MAX_NODE_INDEX * 10;

const double MAX_NODE_SCORE = 1000000000000;
const double MIN_NODE_SCORE = -1;

class CRankingIter;

class CRankingNode
{
public:

    double      m_dScore;
    __int64     m_nValue;

    vector<list<CRankingIter>::iterator>  m_vecLevel;

    CRankingNode()
    {
        m_dScore = 0;
        m_nValue = 0;
    }

    CRankingNode(double dScore = 0, __int64 nValue = 0)
    {
        m_dScore = dScore;
        m_nValue = nValue;
    }

    ~CRankingNode()
    {
    }
};

class CRankingIter
{
public:

	list<CRankingNode*>::iterator	m_rIterator;

	int	m_nCount;

	CRankingIter(list<CRankingNode*>::iterator& rIterator, int nCount = 0): m_rIterator(rIterator)
	{
		m_nCount = nCount;
	}

	~CRankingIter()
	{
	}
};

class CRankingList
{
public:

    map<__int64, CRankingNode*>	m_mapNode;
    list<CRankingNode*>			m_lstNode;
	vector<int>					m_vecCount;
	list<CRankingIter>		    m_lstIter[MAX_NODE_INDEX];

    CRankingList()
    {
        srand(GetTickCount());

		for (int nIndex = 0; nIndex < MAX_NODE_INDEX; ++nIndex)
		{
			m_vecCount.push_back(0);
		}
    }

    ~CRankingList()
    {
    }

    inline bool RandomNext()
    {
        if ((rand() + 1) % 2 == 0)
        {
            return false;
        }

        return true;
    }

	inline void CountAdd()
	{
		for (int nIndex = 0; nIndex < MAX_NODE_INDEX; ++nIndex)
		{
			m_vecCount[nIndex] += 1;
		}
	}

	inline void CountSet(int nIndex)
	{
		m_vecCount[nIndex] = 0;
	}

	inline int CountGet(int nIndex)
	{
		if (nIndex <= 0)
		{
			return 1;
		}

		return m_vecCount[nIndex];
	}

	bool CompareNode(const CRankingNode& rR, const CRankingNode& rL) 
	{
		return rR.m_dScore > rL.m_dScore; // 내림차순 정렬..
	}

    bool Init(list<CRankingNode>& rSortList)
    {       
		rSortList.sort(std::bind(&CRankingList::CompareNode, this, std::placeholders::_1, std::placeholders::_2));

        rSortList.push_front(CRankingNode(MAX_NODE_SCORE, -1));
		rSortList.push_back(CRankingNode(MIN_NODE_SCORE, -2));
 
		int nListCount = 0;

		for (auto iterList : rSortList)
		{
			++nListCount;

			if (iterList.m_dScore <= MIN_NODE_SCORE && MAX_NODE_SCORE <= iterList.m_dScore)
			{
				printf("Error,MIN_NODE_SCORE && MAX_NODE_SCORE,%f,%lld,\n", iterList.m_dScore, iterList.m_nValue);

				continue;
			}

			CRankingNode* pNode = new CRankingNode(iterList.m_dScore, iterList.m_nValue);

			m_mapNode.insert(make_pair(pNode->m_nValue, pNode));

			m_lstNode.push_back(pNode);

			auto iterNode = --m_lstNode.end();

			m_lstIter[0].push_back(CRankingIter(iterNode, CountGet(0)));

			(*iterNode)->m_vecLevel.push_back(--m_lstIter[0].end());

			for (int nIndex = 1; nIndex < MAX_NODE_INDEX; ++nIndex)
			{
				if (0 < pNode->m_nValue && RandomNext() == false)
				{
					break;
				}

				m_lstIter[nIndex].push_back(CRankingIter(iterNode, CountGet(nIndex)));

				(*iterNode)->m_vecLevel.push_back(--m_lstIter[nIndex].end());

				CountSet(nIndex);
			}

			if (nListCount < (rSortList.size() - 1))
			{
				CountAdd();
			}			
        }

        return true;
    }

    bool Delete(CRankingNode* pNode)
    {
        if (pNode == nullptr)
        {
            printf("Error,pNode == nullptr,\n");

            return false;
        }

        int nIndex = 0;

        for (auto iterLevel : pNode->m_vecLevel)
        {
            if (nIndex == 0)
            {
                m_lstNode.erase((*iterLevel).m_rIterator);
            }
			else
			{
				auto iterNext = iterLevel;

				if (++iterNext != m_lstIter[nIndex].end())
				{
					iterNext->m_nCount += iterLevel->m_nCount;
				}
			}

            m_lstIter[nIndex].erase(iterLevel);

            ++nIndex;
        }

        pNode->m_vecLevel.clear();

        return true;
    }

    bool Select(CRankingNode* pNode, int& rRanking, int& rSearch, bool bInsert = false, int nIndex = (MAX_NODE_INDEX - 1),
        list<CRankingIter>::iterator* pIterPoint = nullptr, list<pair<list<CRankingIter>::iterator,int>>* pIterPointList = nullptr)
    {
        if (pNode == nullptr)
        {
            printf("Error,pNode == nullptr,\n");

            return false;
        }

        double dScore = pNode->m_dScore;
        __int64 nValue = pNode->m_nValue;

        if (dScore < MIN_NODE_SCORE || MAX_NODE_SCORE <= dScore)
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

        list<CRankingIter>::iterator iterPoint;

        if (pIterPoint == nullptr)
        {
            iterPoint = m_lstIter[nIndex].begin();

            rSearch = 0;
        }
        else
        {
            iterPoint = *pIterPoint;
        }
        
        if (bInsert == true && pIterPointList == nullptr)
        {
            pIterPointList = new list<pair<list<CRankingIter>::iterator, int>>;
        }

        int nNodeCount = 0;

        for (; iterPoint != m_lstIter[nIndex].end(); ++iterPoint)
        {
            //printf("Search Index,%d,%d,\n", nIndex, rSearch);
           
            CRankingNode* pPoint = *((*iterPoint).m_rIterator);

            if (pPoint == nullptr)
            {
                printf("Error,pPoint == nullptr,\n");

                return false;
            }

            ++rSearch;

            ++nNodeCount;

            ++iterPoint;

            auto iterNext = iterPoint;

            --iterPoint;

            double dNextScore = MIN_NODE_SCORE;

            if (iterNext != m_lstIter[nIndex].end())
            {
                dNextScore = (*((*iterNext).m_rIterator))->m_dScore;
            }

            if (pPoint->m_dScore >= dScore && dScore >= dNextScore)
            {
                if (0 < nIndex)
                {
                    if (bInsert == true)
                    {
                        pIterPointList->push_front(make_pair(iterPoint, iterPoint->m_nCount));
                    }

                    return Select(pNode, rRanking, rSearch, bInsert, nIndex - 1, &pPoint->m_vecLevel[nIndex - 1], pIterPointList);
                }

				++rRanking;

                if (pPoint->m_dScore == dScore && pPoint->m_nValue == nValue)
                {
                    printf("Search Score,%f,Value,%lld,Index,%d,Ranking,%d,Search,%d,\n", dScore, nValue, nIndex, rRanking, rSearch);

                    if (nIndex == 0 && WAR_SEARCH_COUNT < rSearch)
                    {
                        printf("War,WAR_SEARCH_COUNT < rSearch,Score,%f,Value,%lld,Index,%d,Ranking,%d,Search,%d,\n", dScore, nValue, nIndex, rRanking, rSearch);
                    }

                    return true;
                }

                if (bInsert == true && dScore > dNextScore)
                {          
					printf("Insert Score,%f,Value,%lld,Index,%d,Ranking,%d,Search,%d,\n", dScore, nValue, nIndex, rRanking, rSearch);

                    auto iterInsertNode = m_lstNode.insert((*iterNext).m_rIterator, pNode);
                 
                    auto iterInsertIter = m_lstIter[nIndex].insert(iterNext, iterInsertNode);

                    iterInsertIter->m_nCount = 1;

                    pNode->m_vecLevel.push_back(iterInsertIter);

                    for (auto iterInsertPoint : *pIterPointList)
                    {
                        if (RandomNext() == false)
                        {
                            break;
                        }
                        
                        auto iterInsertLevel = m_lstIter[++nIndex].insert(++iterInsertPoint.first, iterInsertNode);

                        if (0 < nIndex) // ??
                        {
                            (*iterInsertLevel).m_nCount = nNodeCount;
                            (*iterInsertPoint.first).m_nCount -= nNodeCount;
                        }

                        pNode->m_vecLevel.push_back(iterInsertLevel);
                    }

                    if (pIterPointList != nullptr)
                    {
                        delete pIterPointList;

                        pIterPointList = nullptr;
                    }

                    if (nIndex == 0 && WAR_SEARCH_COUNT < rSearch)
                    {
                        printf("War,WAR_SEARCH_COUNT < rSearch,Score,%f,Value,%lld,Index,%d,Ranking,%d,Search,%d,\n", dScore, nValue, nIndex, rRanking, rSearch);
                    }

                    return true;
                }

                continue;
            }
			else
			{			   	
				if (dNextScore != MIN_NODE_SCORE)
				{
					rRanking += iterNext->m_nCount;
				}
			}
        }

        printf("Error,return false,\n");

        return false;
    }

    bool Update(CRankingNode& rNode, int& rRanking, int& rSearch)
    {
		rRanking = 0;
		rSearch = 0;

        if (rNode.m_dScore < MIN_NODE_SCORE && MAX_NODE_SCORE <= rNode.m_dScore)
        {
            printf("Error,MIN_NODE_SCORE && MAX_NODE_SCORE,%f,%lld,\n", rNode.m_dScore, rNode.m_nValue);

            return false;
        }

        if (rNode.m_nValue <= 0)
        {
            printf("Error,m_nValue <= 0,\n");

            return false;
        }

        CRankingNode* pNode = nullptr;

        auto iterNode = m_mapNode.find(rNode.m_nValue);

        if (iterNode != m_mapNode.end())
        {
            Delete(iterNode->second);

            iterNode->second->m_dScore = rNode.m_dScore;

            pNode = iterNode->second;
        }
        else
        {
            pNode = new CRankingNode(rNode.m_dScore, rNode.m_nValue);

            m_mapNode.insert(make_pair(pNode->m_nValue, pNode));
        }

        if (Select(pNode, rRanking, rSearch, true) == false)
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

/*
int TestCRankingList()
{
    srand(GetTickCount());

    CRankingList kList;

	int nRanking = 0;
    int nSearchCount = 0;
    
    list<CRankingNode> kNodeList;

    for (int nCount = MAX_NODE_COUNT; nCount > 0; --nCount)
    {
        kNodeList.push_back(CRankingNode(nCount, nCount));
    }

    kList.Init(kNodeList);

    CRankingNode kNode11(80001, 80001);
    kList.Update(kNode11, nRanking, nSearchCount);
    CRankingNode kNode22(88, 66);
    kList.Update(kNode22, nRanking, nSearchCount);
    CRankingNode kNode33(55, 44);
    kList.Update(kNode33, nRanking, nSearchCount);

    printf("List Size,%lld\n", kList.m_lstNode.size());

    CRankingNode kNode1(88, MAX_NODE_COUNT - 10);
    kList.Update(kNode1, nRanking, nSearchCount);
    CRankingNode kNode2(88, 88);
    kList.Update(kNode2, nRanking, nSearchCount);
    CRankingNode kNode3(88, 66);
    kList.Update(kNode3, nRanking, nSearchCount);
    CRankingNode kNode4(55, 55);
    kList.Update(kNode4, nRanking, nSearchCount);
    CRankingNode kNode5(55, 44);
    kList.Update(kNode5, nRanking, nSearchCount);
    CRankingNode kNode6(1, 1);
    kList.Update(kNode6, nRanking, nSearchCount);
    CRankingNode kNode7(MAX_NODE_COUNT, MAX_NODE_COUNT);
    kList.Update(kNode7, nRanking, nSearchCount);

    nSearchCount = 0;

    while (true)
    {
        srand(GetTickCount());

        unsigned __int64 uTick = GetTickCount64();

        int nSerchCount = 0;
        int nIndexCount = 0;
        int nMaxCount = 0;
        int nMinCount = 100000000;

        CRankingNode kNode(0, 0);

        int nCount = 0;

        for (int nIndex = 0; nIndex < 20000; ++nIndex)
        {                
            {
                kNode.m_dScore = MAX_NODE_COUNT - (rand() % (MAX_NODE_COUNT / 1000));
                kNode.m_nValue = (__int64)kNode.m_dScore;

                if (kList.Update(kNode, nRanking, nCount) == false)
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
                kNode.m_dScore = MAX_NODE_COUNT - (rand() % (MAX_NODE_COUNT / 100));
                kNode.m_nValue = (__int64)kNode.m_dScore;

                if (kList.Update(kNode, nRanking, nCount) == false)
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
                kNode.m_dScore = rand() % MAX_NODE_COUNT + 1;
                kNode.m_nValue = (__int64)kNode.m_dScore;

                if (kList.Update(kNode, nRanking, nCount) == false)
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
                kNode.m_dScore = rand() % 100 + 1;
                kNode.m_nValue = (__int64)kNode.m_dScore;

                if (kList.Update(kNode, nRanking, nCount) == false)
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
                kNode.m_dScore = rand() % MAX_NODE_COUNT + 1;
                kNode.m_nValue = rand() % MAX_NODE_COUNT + 1;

                if (kList.Update(kNode, nRanking, nCount) == false)
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

		//if (600 < uTick || nMaxCount < 10 || 100 < nMaxCount || (WAR_SEARCH_COUNT / 2) < (nSerchCount / nIndexCount))
		{
			printf("Tick,%lld,Check,%d,Search,%d,Avg,%d,Min,%d,Max,%d,Tick,%lld\n", GetTickCount64(), nIndexCount, nSerchCount, (nSerchCount / nIndexCount), nMinCount, nMaxCount, uTick);
		}
        
        Sleep(300);
    }

    return 0;
}

*/