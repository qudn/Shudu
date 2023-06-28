#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include <cstring>
#include "unistd.h"
#include "sdk.h"
#include "xgetopt.h"

using namespace std;

void usage() {
    // 打印使用说明

    cout << "Usage: sdk [-c count] [-s file] [-n count] [-m level] [-r range] [-u]" << endl;
    cout << "  -c count: 生成数独终盘数量，范围在1～1000000之间" << endl;
    cout << "  -s file: 从文件中读取数独终盘，文件格式为一行一个终盘" << endl;
    cout << "  -n count: 生成数独游戏数量，范围在1～10000之间" << endl;
    cout << "  -m level: 生成数独游戏的难度，范围在1～3之间" << endl;
    cout << "  -r range: 生成数独游戏的终盘数字范围，格式为 start~end" << endl;
    cout << "  -u: 输出数独游戏的唯一解" << endl;
}

int main(int argc, char* argv[]) {
    sdk player;
    map<char, string> params;
    vector<int> range;
    int compeleteBoardCount;
    int gameNumber;
    int gameLevel = 0;
    int solution_count = 0;
    vector<Board> boards;
    ofstream outfile;
    char opt = 0;
    while ((opt = xgetopt(argc, argv, "c:s:n:m:r:u")) != -1) {
        switch (opt) {
        case 'c': {
            compeleteBoardCount = atoi(xoptarg);
            if (compeleteBoardCount < 1 || compeleteBoardCount > 1000000) {
                cout << "生成数独终盘数量范围在1～1000000之间" << endl;
                exit(1);
            }
            params[opt] = string(xoptarg);
            break;
        }
        case 's': {
            if (access(xoptarg, 0) == -1) {
                cout << "文件 " << xoptarg << " 不存在" << endl;
                exit(1);
            }
            params[opt] = string(xoptarg);
            break;
        }
        case 'n': {
            gameNumber = atoi(xoptarg);
            if (gameNumber < 1 || gameNumber > 10000) {
                cout << "生成数独游戏数量范围在1～10000之间" << endl;
                exit(1);
            }
            params[opt] = string(xoptarg);
            break;
        }
        case 'm': {
            gameLevel = atoi(xoptarg);
            if (gameLevel < 1 || gameLevel > 3) {
                cout << "生成游戏难度的范围在1～3之间" << endl;
                exit(1);
            }
            params[opt] = string(xoptarg);
            break;
        }
        case 'r': {
            char* p;
            p = strtok(xoptarg, "~");
            while (p) {
                range.push_back(atoi(p));
                p = strtok(NULL, "~");
            }
            if (range.size() != 2) {
                cout << "请输入一个范围参数，格式为 start~end" << endl;
                exit(1);
            }
            if (range[0] >= range[1] || range[0] < 20 || range[1] > 55) {
                cout << "请输入合法范围20～55" << endl;
                exit(1);
            }
            params[opt] = string(xoptarg);
            break;
        }
        case 'u': {
            params[opt] = string();
            break;
        }
        default: {
            usage();
            exit(1);
            break;
        }
        }
    }
    for (auto it = params.begin(); it != params.end(); ++it) {
        switch (it->first) {
        case 'c': {
            outfile.open("game.txt", ios::out | ios::trunc);
            range.push_back(0);
            generateGame(atoi(it->second.c_str()), 0, range, outfile, player);
            range.clear();
            outfile.close();
            break;
        }
        case 's': {
            outfile.open("sudoku.txt", ios::out | ios::trunc);
            boards = readFile(it->second);
            for (int i = 0; i < boards.size(); i++) {
                vector<Board> result = player.solveSudoku(boards[i]);
                writeFile(result, outfile);
            }
            outfile.close();
            break;
        }
        case 'n': {
            gameNumber = atoi(it->second.c_str());
            auto tmp = params.find('u');
            if (tmp != params.end()) {
                solution_count = 1;
            }
            tmp = params.find('m');
            if (tmp != params.end()) {
                gameLevel = atoi(tmp->second.c_str());
            }
            tmp = params.find('r');
            if (tmp != params.end()) {
                char* p;
                char* pc = new char[100];
                strcpy(pc, tmp->second.c_str());
                p = strtok(pc, "~");
                while (p) {
                    range.push_back(atoi(p));
                    p = strtok(NULL, "~");
                }
            }
            else {
                if (gameLevel == 1) {
                    range.push_back(20);
                    range.push_back(30);
                }
                else if (gameLevel == 2) {
                    range.push_back(30);
                    range.push_back(40);
                }
                else if (gameLevel == 3) {
                    range.push_back(40);
                    range.push_back(55);
                }
                else {
                    range.push_back(20);
                    range.push_back(55);
                }
            }
            outfile.open("game.txt", ios::out | ios::trunc);
            generateGame(gameNumber, gameLevel, range, outfile, player);
            range.clear();
            outfile.close();
            break;
        }
        default: {
            break;
        }
        }
    }

    return 0;
}
