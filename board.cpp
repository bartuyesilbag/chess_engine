#include "board.h"
#include "pieces.h"
#include "defines.h"
#include "safelist.h"

#include <iostream>
#include <stdlib.h>

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
//    chessBoard[realBoardHorizontal::seven][realBoardVertical::e]   = piece_list[chess_pieces::BlackKing  ];
//    chessBoard[realBoardHorizontal::two  ][realBoardVertical::h]   = piece_list[chess_pieces::WhitePawn  ];
//    chessBoard[realBoardHorizontal::two  ][realBoardVertical::a]   = piece_list[chess_pieces::WhiteKing  ];
//    chessBoard[realBoardHorizontal::seven][realBoardVertical::h]   = piece_list[chess_pieces::WhiteRook  ];
//    chessBoard[realBoardHorizontal::six][realBoardVertical::g  ]   = piece_list[chess_pieces::WhiteQueen ];
//    chessBoard[realBoardHorizontal::two][realBoardVertical::g  ]   = piece_list[chess_pieces::WhitePawn  ];
//    chessBoard[realBoardHorizontal::seven][realBoardVertical::a]   = piece_list[chess_pieces::WhiteRook  ];
        for(int w = realBoardVertical::a; w SM realBoardVertical::h; w++){
            chessBoard[realBoardHorizontal::two][w] = piece_list[chess_pieces::WhitePawn];
        }

        chessBoard[realBoardHorizontal::one][realBoardVertical::a]   = piece_list[chess_pieces::WhiteRook  ];
        chessBoard[realBoardHorizontal::one][realBoardVertical::b]   = piece_list[chess_pieces::WhiteKnight];
        chessBoard[realBoardHorizontal::one][realBoardVertical::c]   = piece_list[chess_pieces::WhiteBishop];
        chessBoard[realBoardHorizontal::one][realBoardVertical::d]   = piece_list[chess_pieces::WhiteKing  ];
        chessBoard[realBoardHorizontal::one][realBoardVertical::e]   = piece_list[chess_pieces::WhiteQueen ];
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
    if(turn % 2 EQ 0){
        pack.flag.capture_the_flag = true;
    }

    if(message EQ "board"){
        show_board();
        emit signal_from_board(turn);
    }
    else if (message EQ "turn?") {
        qDebug() << "turn count: " << turn;
        emit signal_from_board(turn);
    }
    else if (message EQ "restart") {
        setup_board();
        turn = 0;
        qDebug() << "Match Started Again!";
        emit signal_from_board(turn);
    }
    else if (message EQ "clear") {
        system("clear");
        emit signal_from_board(turn);
    }
    else if (message EQ "quit") {
        return;
    }
    else if (message EQ "help") {
        help();
    }
    else if(message.size() NE 4){
        qDebug() << "Wrong Input! Please try again!";
        emit signal_from_board(turn);
    }
    else{
        char c = chessBoard[ex_p.x()][ex_p.y()];
        if(chessBoard[ex_p.x()][ex_p.y()] NE piece_list[chess_pieces::Empty] AND pack.flag.capture_the_flag EQ true){
            chessBoard[ex_p.x()][ex_p.y()]   = piece_list[chess_pieces::Empty];
            chessBoard[new_p.x()][new_p.y()] = c;
            if(turn_counter % 2 EQ 0 AND turn > 0){
                if(check_endgame()){
                    return;
                }
            }
            turn++;
            emit signal_from_board(turn);
        }

        else if(chessBoard[ex_p.x()][ex_p.y()] NE piece_list[chess_pieces::Empty]){
            if(!check(ex_p,new_p)){
                emit signal_from_board(turn);
            }
            else{
                chessBoard[ex_p.x()][ex_p.y()]   = piece_list[chess_pieces::Empty];
                chessBoard[new_p.x()][new_p.y()] = c;
                if(turn_counter % 2 EQ 0 AND turn > 0){
                    if(check_endgame()){
                        return;
                    }
                }
                turn++;
                emit signal_from_board(turn);
            }
        }
        else{
            qDebug() << "there is not a piece at there , please enter a new move!";
            emit signal_from_board(turn);
        }
    }
}

