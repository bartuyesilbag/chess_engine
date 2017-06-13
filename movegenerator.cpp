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

#define MAX 5

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
    int v1 = rand() % 2;
    QPoint ex_p,new_p;
    if(turn EQ 0){
        ex_p  = QPoint(1,4);
        new_p = QPoint(3,4);
    }
    else if(turn EQ 1){
        ex_p  = QPoint(0,1);
        new_p = QPoint(2,2);
    }
    else if (turn EQ 2) {
        if(b->chessBoard[4][2] EQ 'b'){
            ex_p  = QPoint(1,3);
            new_p = QPoint(3,3);
        }
        else{
            if(v1 EQ 0){
                ex_p  = QPoint(0,5);
                new_p = QPoint(1,4);}
            else{
                ex_p  = QPoint(0,6);
                new_p = QPoint(2,5);}
        }
    }
    else if (turn BG 3) {

        char _board[8][8];
        QVector<float> max_five;
        QVector<int> index_of_max_five;

        guess_black(_board,b->chessBoard);

        max_five = get_max(black_pack.score,MAX);
        foreach (float max, max_five) {
            index_of_max_five.push_back(black_pack.score.indexOf(max));
        }
        foreach (int max_five_move, index_of_max_five) {
            char white_board[8][8];
            guess_white(white_board,_board,max_five_move);
        }


        QVector<float> min_white = get_min(white_pack.score,1);
        int index_of_white = white_pack.score.indexOf(min_white.at(0));
        int index_of_total = -1;
        foreach (int j, list_sizes) {
            if(index_of_white BG 0){
                index_of_white = index_of_white -j;
                index_of_total++;
            }
            else{
                break;
            }
        }




        ex_p  = QPoint(list->at(index_of_max_five.at(index_of_total)).X,list->at(index_of_max_five.at(index_of_total)).Y);
        new_p = QPoint(list->at(index_of_max_five.at(index_of_total)).destX,list->at(index_of_max_five.at(index_of_total)).destY);
        qDebug() << "dasdişsldfisdşlfs";
    }

    qDebug() << black_move_to_screen(ex_p,new_p);

    list->clearList();
    black_pack.score.clear();
    white_pack.score.clear();
    list_sizes.clear();
    turn++;
    b->slot_msg_to_board(ex_p,new_p,"aaaa");

}

void moveGenerator::calculateBoard(char board[8][8], int _mode,int count)
{
    char _board[8][8];
    float total_score,pieces_score,mobility_score,king_score,center_control_score,
            extended_center_score,doubled_pieces_score,piece_square_score,w1,w2,w3,w4,w5,w6,w7;
    safelist<chess_pack> temp_list;
    w1 = 1; w2 = 126.5; w3 = 50; w4 = 45; w5 = 20; w6 = 27; w7 = 20;

    fill_board(_board,board);
    b->getBoard(_mode,_board,&temp_list);
    int black_list_size = temp_list.size();
    temp_list.clearList();
    b->getBoard((_mode+1)%2,_board,&temp_list);
    int white_list_size = temp_list.size();


    //calculate points

    pieces_score          = pieces_points(board,_mode);                    //w1

    mobility_score        = (float)(black_list_size - white_list_size);    //w2

    king_score            = kings_landing(board);                          //w3

    center_control_score  = center_control(board);                         //w4

    extended_center_score = extended_center(board);                        //w5

    doubled_pieces_score  = doubled_pieces(board);                         //w6

    piece_square_score    = piece_square_table(board,_mode);               //w7


    total_score = w1*pieces_score          +
            w2*mobility_score        +
            w3*king_score            +
            w4*center_control_score  +
            w5*extended_center_score +
            w6*doubled_pieces_score  +
            w7*piece_square_score;
    if(_mode EQ mode::black){
        black_pack.score.push_back(total_score);
    }
    else{
        white_pack.score.push_back(total_score);
    }
}

