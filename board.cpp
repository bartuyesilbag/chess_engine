#include "board.h"
#include "pieces.h"
#include "defines.h"
#include "safelist.h"

#include <iostream>

#include <QDebug>
#include <QVector>
#include <QPoint>
#include <QObject>

board::board(safelist<chess_pack> *_list, QObject *parent) : QObject(parent)
{
    list = _list;
    setup_board();

}

void board::setup_board()
{
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            chessBoard[h][w] = piece_list[chess_pieces::Empty];
        }
    }
    for(int w = realBoardVertical::a; w SM realBoardVertical::h; w++){
        chessBoard[realBoardHorizontal::two][w] = piece_list[chess_pieces::WhitePawn];
    }
    chessBoard[realBoardHorizontal::one][realBoardVertical::a]   = piece_list[chess_pieces::WhiteRook  ];
    chessBoard[realBoardHorizontal::one][realBoardVertical::b]   = piece_list[chess_pieces::WhiteKnight];
    chessBoard[realBoardHorizontal::one][realBoardVertical::c]   = piece_list[chess_pieces::WhiteBishop];
    chessBoard[realBoardHorizontal::one][realBoardVertical::d]   = piece_list[chess_pieces::WhiteQueen ];
    chessBoard[realBoardHorizontal::one][realBoardVertical::e]   = piece_list[chess_pieces::WhiteKing  ];
    chessBoard[realBoardHorizontal::one][realBoardVertical::f]   = piece_list[chess_pieces::WhiteBishop];
    chessBoard[realBoardHorizontal::one][realBoardVertical::g]   = piece_list[chess_pieces::WhiteKnight];
    chessBoard[realBoardHorizontal::one][realBoardVertical::h]   = piece_list[chess_pieces::WhiteRook  ];

    for(int w = realBoardVertical::a; w SM realBoardVertical::h; w++){
        chessBoard[realBoardHorizontal::seven][w] = piece_list[chess_pieces::BlackPawn];
    }
    chessBoard[realBoardHorizontal::eight][realBoardVertical::a] = piece_list[chess_pieces::BlackRook  ];
    chessBoard[realBoardHorizontal::eight][realBoardVertical::b] = piece_list[chess_pieces::BlackKnight];
    chessBoard[realBoardHorizontal::eight][realBoardVertical::c] = piece_list[chess_pieces::BlackBishop];
    chessBoard[realBoardHorizontal::eight][realBoardVertical::d] = piece_list[chess_pieces::BlackKing  ];
    chessBoard[realBoardHorizontal::eight][realBoardVertical::e] = piece_list[chess_pieces::BlackQueen ];
    chessBoard[realBoardHorizontal::eight][realBoardVertical::f] = piece_list[chess_pieces::BlackBishop];
    chessBoard[realBoardHorizontal::eight][realBoardVertical::g] = piece_list[chess_pieces::BlackKnight];
    chessBoard[realBoardHorizontal::eight][realBoardVertical::h] = piece_list[chess_pieces::BlackRook  ];

}

void board::slot_msg_to_board(QPoint ex_p,QPoint new_p,std::string message)
{
    if(message EQ "board"){
        show_board();
        emit signal_from_board(turn_counter);
    }
    else if (message EQ "turn?") {
        qDebug() << "turn count: " << turn_counter;
        emit signal_from_board(turn_counter);
    }
    else if (message EQ "restart") {
        setup_board();
        turn_counter = 0;
        qDebug() << "Match Started Again!";
        emit signal_from_board(turn_counter);
    }
    else{
        char c = chessBoard[ex_p.x()][ex_p.y()];
        if(chessBoard[ex_p.x()][ex_p.y()] NE piece_list[chess_pieces::Empty]){
            chessBoard[ex_p.x()][ex_p.y()]   = piece_list[chess_pieces::Empty];
            chessBoard[new_p.x()][new_p.y()] = c;
            turn_counter++;
            emit signal_from_board(turn_counter);
        }
        else{
            qDebug() << "there is not a piece in ex_p, please enter a new move!";
            emit signal_from_board(turn_counter);
        }
    }
}