void board::rook(int x, int y)
{
    for(int line=x-1;line BG 0;line--){
        if(chessBoard[line][y]   NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 14, chessBoard[line][y]));
            if(a > 6){
                //qDebug() << "Capturable Piece!rook" << line << y << piece_list[a];
                pack.flag.capture_the_flag = true;
                pack.destX      = line  ;
                pack.destY      = y     ;
                list->push(pack)        ;
            }
            break;
        }
        pack.destX      = line  ;
        pack.destY      = y     ;
        list->push(pack)        ;
        //qDebug() << "Available Rook Move!" << line << y;
    }
    for(int line=x+1;line SM 7;line++){
        if(chessBoard[line][y]   NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][y]));
            if(a > 6){
                //qDebug() << "Capturable Piece!rook" << line << y << piece_list[a];
                pack.flag.capture_the_flag = true;
                pack.destX      = line  ;
                pack.destY      = y     ;
                list->push(pack)        ;
            }
            break;
        }
        pack.destX      = line  ;
        pack.destY      = y     ;
        //pack.name       = piece_list[chess_pieces::BlackRook];
        list->push(pack);
        //qDebug() << "Available Rook Move!" <<< line << y;
    }
    for(int column=y-1;column BG 0;column--){
        if(chessBoard[x][column] NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!rook" << x << column << piece_list[a];
                pack.flag.capture_the_flag = true;
                pack.destX      = x     ;
                pack.destY      = column;
                list->push(pack)        ;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 14, chessBoard[x][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!rook" << x << column << piece_list[a];
                pack.flag.capture_the_flag = true;
                pack.destX      = x     ;
                pack.destY      = column;
                list->push(pack)        ;

            }
            break;
        }
        pack.destX      = x     ;
        pack.destY      = column;
        //pack.name       = piece_list[chess_pieces::BlackRook];
        list->push(pack);
        //qDebug() << "Available Rook Move!" << x << column;
    }
}

void board::knight(int x, int y)
{
    if(chessBoard[x+1][y+2] EQ piece_list[chess_pieces::Empty] && x+1 SM 7 && y+2 SM 7){

        //qDebug() << "Available Knight Move1!" << x+1 << y+2;
        pack.destX      = x+1   ;
        pack.destY      = y+2   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    else if(x+1 SM 7 && y+2 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y+2]));
        if(a > 6){
            //qDebug() << "Capturable Piece!knight" << x+1 << y+2 << piece_list[a];
            pack.flag.capture_the_flag = true;
        }
    }
    if(chessBoard[x+1][y-2] EQ piece_list[chess_pieces::Empty] && x+1 SM 7 && y-2 BG 0){
        //qDebug() << "Available Knight Move2!" << x+1 << y-2;
        pack.destX      = x+1   ;
        pack.destY      = y-2   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    else if(x+1 SM 7 && y-2 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y-2]));
        if(a > 6){
            //qDebug() << "Capturable Piece!knight" << x+1 << y-2 << piece_list[a];
            pack.flag.capture_the_flag = true;
        }
    }
    if(chessBoard[x-1][y+2] EQ piece_list[chess_pieces::Empty] && x-1 BG 0 && y+2 SM 7){

        //qDebug() << "Available Knight Move3!" << x-1 << y+2;
        pack.destX      = x-1   ;
        pack.destY      = y+2   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    else if(x-1 BG 0 && y+2 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y+2]));
        if(a > 6){
            //qDebug() << "Capturable Piece!knight" << x-1 << y+2 << piece_list[a];
            pack.flag.capture_the_flag = true;
        }
    }
    if(chessBoard[x-1][y-2] EQ piece_list[chess_pieces::Empty] && x-1 BG 0 && y-2 BG 0){
        //qDebug() << "Available Knight Move4!" << x-1 << y-2;
        pack.destX      = x-1   ;
        pack.destY      = y-2   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    else if(x-1 BG 0 && y-2 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y-2]));
        if(a > 6){
            //qDebug() << "Capturable Piece!knight" << x-1 << y-2 << piece_list[a];
            pack.flag.capture_the_flag = true;
        }
    }
    if(chessBoard[x+2][y+1] EQ piece_list[chess_pieces::Empty] && x+2 SM 7 && y+1 SM 7){
        //qDebug() << "Available Knight Move5!" << x+2 << y+1;
        pack.destX      = x+2   ;
        pack.destY      = y+1   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    else if(x+2 SM 7 && y+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+2][y+1]));
        if(a > 6){
            //qDebug() << "Capturable Piece!knight" << x+2 << y+1 << piece_list[a];
            pack.flag.capture_the_flag = true;
        }
    }
    if(chessBoard[x+2][y-1] EQ piece_list[chess_pieces::Empty] && x+2 SM 7 && y-1 BG 0){
        //qDebug() << "Available Knight Move6!" << x+2 << y-1;
        pack.destX      = x+2   ;
        pack.destY      = y-1   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    else if(x+2 SM 7 && y-1 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+2][y-1]));
        if(a > 6){
            //qDebug() << "Capturable Piece!knight" << x+2 << y-1 << piece_list[a];
            pack.flag.capture_the_flag = true;
        }
    }
    if(chessBoard[x-2][y+1] EQ piece_list[chess_pieces::Empty] && x-2 BG 0 && y+1 SM 7){
        //qDebug() << "Available Knight Move7!" << x-2 << y+1;
        pack.destX      = x-2   ;
        pack.destY      = y+1   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    else if(x-2 BG 0 && y+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-2][y+1]));
        if(a > 6){
            //qDebug() << "Capturable Piece!knight" << x-2 << y+1 << piece_list[a];
            pack.flag.capture_the_flag = true;
        }
    }
    if(chessBoard[x-2][y-1] EQ piece_list[chess_pieces::Empty] && x-2 BG 0 && y-1 BG 0){

        //qDebug() << "Available Knight Move8!" << x-2 << y-1;
        pack.destX      = x-2   ;
        pack.destY      = y-1   ;
        //pack.name       = piece_list[chess_pieces::BlackKnight];
        list->push(pack);
    }
    else if(x-2 BG 0 && y-1 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-2][y-1]));
        if(a > 6){
            //qDebug() << "Capturable Piece!knight" << x-2 << y-1 << piece_list[a];
            pack.flag.capture_the_flag = true;
        }
    }
    //qDebug() << "knight end";
}

