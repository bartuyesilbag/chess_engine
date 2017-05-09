#include "movegenerator.h"
#include "board.h"
#include "safelist.h"
#include "pack.h"
#include "pieces.h"
#include "iostream"
#include <QVector>

#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
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
    int max   = get_max(general_pack.score);
    int count = std::count (general_pack.score.begin(), general_pack.score.end(),max);
    int index_of;
    if(count EQ 1){
        index_of = general_pack.score.indexOf(max);
    }
    else{
        int v1 = std::rand() % count;
        index_of = general_pack.score.indexOf(max,v1);
    }
    QPoint ex_p,new_p;
    ex_p  = QPoint(list->at(index_of).X,list->at(index_of).Y);
    new_p = QPoint(list->at(index_of).destX,list->at(index_of).destY);
    qDebug() << black_move_to_screen(ex_p,new_p);
    list->clearList();
    general_pack.score.clear();
    b->slot_msg_to_board(ex_p,new_p,"aaaa");

    //    //int index_of = general_pack.score.indexOf(get_max(general_pack.score));
    //    qDebug() << general_pack.score;
    //    qDebug() << "size of vector: " << general_pack.score.size() << "size of list before clear: " << list->size();
    //    qDebug() << "index_of: " << index_of << "element: " << general_pack.score.at(index_of);
    //    qDebug() << "X: " << list->at(index_of).X << "Y: " << list->at(index_of).Y << "destX: " << list->at(index_of).destX << "destY: " << list->at(index_of).destY ;
    //    list->clearList();
    //    qDebug() << "list of list after clearList: " << list->size();

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
    float pieces_point = calculate_pieces_points(board,_mode);
    if(_mode EQ mode::black){
        //int v1 = std::rand() % 100 + 1;
        black_pack.score.push_back(pieces_point);
    }
    else{
        //int v2 = std::rand() % 100 + 1;
        //white_pack.score.push_back(pieces_point);
        white_pack.score.push_back(black_pack.score.at(count)-pieces_point);
    }
}

float moveGenerator::get_max(QVector<float> _scores)
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

float moveGenerator::calculate_pieces_points(char _board[8][8], int _mode)
{
    float point = 0;
    float king_slayer = 0;
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            if(_mode EQ mode::black){
                switch (_board[h][w]) {
                case 'P':
                    point += 1;
                    break;
                case 'R':
                    point += 5;
                    break;
                case 'N':
                    point += 3;
                    break;
                case 'B':
                    point += 3;
                    break;
                case 'Q':
                    point += 9;
                    break;
                case 'K':
                    king_slayer += kings_landing(_board,h,w);
                    break;
                default:
                    break;
                }
            }
            else if(_mode EQ mode::white){
                switch (_board[h][w]) {
                case 'p':
                    point += 1;
                    break;
                case 'r':
                    point += 5;
                    break;
                case 'n':
                    point += 3;
                    break;
                case 'b':
                    point += 3;
                    break;
                case 'q':
                    point += 9;
                    break;
                case 'k':
                    king_slayer += kings_landing(_board,h,w);
                    break;
                default:
                    break;

                }
            }
        }
    }
    return point + 0.3*king_slayer;
}

float moveGenerator::kings_landing(char _board[8][8], int h, int w)
{
    float _point = 0;
    if(h-1 BG 0 AND w-1 BG 0)
        _point += _board[h-1][w-1] NE '0' ? 1 : 0;
    if(h-1 BG 0)
        _point += _board[h-1][w]   NE '0' ? 1 : 0;
    if(h-1 BG 0 AND w+1 SM 7)
        _point += _board[h-1][w+1] NE '0' ? 1 : 0;
    if(w-1 BG 0)
        _point += _board[h][w-1]   NE '0' ? 1 : 0;
    if(w+1 SM 7)
        _point += _board[h][w+1]   NE '0' ? 1 : 0;
    if(h+1 SM 7 AND w-1 BG 0)
        _point += _board[h+1][w-1] NE '0' ? 1 : 0;
    if(h+1 SM 7)
        _point += _board[h+1][w]   NE '0' ? 1 : 0;
    if(h+1 SM 7 AND w+1 SM 7)
        _point += _board[h+1][w+1] NE '0' ? 1 : 0;
    return _point;
}