void board::rook(int x, int y)
{
    //qDebug() << "rook";
    for(int line=x-1;line BG 0;line--){
        if(chessBoard[line][y]   NE piece_list[chess_pieces::Empty]){

            break;
        }
        pack.destX      = line  ;
        pack.destY      = y     ;
        //pack.name       = piece_list[chess_pieces::BlackRook];
        list->push(pack);
        //qDebug() << "Available Rook Move!" << line << y;
    }
    for(int line=x+1;line SM 7;line++){
        if(chessBoard[line][y]   NE piece_list[chess_pieces::Empty]){
            break;
        }
        pack.destX      = line  ;
        pack.destY      = y     ;
        //pack.name       = piece_list[chess_pieces::BlackRook];
        list->push(pack);
        //qDebug() << "Available Rook Move!" << line << y;
    }
    for(int column=y-1;column BG 0;column--){
        if(chessBoard[x][column] NE piece_list[chess_pieces::Empty]){
            break;
        }
        pack.destX      = x     ;
        pack.destY      = column;
        //pack.name       = piece_list[chess_pieces::BlackRook];
        list->push(pack);
        //qDebug() << "Available Rook Move!" << x << column;
    }
    for(int column=y+1;column SM 7;column++){
        if(chessBoard[x][column] NE piece_list[chess_pieces::Empty]){
            break;
        }
        pack.destX      = x     ;
        pack.destY      = column;
        //pack.name       = piece_list[chess_pieces::BlackRook];
        list->push(pack);
        //qDebug() << "Available Rook Move!" << x << column;
    }
}

void board::knight(int x, int y){
    if(chessBoard[x+1][y+2] EQ piece_list[chess_pieces::Empty] && x+1 SM 7 && y+2 SM 7){
        //qDebug() << "Available Knight Move1!" << x+1 << y+2;
        pack.destX      = x+1   ;
        pack.destY      = y+2   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    if(chessBoard[x+1][y-2] EQ piece_list[chess_pieces::Empty] && x+1 SM 7 && y-2 BG 0){
        //qDebug() << "Available Knight Move2!" << x+1 << y-2;
        pack.destX      = x+1   ;
        pack.destY      = y-2   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    if(chessBoard[x-1][y+2] EQ piece_list[chess_pieces::Empty] && x-1 BG 0 && y+2 SM 7){
        //qDebug() << "Available Knight Move3!" << x-1 << y+2;
        pack.destX      = x-1   ;
        pack.destY      = y+2   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    if(chessBoard[x-1][y-2] EQ piece_list[chess_pieces::Empty] && x-1 BG 0 && y-2 BG 0){
        //qDebug() << "Available Knight Move4!" << x-1 << y-2;
        pack.destX      = x-1   ;
        pack.destY      = y-2   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    if(chessBoard[x+2][y+1] EQ piece_list[chess_pieces::Empty] && x+2 SM 7 && y+1 SM 7){
        //qDebug() << "Available Knight Move5!" << x+2 << y+1;
        pack.destX      = x+2   ;
        pack.destY      = y+1   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    if(chessBoard[x+2][y-1] EQ piece_list[chess_pieces::Empty] && x+2 SM 7 && y-1 BG 0){
        //qDebug() << "Available Knight Move6!" << x+2 << y-1;
        pack.destX      = x+2   ;
        pack.destY      = y-1   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    if(chessBoard[x-2][y+1] EQ piece_list[chess_pieces::Empty] && x-2 BG 0 && y+1 SM 7){
        //qDebug() << "Available Knight Move7!" << x-2 << y+1;
        pack.destX      = x-2   ;
        pack.destY      = y+1   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    if(chessBoard[x-2][y-1] EQ piece_list[chess_pieces::Empty] && x-2 BG 0 && y-1 BG 0){
        //qDebug() << "Available Knight Move8!" << x-2 << y-1;
        pack.destX      = x-2   ;
        pack.destY      = y-1   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    //qDebug() << "knight end";
}

void board::bishop(int x, int y){
    //qDebug() << "bishop";
    int column = y;
    for(int line = x-1;line BG 0; line--){
        column = column - 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column < 0){
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackBishop];
        list->push(pack);
        //qDebug() << "Available Bishop Move!" << line << column;
    }
    for(int line = x+1;line SM 7; line++){
        column = column + 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column > 7){
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackBishop];
        list->push(pack);
        //qDebug() << "Available Bishop Move!" << line << column;
    }
    for(int line = x+1;line SM 7; line++){
        column = column - 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column < 0){
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackBishop];
        list->push(pack);
        //qDebug() << "Available Bishop Move!" << line << column;
    }
    for(int line = x-1;line BG 0; line--){
        column = column + 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column > 7){
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackBishop];
        list->push(pack);
        //qDebug() << "Available Bishop Move!" << line << column;
    }
}

void board::queen(int x, int y){
    //qDebug() << "queen";
    int column = y;
    for(int line = x-1;line BG 0; line--){
        column = column - 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column < 0){
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackQueen];
        list->push(pack);
        //qDebug() << "Available Queen Move!" << line << column;
    }
    for(int line = x+1;line SM 7; line++){
        column = column + 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column > 7){
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackQueen];
        list->push(pack);
        //qDebug() << "Available Queen Move!" << line << column;
    }
    for(int line = x+1;line SM 7; line++){
        column = column - 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column < 0){
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackQueen];
        list->push(pack);
        //qDebug() << "Available Queen Move!" << line << column;
    }
    for(int line = x-1;line BG 0; line--){
        column = column + 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column > 7){
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackQueen];
        list->push(pack);
        //qDebug() << "Available Queen Move!" << line << column;
    }
    for(int line=x-1;line BG 0;line--){
        if(chessBoard[line][y]   NE piece_list[chess_pieces::Empty]){
            break;
        }
        pack.destX      = line   ;
        pack.destY      = y      ;
        //pack.name       = piece_list[chess_pieces::BlackQueen];
        list->push(pack);
        //qDebug() << "Available Queen Move!" << line << y;
    }
    for(int line=x+1;line SM 7;line++){
        if(chessBoard[line][y]   NE piece_list[chess_pieces::Empty]){
            break;
        }
        pack.destX      = line   ;
        pack.destY      = y      ;
        //pack.name       = piece_list[chess_pieces::BlackQueen];
        list->push(pack);
        //qDebug() << "Available Queen Move!" << line << y;
    }
    for(int column=y-1;column BG 0;column--){
        if(chessBoard[x][column] NE piece_list[chess_pieces::Empty]){
            break;
        }
        pack.destX      = x      ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackQueen];
        list->push(pack);
        //qDebug() << "Available Queen Move!" << x << column;
    }
    for(int column=y+1;column SM 7;column++){
        if(chessBoard[x][column] NE piece_list[chess_pieces::Empty]){
            break;
        }
        pack.destX      = x      ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackQueen];
        list->push(pack);
        //qDebug() << "Available Queen Move!" << x << column;
    }
}

