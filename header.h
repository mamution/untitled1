//
// Created by ?@? on 2022/3/11.
//
#define UNIT 32        //???????????��
#define Height 21      //?????????????????��
#define ToolSize 3   //???????????��
#define SIZE 20        //?????��
#define Axis SIZE-1     //??????????0???
typedef struct information{
    int x;
    int y;
    int color;        //0:�հ� 1:��ɫ 2:��ɫ
} coordinate;
typedef struct {
    int JumpSpace1;
    int JumpSpace2;
    int ChessSame1;
    int ChessSame2;
    int SpaceSame1;
    int SpaceSame2;
    int JumpChess1;
    int JumpChess2;
    int NSameChess1;
    int NSameChess2;
} ChessState;

#ifndef UNTITLED1_HEADER_H
#define UNTITLED1_HEADER_H

#endif //UNTITLED1_HEADER_H
