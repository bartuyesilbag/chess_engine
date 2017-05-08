#ifndef PACK_H
#define PACK_H
#include <safelist.h>
#include <QVector>

enum castling_status{
    none = 0 ,
    short_castling,
    long_castling
};

struct castling_flags{
    int cast_stat = castling_status::none;
    bool r1_flag = true;
    bool r2_flag = true;
    bool k_flag = true;
    bool R1_flag = true;
    bool R2_flag = true;
    bool K_flag = true;
};


struct flags{
    bool capture_the_flag = false;
    bool check_mate_flag_black = false;
    bool check_mate_flag_white = false;
    bool pawn_change_flag_black = false;
    bool pawn_change_flag_white = false;
    castling_flags castling;
};

struct score_pack{
    QVector<int> score;
};

struct chess_pack{
    int           X;
    int           Y;
    int           destX;
    int           destY;
    char          name;
    flags         flag;
};





#endif // PACK_H