void board::bishop(int x, int y)
{
    //qDebug() << "bishop";
    int column = y;
    for(int line = x-1;line BG 0; line--){
        column = column - 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column < 0){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!bishop" << line << column << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!bishop" << line << column << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!bishop" << line << column << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!bishop" << line << column << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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

void board::queen(int x, int y)
{
    //qDebug() << "queen";
    int column = y;
    for(int line = x-1;line BG 0; line--){
        column = column - 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column < 0){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!queen" << line << column << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!queen" << line << column << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!queen" << line << column << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!queen" << line << column << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][y]));
            if(a > 6){
                //qDebug() << "Capturable Piece!queen" << line << y << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][y]));
            if(a > 6){
                //qDebug() << "Capturable Piece!queen" << line << y << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!queen" << x << column << piece_list[a];
                pack.flag.capture_the_flag = true;

            }
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
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x][column]));
            if(a > 6){
                //qDebug() << "Capturable Piece!queen" << x << column << piece_list[a];

            }
            break;
        }
        pack.destX      = x      ;
        pack.destY      = column ;
        //pack.name       = piece_list[chess_pieces::BlackQueen];
        list->push(pack);
        //qDebug() << "Available Queen Move!" << x << column;
    }
}

void board::king(int x, int y)
{
    QPoint ex_p(x,y);
    //qDebug() << "king";
    if(chessBoard[x+1][y]   EQ piece_list[chess_pieces::Empty] && x+1 SM 7){
        //checkmate(mode::black,ex_p,QPoint(x+1,y));
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x+1,y))){
            qDebug() << "Available King Move!" << x+1 << y << 0;
            pack.destX      = x+1    ;
            pack.destY      = y      ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    else if(x+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y]));
        if(a > 6){
            //qDebug() << "Capturable Piece!king" << x+1 << y << piece_list[a];
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1    ;
            pack.destY      = y      ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    if(chessBoard[x-1][y]   EQ piece_list[chess_pieces::Empty] && x-1 BG 0){
        //checkmate(mode::black,ex_p,QPoint(x-1,y));
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x-1,y))){
            qDebug() << "Available King Move!" << x-1 << y <<1 ;
            pack.destX      = x-1    ;
            pack.destY      = y      ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    else if(x-1 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y]));
        if(a > 6){
            //qDebug() << "Capturable Piece!king" << x-1 << y << piece_list[a];
            pack.flag.capture_the_flag = true;
            pack.destX      = x-1    ;
            pack.destY      = y      ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    if(chessBoard[x+1][y+1] EQ piece_list[chess_pieces::Empty] && x+1 SM 7 && y+1 SM 7){
        //checkmate(mode::black, ex_p,QPoint(x+1,y+1));
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x+1,y+1))){
            qDebug() << "Available King Move!" << x+1 << y+1<<2;
            pack.destX      = x+1    ;
            pack.destY      = y+1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    else if(x+1 SM 7 && y+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y+1]));
        if(a > 6){
            qDebug() << "Capturable Piece!king" << x+1 << y+1 << piece_list[a];
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1    ;
            pack.destY      = y+1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    if(chessBoard[x+1][y-1] EQ piece_list[chess_pieces::Empty] && x+1 SM 7 && y-1 BG 0){
        //checkmate(mode::black, ex_p,QPoint(x+1,y-1));
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x+1,y-1))){
            qDebug() << "Available King Move!" << x+1 << y-1<<3;
            pack.destX      = x+1    ;
            pack.destY      = y-1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    else if(x+1 SM 7 && y-1 BG 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y-1]));
        if(a > 6){
            qDebug() << "Capturable Piece!king" << x+1 << y-1 << piece_list[a];
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1    ;
            pack.destY      = y-1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    if(chessBoard[x][y+1]   EQ piece_list[chess_pieces::Empty] && y+1 SM 7){
        //checkmate(mode::black, ex_p,QPoint(x,y+1));
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x,y+1))){
            qDebug() << "Available King Move!" << x   << y+1<<4;
            pack.destX      = x      ;
            pack.destY      = y+1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    else if(y+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x][y+1]));
        if(a > 6){
            qDebug() << "Capturable Piece!king" << x << y+1 << piece_list[a];
            pack.flag.capture_the_flag = true;
            pack.destX      = x      ;
            pack.destY      = y+1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    if(chessBoard[x][y-1]   EQ piece_list[chess_pieces::Empty] && y-1 BG 0){
        //checkmate(mode::black, ex_p,QPoint(x,y-1));
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x,y-1))){
            qDebug() << "Available King Move!" << x   << y-1<<5;
            pack.destX      = x      ;
            pack.destY      = y-1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    else if(y-1 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x][y-1]));
        if(a > 6){
            qDebug() << "Capturable Piece!king" << x << y-1 << piece_list[a];
            pack.flag.capture_the_flag = true;
            pack.destX      = x      ;
            pack.destY      = y-1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    if(chessBoard[x-1][y+1] EQ piece_list[chess_pieces::Empty] && x-1 BG 0 && y+1 SM 7){
        //checkmate(mode::black, ex_p,QPoint(x-1,y+1));
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x-1,y+1))){
            qDebug() << "Available King Move!" << x-1 << y+1<<6;
            pack.destX      = x-1    ;
            pack.destY      = y+1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    else if(x-1 BG 0 && y+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y+1]));
        if(a > 6){
            qDebug() << "Capturable Piece!king" << x-1 << y+1 << piece_list[a];
            pack.flag.capture_the_flag = true;
            pack.destX      = x-1    ;
            pack.destY      = y+1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    if(chessBoard[x-1][y-1] EQ piece_list[chess_pieces::Empty] && x-1 BG 0 && y-1 BG 0){
        //checkmate(mode::black, ex_p,QPoint(x-1,y-1));
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x-1,y-1))){
            qDebug() << "Available King Move!" << x-1 << y-1<<7;
            pack.destX      = x-1    ;
            pack.destY      = y-1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
    else if(x-1 BG 0 && y-1 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y-1]));
        if(a > 6){
            qDebug() << "Capturable Piece!king" << x-1 << y-1 << piece_list[a];
            pack.flag.capture_the_flag = true;
            pack.destX      = x-1    ;
            pack.destY      = y-1    ;
            //pack.name       = piece_list[chess_pieces::BlackKing];
            list->push(pack);
        }
    }
}

