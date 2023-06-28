/*
 * Copyright (c) 2023 LYC&MXY
 */

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include "ce/unistd.h"
#include "ce/sdk.h"
#include "ce/xgetopt.h"

using std::ifstream;
using std::ofstream;
using std::vector;
using std::utility;
using std::string;
using std::endl;

// 从文件中读取数独
vector<Board> readFile(string filePath) {
    ifstream infile;
    infile.open(filePath);

    char data[100];

    vector<char> row;
    vector<Board> boards;
    Board tmp;

    while (!infile.eof()) {
        infile.getline(data, 100);
        // 根据分隔符判断当前是否为一个新的数独
        if (data[0] == '=') {
            boards.push_back(Board(tmp));
            tmp.clear();
            continue;
        }
        for (int i = 0; i < strlen(data); i++) {
            if (('1' <= data[i] && data[i] <= '9') || data[i] == '$') {
                row.push_back(data[i]);
            }
        }
        tmp.push_back(vector<char>(row));
        row.clear();
    }
    infile.close();
    return boards;
}

// 将数独写入文件
void writeFile(vector<Board> board, ofstream& f) {
    for (int i = 0; i < board[0].size(); i++) {
        for (int j = 0; j < board[0][i].size(); j++) {
            f << board[0][i][j] << " ";
        }
        f << "\n";
    }
    f << "===========" << endl;
}

// 生成数独游戏
void generateGame(int gameNumber,
                  int gameLevel,
                  vector<int> digCount,
                  ofstream& outfile,
                  sdk& player) {
    int min_count = digCount[0];
    int max_count = digCount.size() > 1 ? digCount[1] : digCount[0];
    for (int i = 0; i < gameNumber; i++) {
        int count = rand_r() % (max_count - min_count + 1) + min_count;
        Board board = player.generateBoard(count);
        vector<Board> boards = { board };
        writeFile(boards, outfile);
    }
    outfile.close();
}