QVector<float> moveGenerator::get_max(QVector<float> _scores,int max_number)
{
    QVector<float> _scores_ = _scores;
    std::sort(_scores_.begin(),_scores_.end());
    QVector<float> max_five;

    for(int i=1; i<=max_number;i++){
        max_five.push_back(_scores_.at(_scores_.length()-i));
    }
    //    max_five.push_back(_scores_.at(_scores_.length()-1));
    //    max_five.push_back(_scores_.at(_scores_.length()-2));
    //    max_five.push_back(_scores_.at(_scores_.length()-3));
    //    max_five.push_back(_scores_.at(_scores_.length()-4));
    //    max_five.push_back(_scores_.at(_scores_.length()-5));
    //float max = *std::max_element(_scores.begin(), _scores.end());
    return max_five;
}

QVector<float> moveGenerator::get_min(QVector<float> _scores,int min_number)
{
    QVector<float> _scores_ = _scores;
    std::sort(_scores_.begin(),_scores_.end());
    QVector<float> min_numbers;

    for(int i=0; i<min_number;i++){
        min_numbers.push_back(_scores_.at(i));
    }
    //    max_five.push_back(_scores_.at(_scores_.length()-1));
    //    max_five.push_back(_scores_.at(_scores_.length()-2));
    //    max_five.push_back(_scores_.at(_scores_.length()-3));
    //    max_five.push_back(_scores_.at(_scores_.length()-4));
    //    max_five.push_back(_scores_.at(_scores_.length()-5));
    //float max = *std::max_element(_scores.begin(), _scores.end());
    return min_numbers;
}

QString moveGenerator::black_move_to_screen(QPoint ex_p, QPoint new_p)
{
    system("clear");

    qDebug() << "|---------------------------------------------|";
    qDebug() << "|Yapay Zeka Temelli Satranç Simulasyonu v0.0.1|";
    qDebug() << "|Bartu Yeşilbağ                               |";
    qDebug() << "|Tez Danışmanı: Nihan Kahraman                |";
    qDebug() << "|YTU EHM 2016-2017 Bitirme Tezi               |";
    qDebug() << "|---------------------------------------------|";
    qDebug() << "   ";
    qDebug() << "Game Start! Play a Move!";
    qDebug() << "for help, write help";

    QString move;
    move  = "Black Moved ";
    move += horizontal[ex_p.y()] ;
    move += verticle[ex_p.x()]   ;

    move += horizontal[new_p.y()];
    move += verticle[new_p.x()]  ;
    return move;
}

void moveGenerator::fill_board(char _board[8][8],char chessboard[8][8])
{
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            _board[h][w] = chessboard[h][w];
        }
    }
}

//evaluation function

float moveGenerator::pieces_points(char _board[8][8], int _mode)
{
    float point          = 0;
    float doubled_pawns  = 0;

    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            switch (_board[h][w]) {
            case 'P':
                point += 100;
                doubled_pawns  += _board[h+1][w] EQ 'P' ? 14.3 : 0;
                break;
            case 'R':
                point += 489.7;
                break;
            case 'N':
                point += 295.2;
                break;
            case 'B':
                point += 315.4;
                break;
            case 'Q':
                point += 921.3;
                break;
                //            case 'K':
                //                point += 1000000;
                //                break;
            case 'p':
                point -= 100;
                doubled_pawns  -= _board[h-1][w] EQ 'p' ? 1 :0;
                break;
            case 'r':
                point -= 489.7;
                break;
            case 'n':
                point -= 295.2;
                break;
            case 'b':
                point -= 315.4;
                break;
            case 'q':
                point -= 921.3;
                break;
                //            case 'k':
                //                point -= 200;
                //                break;
            default:
                break;
            }

        }
    }

    return point - 0.1*doubled_pawns;
}

float moveGenerator::center_control(char _board[8][8])
{
    float _score = 0;
    char piece_list[14] = {'.','P','R','N','B','Q','K','p','r','n','b','q','k'};
    for(int i = 3; i<=4;i++){
        for(int j = 3; j<=4;j++){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, _board[i][j]));
            //qDebug() << a;
            if(a > 6){
                _score -= 1;
            }
            else if(a > 0 AND a <= 6){
                _score += 1;
            }
        }
    }
    return _score;
}

