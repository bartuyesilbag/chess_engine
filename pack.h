#ifndef PACK_H
#define PACK_H
#include <QVector>

struct chess_pack{
    int X;
    int Y;
    int destX;
    int destY;
    char name;
    bool capture_the_flag = false;
};

#endif // PACK_H
