#include <graphics.h>
#include<easyx.h>
#include <cstdio>
#include <cstdlib>
#include<ctime>
#include "header.h"

int xx, yy;
enum Turn {
    HUMAN, AI
} turn;
coordinate ChessDate[Axis][Axis];

int WeightSumWB[Axis][Axis];

bool debug = false;

void DrawHeadboard() {
    setfillcolor(RGB(147, 147, 136));
    solidrectangle(0, 0, Height * UNIT, ToolSize * UNIT);
    char title[] = "霡霂的五子棋";
    settextstyle(UNIT + 10, 0, "楷体");
    static COLORREF textColor;
    static int frame = -1;
    if (frame-- < 0) {
        textColor = RGB((int) rand() % 255, (int) rand() % 122 * 2, (int) rand() % 86 * 3);
        frame = 13;
    }
    settextcolor(textColor);
    setbkmode(TRANSPARENT);
    int a = textwidth(title);
    outtextxy((Height * UNIT - a) / 2, UNIT, title);
}

int DrawGameInterface() {
    IMAGE img;
    loadimage(&img, "../800.jpg", Height * UNIT, (Height + ToolSize) * UNIT);
    putimage(0, 0, &img);
    char *title1 = "霡霂的五子棋";
    char *title2 = "人人对决";
    char *title3 = "人机对决";
    settextcolor(BLACK);
    settextstyle(1.5 * UNIT, 0, "楷体");
    setbkmode(TRANSPARENT);
    int a1 = textwidth(title1);
    int a2 = textwidth(title2);
    int a3 = textwidth(title3);
    outtextxy((Height * UNIT - a1) / 2, 3 * UNIT, title1);
    outtextxy((Height * UNIT - a2) / 2, 11 * UNIT, title2);
    outtextxy((Height * UNIT - a3) / 2, 14 * UNIT, title3);
    setlinecolor(BLACK);
    for (int i = 0; i < 3; ++i) {
        rectangle((Height * UNIT - a2) / 2 + i, 11 * UNIT, (Height * UNIT + a2) / 2 + i, 11 * UNIT + 1.5 * UNIT);
        rectangle((Height * UNIT - a3) / 2 + i, 14 * UNIT, (Height * UNIT + a3) / 2 + i, 14 * UNIT + 1.5 * UNIT);
    }
    ExMessage msg{};
    while (true)
        if (peekmessage(&msg, EM_MOUSE))
            if (msg.message == WM_LBUTTONDOWN) {
                if (msg.x<(Height * UNIT + a2) / 2 && msg.x>(Height * UNIT - a2) / 2 && msg.y > 11 * UNIT &&
                    msg.y < 11 * UNIT + 1.5 * UNIT)
                    return 1;
                else if (msg.x<(Height * UNIT + a3) / 2 && msg.x>(Height * UNIT - a3) / 2 && msg.y > 14 * UNIT &&
                         msg.y < 14 * UNIT + 1.5 * UNIT)
                    return 2;
            }
}

void DrawCheckerboard() {
    int cx, cy;
    setfillcolor(RGB(73, 57, 45));
    solidrectangle(0, ToolSize * UNIT, Height * UNIT, (ToolSize + Height) * UNIT);
    setfillcolor(RGB(123, 81, 65));
    solidrectangle(UNIT / 2, ToolSize * UNIT + UNIT / 2, Height * UNIT - UNIT / 2,
                   (ToolSize + Height) * UNIT - UNIT / 2);         //棋盘
    cy = ToolSize * UNIT + Height * UNIT / 2;
    cx = Height * UNIT / 2;
    setlinecolor(BLACK);
    for (int i = 0; i < SIZE + 1; ++i) {
        line(cx - SIZE * UNIT / 2, cy - SIZE * UNIT / 2 + UNIT * i, cx + SIZE * UNIT / 2,
             cy - SIZE * UNIT / 2 + UNIT * i);
        line(cx - SIZE * UNIT / 2 + UNIT * i, cy - SIZE * UNIT / 2, cx - SIZE * UNIT / 2 + UNIT * i,
             cy + SIZE * UNIT / 2);
    }
    for (int i = 0; i < Axis; ++i) {
        for (int j = 0; j < Axis; ++j) {
            if (ChessDate[i][j].color == 0)continue;
            setfillcolor(ChessDate[i][j].color == 1 ? BLACK : WHITE);
            solidcircle(UNIT * (1 + i) + UNIT / 2, UNIT * (ToolSize + 1 + j) + UNIT / 2, UNIT / 2 - UNIT / 10);
        }
    }
}

