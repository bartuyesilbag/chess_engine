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

    //chessBoard[1][1]   = piece_list[chess_pieces::WhiteRook  ];
    chessBoard[3][1]   = piece_list[chess_pieces::WhiteRook  ];
    chessBoard[3][0]   = piece_list[chess_pieces::WhiteQueen ];
    chessBoard[2][4]   = piece_list[chess_pieces::BlackKing  ];


    /*chessBoard[realBoardHorizontal::one][realBoardVertical::h]   = piece_list[chess_pieces::WhitePawn  ];
    chessBoard[realBoardHorizontal::seven][realBoardVertical::a]   = piece_list[chess_pieces::WhiteRook  ];
    chessBoard[realBoardHorizontal::four ][realBoardVertical::d]   = piece_list[chess_pieces::BlackPawn  ];
    chessBoard[realBoardHorizontal::four ][realBoardVertical::e]   = piece_list[chess_pieces::BlackPawn  ];
    chessBoard[realBoardHorizontal::four ][realBoardVertical::f]   = piece_list[chess_pieces::BlackPawn  ];
    chessBoard[realBoardHorizontal::five ][realBoardVertical::d]   = piece_list[chess_pieces::BlackPawn  ];
    chessBoard[realBoardHorizontal::five ][realBoardVertical::f]   = piece_list[chess_pieces::BlackPawn  ];
    chessBoard[realBoardHorizontal::five ][realBoardVertical::e]   = piece_list[chess_pieces::BlackKing];*/

    //    for(int w = realBoardVertical::a; w SM realBoardVertical::h; w++){
    //        chessBoard[realBoardHorizontal::two][w] = piece_list[chess_pieces::WhitePawn];
    //    }

    //    chessBoard[realBoardHorizontal::one][realBoardVertical::a]   = piece_list[chess_pieces::WhiteRook  ];
    //    chessBoard[realBoardHorizontal::one][realBoardVertical::b]   = piece_list[chess_pieces::WhiteKnight];
    //    chessBoard[realBoardHorizontal::one][realBoardVertical::c]   = piece_list[chess_pieces::WhiteBishop];
    //    chessBoard[realBoardHorizontal::one][realBoardVertical::d]   = piece_list[chess_pieces::WhiteQueen ];
    //    chessBoard[realBoardHorizontal::one][realBoardVertical::e]   = piece_list[chess_pieces::WhiteKing  ];
    //    chessBoard[realBoardHorizontal::one][realBoardVertical::f]   = piece_list[chess_pieces::WhiteBishop];
    //    chessBoard[realBoardHorizontal::one][realBoardVertical::g]   = piece_list[chess_pieces::WhiteKnight];
    //    chessBoard[realBoardHorizontal::one][realBoardVertical::h]   = piece_list[chess_pieces::WhiteRook  ];

    //    for(int w = realBoardVertical::a; w SM realBoardVertical::h; w++){
    //        chessBoard[realBoardHorizontal::seven][w] = piece_list[chess_pieces::BlackPawn];
    //    }

    //    chessBoard[realBoardHorizontal::eight][realBoardVertical::a] = piece_list[chess_pieces::BlackRook  ];
    //    chessBoard[realBoardHorizontal::eight][realBoardVertical::b] = piece_list[chess_pieces::BlackKnight];
    //    chessBoard[realBoardHorizontal::eight][realBoardVertical::c] = piece_list[chess_pieces::BlackBishop];
    //    chessBoard[realBoardHorizontal::eight][realBoardVertical::d] = piece_list[chess_pieces::BlackQueen ];
    //    chessBoard[realBoardHorizontal::eight][realBoardVertical::e] = piece_list[chess_pieces::BlackKing  ];
    //    chessBoard[realBoardHorizontal::eight][realBoardVertical::f] = piece_list[chess_pieces::BlackBishop];
    //    chessBoard[realBoardHorizontal::eight][realBoardVertical::g] = piece_list[chess_pieces::BlackKnight];
    //    chessBoard[realBoardHorizontal::eight][realBoardVertical::h] = piece_list[chess_pieces::BlackRook  ];

}

