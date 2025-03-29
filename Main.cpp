
#include "Main.h"

int main() 
{   
    CSkipList kList;

    list<CSkipNode> kNodeList;

    for (int nCount = 1000000; nCount > 0; --nCount)
    {
        kNodeList.push_back(CSkipNode(nCount, nCount));
    }

    kList.Init(kNodeList);

    int nCount = 0;

    kList.Search(1, nCount);
    kList.Search(2, nCount);
    kList.Search(3, nCount);

    while (true)
    {
        nCount = 0;

        unsigned __int64 uTick = GetTickCount64();

        kList.Search(rand() % 1000000, nCount);

        printf("Tick,%lld\n", GetTickCount64() - uTick);

        Sleep(500);
    }

    return 0;
}