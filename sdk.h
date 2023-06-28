#pragma once
#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<char> > Board; //存储数独数据

static const int N = 9;
class sdk
{
private:
    // 使用位运算来表示在某行、列、块中某个数是否出现过
    int rowUsed[N];
    int columnUsed[N];
    int blockUsed[N];

public:
    vector<Board> result; // 存储数独结果
    
public:
    sdk()
    {
        initState();
    }
    void initState();

};

vector<Board> readFile(string filePath);
void writeFile(vector<Board> board, ofstream& f);