void board::slot_msg_to_board(QPoint ex_p,QPoint new_p,std::string message)
{
    if(turn_counter % 2 EQ 0){
        pack.flag.capture_the_flag = true;
    }

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
    else if (message EQ "clear") {
        system("clear");
        emit signal_from_board(turn_counter);
    }
    else if (message EQ "quit") {
        return;
    }
    else if (message EQ "help") {
        help();
        emit signal_from_board(turn_counter);
    }
    else if(message.size() NE 4){
        qDebug() << "Wrong Input! Please try again!";
        emit signal_from_board(turn_counter);
    }
    else{
        char c = chessBoard[ex_p.x()][ex_p.y()];
        castling_conditions_check(ex_p);
        if(chessBoard[ex_p.x()][ex_p.y()] NE piece_list[chess_pieces::Empty]){
            int _mode;
            if(turn_counter % 2 EQ 0){
                _mode = mode::white;
            }
            else if (turn_counter % 2 EQ 1) {
                _mode = mode::black;
            }
            if(!check(ex_p,new_p,_mode)){
                qDebug() << "Wrong Move! Please enter a new move! ";
                emit signal_from_board(turn_counter);
            }
            else{
                chessBoard[ex_p.x()][ex_p.y()]   = piece_list[chess_pieces::Empty];
                chessBoard[new_p.x()][new_p.y()] = c;

                /* siyaha şah çekilme durumu */

                if(pack.flag.check_mate_flag_black EQ true AND !checkmate((turn_counter) % 2,chessBoard)){
                }
                else{
                    pack.flag.check_mate_flag_black = false;
                }
                if(!checkmate((turn_counter + 1) % 2,chessBoard)){
                    pack.flag.check_mate_flag_black = true;
                }


                /* beyaza şah çekilme durumu */

                if(pack.flag.check_mate_flag_white EQ true AND !checkmate((turn_counter) % 1,chessBoard)){
                    qDebug() << "Check-Mate! Black Wins!";
                    return;
                }
                else{
                    pack.flag.check_mate_flag_white = false;
                }
                if(!checkmate((turn_counter + 1) % 1,chessBoard)){
                    pack.flag.check_mate_flag_white = true;
                    qDebug() << "Check for White King!";
                }
                turn_counter++;
                emit signal_from_board(turn_counter);
            }
        }
        else if (pack.flag.castling.k_flag EQ true) {
            if(pack.flag.castling.r1_flag EQ true AND new_p.y() EQ 2 AND c EQ piece_list[chess_pieces::WhiteKing] //uzun rok durumu
                    AND chessBoard[7][1] EQ piece_list[chess_pieces::Empty]
                    AND chessBoard[7][2] EQ piece_list[chess_pieces::Empty]
                    AND chessBoard[7][3] EQ piece_list[chess_pieces::Empty]){
                chessBoard[ex_p.x()][ex_p.y()]   = piece_list[chess_pieces::Empty];
                chessBoard[7][0]                 = piece_list[chess_pieces::Empty];
                chessBoard[7][3]                 = piece_list[chess_pieces::WhiteRook];
                chessBoard[new_p.x()][new_p.y()] = c;
                if(!checkmate((turn_counter + 1) % 2,chessBoard)){
                    pack.flag.check_mate_flag_black = true;
                    qDebug() << "pack.flag.check_mate_flag_black = true; 1" << turn_counter;
                }
                turn_counter++;
                emit signal_from_board(turn_counter);
            }
            else if(pack.flag.castling.r2_flag EQ true AND new_p.y() EQ 6 AND c EQ piece_list[chess_pieces::WhiteKing] // kısa rok durumu
                    AND chessBoard[7][5] EQ piece_list[chess_pieces::Empty]
                    AND chessBoard[7][6] EQ piece_list[chess_pieces::Empty]){
                chessBoard[ex_p.x()][ex_p.y()]   = piece_list[chess_pieces::Empty];
                chessBoard[7][7]                 = piece_list[chess_pieces::Empty];
                chessBoard[7][5]                 = piece_list[chess_pieces::WhiteRook];
                chessBoard[new_p.x()][new_p.y()] = c;
                if(!checkmate((turn_counter + 1) % 2,chessBoard)){
                    pack.flag.check_mate_flag_black = true;
                    qDebug() << "pack.flag.check_mate_flag_black = true; 2" << turn_counter;
                }
                turn_counter++;
                emit signal_from_board(turn_counter);
            }
        }
        else{
            qDebug() << "there is not a piece at there , please enter a new move!";
            emit signal_from_board(turn_counter);
        }
    }
}