float moveGenerator::extended_center(char _board[8][8])
{
    float _score = 0;
    char piece_list[14] = {'.','P','R','N','B','Q','K','p','r','n','b','q','k'};
    for(int i = 2; i<=5;i++){
        for(int j = 2; j<=5;j++){
            int a = std::distance(piece_list, std::find(piece_list, piece_list + 13, _board[i][j]));
            //qDebug() << a;
            if(a > 6){
                _score  -= 1;
            }
            else if(a > 0 AND a <= 6){
                _score  += 1;
            }
        }
    }
    return _score;

}

float moveGenerator::doubled_pieces(char _board[8][8])
{
    float _score = 0;

    for(int i = 0; i<=7;i++){
        for(int j = 0; j<=7;j++){
            if(_board[i][j] EQ ('B' OR 'N')){
                _score += 1;
            }
            else if(_board[i][j] EQ ('B' OR 'N')){
                _score -= 1;
            }
        }
    }
    return _score;
}

float moveGenerator::piece_square_table(char _board[8][8],int _mode)
{
    float _score = 0;

    for(int i = 0; i<=7;i++){
        for(int j = 0; j<=7;j++){
            if(_board[i][j] NE '.'){
                _score += square_tables(_board[i][j],i,j,_mode);
            }
        }
    }

    return _score;
}