void board::king(int x, int y){
    //qDebug() << "king";
    if(chessBoard[x+1][y]   EQ piece_list[chess_pieces::Empty]){
        pack.destX      = x+1    ;
        pack.destY      = y      ;
        //pack.name       = piece_list[chess_pieces::BlackKing];
        list->push(pack);
        //qDebug() << "Available King Move!" << x+1 << y  ;
    }
    if(chessBoard[x-1][y]   EQ piece_list[chess_pieces::Empty]){
        pack.destX      = x-1    ;
        pack.destY      = y      ;
        //pack.name       = piece_list[chess_pieces::BlackKing];
        list->push(pack);
        //qDebug() << "Available King Move!" << x-1 << y  ;
    }
    if(chessBoard[x+1][y+1] EQ piece_list[chess_pieces::Empty]){
        //qDebug() << "Available King Move!" << x+1 << y+1;
        pack.destX      = x+1    ;
        pack.destY      = y+1    ;
        //pack.name       = piece_list[chess_pieces::BlackKing];
        list->push(pack);
    }
    if(chessBoard[x+1][y-1] EQ piece_list[chess_pieces::Empty]){
        //qDebug() << "Available King Move!" << x+1 << y-1;
        pack.destX      = x+1    ;
        pack.destY      = y-1    ;
        //pack.name       = piece_list[chess_pieces::BlackKing];
        list->push(pack);
    }
    if(chessBoard[x][y+1]   EQ piece_list[chess_pieces::Empty]){
        //qDebug() << "Available King Move!" << x   << y+1;
        pack.destX      = x      ;
        pack.destY      = y+1    ;
        //pack.name       = piece_list[chess_pieces::BlackKing];
        list->push(pack);
    }
    if(chessBoard[x][y-1]   EQ piece_list[chess_pieces::Empty]){
        //qDebug() << "Available King Move!" << x   << y-1;
        pack.destX      = x      ;
        pack.destY      = y-1    ;
        //pack.name       = piece_list[chess_pieces::BlackKing];
        list->push(pack);
    }
    if(chessBoard[x-1][y+1] EQ piece_list[chess_pieces::Empty]){
        //qDebug() << "Available King Move!" << x-1 << y+1;
        pack.destX      = x-1    ;
        pack.destY      = y+1    ;
        //pack.name       = piece_list[chess_pieces::BlackKing];
        list->push(pack);
    }
    if(chessBoard[x-1][y-1] EQ piece_list[chess_pieces::Empty]){
        //qDebug() << "Available King Move!" << x-1 << y-1;
        pack.destX      = x-1    ;
        pack.destY      = y-1    ;
        //pack.name       = piece_list[chess_pieces::BlackKing];
        list->push(pack);
    }
}

