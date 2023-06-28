"Copyright [2023] <MXY>"
#include "ce/sdk.h"
#include <set>
#include <utility>

void sdk::initState() {
    // 清空所有当前数独存储数据
    memset(rowUsed, 0, sizeof(rowUsed));
    memset(columnUsed, 0, sizeof(columnUsed));
    memset(blockUsed, 0, sizeof(blockUsed));
    result.clear();
}

// 获得9以内的随机数序列
vector<int> sdk::getRand9() {
    vector<int> result;
    set<int> usedNumbers;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 8);

    while (result.size() != 9) {
        int num = dis(gen);
        if (usedNumbers.count(num) == 0) {
            result.push_back(num);
            usedNumbers.insert(num);
        }
    }
    return result;
}

// 生成终局数独
Board sdk::generateBoard(int digNum) {
    // 使用$填充初始数独
    vector<vector<char> > board(N, vector<char>(N, '$'));
    // 使用随机生成的9以内的数字序列初始化数度中间的九宫格块
    vector<int> row = getRand9();
    for (int i = 0; i < 3; i++) {
        board[3][i + 3] = row[i] + '1';
        board[4][i + 3] = row[i + 3] + '1';
        board[5][i + 3] = row[i + 6] + '1';
    }

    // 将中间的九宫格块经过一定的变换对九宫格左右与上下的九宫格进行填充
    copyNineSquareGrid(board, 3, 3, true);
    copyNineSquareGrid(board, 3, 3, false);
    copyNineSquareGrid(board, 3, 0, false);
    copyNineSquareGrid(board, 3, 6, false);

    // 根据所要挖的空数进行挖空
    while (digNum) {
        int x = rand_r() % 9;
        int y = rand_r() % 9;
        if (board[x][y] == '$')
            continue;
        char tmp = board[x][y];
        board[x][y] = '$';

        // 对熟读进行求解调整其难易程度
        solveSudoku(board);
        if (result.size() == 1) {
            digNum--;
        } else {
            board[x][y] = tmp;
        }
    }
    // 对数独是否合格进行检查
    if (!checkBoard(board)) {
        cout << "wrong board" << endl;
    }

    return board;
}

// 九宫格块复制函数
void sdk::copyNineSquareGrid(Board& board, int src_x, int src_y, bool isRow) {
    int order_first[3] = { 1, 2, 0 };
    int order_second[3] = { 2, 0, 1 };
    if (rand_r() % 2 == 1) {
        swap(order_first[0], order_second[0]);
        swap(order_first[1], order_second[1]);
        swap(order_first[2], order_second[2]);
    }

    for (int i = 0; i < 3; i++) {
        if (isRow) {
            for (int j = 0; j < 3; j++) {
                board[src_x + j][i] = board[src_x + order_first[j]][src_y + i];
                board[src_x + j][i+6] =
                board[src_x + order_second[j]][src_y + i];
            }
        } else {
            for (int j = 0; j < 3; j++) {
                board[i][src_y + j] = board[src_x + i][src_y + order_first[j]];
                board[i + 6][src_y + j] =
                board[src_x + i][src_y + order_second[j]];
            }
        }
    }
}

// 检查当前数独是否合格
bool sdk::checkBoard(Board& board) {
    initState();
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != '$') {
                int digit = board[i][j] - '1';
                if ((rowUsed[i] | columnUsed[j] |
                blockUsed[(i / 3) * 3 + j / 3]) &
                (1 << digit)) {
                    return false;
                }
                flip(i, j, digit);
            }
        }
    }
    return true;
}

// 解数独函数
vector<Board> sdk::solveSudoku(Board board) {
    // 所有参数初始化
    initState();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // 对未填充的二维位置数据进行记录
            if (board[i][j] == '$') {
                spaces.push_back(pair<int, int>(i, j));
            } else {
                // 获取当前位置上的数据，并通过-1从而便于后续通过位变化改变数独坐标记录
                int digit = board[i][j] - '1';
                UpdateCoordinate(i, j, digit);
            }
        }
    }
    // 回溯
    DFS(board, 0);
    return result;
}

// 数独坐标填充更新
void sdk::UpdateCoordinate(int i, int j, int digit) {
    // digit的范围是0~8对应数字1~9，故而通过9个二进制位代表行列块中数据填充信息用以后续判断数独合法与否
    rowUsed[i] ^= (1 << digit);
    columnUsed[j] ^= (1 << digit);
    blockUsed[(i / 3) * 3 + j / 3] ^= (1 << digit);
}

// 回溯函数
void sdk::DFS(Board& board, int pos) {
    // 当填充完成时将数独添加至结果存储容器中
    if (pos == spaces.size()) {
        vector<vector<char> > object(board);
        result.push_back(object);
        return;
    }
    // 获取尚未填充数据的位置信息
    int i = spaces[pos].first;
    int j = spaces[pos].second;
    // 通过将行、列、块的数据填充信息进行或操作后取反
    // 再与0001 1111 1111掩码进行与操作，获取当前格可以进行填充的数据
    int mask = ~(rowUsed[i] | columnUsed[j] |
    blockUsed[(i / 3) * 3 + j / 3]) & 0x1ff;
    // 为方便位运算，故而digit从0取起
    int digit = 0;
    while (mask) {
        if (mask & 1) {
            // 对当前位置进行更新
            UpdateCoordinate(i, j, digit);
            board[i][j] = '1' + digit;
            // 回溯并再次更新位置信息
            DFS(board, pos + 1);
            UpdateCoordinate(i, j, digit);
        }
        mask = mask >> 1;
        digit++;
    }
}