int Is_Finish(int TimeX, int TimeY) {
    if (ChessDate[xx][yy].color != 0) {
        int num = 0;
        for (int i = TimeX; i < Axis; ++i) {
            if (ChessDate[i][TimeY].color == ChessDate[TimeX][TimeY].color)
                num++;
            else
                break;
        }
        for (int i = TimeX; i >= 0; --i) {
            if (ChessDate[i][TimeY].color == ChessDate[TimeX][TimeY].color)
                num++;
            else
                break;
        }
        num--;
        if (num >= 5)
            return num;
        num = 0;
        for (int i = TimeY; i < Axis; ++i) {
            if (ChessDate[TimeX][i].color == ChessDate[TimeX][TimeY].color)
                num++;
            else
                break;
        }
        for (int i = TimeY; i >= 0; --i) {
            if (ChessDate[TimeX][i].color == ChessDate[TimeX][TimeY].color)
                num++;
            else
                break;
        }
        num--;
        if (num >= 5)
            return num;
        num = 0;
        for (int i = TimeX, j = TimeY; i < Axis && j >= 0; ++i, --j) {
            if (ChessDate[i][j].color == ChessDate[TimeX][TimeY].color)
                num++;
            else
                break;
        }
        for (int i = TimeX, j = TimeY; i >= 0 && j < Axis; --i, ++j) {
            if (ChessDate[i][j].color == ChessDate[TimeX][TimeY].color)
                num++;
            else
                break;
        }
        num--;
        if (num >= 5)
            return num;
        num = 0;
        for (int i = TimeX, j = TimeY; i >= 0 && j >= 0; --i, --j) {
            if (ChessDate[i][j].color == ChessDate[TimeX][TimeY].color)
                num++;
            else
                break;
        }
        for (int i = TimeX, j = TimeY; i < Axis && j < Axis; ++i, ++j) {
            if (ChessDate[i][j].color == ChessDate[TimeX][TimeY].color)
                num++;
            else
                break;
        }
        num--;
        if (num >= 5)
            return num;
        else return 0;
    }
    return 0;
}