float moveGenerator::square_tables(char piece,int h,int w,int _mode)
{

    float _score;

    int white_pawn_board[8][8]      =  {{0,  0,  0,  0,  0,  0,  0,  0 },
                                        {50, 50, 50, 50, 50, 50, 50, 50},
                                        {10, 10, 20, 30, 30, 20, 10, 10},
                                        { 5,  5, 10, 25, 25, 10,  5,  5},
                                        { 0,  0,  0, 20, 20,  0,  0,  0},
                                        { 5, -5,-10,  0,  0,-10, -5,  5},
                                        { 5, 10, 10,-20,-20, 10, 10,  5},
                                        { 0,  0,  0,  0,  0,  0,  0,  0}};

    int black_pawn_board[8][8]      =  {{0,  0,  0,  0,  0,  0,  0,  0 },
                                        { 5, 10, 10,-20,-20, 10, 10,  5},
                                        { 5, -5,-10,  0,  0,-10, -5,  5},
                                        { 0,  0,  0, 20, 20,  0,  0,  0},
                                        { 5,  5, 10, 25, 25, 10,  5,  5},
                                        {10, 10, 20, 30, 30, 20, 10, 10},
                                        {50, 50, 50, 50, 50, 50, 50, 50},
                                        { 0,  0,  0,  0,  0,  0,  0,  0}};

    int white_knight_board[8][8]    = {{-50,-40,-30,-30,-30,-30,-40,-50},
                                       {-40,-20,  0,  0,  0,  0,-20,-40},
                                       {-30,  0, 10, 15, 15, 10,  0,-30},
                                       {-30,  5, 15, 20, 20, 15,  5,-30},
                                       {-30,  0, 15, 20, 20, 15,  0,-30},
                                       {-30,  5, 10, 15, 15, 10,  5,-30},
                                       {-40,-20,  0,  5,  5,  0,-20,-40},
                                       {-50,-40,-30,-30,-30,-30,-40,-50}};

    int black_knight_board[8][8]    = {{-50,-40,-30,-30,-30,-30,-40,-50},
                                       {-40,-20,  0,  5,  5,  0,-20,-40},
                                       {-30,  5, 10, 15, 15, 10,  5,-30},
                                       {-30,  0, 15, 20, 20, 15,  0,-30},
                                       {-30,  5, 15, 20, 20, 15,  5,-30},
                                       {-30,  0, 10, 15, 15, 10,  0,-30},
                                       {-40,-20,  0,  0,  0,  0,-20,-40},
                                       {-50,-40,-30,-30,-30,-30,-40,-50}};


    int white_bishop_board[8][8]    = {{-20,-10,-10,-10,-10,-10,-10,-20},
                                       {-10,  0,  0,  0,  0,  0,  0,-10},
                                       {-10,  0,  5, 10, 10,  5,  0,-10},
                                       {-10,  5,  5, 10, 10,  5,  5,-10},
                                       {-10,  0, 10, 10, 10, 10,  0,-10},
                                       {-10, 10, 10, 10, 10, 10, 10,-10},
                                       {-10,  5,  0,  0,  0,  0,  5,-10},
                                       {-20,-10,-10,-10,-10,-10,-10,-20}};

    int black_bishop_board[8][8]    = {{-20,-10,-10,-10,-10,-10,-10,-20},
                                       {-10,  5,  0,  0,  0,  0,  5,-10},
                                       {-10, 10, 10, 10, 10, 10, 10,-10},
                                       {-10,  0, 10, 10, 10, 10,  0,-10},
                                       {-10,  5,  5, 10, 10,  5,  5,-10},
                                       {-10,  0,  5, 10, 10,  5,  0,-10},
                                       {-10,  0,  0,  0,  0,  0,  0,-10},
                                       {-20,-10,-10,-10,-10,-10,-10,-20}};

    int white_rook_board[8][8]      = {{  0,  0,  0,  0,  0,  0,  0,  0},
                                       {  5, 10, 10, 10, 10, 10, 10,  5},
                                       { -5,  0,  0,  0,  0,  0,  0, -5},
                                       { -5,  0,  0,  0,  0,  0,  0, -5},
                                       { -5,  0,  0,  0,  0,  0,  0, -5},
                                       { -5,  0,  0,  0,  0,  0,  0, -5},
                                       { -5,  0,  0,  0,  0,  0,  0, -5},
                                       {  0,  0,  0,  5,  5,  0,  0,  0}};

    int black_rook_board[8][8]      = {{  0,  0,  0,  5,  5,  0,  0,  0},
                                       { -5,  0,  0,  0,  0,  0,  0, -5},
                                       { -5,  0,  0,  0,  0,  0,  0, -5},
                                       { -5,  0,  0,  0,  0,  0,  0, -5},
                                       { -5,  0,  0,  0,  0,  0,  0, -5},
                                       { -5,  0,  0,  0,  0,  0,  0, -5},
                                       {  5, 10, 10, 10, 10, 10, 10,  5},
                                       {  0,  0,  0,  0,  0,  0,  0,  0}};

    int white_queen_board[8][8]     = {{-20,-10,-10, -5, -5,-10,-10,-20},
                                       {-10,  0,  0,  0,  0,  0,  0,-10},
                                       {-10,  0,  5,  5,  5,  5,  0,-10},
                                       { -5,  0,  5,  5,  5,  5,  0, -5},
                                       {  0,  0,  5,  5,  5,  5,  0, -5},
                                       {-10,  5,  5,  5,  5,  5,  0,-10},
                                       {-10,  0,  5,  0,  0,  0,  0,-10},
                                       {-20,-10,-10, -5, -5,-10,-10,-20}};

    int black_queen_board[8][8]     = {{-20,-10,-10, -5, -5,-10,-10,-20},
                                       {-10,  0,  0,  0,  0,  5,  0,-10},
                                       {-10,  5,  5,  5,  5,  5,  0,-10},
                                       {  0,  0,  5,  5,  5,  5,  0, -5},
                                       { -5,  0,  5,  5,  5,  5,  0, -5},
                                       {-10,  0,  5,  5,  5,  5,  0,-10},
                                       {-10,  0,  0,  0,  0,  0,  0,-10},
                                       {-20,-10,-10, -5, -5,-10,-10,-20}};

    int white_kingM_board[8][8]    = {{-30,-40,-40,-50,-50,-40,-40,-30},
                                      {-30,-40,-40,-50,-50,-40,-40,-30},
                                      {-30,-40,-40,-50,-50,-40,-40,-30},
                                      {-30,-40,-40,-50,-50,-40,-40,-30},
                                      {-20,-30,-30,-40,-40,-30,-30,-20},
                                      {-10,-20,-20,-20,-20,-20,-20,-10},
                                      { 20, 20,  0,  0,  0,  0, 20, 20},
                                      { 20, 30, 10,  0,  0, 10, 30, 20}};

    int black_kingM_board[8][8]    = {{ 20, 30, 10,  0,  0, 10, 30, 20},
                                      { 20, 20,  0,  0,  0,  0, 20, 20},
                                      {-10,-20,-20,-20,-20,-20,-20,-10},
                                      {-20,-30,-30,-40,-40,-30,-30,-20},
                                      {-30,-40,-40,-50,-50,-40,-40,-30},
                                      {-30,-40,-40,-50,-50,-40,-40,-30},
                                      {-30,-40,-40,-50,-50,-40,-40,-30},
                                      {-30,-40,-40,-50,-50,-40,-40,-30}};

    int white_kingE_board[8][8]    = {{-50,-40,-30,-20,-20,-30,-40,-50},
                                      {-30,-20,-10,  0,  0,-10,-20,-30},
                                      {-30,-10, 20, 30, 30, 20,-10,-30},
                                      {-30,-10, 30, 40, 40, 30,-10,-30},
                                      {-30,-10, 30, 40, 40, 30,-10,-30},
                                      {-30,-10, 20, 30, 30, 20,-10,-30},
                                      {-30,-30,  0,  0,  0,  0,-30,-30},
                                      {-50,-30,-30,-30,-30,-30,-30,-50}};

    int black_kingE_board[8][8]    = {{-50,-30,-30,-30,-30,-30,-30,-50},
                                      {-30,-30,  0,  0,  0,  0,-30,-30},
                                      {-30,-10, 20, 30, 30, 20,-10,-30},
                                      {-30,-10, 30, 40, 40, 30,-10,-30},
                                      {-30,-10, 30, 40, 40, 30,-10,-30},
                                      {-30,-10, 20, 30, 30, 20,-10,-30},
                                      {-30,-20,-10,  0,  0,-10,-20,-30},
                                      {-50,-40,-30,-20,-20,-30,-40,-50}};
    if(_mode = mode::black){
        switch (piece) {
        case 'P':
            _score += black_pawn_board[h][w];
            break;
        case 'R':
            _score += black_rook_board[h][w];
            break;
        case 'N':
            _score += black_knight_board[h][w];
            break;
        case 'B':
            _score += black_bishop_board[h][w];
            break;
        case 'Q':
            _score += black_queen_board[h][w];
            break;
        case 'K':
            _score += black_kingM_board[h][w];
            break;
        default:
            break;
        }
    }
    else{
        switch (piece) {
        case 'p':
            _score -= white_pawn_board[h][w];
            break;
        case 'r':
            _score -= white_rook_board[h][w];
            break;
        case 'n':
            _score -= white_knight_board[h][w];
            break;
        case 'b':
            _score -= white_bishop_board[h][w];
            break;
        case 'q':
            _score -= white_queen_board[h][w];
            break;
        case 'k':
            _score -= white_kingM_board[h][w];
            break;
        default:
            break;
        }
    }
    _score = _score/10;
    return _score;
}

