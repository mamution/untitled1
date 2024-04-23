//
// Created by ?@? on 2022/3/11.
//
#define UNIT 32        //???????????§³
#define Height 21      //?????????????????§³
#define ToolSize 3   //???????????§³
#define SIZE 20        //?????§³
#define Axis SIZE-1     //??????????0???
typedef struct information{
    int x;
    int y;
    int color;        //0:¿Õ°× 1:ºÚÉ« 2:°×É«
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