void board::rook(int x, int y)
{
    for(int line=x-1;line BG 0;line--){
        if(chessBoard[line][y]   NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 14, chessBoard[line][y]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
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
    }
    for(int line=x+1;line SM 7;line++){
        if(chessBoard[line][y]   NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][y]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line  ;
                pack.destY      = y     ;
                list->push(pack)        ;
            }
            break;
        }
        pack.destX      = line  ;
        pack.destY      = y     ;
        list->push(pack);
    }
    for(int column=y-1;column BG 0;column--){
        if(chessBoard[x][column] NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = x     ;
                pack.destY      = column;
                list->push(pack)        ;

            }
            break;
        }
        pack.destX      = x     ;
        pack.destY      = column;
        list->push(pack);
    }
    for(int column=y+1;column SM 7;column++){
        if(chessBoard[x][column] NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 14, chessBoard[x][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = x     ;
                pack.destY      = column;
                list->push(pack)        ;

            }
            break;
        }
        pack.destX      = x     ;
        pack.destY      = column;
        list->push(pack);
    }
}

void board::knight(int x, int y)
{
    if(chessBoard[x+1][y+2] EQ piece_list[chess_pieces::Empty] && x+1 SM 7 && y+2 SM 7){

        pack.destX      = x+1   ;
        pack.destY      = y+2   ;
        list->push(pack);
    }
    else if(x+1 SM 7 && y+2 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y+2]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1   ;
            pack.destY      = y+2   ;
            list->push(pack);
        }
    }
    if(chessBoard[x+1][y-2] EQ piece_list[chess_pieces::Empty] && x+1 SM 7 && y-2 BG 0){
        pack.destX      = x+1   ;
        pack.destY      = y-2   ;
        list->push(pack);
    }
    else if(x+1 SM 7 && y-2 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y-2]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1   ;
            pack.destY      = y-2   ;
            list->push(pack);
        }
    }
    if(chessBoard[x-1][y+2] EQ piece_list[chess_pieces::Empty] && x-1 BG 0 && y+2 SM 7){
        pack.destX      = x-1   ;
        pack.destY      = y+2   ;
        list->push(pack);
    }
    else if(x-1 BG 0 && y+2 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y+2]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x-1   ;
            pack.destY      = y+2   ;
            list->push(pack);
        }
    }
    if(chessBoard[x-1][y-2] EQ piece_list[chess_pieces::Empty] && x-1 BG 0 && y-2 BG 0){
        pack.destX      = x-1   ;
        pack.destY      = y-2   ;
        list->push(pack);
    }
    else if(x-1 BG 0 && y-2 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y-2]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x-1   ;
            pack.destY      = y-2   ;
            list->push(pack);
        }
    }
    if(chessBoard[x+2][y+1] EQ piece_list[chess_pieces::Empty] && x+2 SM 7 && y+1 SM 7){
        pack.destX      = x+2   ;
        pack.destY      = y+1   ;
        list->push(pack);
    }
    else if(x+2 SM 7 && y+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+2][y+1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x+2   ;
            pack.destY      = y+1   ;
            list->push(pack);
        }
    }
    if(chessBoard[x+2][y-1] EQ piece_list[chess_pieces::Empty] && x+2 SM 7 && y-1 BG 0){
        pack.destX      = x+2   ;
        pack.destY      = y-1   ;
        list->push(pack);
    }
    else if(x+2 SM 7 && y-1 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+2][y-1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x+2   ;
            pack.destY      = y-1   ;
            list->push(pack);
        }
    }
    if(chessBoard[x-2][y+1] EQ piece_list[chess_pieces::Empty] && x-2 BG 0 && y+1 SM 7){
        pack.destX      = x-2   ;
        pack.destY      = y+1   ;
        list->push(pack);
    }
    else if(x-2 BG 0 && y+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-2][y+1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x-2   ;
            pack.destY      = y+1   ;
            list->push(pack);
        }
    }
    if(chessBoard[x-2][y-1] EQ piece_list[chess_pieces::Empty] && x-2 BG 0 && y-1 BG 0){
        pack.destX      = x-2   ;
        pack.destY      = y-1   ;
        list->push(pack);
    }
    else if(x-2 BG 0 && y-1 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-2][y-1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x-2   ;
            pack.destY      = y-1   ;
            list->push(pack);
        }
    }
}