ChessState &JudgeXYState(int mode, int x, int y, int color) {
    ChessState *chess = new ChessState;
    chess->ChessSame1 = chess->ChessSame2 = chess->JumpSpace1 = chess->JumpSpace2 =
    chess->SpaceSame1 = chess->SpaceSame2 = chess->JumpChess1 = chess->JumpChess2 =
    chess->NSameChess1 = chess->NSameChess2 = 0;
    switch (mode) {
        case 0:
            for (int i = x + 1; i < (Axis > x + 6 ? x + 6 : Axis); ++i) {                            //右边
                if (ChessDate[i][y].color == color)
                    chess->ChessSame1++;
                else
                    break;
            }
            for (int i = x - 1; i >= (0 > x - 6 ? 0 : x - 6); --i) {                              //左边
                if (ChessDate[i][y].color == color)
                    chess->ChessSame2++;
                else
                    break;
            }
            for (int i = x + 1; i < (Axis > x + 6 ? x + 6 : Axis); ++i) {                            //右边
                if (ChessDate[i][y].color == 0)
                    chess->SpaceSame1++;
                else
                    break;
            }
            for (int i = x - 1; i >= (0 > x - 6 ? 0 : x - 6); --i) {                              //左边
                if (ChessDate[i][y].color == 0)
                    chess->SpaceSame2++;
                else
                    break;
            }
            for (int a = 1, i = x + 1; i < (Axis > x + 6 ? x + 6 : Axis); ++i) {                        //右边
                if (a == 1) {
                    if (ChessDate[i][y].color != color)
                        a = 0;
                    else
                        continue;
                }
                if (ChessDate[i][y].color == 0)
                    chess->JumpSpace1++;
                else
                    break;
            }
            for (int a = 1, i = x - 1; i >= (0 > x - 6 ? 0 : x - 6); --i) {                           //左边
                if (a == 1) {
                    if (ChessDate[i][y].color != color)
                        a = 0;
                    else
                        continue;
                }
                if (ChessDate[i][y].color == 0)
                    chess->JumpSpace2++;
                else
                    break;
            }
            for (int b = 0, i = x + 1; i < (Axis > x + 6 ? x + 6 : Axis); ++i) {                              //右边
                if (b == 0 && ChessDate[i][y].color == 0) {
                    b = 1;
                    continue;
                }
                if (ChessDate[i][y].color == color)
                    chess->JumpChess1++;
                else
                    break;
            }
            for (int b = 0, i = x + 1; i >= (0 > x - 6 ? 0 : x - 6); --i) {                              //左边
                if (b == 0 && ChessDate[i][y].color == 0) {
                    b = 1;
                    continue;
                }
                if (ChessDate[i][y].color == color)
                    chess->JumpChess2++;
                else
                    break;
            }
            for (int i = x + 1; i < (Axis > x + 6 ? x + 6 : Axis); ++i) {                              //右边
                if (ChessDate[i][y].color + color != 3)
                    chess->NSameChess1++;
                else
                    break;
            }
            for (int i = x - 1; i >= (0 > x - 6 ? 0 : x - 6); --i) {                              //左边
                if (ChessDate[i][y].color + color != 3)
                    chess->NSameChess2++;
                else
                    break;
            }
            break;
        case 1:
            for (int i = y - 1; i >= (0 > y - 6 ? 0 : y - 6); --i) {                              //上边
                if (ChessDate[x][i].color == color)
                    chess->ChessSame1++;
                else
                    break;

            }
            for (int i = y + 1; i < (Axis > y + 6 ? y + 6 : Axis); ++i) {                            //下边
                if (ChessDate[x][i].color == color)
                    chess->ChessSame2++;
                else
                    break;
            }
            for (int i = y - 1; i >= (0 > y - 6 ? 0 : y - 6); --i) {                              //上边
                if (ChessDate[x][i].color == 0)
                    chess->SpaceSame1++;
                else
                    break;
            }
            for (int i = y + 1; i < (Axis > y + 6 ? y + 6 : Axis); ++i) {                            //下边
                if (ChessDate[x][i].color == 0)
                    chess->SpaceSame2++;
                else
                    break;
            }
            for (int a = 1, i = y - 1; i >= (0 > y - 6 ? 0 : y - 6); --i) {                           //上边
                if (a == 1) {
                    if (ChessDate[x][i].color != color)
                        a = 0;
                    else
                        continue;
                }
                if (ChessDate[x][i].color == 0)
                    chess->JumpSpace1++;
                else
                    break;
            }
            for (int a = 1, i = y + 1; i < (Axis > y + 6 ? y + 6 : Axis); ++i) {                        //下边
                if (a == 1) {
                    if (ChessDate[x][i].color != color)
                        a = 0;
                    else
                        continue;
                }
                if (ChessDate[x][i].color == 0)
                    chess->JumpSpace2++;
                else
                    break;
            }
            for (int b = 0, i = y + 1; i >= (0 > y - 6 ? 0 : y - 6); --i) {                              //上边
                if (b == 0 && ChessDate[x][i].color == 0) {
                    b = 1;
                    continue;
                }
                if (ChessDate[x][i].color == color)
                    chess->JumpChess1++;
                else
                    break;
            }
            for (int b = 0, i = y + 1; i < (Axis > y + 6 ? y + 6 : Axis); ++i) {                              //下边
                if (b == 0 && ChessDate[x][i].color == 0) {
                    b = 1;
                    continue;
                }
                if (ChessDate[x][i].color == color)
                    chess->JumpChess2++;
                else
                    break;
            }
            for (int i = y - 1; i >= (0 > y - 6 ? 0 : y - 6); --i) {                              //上边
                if (ChessDate[x][i].color + color != 3)
                    chess->NSameChess1++;
                else
                    break;
            }
            for (int i = y + 1; i < (Axis > y + 6 ? y + 6 : Axis); ++i) {                            //下边
                if (ChessDate[x][i].color + color != 3)
                    chess->NSameChess2++;
                else
                    break;
            }
            break;
        case 2:
            for (int i = x + 1, j = y - 1;
                 i < (Axis > x + 6 ? x + 6 : Axis) && j >= (0 > y - 6 ? 0 : y - 6); ++i, --j) {                  //右上
                if (ChessDate[i][j].color == color)
                    chess->ChessSame1++;
                else
                    break;
            }
            for (int i = x - 1, j = y + 1; i >= (0 > x - 6 ? 0 : x - 6) &&
                                           j < (Axis > y + 6 ? y + 6 : Axis); --i, ++j) {                         //左下
                if (ChessDate[i][j].color == color)
                    chess->ChessSame2++;
                else
                    break;
            }
            for (int i = x + 1, j = y - 1; i < (Axis > x + 6 ? x + 6 : Axis) &&
                                           j >= (0 > y - 6 ? 0 : y - 6); ++i, --j) {                      //右上
                if (ChessDate[i][j].color == 0)
                    chess->SpaceSame1++;
                else
                    break;
            }
            for (int i = x - 1, j = y + 1; i >= (0 > x - 6 ? 0 : x - 6) &&
                                           j < (Axis > y + 6 ? y + 6 : Axis); --i, ++j) {                         //左下
                if (ChessDate[i][j].color == 0)
                    chess->SpaceSame2++;
                else
                    break;
            }
            for (int a = 1, i = x + 1, j = y - 1;
                 i < (Axis > x + 6 ? x + 6 : Axis) && j >= (0 > y - 6 ? 0 : y - 6); ++i, --j) {                 //右上
                if (a == 1) {
                    if (ChessDate[i][j].color != color)
                        a = 0;
                    else
                        continue;
                }
                if (ChessDate[i][j].color == 0)
                    chess->JumpSpace1++;
                else
                    break;
            }
            for (int a = 1, i = x - 1, j = y + 1;
                 i >= (0 > x - 6 ? 0 : x - 6) && j < (Axis > y + 6 ? y + 6 : Axis); --i, ++j) {                    //左下
                if (a == 1) {
                    if (ChessDate[i][j].color != color)
                        a = 0;
                    else
                        continue;
                }
                if (ChessDate[i][j].color == 0)
                    chess->JumpSpace2++;
                else
                    break;
            }
            for (int b = 0, i = x + 1, j = y - 1;
                 i < (Axis > x + 6 ? x + 6 : Axis) && j >= (0 > y - 6 ? 0 : y - 6); ++i, --j) {                  //右上
                if (b == 0 && ChessDate[i][j].color == 0) {
                    b = 1;
                    continue;
                }
                if (ChessDate[i][j].color == color)
                    chess->JumpChess1++;
                else
                    break;
            }
            for (int b = 0, i = x - 1, j = y + 1;
                 i >= (0 > x - 6 ? 0 : x - 6) && j < (Axis > y + 6 ? y + 6 : Axis); --i, ++j) {                     //左下
                if (b == 0 && ChessDate[i][j].color == 0) {
                    b = 1;
                    continue;
                }
                if (ChessDate[i][j].color == color)
                    chess->JumpChess2++;
                else
                    break;
            }
            for (int i = x + 1, j = y - 1;
                 i < (Axis > x + 6 ? x + 6 : Axis) && j >= (0 > y - 6 ? 0 : y - 6); ++i, --j) {                  //右上
                if (ChessDate[i][j].color + color != 3)
                    chess->NSameChess1++;
                else
                    break;
            }
            for (int i = x - 1, j = y + 1; i >= (0 > x - 6 ? 0 : x - 6) &&
                                           j < (Axis > y + 6 ? y + 6 : Axis); --i, ++j) {                         //左下
                if (ChessDate[i][j].color + color != 3)
                    chess->NSameChess2++;
                else
                    break;
            }
            break;
        case 3:
            for (int i = x + 1, j = y + 1; i < (Axis > x + 6 ? x + 6 : Axis) &&
                                           j < (Axis > y + 6 ? y + 6 : Axis); ++i, ++j) {                  //右下
                if (ChessDate[i][j].color == color)
                    chess->ChessSame1++;
                else
                    break;
            }
            for (int i = x - 1, j = y - 1;
                 i >= (0 > x - 6 ? 0 : x - 6) && j >= (0 > y - 6 ? 0 : y - 6); --i, --j) {                         //左上
                if (ChessDate[i][j].color == color)
                    chess->ChessSame2++;
                else
                    break;
            }
            for (int i = x + 1, j = y + 1; i < (Axis > x + 6 ? x + 6 : Axis) &&
                                           j < (Axis > y + 6 ? y + 6 : Axis); ++i, ++j) {                  //右下
                if (ChessDate[i][j].color == 0)
                    chess->SpaceSame1++;
                else
                    break;
            }
            for (int i = x - 1, j = y - 1;
                 i >= (0 > x - 6 ? 0 : x - 6) && j >= (0 > y - 6 ? 0 : y - 6); --i, --j) {                         //左上
                if (ChessDate[i][j].color == 0)
                    chess->SpaceSame2++;
                else
                    break;
            }
            for (int a = 1, i = x + 1, j = y + 1; i < (Axis > x + 6 ? x + 6 : Axis) &&
                                                  j < (Axis > y + 6 ? y + 6 : Axis); ++i, ++j) {                  //右下
                if (a == 1) {
                    if (ChessDate[i][j].color != color)
                        a = 0;
                    else
                        continue;
                }
                if (ChessDate[i][j].color == 0)
                    chess->JumpSpace1++;
                else
                    break;
            }
            for (int a = 1, i = x - 1, j = y - 1;
                 i >= (0 > x - 6 ? 0 : x - 6) && j >= (0 > y - 6 ? 0 : y - 6); --i, --j) {                         //左上
                if (a == 1) {
                    if (ChessDate[i][j].color != color)
                        a = 0;
                    else
                        continue;
                }
                if (ChessDate[i][j].color == 0)
                    chess->JumpSpace2++;
                else
                    break;
            }
            for (int b = 0, i = x + 1, j = y + 1; i < (Axis > x + 6 ? x + 6 : Axis) &&
                                                  j < (Axis > y + 6 ? y + 6 : Axis); ++i, ++j) {                  //右下
                if (b == 0 && ChessDate[i][j].color == 0) {
                    b = 1;
                    continue;
                }
                if (ChessDate[i][j].color == color)
                    chess->JumpChess1++;
                else
                    break;
            }
            for (int b = 0, i = x - 1, j = y - 1;
                 i >= (0 > x - 6 ? 0 : x - 6) && j >= (0 > y - 6 ? 0 : y - 6); --i, --j) {                         //左上
                if (b == 0 && ChessDate[i][j].color == 0) {
                    b = 1;
                    continue;
                }
                if (ChessDate[i][j].color == color)
                    chess->JumpChess2++;
                else
                    break;
            }
            for (int i = x + 1, j = y + 1; i < (Axis > x + 6 ? x + 6 : Axis) &&
                                           j < (Axis > y + 6 ? y + 6 : Axis); ++i, ++j) {                  //右下
                if (ChessDate[i][j].color + color != 3)
                    chess->NSameChess1++;
                else
                    break;
            }
            for (int i = x - 1, j = y - 1;
                 i >= (0 > x - 6 ? 0 : x - 6) && j >= (0 > y - 6 ? 0 : y - 6); --i, --j) {                         //左上
                if (ChessDate[i][j].color + color != 3)
                    chess->NSameChess2++;
                else
                    break;
            }
            break;
    }
    return *chess;
}