void board::whitePawn(int x, int y)
{
    if(chessBoard[x-1][y] EQ piece_list[chess_pieces::Empty]){
        //qDebug() << "Available Pawn Move!" << x-1 << y  ;
        pack.destX      = x-1    ;
        pack.destY      = y      ;
        list->push(pack);
    }
    if(chessBoard[x-2][y] EQ piece_list[chess_pieces::Empty] AND y EQ 6){
        qDebug() << x << y;
        pack.destX      = x-2    ;
        pack.destY      = y      ;
        list->push(pack);
    }
    if(chessBoard[x-1][y+1] NE piece_list[chess_pieces::Empty]){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y+1]));
        if(a > 6){
            //qDebug() << "Capturable Piece!whitePawn" << x-1 << y+1;
            pack.flag.capture_the_flag = true;
        }
    }
    if(chessBoard[x-1][y-1] NE piece_list[chess_pieces::Empty]){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y-1]));
        if(a > 6){
            //qDebug() << "Capturable Piece!whitePawn" << x-1 << y-1;
            pack.flag.capture_the_flag = true;
        }
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
    if(chessBoard[x+2][y] EQ piece_list[chess_pieces::Empty] AND y EQ 1){
        pack.destX      = x+2    ;
        pack.destY      = y      ;
        list->push(pack);
    }
    if(chessBoard[x+1][y+1] NE piece_list[chess_pieces::Empty]){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y+1]));
        if(a > 6){
            //qDebug() << "Capturable Piece!blackPawn" << x+1 << y+1;
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1    ;
            pack.destY      = y+1    ;
            list->push(pack);
        }
    }
    if(chessBoard[x+1][y-1] NE piece_list[chess_pieces::Empty]){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y-1]));
        if(a > 6){
            //qDebug() << "Capturable Piece!blackPawn" << x+1 << y-1;
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1    ;
            pack.destY      = y-1    ;
            list->push(pack);
        }
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
                case mode::both:
                    getMoves(p.name,p.destX,p.destY);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

