#include "sdk.h"

void sdk::initState()
{
    //清空所有当前数独存储数据
    memset(rowUsed, 0, sizeof(rowUsed));
    memset(columnUsed, 0, sizeof(columnUsed));
    memset(blockUsed, 0, sizeof(blockUsed));
    result.clear();
}