//int JumpSpace1;
//int JumpSpace2;       跳跃的空格
//int ChessSame1;
//int ChessSame2;       连续的棋子
//int SpaceSame1;
//int SpaceSame2;       连续的空格
//int JumpChess1;
//int JumpChess2;       跳跃的棋子
//int NSameChess1;
//int NSameChess2;      不同颜色的棋子距离
//1:右，上，↗，↘    2:左，下，↙，↖
//这个函数是判断权重所根据的重要函数，但因为时间紧张，我所利用的不是很多，如果更好的利用这个函数，ai将得到质的飞跃。
int ChessWeight(ChessState chess, int color, int &weight) {
    if (color == 1) {
        if (chess.NSameChess1 + chess.NSameChess2 < 4) {
            return 0;
        } else if (chess.ChessSame1 + chess.ChessSame2 >= 4) {
            weight = 20;
            return 999999999 - 1000;                //连四
        } else if ((chess.ChessSame1 + chess.ChessSame2 == 3) && (chess.JumpSpace1 * chess.JumpSpace2 > 0)) {
            weight = 10;
            return 99999999 - 1000;                //活三
        } else if ((chess.ChessSame1 + chess.ChessSame2 == 3) && (chess.JumpSpace1 * chess.JumpSpace2 == 0)) {
            weight++;
            return 300;                //死三
        } else if ((chess.ChessSame1 + chess.ChessSame2 == 2) && (chess.JumpSpace1 * chess.JumpSpace2 > 0)) {
            weight++;
            return 3500;       //连活二90101
        } else if ((chess.JumpChess1 + chess.ChessSame2 == 3) &&
                   ((chess.NSameChess1 - chess.JumpChess1 - 1) * (chess.NSameChess2 - chess.ChessSame2) > 0)) {
            weight++;
            return 850;                 //活跳三
        } else if ((chess.JumpChess1 + chess.ChessSame2 == 3) &&
                   ((chess.NSameChess1 - chess.JumpChess1 - 1) * (chess.NSameChess2 - chess.ChessSame2) == 0)) {
            weight++;
            return 300;                 //死跳三
        } else if ((chess.JumpChess2 + chess.ChessSame1 == 3) &&
                   ((chess.NSameChess2 - chess.JumpChess2 - 1) * (chess.NSameChess1 - chess.ChessSame1) > 0)) {
            weight++;
            return 850;                 //活跳三
        } else if ((chess.JumpChess2 + chess.ChessSame1 == 3) &&
                   ((chess.NSameChess2 - chess.JumpChess2 - 1) * (chess.NSameChess1 - chess.ChessSame1) == 0)) {
            weight++;
            return 300;                 //死跳三
        } else if (
                ((chess.ChessSame1 == 1 && chess.JumpChess1 == 2) && chess.JumpSpace2 >= 1 && chess.NSameChess1 >= 4) ||
                ((chess.ChessSame2 == 1 && chess.JumpChess2 == 2) && chess.JumpSpace1 >= 1 && chess.NSameChess2 >= 4))
            return 550;                 //单跳活二
        else if ((chess.ChessSame1 + chess.ChessSame2 == 1) && (chess.JumpSpace1 + chess.JumpSpace2 > 2) &&
                 (chess.JumpSpace1 * chess.JumpSpace2 != 0)) {
            return 150;                //活一
        }
        return 0;
    } else {
        if (chess.NSameChess1 + chess.NSameChess2 < 4) {
            return 0;
        } else if (chess.ChessSame1 + chess.ChessSame2 >= 4) {
            weight = 20;
            return 999999999;                //连四
        } else if ((chess.ChessSame1 + chess.ChessSame2 == 3) && (chess.JumpSpace1 * chess.JumpSpace2 > 0)) {
            weight = 10;
            return 99999999;                //活三
        } else if ((chess.ChessSame1 + chess.ChessSame2 == 3) && (chess.JumpSpace1 * chess.JumpSpace2 == 0)) {
            weight++;
            return 250;                //死三
        } else if ((chess.ChessSame1 + chess.ChessSame2 == 2) && (chess.JumpSpace1 * chess.JumpSpace2 > 0)) {
            weight++;
            return 3000;       //连活二90101
        } else if ((chess.JumpChess1 + chess.ChessSame2 == 3) &&
                   ((chess.NSameChess1 - chess.JumpChess1 - 1) * (chess.NSameChess2 - chess.ChessSame2) > 0)) {
            weight++;
            return 800;                 //活跳三
        } else if ((chess.JumpChess1 + chess.ChessSame2 == 3) &&
                   ((chess.NSameChess1 - chess.JumpChess1 - 1) * (chess.NSameChess2 - chess.ChessSame2) == 0)) {
            weight++;
            return 250;                 //死跳三
        } else if ((chess.JumpChess2 + chess.ChessSame1 == 3) &&
                   ((chess.NSameChess2 - chess.JumpChess2 - 1) * (chess.NSameChess1 - chess.ChessSame1) > 0)) {
            weight++;
            return 800;                 //活跳三
        } else if ((chess.JumpChess2 + chess.ChessSame1 == 3) &&
                   ((chess.NSameChess2 - chess.JumpChess2 - 1) * (chess.NSameChess1 - chess.ChessSame1) == 0)) {
            weight++;
            return 250;                 //死跳三
        } else if (
                ((chess.ChessSame1 == 1 && chess.JumpChess1 == 2) && chess.JumpSpace2 >= 1 && chess.NSameChess1 >= 4) ||
                ((chess.ChessSame2 == 1 && chess.JumpChess2 == 2) && chess.JumpSpace1 >= 1 && chess.NSameChess2 >= 4))
            return 500;                 //单跳活二
        else if ((chess.ChessSame1 + chess.ChessSame2 == 1) && (chess.JumpSpace1 + chess.JumpSpace2 > 2) &&
                 (chess.JumpSpace1 * chess.JumpSpace2 != 0)) {
            return 100;                //活一
        }
        return 0;
    }
}