void board::bishop(int x, int y)
{
    int column = y;
    for(int line = x-1;line BG 0; line--){
        column = column - 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column < 0){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line   ;
                pack.destY      = column ;
                list->push(pack);
            }
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        list->push(pack);
    }
    for(int line = x+1;line SM 7; line++){
        column = column + 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column > 7){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line   ;
                pack.destY      = column ;
                list->push(pack);
            }
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        list->push(pack);
    }
    for(int line = x+1;line SM 7; line++){
        column = column - 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column < 0){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line   ;
                pack.destY      = column ;
                list->push(pack);
            }
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        list->push(pack);
    }
    for(int line = x-1;line BG 0; line--){
        column = column + 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column > 7){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line   ;
                pack.destY      = column ;
                list->push(pack);
            }
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        list->push(pack);
    }
}

void board::queen(int x, int y)
{
    int column = y;
    for(int line = x-1;line BG 0; line--){
        column = column - 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column < 0){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line   ;
                pack.destY      = column ;
                list->push(pack);
            }
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        list->push(pack);
    }
    for(int line = x+1;line SM 7; line++){
        column = column + 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column > 7){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line   ;
                pack.destY      = column ;
                list->push(pack);
            }
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        list->push(pack);
    }
    for(int line = x+1;line SM 7; line++){
        column = column - 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column < 0){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line   ;
                pack.destY      = column ;
                list->push(pack);
            }
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        list->push(pack);
    }
    for(int line = x-1;line BG 0; line--){
        column = column + 1;
        if(chessBoard[line][column] NE piece_list[chess_pieces::Empty] || column > 7){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line   ;
                pack.destY      = column ;
                list->push(pack);
            }
            column = y;
            break;
        }
        pack.destX      = line   ;
        pack.destY      = column ;
        list->push(pack);
    }
    for(int line=x-1;line BG 0;line--){
        if(chessBoard[line][y]   NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][y]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line   ;
                pack.destY      = y ;
                list->push(pack);

            }
            break;
        }
        pack.destX      = line   ;
        pack.destY      = y      ;
        list->push(pack);
    }
    for(int line=x+1;line SM 7;line++){
        if(chessBoard[line][y]   NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[line][y]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = line   ;
                pack.destY      = y ;
                list->push(pack);

            }
            break;
        }
        pack.destX      = line   ;
        pack.destY      = y      ;
        list->push(pack);
    }
    for(int column=y-1;column BG 0;column--){
        if(chessBoard[x][column] NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = x   ;
                pack.destY      = column ;
                list->push(pack);
            }
            break;
        }
        pack.destX      = x      ;
        pack.destY      = column ;
        list->push(pack);
    }
    for(int column=y+1;column SM 7;column++){
        if(chessBoard[x][column] NE piece_list[chess_pieces::Empty]){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x][column]));
            if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
                pack.flag.capture_the_flag = true;
                pack.destX      = x   ;
                pack.destY      = column ;
                list->push(pack);
            }
            break;
        }
        pack.destX      = x      ;
        pack.destY      = column ;
        list->push(pack);
    }
}

