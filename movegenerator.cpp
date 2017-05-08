#include "movegenerator.h"
#include "board.h"
#include "safelist.h"
#include "pack.h"
#include "pieces.h"
#include "iostream"
#include <QVector>

#include <cstdlib>
#include <ctime>

moveGenerator::moveGenerator(safelist<chess_pack> *_list, safelist<chess_pack> *_white_list, board *_b): QObject(NULL)
{
    list = _list;
    white_list = _white_list;
    b    = _b;
    std::srand (time(NULL));
}

moveGenerator::~moveGenerator()
{
    qDebug() << "moveGenerator deleted!";
}

void moveGenerator::findLegalBlackMoves()
{
    char _board[8][8];
    fill_board(_board);
    b->getBoard(1,_board);
    int listSize = list->size();
    for(int i = 0; i < listSize;i++){
        //list->pop(pack);
        if(b->boardSimulate(QPoint(list->at(i).X,list->at(i).Y),QPoint(list->at(i).destX,list->at(i).destY),_board,list->at(i).flag.castling.cast_stat)){
            calculateBoard(_board,mode::black,i);
            find_legal_white_moves(i);
            general_pack.score.push_back(get_max(white_pack.score));
            white_pack.score.clear();
        }
        else{
            general_pack.score.push_back(-10000);
        }
    }
    int index_of = general_pack.score.indexOf(get_max(general_pack.score));
    qDebug() << black_move_to_screen(QPoint(list->at(index_of).X,list->at(index_of).Y),QPoint(list->at(index_of).destX,list->at(index_of).destY));
    b->slot_msg_to_board(QPoint(list->at(index_of).X,list->at(index_of).Y),QPoint(list->at(index_of).destX,list->at(index_of).destY),"aaaa");
    list->clearList();
    /*
    int index_of = general_pack.score.indexOf(get_max(general_pack.score));
    qDebug() << general_pack.score;
    qDebug() << "size of vector: " << general_pack.score.size() << "size of list before clear: " << list->size();
    qDebug() << "index_of: " << index_of << "element: " << general_pack.score.at(index_of);
    qDebug() << "X: " << list->at(index_of).X << "Y: " << list->at(index_of).Y << "destX: " << list->at(index_of).destX << "destY: " << list->at(index_of).destY ;
    list->clearList();
    qDebug() << "list of list after clearList: " << list->size();*/


}

void moveGenerator::find_legal_white_moves(int count)
{
    char white_board[8][8];
    fill_board(white_board);
    b->getBoard(0,white_board);
    int listSize = white_list->size();
    for(int i = 0; i < listSize;i++){
        white_list->pop(pack);
        if(b->boardSimulate(QPoint(pack.X,pack.Y),QPoint(pack.destX,pack.destY),white_board,pack.flag.castling.cast_stat)){
            calculateBoard(white_board,mode::white,count);
        }
    }
}

void moveGenerator::calculateBoard(char board[8][8], int _mode,int count)
{
    if(_mode EQ mode::black){
//        //black
//        for(int h = 0 ; h<8; h++){
//            qDebug() << h << 8 - h << " " << board[h][0] << board[h][1] << board[h][2] << board[h][3]
//                     << board[h][4] << board[h][5] << board[h][6] << board[h][7];
//        }
//        qDebug() << "\n      A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";
//        qDebug() << "\n      0" << "1" << "2" << "3" << "4" << "5" << "6" << "7";
//        qDebug() << "--------------------------------------------------------------------";
        int v1 = std::rand() % 100 + 1;
        black_pack.score.push_back(v1);
    }
    else{
        //white
        int v2 = std::rand() % 100 + 1;
        white_pack.score.push_back(black_pack.score.at(count)-v2);
    }
}

int moveGenerator::get_max(QVector<int> _scores)
{
    int max = *std::max_element(_scores.begin(), _scores.end());
    return max;
}



QString moveGenerator::black_move_to_screen(QPoint ex_p, QPoint new_p)
{
    QString move;
    move  = "Black Moved ";
    move += horizontal[ex_p.y()] ;
    move += verticle[ex_p.x()]   ;

    move += horizontal[new_p.y()];
    move += verticle[new_p.x()]  ;
    return move;
}

void moveGenerator::fill_board(char _board[8][8])
{
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            _board[h][w] = b->chessBoard[h][w];
        }
    }
}
