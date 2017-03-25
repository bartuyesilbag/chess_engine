#ifndef PIECES_H
#define PIECES_H

enum chess_pieces{
    Empty       ,
    BlackPawn   ,
    BlackRook   ,
    BlackKnight ,
    BlackBishop ,
    BlackQueen  ,
    BlackKing   ,
    WhitePawn   ,
    WhiteRook   ,
    WhiteKnight ,
    WhiteBishop ,
    WhiteQueen  ,
    WhiteKing
};


enum realBoardVertical{
    a=0         ,
    b           ,
    c           ,
    d           ,
    e           ,
    f           ,
    g           ,
    h
};

enum realBoardHorizontal{
    eight=0     ,
    seven       ,
    six         ,
    five        ,
    four        ,
    three       ,
    two         ,
    one
};

enum mode{
    white       ,
    black       ,
    both        ,
};

#endif // PIECES_H
