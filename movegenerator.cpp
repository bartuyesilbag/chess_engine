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
        fill_board(_board,b->chessBoard);
        b->getBoard(mode::black,_board,list);
        int listSize = list->size();
        for(int i = 0; i < listSize;i++){
            if(b->boardSimulate(QPoint(list->at(i).X,list->at(i).Y),QPoint(list->at(i).destX,list->at(i).destY),_board,b->chessBoard,list->at(i).flag.castling.cast_stat)){
                calculateBoard(_board,mode::black,i);
            }
            else{
                black_pack.score.push_back(-10000);
            }
        }
        QVector<float> max_five = get_max(black_pack.score);
        QVector<int> index_of_max_five;
        foreach (float max, max_five) {
            index_of_max_five.push_back(black_pack.score.indexOf(max));
        }
        int count = std::count (black_pack.score.begin(), black_pack.score.end(),max_five.at(0)); //find max prob. move's count (in case of )

        foreach (int moves, index_of_max_five){
            char white_board[8][8];
            char temp_wboard[8][8];
            fill_board(white_board,b->chessBoard);
            if(b->boardSimulate(QPoint(list->at(moves).X,list->at(moves).Y),QPoint(list->at(moves).destX,list->at(moves).destY)
                                ,white_board,b->chessBoard,list->at(moves).flag.castling.cast_stat)){
                b->getBoard(mode::white,white_board,white_list);
                int listSize = white_list->size();
                for(int i = 0; i < listSize;i++){
                    if(b->boardSimulate(QPoint(white_list->at(i).X,white_list->at(i).Y),QPoint(white_list->at(i).destX,white_list->at(i).destY),
                                        temp_wboard,white_board,white_list->at(i).flag.castling.cast_stat)){
                        calculateBoard(temp_wboard,mode::white,i);
                        qDebug() << "i : " << i;
                        show_board(temp_wboard);
                    }
                    else{
                        white_pack.score.push_back(-10000);
                    }
                }
                white_list->clearList();
            }
            qDebug() << white_pack.score;
             white_pack.score.clear();
        }

        int index_of;
        if(count EQ 1){
            index_of = black_pack.score.indexOf(max_five.at(0));
        }
        else{
            int v1 = std::rand() % count;
            index_of = black_pack.score.indexOf(max_five.at(0),v1);
        }

        ex_p  = QPoint(list->at(index_of).X,list->at(index_of).Y);
        new_p = QPoint(list->at(index_of).destX,list->at(index_of).destY);
    }

    qDebug() << black_move_to_screen(ex_p,new_p);

    list->clearList();
    black_pack.score.clear();

    turn++;

    b->slot_msg_to_board(ex_p,new_p,"aaaa");

}


void moveGenerator::calculateBoard(char board[8][8], int _mode,int count)
{
    char _board[8][8];
    float total_score,pieces_score,mobility_score,king_score,center_control_score,
            extended_center_score,doubled_pieces_score,piece_square_score,w1,w2,w3,w4,w5,w6,w7;
    safelist<chess_pack> temp_list;
    w1 = 3.114; w2 = 1.245; w3 = 0.714; w4 = 0.653; w5 = 0.312; w6 = 0.351; w7 = 0.332;

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

    piece_square_score    = piece_square_table(board);                     //w7


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

QVector<float> moveGenerator::get_max(QVector<float> _scores)
{
    QVector<float> _scores_ = _scores;
    std::sort(_scores_.begin(),_scores_.end());
    QVector<float> max_five;
    max_five.push_back(_scores_.at(_scores_.length()-1));
    max_five.push_back(_scores_.at(_scores_.length()-2));
    max_five.push_back(_scores_.at(_scores_.length()-3));
    max_five.push_back(_scores_.at(_scores_.length()-4));
    max_five.push_back(_scores_.at(_scores_.length()-5));
    //float max = *std::max_element(_scores.begin(), _scores.end());
    return max_five;
}

QString moveGenerator::black_move_to_screen(QPoint ex_p, QPoint new_p)
{
    //system("clear");

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
    float isolated_pawns = 0;
    float double_knight = 0;
    float double_bishop = 0;

    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            switch (_board[h][w]) {
            case 'P':
                point += 1;
                doubled_pawns  += _board[h+1][w] EQ 'P' ? 1 : 0;
                isolated_pawns += _board[h][w+1] EQ 'P' ? 0.5 : 0;
                isolated_pawns += _board[h][w-1] EQ 'P' ? 0.5 : 0;
                break;
            case 'R':
                point += 5;
                break;
            case 'N':
                point += 3;
                double_knight += 1;
                break;
            case 'B':
                point += 3;
                double_bishop += 1;
                break;
            case 'Q':
                point += 9;
                break;
            case 'K':
                point += 200;
                break;
            case 'p':
                point -= 1;
                doubled_pawns  -= _board[h-1][w] EQ 'p' ? 1 :0;
                isolated_pawns -= _board[h][w+1] EQ 'p' ? 0.5 : 0;
                isolated_pawns -= _board[h][w-1] EQ 'p' ? 0.5 : 0;
                break;
            case 'r':
                point -= 5;
                break;
            case 'n':
                point -= 3;
                break;
            case 'b':
                point -= 3;
                break;
            case 'q':
                point -= 9;
                break;
            case 'k':
                point -= 200;
                break;
            default:
                break;
            }

        }
    }
    double_knight  = double_knight BG 1 ? 1 : 0;
    double_bishop  = double_bishop BG 1 ? 1 : 0;
    return point + 0.1*double_bishop + 0.1*double_knight - 0.1*doubled_pawns - 0.1*isolated_pawns;
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

float moveGenerator::piece_square_table(char _board[8][8])
{
    float _score = 0;

    for(int i = 0; i<=7;i++){
        for(int j = 0; j<=7;j++){
            if(_board[i][j] NE '.'){
                _score += square_tables(_board[i][j],i,j);
            }
        }
    }

    return _score;
}

float moveGenerator::square_tables(char piece,int h,int w)
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

    switch (piece) {
    case 'P':
        _score = black_pawn_board[h][w];
        break;
    case 'R':
        _score = black_rook_board[h][w];
        break;
    case 'N':
        _score = black_knight_board[h][w];
        break;
    case 'B':
        _score = black_bishop_board[h][w];
        break;
    case 'Q':
        _score = black_queen_board[h][w];
        break;
    case 'K':
        _score = black_kingM_board[h][w];
        break;
    default:
        break;
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