float moveGenerator::kings_landing(char _board[8][8])
{
    float _score = 0;
    QPoint K_point = b->getKingPoint(_board,'K');

    int h = K_point.x();
    int w = K_point.y();

    if(h-1 BG 0 AND w-1 BG 0)
        _score += _board[h-1][w-1] NE '.' ? 1 : 0;
    if(h-1 BG 0)
        _score += _board[h-1][w]   NE '.' ? 1 : 0;
    if(h-1 BG 0 AND w+1 SM 7)
        _score += _board[h-1][w+1] NE '.' ? 1 : 0;
    if(w-1 BG 0)
        _score += _board[h][w-1]   NE '.' ? 1 : 0;
    if(w+1 SM 7)
        _score += _board[h][w+1]   NE '.' ? 1 : 0;
    if(h+1 SM 7 AND w-1 BG 0)
        _score += _board[h+1][w-1] NE '.' ? 1 : 0;
    if(h+1 SM 7)
        _score += _board[h+1][w]   NE '.' ? 1 : 0;
    if(h+1 SM 7 AND w+1 SM 7)
        _score += _board[h+1][w+1] NE '.' ? 1 : 0;

    QPoint k_point = b->getKingPoint(_board,'k');

    h = k_point.x();
    w = k_point.y();

    if(h-1 BG 0 AND w-1 BG 0)
        _score += _board[h-1][w-1] NE '.' ? -1 : 0;
    if(h-1 BG 0)
        _score += _board[h-1][w]   NE '.' ? -1 : 0;
    if(h-1 BG 0 AND w+1 SM 7)
        _score += _board[h-1][w+1] NE '.' ? -1 : 0;
    if(w-1 BG 0)
        _score += _board[h][w-1]   NE '.' ? -1 : 0;
    if(w+1 SM 7)
        _score += _board[h][w+1]   NE '.' ? -1 : 0;
    if(h+1 SM 7 AND w-1 BG 0)
        _score += _board[h+1][w-1] NE '.' ? -1 : 0;
    if(h+1 SM 7)
        _score += _board[h+1][w]   NE '.' ? -1 : 0;
    if(h+1 SM 7 AND w+1 SM 7)
        _score += _board[h+1][w+1] NE '.' ? -1 : 0;
    return _score;
}

