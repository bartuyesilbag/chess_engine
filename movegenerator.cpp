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
    fill_board(_board,b->chessBoard);
    b->getBoard(1,_board,list);
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
    float max   = get_max(general_pack.score);
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
    qDebug() << black_move_to_screen(ex_p,new_p) << max;
    list->clearList();
//    qDebug() << general_pack.score;
    general_pack.score.clear();
    b->slot_msg_to_board(ex_p,new_p,"aaaa");

}

void moveGenerator::find_legal_white_moves(int count)
{
    char white_board[8][8];
    fill_board(white_board,b->chessBoard);
    b->getBoard(0,white_board,white_list);
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

    char _board[8][8];
    float total_score,pieces_score,mobility_score,king_score,center_control_score,
            next_move_score,extended_center_score,doubled_pieces_score,piece_square_score,w1,w2,w3,w4,w5,w6,w7,w8;
    safelist<chess_pack> temp_list;
    w1 = 55.114; w2 = 0.245; w3 = 0.714; w4 = 0.653; w5 = 0.079; w6 = 0.312; w7 = 0.351; w8 = 0.332;


    fill_board(_board,board);
    b->getBoard(_mode,_board,&temp_list);

    //calculate points

    pieces_score          = pieces_points(board,_mode);                    //w1

    mobility_score        = (float)temp_list.size();                       //w2

    king_score            = kings_landing(board,_mode);                    //w3

    center_control_score  = center_control(board);                         //w4

    next_move_score       = next_move(_board,_mode,w1,w2,w3,0,0,w6,w7,w8); //w5

    extended_center_score = extended_center(board);                        //w6

    doubled_pieces_score  = doubled_pieces(board,_mode);                   //w7

    piece_square_score    = piece_square_table(board,_mode);               //w7


    total_score = w1*pieces_score          +
                  w2*mobility_score        +
                  w3*king_score            +
                  w4*center_control_score  +
                  w5*next_move_score       +
                  w6*extended_center_score +
                  w7*doubled_pieces_score  +
                  w8*piece_square_score;
    if(_mode EQ mode::black){
        black_pack.score.push_back(total_score);
    }
    else{
        white_pack.score.push_back(black_pack.score.at(count)-total_score);
    }
//    qDebug() << "_mode: " << _mode << "pieces_score: " << w1*pieces_score << "mobility_score: "
//             << w2*mobility_score << "king_score: " << w3*king_score << "center_control_score: "
//             << w4*center_control_score << "next_move_score: " << w5*next_move_score;
}

float moveGenerator::get_max(QVector<float> _scores)
{

    float max = *std::max_element(_scores.begin(), _scores.end());
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
    for(int h = 0; h<8; h++){
        for(int w = 0 ; w<8; w++){
            if(_mode EQ mode::black){
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
                    break;
                case 'B':
                    point += 3;
                    break;
                case 'Q':
                    point += 9;
                    break;
                case 'K':
                    point += 200;
                    break;
                default:
                    break;
                }
            }
            else if(_mode EQ mode::white){
                switch (_board[h][w]) {
                case 'p':
                    point += 1;
                    doubled_pawns  += _board[h-1][w] EQ 'p' ? 1 :0;
                    isolated_pawns += _board[h][w+1] EQ 'p' ? 0.5 : 0;
                    isolated_pawns += _board[h][w-1] EQ 'p' ? 0.5 : 0;
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
                    point += 200;
                    break;
                default:
                    break;

                }
            }
        }
    }
    return point - 0.1*doubled_pawns - 0.1*isolated_pawns;
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
                _score  += -1;
            }
            else if(a > 0 AND a <= 6){
                _score += 1;
            }
        }
    }
    return _score;
}

float moveGenerator::next_move(char _board[8][8], int _mode,float w1,float w2,float w3,float w4,float w5,float w6,float w7,float w8)
{
    float _score = 0;
    float pieces_score;
    float king_score;
    float center_control_score;
    float extended_center_score;
    float doubled_pieces_score;
    float piece_square_score;

    pieces_score          = pieces_points(_board,_mode);

    king_score            = kings_landing(_board,_mode);

    center_control_score  = center_control(_board);

    extended_center_score = extended_center(_board);

    doubled_pieces_score  = doubled_pieces(_board,_mode);

    piece_square_score    = piece_square_table(_board,_mode);


    _score += w1*pieces_score/100     +
             w2*king_score            +
             w3*center_control_score  +
             w6*extended_center_score +
             w7*doubled_pieces_score  +
             w8*piece_square_score/10 + w4 + w5;
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
                _score  += -1;
            }
            else if(a > 0 AND a <= 6){
                _score += 1;
            }
        }
    }
    return _score;

}

float moveGenerator::doubled_pieces(char _board[8][8], int _mode)
{
    float _score = 0;
    char bishop = _mode EQ mode::black ? 'B' : 'b';
    char knight = _mode EQ mode::black ? 'N' : 'n';

    for(int i = 0; i<=7;i++){
        for(int j = 0; j<=7;j++){
            if(_board[i][j] EQ (bishop OR knight)){
                _score += 1;
            }
        }
    }
    return _score;
}

float moveGenerator::piece_square_table(char _board[8][8], int _mode)
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
    if(_mode EQ mode::black){

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
    }
    else if(_mode EQ mode::white){

        switch (piece) {
        case 'p':
            _score = white_pawn_board[h][w];
            break;
        case 'r':
            _score = white_rook_board[h][w];
            break;
        case 'n':
            _score = white_knight_board[h][w];
            break;
        case 'b':
            _score = white_bishop_board[h][w];
            break;
        case 'q':
            _score = white_queen_board[h][w];
            break;
        case 'k':
            _score = white_kingM_board[h][w];
            break;
        default:
            break;
        }
    }
    _score = _score/10;
    return _score;


}

float moveGenerator::kings_landing(char _board[8][8],int _mode)
{
    float _score = 0;
    char piece = _mode EQ mode::black ? 'K' : 'k';
    QPoint kings_point = b->getKingPoint(_board,piece);

    int h = kings_point.x();
    int w = kings_point.y();

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
    return _score;
}