int  board::getPiecesNumber()
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

void board::boardSimulate(QPoint ex_p, QPoint new_p, char board[8][8])
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

bool board::checkmate(int mode,  char board[8][8], QPoint ex_p, QPoint new_p)
{
    bool status = true;
    char _board[8][8];
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            _board[h][w] = board[h][w];
        }
    }
    if(ex_p EQ QPoint(10,10)){
        if(mode EQ 1){
            new_p = getPiecePoint(_board,'K');
        }
        else if (mode EQ 0) {
            new_p = getPiecePoint(_board,'k');
        }
    }
    else{
        _board[ex_p.x()][ex_p.y()]   = piece_list[chess_pieces::Empty];
        _board[new_p.x()][new_p.y()] = piece_list[chess_pieces::BlackKing];
    }

    if(mode EQ 1){
        if(new_p.x()+1 SM 7 && new_p.y()+2 SM 7){
            if(_board[new_p.x()+1][new_p.y()+2]  EQ piece_list[chess_pieces::WhiteKnight]){
                status = false;
            }
        }
        if(new_p.x()+1 SM 7 && new_p.y()-2 BG 0){
            if(_board[new_p.x()+1][new_p.y()-2]  EQ piece_list[chess_pieces::WhiteKnight]){
                status = false;
            }
        }
        if(new_p.x()-1 BG 0 && new_p.y()+2 SM 7){
            if(_board[new_p.x()-1][new_p.y()+2]  EQ piece_list[chess_pieces::WhiteKnight]){
                status = false;
            }
        }
        if(new_p.x()-1 BG 0 && new_p.y()-2 BG 0){
            if(_board[new_p.x()-1][new_p.y()-2]  EQ piece_list[chess_pieces::WhiteKnight]){
                status = false;
            }
        }
        if(new_p.x()+2 SM 7 && new_p.y()+1 SM 7){
            if(_board[new_p.x()+2][new_p.y()+1]  EQ piece_list[chess_pieces::WhiteKnight]){
                status = false;
            }
        }
        if(new_p.x()+2 SM 7 && new_p.y()-1 BG 0){
            if(_board[new_p.x()+2][new_p.y()-1]  EQ piece_list[chess_pieces::WhiteKnight]){
                status = false;
            }
        }
        if(new_p.x()-2 BG 0 && new_p.y()+1 SM 7){
            if(_board[new_p.x()-2][new_p.y()+1]  EQ piece_list[chess_pieces::WhiteKnight]){
                status = false;
            }
        }
        if(new_p.x()-2 BG 0 && new_p.y()-1 BG 0){
            if(_board[new_p.x()-2][new_p.y()-1]  EQ piece_list[chess_pieces::WhiteKnight]){
                status = false;
            }
        }

        int column = new_p.y();
        for(int line = new_p.x()-1;line BG 0; line--){
            column = column - 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column BG 0){
                if(_board[line][column] EQ piece_list[chess_pieces::WhiteBishop] OR _board[line][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
            }
        }
        column = new_p.y();
        for(int line = new_p.x()+1;line SM 7; line++){
            column = column + 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column SM 7){
                if(_board[line][column] EQ piece_list[chess_pieces::WhiteBishop] OR _board[line][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
            }
        }
        column = new_p.y();
        for(int line = new_p.x()+1;line SM 7; line++){
            column = column - 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column BG 0){
                if(_board[line][column] EQ piece_list[chess_pieces::WhiteBishop] OR _board[line][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
            }
        }
        column = new_p.y();
        for(int line = new_p.x()-1;line BG 0; line--){
            column = column + 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column SM 7){
                if(_board[line][column] EQ piece_list[chess_pieces::WhiteBishop] OR _board[line][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
            }
        }
        column = new_p.y();
        for(int line= new_p.x()-1;line BG 0;line--){
            if(_board[line][new_p.y()]   NE piece_list[chess_pieces::Empty]){
                if(_board[line][new_p.y()] EQ piece_list[chess_pieces::WhiteRook] OR _board[line][new_p.y()] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
            }
        }
        for(int line=new_p.x()+1;line SM 7;line++){
            if(_board[line][new_p.y()]   NE piece_list[chess_pieces::Empty]){
                if(_board[line][new_p.y()] EQ piece_list[chess_pieces::WhiteRook] OR _board[line][new_p.y()] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
            }
        }
        for(int column=new_p.y()-1;column BG 0;column--){
            if(_board[new_p.x()][column] NE piece_list[chess_pieces::Empty]){
                if(_board[new_p.x()][column] EQ piece_list[chess_pieces::WhiteRook] OR _board[new_p.x()][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
            }
        }
        for(int column=new_p.y()+1;column SM 7;column++){
            if(_board[new_p.x()][column] NE piece_list[chess_pieces::Empty]){
                if(_board[new_p.x()][column] EQ piece_list[chess_pieces::WhiteRook] OR _board[new_p.x()][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
            }
        }
    }
    else if (mode EQ 0) {
        if(new_p.x()+1 SM 7 && new_p.y()+2 SM 7){
            if(_board[new_p.x()+1][new_p.y()+2]  EQ piece_list[chess_pieces::BlackKnight]){
                status = false;
            }
        }
        if(new_p.x()+1 SM 7 && new_p.y()-2 BG 0){
            if(_board[new_p.x()+1][new_p.y()-2]  EQ piece_list[chess_pieces::BlackKnight]){
                status = false;
            }
        }
        if(new_p.x()-1 BG 0 && new_p.y()+2 SM 7){
            if(_board[new_p.x()-1][new_p.y()+2]  EQ piece_list[chess_pieces::BlackKnight]){
                status = false;
            }
        }
        if(new_p.x()-1 BG 0 && new_p.y()-2 BG 0){
            if(_board[new_p.x()-1][new_p.y()-2]  EQ piece_list[chess_pieces::BlackKnight]){
                status = false;
            }
        }
        if(new_p.x()+2 SM 7 && new_p.y()+1 SM 7){
            if(_board[new_p.x()+2][new_p.y()+1]  EQ piece_list[chess_pieces::BlackKnight]){
                status = false;
            }
        }
        if(new_p.x()+2 SM 7 && new_p.y()-1 BG 0){
            if(_board[new_p.x()+2][new_p.y()-1]  EQ piece_list[chess_pieces::BlackKnight]){
                status = false;
            }
        }
        if(new_p.x()-2 BG 0 && new_p.y()+1 SM 7){
            if(_board[new_p.x()-2][new_p.y()+1]  EQ piece_list[chess_pieces::BlackKnight]){
                status = false;
            }
        }
        if(new_p.x()-2 BG 0 && new_p.y()-1 BG 0){
            if(_board[new_p.x()-2][new_p.y()-1]  EQ piece_list[chess_pieces::BlackKnight]){
                status = false;
            }
        }

        int column = new_p.y();
        for(int line = new_p.x()-1;line BG 0; line--){
            column = column - 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column BG 0){
                if(_board[line][column] EQ piece_list[chess_pieces::BlackBishop] OR _board[line][column] EQ piece_list[chess_pieces::BlackQueen]){
                    status = false;
                }
            }
        }
        column = new_p.y();
        for(int line = new_p.x()+1;line SM 7; line++){
            column = column + 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column SM 7){
                if(_board[line][column] EQ piece_list[chess_pieces::BlackBishop] OR _board[line][column] EQ piece_list[chess_pieces::BlackQueen]){
                    status = false;
                }
            }
        }
        column = new_p.y();
        for(int line = new_p.x()+1;line SM 7; line++){
            column = column - 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column BG 0){
                if(_board[line][column] EQ piece_list[chess_pieces::BlackBishop] OR _board[line][column] EQ piece_list[chess_pieces::BlackQueen]){
                    status = false;
                }
            }
        }
        column = new_p.y();
        for(int line = new_p.x()-1;line BG 0; line--){
            column = column + 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column SM 7){
                if(_board[line][column] EQ piece_list[chess_pieces::BlackBishop] OR _board[line][column] EQ piece_list[chess_pieces::BlackQueen]){
                    status = false;
                }
            }
        }
        column = new_p.y();
        for(int line= new_p.x()-1;line BG 0;line--){
            if(_board[line][new_p.y()]   NE piece_list[chess_pieces::Empty]){
                if(_board[line][new_p.y()] EQ piece_list[chess_pieces::BlackRook] OR _board[line][new_p.y()] EQ piece_list[chess_pieces::BlackQueen]){
                    status = false;
                }
            }
        }
        for(int line=new_p.x()+1;line SM 7;line++){
            if(_board[line][new_p.y()]   NE piece_list[chess_pieces::Empty]){
                if(_board[line][new_p.y()] EQ piece_list[chess_pieces::BlackRook] OR _board[line][new_p.y()] EQ piece_list[chess_pieces::BlackQueen]){
                    status = false;
                }
            }
        }
        for(int column=new_p.y()-1;column BG 0;column--){
            if(_board[new_p.x()][column] NE piece_list[chess_pieces::Empty]){
                if(_board[new_p.x()][column] EQ piece_list[chess_pieces::BlackRook] OR _board[new_p.x()][column] EQ piece_list[chess_pieces::BlackQueen]){
                    status = false;
                }
            }
        }
        for(int column=new_p.y()+1;column SM 7;column++){
            if(_board[new_p.x()][column] NE piece_list[chess_pieces::Empty]){
                if(_board[new_p.x()][column] EQ piece_list[chess_pieces::BlackRook] OR _board[new_p.x()][column] EQ piece_list[chess_pieces::BlackQueen]){
                    status = false;
                }
            }
        }
    }
    return status;
}

bool board::check_endgame()
{
    if(!checkmate(mode::black,chessBoard)){
        qDebug() << "Check Mate! White Win!";
        return true;
    }
    if(!checkmate(mode::white,chessBoard)){
        qDebug() << "Check Mate! Black Win!";
        return true;
    }


    return false;
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
        qDebug() << h << 8 - h << " " << chessBoard[h][0] << chessBoard[h][1] << chessBoard[h][2] << chessBoard[h][3]
                 << chessBoard[h][4] << chessBoard[h][5] << chessBoard[h][6] << chessBoard[h][7];
    }
    qDebug() << "\n      A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";
    qDebug() << "\n      0" << "1" << "2" << "3" << "4" << "5" << "6" << "7";
}

bool board::check(QPoint ex_p, QPoint new_p)
{
    char c = chessBoard[ex_p.x()][ex_p.y()];
    getMoves(c,ex_p.x(),ex_p.y());
    int list_size = list->size();
    for(int i = 0; i < list_size; i++){
        list->pop(pack);
        if(pack.destX EQ new_p.x() AND pack.destY EQ new_p.y()){
            list->clearList();
            return true;
        }
    }
    list->clearList();
    return false;
}

QPoint board::getPiecePoint(char _board[8][8],char piece)
{
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            if(_board[h][w] EQ piece){
                return QPoint(h,w);
            }
        }
    }
}

void board::help(){
    qDebug() << "|-----------------------------------------|";
    qDebug() << "|COMMANDS---------------------------------|";
    qDebug() << "|show chessboard - board                  |";
    qDebug() << "|show turn count - turn?                  |";
    qDebug() << "|restart game    - restart                |";
    qDebug() << "|clear console   - clear                  |";
    qDebug() << "|quit game       - quit                   |";
    qDebug() << "|PLAYING----------------------------------|";
    qDebug() << "|game start when you make your first move |";
    qDebug() << "|moves be like 'h2h3'                     |";
    qDebug() << "|it means, piece moves from h2 to h3      |";
    qDebug() << "|-----------------------------------------|";
}
