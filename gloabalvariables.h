#ifndef GLOABALVARIABLES_H
#define GLOABALVARIABLES_H
#include "safelist.h"
#include "pack.h"

struct glob{
    safelist<chess_pack> *list;
    safelist<chess_pack> *white_list;
    chess_pack  pack;
    char chessBoard[8][8];
    char piece_list[14] = {'.','P','R','N','B','Q','K','p','r','n','b','q','k'};
};


#endif // GLOABALVARIABLES_H