void moveGenerator::show_board(char _board[8][8])
{
    qDebug() << "";
    for(int h = 0 ; h<8; h++){
        qDebug() << 8 - h << " " << _board[h][0] << _board[h][1] << _board[h][2] << _board[h][3]
                 << _board[h][4] << _board[h][5] << _board[h][6] << _board[h][7];
    }
    qDebug() << "\n    A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";
    qDebug() << "\n      0" << "1" << "2" << "3" << "4" << "5" << "6" << "7";
}

void moveGenerator::guess_black(char temp_board[8][8], char chess_board[8][8]){
    fill_board(temp_board,chess_board);
    b->getBoard(mode::black,temp_board,list);
    int listSize = list->size();
    for(int i = 0; i < listSize;i++){
        if(b->boardSimulate(QPoint(list->at(i).X,list->at(i).Y),QPoint(list->at(i).destX,list->at(i).destY),temp_board,chess_board,list->at(i).flag.castling.cast_stat)){
            calculateBoard(temp_board,mode::black,i);
        }
        else{
            black_pack.score.push_back(-10000);
        }
    }
}

void moveGenerator::guess_white(char white_board[8][8], char black_board[8][8],int move_index){

    fill_board(white_board,black_board);

    b->boardSimulate(QPoint(list->at(move_index).X,list->at(move_index).Y),QPoint(list->at(move_index).destX,list->at(move_index).destY),
                     white_board,black_board,list->at(move_index).flag.castling.cast_stat);
    b->getBoard(mode::white,white_board,white_list);
    int whiteListSize = white_list->size();
    qDebug() << whiteListSize;
    list_sizes.push_back(whiteListSize);
    for(int i = 0; i < whiteListSize;i++){
        char white_temp_board[8][8];
        if(b->boardSimulate(QPoint(white_list->at(i).X,white_list->at(i).Y),QPoint(white_list->at(i).destX,white_list->at(i).destY),
                            white_temp_board,white_board,white_list->at(i).flag.castling.cast_stat)){
            calculateBoard(white_temp_board,mode::white,i);
        }
        else{
            white_pack.score.push_back(10000000);
        }
    }
    white_list->clearList();
}
