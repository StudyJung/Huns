
#include "Main.h"

int main() 
{   
    srand(GetTickCount());

    int nRand = ( rand() * 10000 + rand()) % MAX_NODE_COUNT;

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