void WeightSum(coordinate *Ai) {
    for (int i = 0; i < Axis; ++i) {
        for (int j = 0; j < Axis; ++j) {
            int line[4];
            if (ChessDate[i][j].color == 0) {
                int weight = 0;
                for (int mode = 0; mode < 4; ++mode) {
                    line[mode] = ChessWeight(JudgeXYState(mode, i, j, 2), 2, weight);
                    if (weight == 20||weight == 10){
                        break;
                    }

                }
                if (weight == 20){
                    WeightSumWB[i][j]=INT_MAX;
                    continue;
                }else if(weight == 10){
                    WeightSumWB[i][j]=INT_MAX;
                    continue;
                }else if (weight > 1 && weight < 10) {
                    WeightSumWB[i][j] = INT_MAX;
                    continue;
                }
                WeightSumWB[i][j] = line[0] + line[1] + line[2] + line[3];

                weight = 0;
                for (int mode = 0; mode < 4; ++mode) {
                    line[mode] = ChessWeight(JudgeXYState(mode, i, j, 1), 1, weight);
                    if (weight == 20||weight == 10){
                        break;
                    }
                }
                if (weight == 20){
                    WeightSumWB[i][j]=INT_MAX;
                    continue;
                }else if(weight == 10){
                    WeightSumWB[i][j]=INT_MAX;
                    continue;
                }else if (weight > 1 && weight < 10) {
                    WeightSumWB[i][j] = INT_MAX;
                    continue;
                }
                WeightSumWB[i][j] += line[0] + line[1] + line[2] + line[3];

            } else
                WeightSumWB[i][j] = -1;
        }
    }
    for (int i = 0; i < Axis; ++i) {
        for (int j = 0; j < Axis; ++j) {
            if (WeightSumWB[Ai->x][Ai->y] < WeightSumWB[i][j])
                Ai->x = i, Ai->y = j;
            if (WeightSumWB[Ai->x][Ai->y] == WeightSumWB[i][j]) {
                if (rand() % 100 > 50) {
                    Ai->x = i, Ai->y = j;
                }
            }
        }
    }
    system("cls");
    for (int j = 0; j < Axis; ++j) {
        for (int i = 0; i < Axis; ++i) {
            printf("%9d", WeightSumWB[i][j]);
        }
        printf("\n");
    }
}


