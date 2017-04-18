#ifndef PACK_H
#define PACK_H
#include <QVector>

struct flags{
    bool capture_the_flag = false;
    bool check_mate_flag;
};

struct chess_pack{
    int X;
    int Y;
    int destX;
    int destY;
    char name;
    flags flag;
};



#endif // PACK_H
