
#include "RankingList.h"

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
    CRankingNode kNode123(8, 8);
    kList.Update(kNode123, nRanking, nSearchCount);

    while (true)
    {
        int nRanking = 0;
        int nCount = 0;

        CRankingNode kNode(0, 0);

        kNode.m_dScore = rand() % MAX_NODE_COUNT + 1;
        kNode.m_nValue = (__int64)kNode.m_dScore;

        if (kList.Update(kNode, nRanking, nCount) == false)
        {
            printf("Error,Update == false,\n");
        }

        if (kNode.m_dScore + nRanking - 1 != MAX_NODE_COUNT)
        {
            int i = 0;
        }

        Sleep(10);
    }

    CRankingNode kNode11(7, 7);
    kList.Update(kNode11, nRanking, nSearchCount);
    CRankingNode kNode22(8, 8);
    kList.Update(kNode22, nRanking, nSearchCount);
    CRankingNode kNode33(6, 6);


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

int main() 
{   
    TestCRankingList();

    return 0;
}