void FindMouse(int Is_Game) {
    if (MouseHit()) {
        MOUSEMSG msg = GetMouseMsg();
        if (msg.x > UNIT && msg.x<(Height - 1) * UNIT && msg.y>(ToolSize + 1) * UNIT &&
            msg.y < (ToolSize + Height - 1) * UNIT) {                       //边界判断
            xx = msg.x / UNIT - 1;
            yy = msg.y / UNIT - ToolSize - 1;
            if (ChessDate[xx][yy].color != 0)return;
            if (msg.uMsg == WM_LBUTTONDOWN && msg.uMsg != WM_MOUSEMOVE) {
                if (turn == HUMAN) {
                    turn = AI;
                    ChessDate[xx][yy].color = 1;
                    if (Is_Finish(xx, yy) >= 5)return;
                } else if (turn == AI && Is_Game == 1) {
                    turn = HUMAN;
                    ChessDate[xx][yy].color = 2;
                    if (Is_Finish(xx, yy) >= 5)return;
                }
            }
            if (turn == AI && Is_Game == 2) {
                coordinate Ai;
                Ai.x = Ai.y = 0;
                turn = HUMAN;
                WeightSum(&Ai);
                xx = Ai.x, yy = Ai.y;
                ChessDate[xx][yy].color = 2;
                if (Is_Finish(xx, yy) >= 5)return;
            }
        }
        FlushMouseMsgBuffer();
    }
}

