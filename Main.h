#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <list>
#include <map>

using namespace std;

class CSkipNode
{
public:

    double      m_dPoint;
    __int64     m_nValue;

    vector<list<list<CSkipNode*>::iterator>::iterator>  m_vecNext;

    CSkipNode()
    {
        m_dPoint = 0;
        m_nValue = 0;
    }

    CSkipNode(double dPoint = 0, __int64 nValue = 0)
    {
        m_dPoint = dPoint;
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

    list<list<CSkipNode*>::iterator> m_lstIter[7];

    CSkipList()
    {
        for (size_t uCount = 0; uCount < 100000; ++uCount)
        {
            CSkipNode* pNode = new CSkipNode(uCount, uCount);

            m_lstNode.push_front(pNode);

            auto iterNode = m_lstNode.begin();

            m_lstIter[0].push_front(iterNode);

            (*iterNode)->m_vecNext.push_back(m_lstIter[0].begin());

            if (uCount % 10 != 0)
            {
                continue;
            }

            m_lstIter[1].push_front(iterNode);

            (*iterNode)->m_vecNext.push_back(m_lstIter[1].begin());

            if (uCount % 100 != 0)
            {
                continue;
            }

            m_lstIter[2].push_front(iterNode);

            (*iterNode)->m_vecNext.push_back(m_lstIter[2].begin());

            if (uCount % 1000 != 0)
            {
                continue;
            }

            m_lstIter[3].push_front(iterNode);

            (*iterNode)->m_vecNext.push_back(m_lstIter[3].begin());

            if (uCount % 10000 != 0)
            {
                continue;
            }

            m_lstIter[4].push_front(iterNode);

            (*iterNode)->m_vecNext.push_back(m_lstIter[4].begin());

            if (uCount % 100000 != 0)
            {
                continue;
            }

            m_lstIter[5].push_front(iterNode);

            (*iterNode)->m_vecNext.push_back(m_lstIter[5].begin());

            if (uCount % 1000000 != 0)
            {
                continue;
            }

            m_lstIter[6].push_front(iterNode);

            (*iterNode)->m_vecNext.push_back(m_lstIter[6].begin());
        }
    }

    ~CSkipList()
    {
    }

    int Search(list<list<CSkipNode*>::iterator>::iterator iterFind, int nPoint, int nIndex)
    {
        int nRanking = 0;

        for (; iterFind != m_lstIter[nIndex - 1].begin(); --iterFind)
        {
            auto iterNode = (*(*iterFind));

            if (iterNode->m_dPoint == nPoint)
            {
                return nRanking;
            }

            if (iterNode->m_dPoint < nPoint)
            {
                if (1 < nIndex)
                {
                    return Search(iterNode->m_vecNext[nIndex - 2], nPoint, nIndex - 1);
                }
                else
                {
                    ++nRanking;
                }
            }
        }

        return 0;
    }

    int Search(int nPoint, int nIndex = 4)
    {
        int nRanking = 0;

        list<list<CSkipNode*>::iterator>::iterator  iterFind;

        for (list<CSkipNode*>::iterator iterList : m_lstIter[nIndex])
        {
            if ((*iterList)->m_dPoint < nPoint)
            {
                int nSearch = Search((*iterList)->m_vecNext[nIndex - 1], nPoint, nIndex);

                if (nSearch == 0)
                {
                    return 0;
                }

                return nRanking + nSearch;
            }

            nRanking += 10000;
        }

        return 0;
    }
};