void board::king(int x, int y)
{
    QPoint ex_p(x,y);
    if(chessBoard[x+1][y]   EQ piece_list[chess_pieces::Empty] && x+1 SM 7){
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x+1,y))){
            pack.destX      = x+1    ;
            pack.destY      = y      ;
            list->push(pack);
        }
    }
    else if(x+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1    ;
            pack.destY      = y      ;
            list->push(pack);
        }
    }
    if(chessBoard[x-1][y]   EQ piece_list[chess_pieces::Empty] && x-1 BG 0){
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x-1,y))){
            pack.destX      = x-1    ;
            pack.destY      = y      ;
            list->push(pack);
        }
    }
    else if(x-1 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x-1    ;
            pack.destY      = y      ;
            list->push(pack);
        }
    }
    if(chessBoard[x+1][y+1] EQ piece_list[chess_pieces::Empty] && x+1 SM 7 && y+1 SM 7){
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x+1,y+1))){
            pack.destX      = x+1    ;
            pack.destY      = y+1    ;
            list->push(pack);
        }
    }
    else if(x+1 SM 7 && y+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y+1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1    ;
            pack.destY      = y+1    ;
            list->push(pack);
        }
    }
    if(chessBoard[x+1][y-1] EQ piece_list[chess_pieces::Empty] && x+1 SM 7 && y-1 BG 0){
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x+1,y-1))){
            pack.destX      = x+1    ;
            pack.destY      = y-1    ;
            list->push(pack);
        }
    }
    else if(x+1 SM 7 && y-1 BG 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y-1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1    ;
            pack.destY      = y-1    ;
            list->push(pack);
        }
    }
    if(chessBoard[x][y+1]   EQ piece_list[chess_pieces::Empty] && y+1 SM 7){
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x,y+1))){
            pack.destX      = x      ;
            pack.destY      = y+1    ;
            list->push(pack);
        }
    }
    else if(y+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x][y+1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x      ;
            pack.destY      = y+1    ;
            list->push(pack);
        }
    }
    if(chessBoard[x][y-1]   EQ piece_list[chess_pieces::Empty] && y-1 BG 0){
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x,y-1))){
            pack.destX      = x      ;
            pack.destY      = y-1    ;
            list->push(pack);
        }
    }
    else if(y-1 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x][y-1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x      ;
            pack.destY      = y-1    ;
            list->push(pack);
        }
    }
    if(chessBoard[x-1][y+1] EQ piece_list[chess_pieces::Empty] && x-1 BG 0 && y+1 SM 7){
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x-1,y+1))){
            pack.destX      = x-1    ;
            pack.destY      = y+1    ;
            list->push(pack);
        }
    }
    else if(x-1 BG 0 && y+1 SM 7){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y+1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x-1    ;
            pack.destY      = y+1    ;
            list->push(pack);
        }
    }
    if(chessBoard[x-1][y-1] EQ piece_list[chess_pieces::Empty] && x-1 BG 0 && y-1 BG 0){
        if(checkmate(mode::black,chessBoard,ex_p,QPoint(x-1,y-1))){
            pack.destX      = x-1    ;
            pack.destY      = y-1    ;
            list->push(pack);
        }
    }
    else if(x-1 BG 0 && y-1 BG 0){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y-1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x-1    ;
            pack.destY      = y-1    ;
            list->push(pack);
        }
    }
    //    if(pack.flag.castling.K_flag EQ true AND pack.flag.castling.R1_flag EQ true){
    //        if(chessBoard[0][1] EQ piece_list[chess_pieces::Empty] AND chessBoard[0][2] EQ piece_list[chess_pieces::Empty]
    //                AND chessBoard[0][3] EQ piece_list[chess_pieces::Empty]){
    //            pack.flag.castling.cast_stat = castling_status::long_castling;
    //            pack.destX      = x    ;
    //            pack.destY      = y-2  ;
    //            list->push(pack);
    //        }
    //    }
    //    if(pack.flag.castling.K_flag EQ true AND pack.flag.castling.R2_flag EQ true){
    //        if(chessBoard[0][6] EQ piece_list[chess_pieces::Empty] AND chessBoard[0][5] EQ piece_list[chess_pieces::Empty]){
    //            pack.flag.castling.cast_stat = castling_status::short_castling;
    //            pack.destX      = x    ;
    //            pack.destY      = y+2  ;
    //            list->push(pack);
    //        }
    //    }
    //    if(pack.flag.castling.k_flag EQ true AND pack.flag.castling.r1_flag EQ true){
    //        if(chessBoard[7][1] EQ piece_list[chess_pieces::Empty] AND chessBoard[7][2] EQ piece_list[chess_pieces::Empty]
    //                AND chessBoard[7][3] EQ piece_list[chess_pieces::Empty]){
    //            pack.flag.castling.cast_stat = castling_status::long_castling;
    //            pack.destX      = x    ;
    //            pack.destY      = y-2  ;
    //            list->push(pack);
    //        }
    //    }
    //    if(pack.flag.castling.k_flag EQ true AND pack.flag.castling.r2_flag EQ true){
    //        qDebug() << "test 0";
    //        if(chessBoard[7][6] EQ piece_list[chess_pieces::Empty] AND chessBoard[7][5] EQ piece_list[chess_pieces::Empty]){
    //            qDebug() << "test 1";
    //            pack.flag.castling.cast_stat = castling_status::short_castling;
    //            pack.destX      = x    ;
    //            pack.destY      = y+2  ;
    //            list->push(pack);
    //        }
    //    }
}