void board::whitePawn(int x, int y){
    //qDebug() << "pawn";
    if(chessBoard[x-1][y] EQ piece_list[chess_pieces::Empty]){
        //qDebug() << "Available Pawn Move!" << x-1 << y  ;
        pack.destX      = x-1    ;
        pack.destY      = y      ;
        //pack.name       = c      ;
        list->push(pack);
    }
    if(chessBoard[x-1][y+1] NE piece_list[chess_pieces::Empty]){
        //qDebug() << "Capturable Piece!" << x-1 << y+1 << chessBoard[x-1][y+1];
    }
    if(chessBoard[x-1][y-1] NE piece_list[chess_pieces::Empty]){
        //qDebug() << "Capturable Piece!" << x-1 << y-1 << chessBoard[x-1][y-1];
    }
}

void board::blackPawn(int x, int y)
{
    if(chessBoard[x+1][y] EQ piece_list[chess_pieces::Empty]){
        //qDebug() << "Available Pawn Move!" << x-1 << y  ;
        pack.destX      = x+1    ;
        pack.destY      = y      ;
        //pack.name       = c      ;
        list->push(pack);
    }
    if(chessBoard[x+1][y+1] NE piece_list[chess_pieces::Empty]){
        //qDebug() << "Capturable Piece!" << x-1 << y+1 << chessBoard[x-1][y+1];
    }
    if(chessBoard[x+1][y-1] NE piece_list[chess_pieces::Empty]){
        //qDebug() << "Capturable Piece!" << x-1 << y-1 << chessBoard[x-1][y-1];
    }
}

void board::getBoard(int m)
{
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            if(chessBoard[h][w] NE '0'){
                chess_pack p;
                p.destX = h;
                p.destY = w;
                p.name  = chessBoard[h][w];
                //qDebug() << p.destX << p.destY << p.name;
                switch (m) {
                case mode::white:
                    getWhiteMoves(p.name,p.destX,p.destY);
                    break;
                case mode::black:
                    getBlackMoves(p.name,p.destX,p.destY);
                    break;
                default:
                case mode::both:
                    getMoves(p.name,p.destX,p.destY);
                    break;
                }
            }
        }
    }
}

int board::getPiecesNumber()
{
    int size = 0;
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            if(chessBoard[h][w] NE '0'){
                size++;
            }
        }
    }
    return size;
}

char board::boardSimulate(QPoint ex_p, QPoint new_p, char board[8][8])
{
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            board[h][w] = chessBoard[h][w];
        }
    }
    char c = board[ex_p.x()][ex_p.y()];
    board[ex_p.x()][ex_p.y()]   = piece_list[chess_pieces::Empty];
    board[new_p.x()][new_p.y()] = c;
}

void board::getMoves(char pieceName, int x, int y)
{
    getBlackMoves(pieceName,x,y);
    getWhiteMoves(pieceName,x,y);
}

void board::getBlackMoves(char pieceName, int x, int y)
{
    pack.X = x;
    pack.Y = y;
    pack.name = pieceName;
    switch (pieceName) {
    case 'P':
        blackPawn(x,y);
        break;
    case 'R':
        rook(x,y);
        break;
    case 'N':
        knight(x,y);
        break;
    case 'B':
        bishop(x,y);
        break;
    case 'Q':
        queen(x,y);
        break;
    case 'K':
        king(x,y);
        break;
    default:
        break;
    }
}

void board::getWhiteMoves(char pieceName, int x, int y)
{
    pack.X = x;
    pack.Y = y;
    pack.name = pieceName;
    switch (pieceName) {
    case 'p':
        whitePawn(x,y);
        break;
    case 'r':
        rook(x,y);
        break;
    case 'n':
        knight(x,y);
        break;
    case 'b':
        bishop(x,y);
        break;
    case 'q':
        queen(x,y);
        break;
    case 'k':
        king(x,y);
        break;
    default:
        break;
    }
}

void board::show_board()
{
    qDebug() << "";
    for(int h = 0 ; h<8; h++){
        qDebug() << 8 - h << " " << chessBoard[h][0] << chessBoard[h][1] << chessBoard[h][2] << chessBoard[h][3]
                 << chessBoard[h][4] << chessBoard[h][5] << chessBoard[h][6] << chessBoard[h][7];
    }
    qDebug() << "\n    A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";
}