int main() {
    while (true) {
        clock_t start;
        turn = HUMAN;
        int Is_Game;
        for (int i = 0; i < Axis; ++i) {
            for (int j = 0; j < Axis; ++j) {
                ChessDate[i][j].color = WeightSumWB[i][j] = 0;
            }
        }
        initgraph(Height * UNIT, (Height + ToolSize) * UNIT, SHOWCONSOLE);
        HWND hnd = GetHWnd();
        SetWindowText(hnd, "我是霡霂版的EasyX");
        Is_Game = DrawGameInterface();
        start = clock();
        srand(time(NULL));
        while (true) {
            BeginBatchDraw();
            Sleep(1);
            DrawHeadboard();
            DrawCheckerboard();
            if (Is_Finish(xx, yy) >= 5)break;
            FindMouse(Is_Game);
            if (ChessDate[xx][yy].color == 0) {
                setlinecolor(RGB(31, 200, 146));
                circle((xx + 1) * UNIT + UNIT / 2, (yy + 1) * UNIT + ToolSize * UNIT + UNIT / 2, UNIT - 8);
            }
            char time[30];
            settextcolor(BLUE);
            settextstyle(UNIT, 0, "楷体");
            sprintf(time, "已运行时长:%d 秒", (clock() - start) / CLOCKS_PER_SEC);
            outtextxy(0, 2 * UNIT, time);
            EndBatchDraw();

        }
        const char *text = (turn == AI ? "恭喜黑棋胜利" : "恭喜白旗胜利");
        settextcolor(RED);
        outtextxy(0, 0, text);
        setfillcolor(ChessDate[xx][yy].color == 1 ? BLACK : WHITE);
        solidcircle(UNIT * (1 + xx) + UNIT / 2, UNIT * (ToolSize + 1 + yy) + UNIT / 2, UNIT / 2 - UNIT / 10);
        char time[30];
        settextcolor(BLUE);
        settextstyle(UNIT, 0, "楷体");
        sprintf(time, "运行时长:%d 秒", (clock() - start) / CLOCKS_PER_SEC);
        outtextxy(0, 2 * UNIT, time);
        EndBatchDraw();
        int Is_OK = MessageBox(hnd, "亲，是否再玩一局？", "提示", MB_OKCANCEL);
        if (Is_OK == IDCANCEL)
            break;
    }
    return 0;
}