void board::whitePawn(int x, int y)
{
    if(chessBoard[x-1][y] EQ piece_list[chess_pieces::Empty]){
        pack.destX      = x-1    ;
        pack.destY      = y      ;
        list->push(pack);
    }
    if(chessBoard[x-2][y] EQ piece_list[chess_pieces::Empty] AND x EQ 6){
        pack.destX      = x-2    ;
        pack.destY      = y      ;
        list->push(pack);
    }
    if(chessBoard[x-1][y+1] NE piece_list[chess_pieces::Empty]){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y+1]));
        if(a <= 6){
            pack.flag.capture_the_flag = true;
            pack.destX      = x-1;
            pack.destY      = y+1;
            list->push(pack);
        }
    }
    if(chessBoard[x-1][y-1] NE piece_list[chess_pieces::Empty]){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x-1][y-1]));
        if(a <= 6){
            pack.flag.capture_the_flag = true;
            pack.destX      = x-1;
            pack.destY      = y-1;
            list->push(pack);
        }
    }
}

void board::blackPawn(int x, int y)
{
    if(chessBoard[x+1][y] EQ piece_list[chess_pieces::Empty]){
        pack.destX      = x+1    ;
        pack.destY      = y      ;
        list->push(pack);
    }
    if(chessBoard[x+2][y] EQ piece_list[chess_pieces::Empty] AND x EQ 1){
        pack.destX      = x+2    ;
        pack.destY      = y      ;
        list->push(pack);
    }
    if(chessBoard[x+1][y+1] NE piece_list[chess_pieces::Empty]){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y+1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
            pack.flag.capture_the_flag = true;
            pack.destX      = x+1    ;
            pack.destY      = y+1    ;
            list->push(pack);
        }
    }
    if(chessBoard[x+1][y-1] NE piece_list[chess_pieces::Empty]){
        int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, chessBoard[x+1][y-1]));
        if(a > 6 AND turn_counter % 2 EQ 1 OR a <= 6 AND turn_counter % 2 EQ 0){
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
                switch (m) {
                case mode::white:
                    getWhiteMoves(p.name,p.destX,p.destY);
                    break;
                case mode::black:
                    getBlackMoves(p.name,p.destX,p.destY);
                    break;
                case mode::both:
                    getMoves(p.name,p.destX,p.destY,mode::both);
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

bool board::boardSimulate(QPoint ex_p, QPoint new_p, char board[8][8],int casling_status)
{
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            board[h][w] = chessBoard[h][w];
        }
    }
    if(casling_status EQ castling_status::short_castling){
        board[0][7] = piece_list[chess_pieces::Empty];
        board[0][5] = piece_list[chess_pieces::BlackRook];
    }
    else if (casling_status EQ castling_status::long_castling) {
        board[0][0] = piece_list[chess_pieces::Empty];
        board[0][3] = piece_list[chess_pieces::BlackRook];
    }
    char c = board[ex_p.x()][ex_p.y()];
    board[ex_p.x()][ex_p.y()]   = piece_list[chess_pieces::Empty];
    board[new_p.x()][new_p.y()] = c;
    if(checkmate(1,board,QPoint(10,10),new_p)){
        return true;
    }
    else{
        return false;
    }
}

void board::getMoves(char pieceName, int x, int y,int mode)
{
    if(mode EQ 0 OR mode EQ 2){
        getWhiteMoves(pieceName,x,y);
    }
    if(mode EQ 1 OR mode EQ 2){
        getBlackMoves(pieceName,x,y);
    }

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
        if(mode EQ 1){//black
            new_p = getKingPoint(_board,'K');
        }
        else if (mode EQ 0) {//white
            new_p = getKingPoint(_board,'k');
        }
    }
    else{
        _board[ex_p.x()][ex_p.y()]   = piece_list[chess_pieces::Empty];
        _board[new_p.x()][new_p.y()] = piece_list[chess_pieces::BlackKing];
    }

    if(mode EQ 1){//black
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
                break;
            }
        }
        column = new_p.y();
        for(int line = new_p.x()+1;line SM 7; line++){
            column = column + 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column SM 7){
                if(_board[line][column] EQ piece_list[chess_pieces::WhiteBishop] OR _board[line][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
                break;
            }
        }
        column = new_p.y();
        for(int line = new_p.x()+1;line SM 7; line++){
            column = column - 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column BG 0){
                if(_board[line][column] EQ piece_list[chess_pieces::WhiteBishop] OR _board[line][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
                break;
            }
        }
        column = new_p.y();
        for(int line = new_p.x()-1;line BG 0; line--){
            column = column + 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column SM 7){
                if(_board[line][column] EQ piece_list[chess_pieces::WhiteBishop] OR _board[line][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
                break;
            }
        }
        column = new_p.y();
        for(int line= new_p.x()-1;line BG 0;line--){
            if(_board[line][new_p.y()]   NE piece_list[chess_pieces::Empty]){
                if(_board[line][new_p.y()] EQ piece_list[chess_pieces::WhiteRook] OR _board[line][new_p.y()] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
                break;
            }
        }
        for(int line=new_p.x()+1;line SM 7;line++){
            if(_board[line][new_p.y()]   NE piece_list[chess_pieces::Empty]){
                if(_board[line][new_p.y()] EQ piece_list[chess_pieces::WhiteRook] OR _board[line][new_p.y()] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
                break;
            }
        }
        for(int column=new_p.y()-1;column BG 0;column--){
            if(_board[new_p.x()][column] NE piece_list[chess_pieces::Empty]){
                if(_board[new_p.x()][column] EQ piece_list[chess_pieces::WhiteRook] OR _board[new_p.x()][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
                break;
            }
        }
        for(int column=new_p.y()+1;column SM 7;column++){
            if(_board[new_p.x()][column] NE piece_list[chess_pieces::Empty]){
                if(_board[new_p.x()][column] EQ piece_list[chess_pieces::WhiteRook] OR _board[new_p.x()][column] EQ piece_list[chess_pieces::WhiteQueen]){
                    status = false;
                }
                break;
            }
        }
    }
    else if (mode EQ 0) {
        if(new_p.x()+1 SM 7 && new_p.y()+2 SM 7){
            if(_board[new_p.x()+1][new_p.y()+2]  EQ piece_list[chess_pieces::BlackKnight]){
                //qDebug() << "status 0" ;
                status = false;
            }
        }
        if(new_p.x()+1 SM 7 && new_p.y()-2 BG 0){
            if(_board[new_p.x()+1][new_p.y()-2]  EQ piece_list[chess_pieces::BlackKnight]){
                //qDebug() << "status 1";
                status = false;
            }
        }
        if(new_p.x()-1 BG 0 && new_p.y()+2 SM 7){
            if(_board[new_p.x()-1][new_p.y()+2]  EQ piece_list[chess_pieces::BlackKnight]){
                //qDebug() << "status 02";
                status = false;
            }
        }
        if(new_p.x()-1 BG 0 && new_p.y()-2 BG 0){
            if(_board[new_p.x()-1][new_p.y()-2]  EQ piece_list[chess_pieces::BlackKnight]){
                //qDebug() << "status 03";
                status = false;
            }
        }
        if(new_p.x()+2 SM 7 && new_p.y()+1 SM 7){
            if(_board[new_p.x()+2][new_p.y()+1]  EQ piece_list[chess_pieces::BlackKnight]){
                //qDebug() << "status 04";
                status = false;
            }
        }
        if(new_p.x()+2 SM 7 && new_p.y()-1 BG 0){
            if(_board[new_p.x()+2][new_p.y()-1]  EQ piece_list[chess_pieces::BlackKnight]){
                //qDebug() << "status 05";
                status = false;
            }
        }
        if(new_p.x()-2 BG 0 && new_p.y()+1 SM 7){
            if(_board[new_p.x()-2][new_p.y()+1]  EQ piece_list[chess_pieces::BlackKnight]){
                //qDebug() << "status 06";
                status = false;
            }
        }
        if(new_p.x()-2 BG 0 && new_p.y()-1 BG 0){
            if(_board[new_p.x()-2][new_p.y()-1]  EQ piece_list[chess_pieces::BlackKnight]){
                //qDebug() << "status 07";
                status = false;
            }
        }

        int column = new_p.y();
        for(int line = new_p.x()-1;line BG 0; line--){
            column = column - 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column BG 0){
                if(_board[line][column] EQ piece_list[chess_pieces::BlackBishop] OR _board[line][column] EQ piece_list[chess_pieces::BlackQueen]){
                    //qDebug() << "status 08" << line << column;
                    status = false;
                }
                break;
            }
        }
        column = new_p.y();
        for(int line = new_p.x()+1;line SM 7; line++){
            column = column + 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column SM 7){
                if(_board[line][column] EQ piece_list[chess_pieces::BlackBishop] OR _board[line][column] EQ piece_list[chess_pieces::BlackQueen]){
                    //qDebug() << "status 09" << line << column;
                    status = false;
                }
                break;
            }
        }
        column = new_p.y();
        for(int line = new_p.x()+1;line SM 7; line++){
            column = column - 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column BG 0){
                if(_board[line][column] EQ piece_list[chess_pieces::BlackBishop] OR _board[line][column] EQ piece_list[chess_pieces::BlackQueen]){
                    //qDebug() << "status 10" << line << column;
                    status = false;
                }
                break;
            }
        }
        column = new_p.y();
        for(int line = new_p.x()-1;line BG 0; line--){
            column = column + 1;
            if(_board[line][column] NE piece_list[chess_pieces::Empty] AND column SM 7){
                if(_board[line][column] EQ piece_list[chess_pieces::BlackBishop] OR _board[line][column] EQ piece_list[chess_pieces::BlackQueen]){
                    //qDebug() << "status 011" << line << column;
                    status = false;
                }
                break;
            }
        }
        column = new_p.y();
        for(int line= new_p.x()-1;line BG 0;line--){
            if(_board[line][new_p.y()]   NE piece_list[chess_pieces::Empty]){
                if(_board[line][new_p.y()] EQ piece_list[chess_pieces::BlackRook] OR _board[line][new_p.y()] EQ piece_list[chess_pieces::BlackQueen]){
                    //qDebug() << "status 012" << line << new_p.y();
                    status = false;
                }
                break;
            }
        }
        for(int line=new_p.x()+1;line SM 7;line++){
            if(_board[line][new_p.y()]   NE piece_list[chess_pieces::Empty]){
                if(_board[line][new_p.y()] EQ piece_list[chess_pieces::BlackRook] OR _board[line][new_p.y()] EQ piece_list[chess_pieces::BlackQueen]){
                    //qDebug() << "status 013"<< line << new_p.y();
                    status = false;
                }
                break;
            }
        }
        for(int column=new_p.y()-1;column BG 0;column--){
            if(_board[new_p.x()][column] NE piece_list[chess_pieces::Empty]){
                if(_board[new_p.x()][column] EQ piece_list[chess_pieces::BlackRook] OR _board[new_p.x()][column] EQ piece_list[chess_pieces::BlackQueen]){
                    //qDebug() << "status 014"<< new_p.x() << column;
                    status = false;
                }
                break;
            }
        }
        for(int column=new_p.y()+1;column SM 7;column++){
            if(_board[new_p.x()][column] NE piece_list[chess_pieces::Empty]){
                if(_board[new_p.x()][column] EQ piece_list[chess_pieces::BlackRook] OR _board[new_p.x()][column] EQ piece_list[chess_pieces::BlackQueen]){
                    //qDebug() << "status 015"<< new_p.x() << column;
                    status = false;
                }
                break;
            }
        }
    }
    //qDebug() << "status 16";
    return status;
}

bool board::check_endgame()
{
    if(!checkmate(mode::black,chessBoard)){
        qDebug() << "Check Mate! White Win!" << turn_counter;
        show_board();
        return true;
    }
    if(!checkmate(mode::white,chessBoard)){
        qDebug() << "Check Mate! Black Win!" << turn_counter;
        show_board();
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

bool board::check(QPoint ex_p, QPoint new_p,int mode)
{
    char c = chessBoard[ex_p.x()][ex_p.y()];
    getMoves(c,ex_p.x(),ex_p.y(),mode);
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

QPoint board::getKingPoint(char _board[8][8],char piece)
{
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            if(_board[h][w] EQ piece){
                return QPoint(h,w);
            }
        }
    }
}

void board::castling_conditions_check(QPoint ex_p)
{
    if(turn_counter % 2 EQ 0 AND ex_p.x() EQ 7){
        if(ex_p.y() EQ 0){
            qDebug() << "pack.flag.castling_r1_flag";
            pack.flag.castling.r1_flag = false;
        }
        else if (ex_p.y() EQ 7) {
            qDebug() << "pack.flag.castling_r2_flag";
            pack.flag.castling.r2_flag = false;
        }
        else if (ex_p.y() EQ 4) {
            qDebug() << "pack.flag.castling_k_flag";
            pack.flag.castling.k_flag = false;
        }
    }
    if(turn_counter % 2 EQ 1 AND ex_p.x() EQ 0){
        if(ex_p.y() EQ 0){
            qDebug() << "pack.flag.castling_R1_flag";
            pack.flag.castling.R1_flag = false;
        }
        else if (ex_p.y() EQ 7) {
            qDebug() << "pack.flag.castling_R2_flag";
            pack.flag.castling.R2_flag = false;
        }
        else if (ex_p.y() EQ 4) {
            qDebug() << "pack.flag.castling_K_flag";
            pack.flag.castling.K_flag = false;
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
