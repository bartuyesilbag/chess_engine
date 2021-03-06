#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H
#include "safelist.h"
#include "pack.h"
#include "board.h"
#include "defines.h"
#include <QObject>

class moveGenerator : public QObject
{
    Q_OBJECT
public:
    moveGenerator(safelist<chess_pack> *_list, safelist<chess_pack> *_white_list, board *_b);
    ~moveGenerator();
private:
    void   msgToBoard(QPoint ex_p, QPoint new_p);
    void   calculateBoard(char board[][8], int _mode, int count = 0);
    QVector<float> get_max(QVector<float> _scores, int max_number);
    QVector<float> get_min(QVector<float> _scores,int min_number);
    void   find_legal_white_moves(int count);
    void   fill_board(char _board[][8], char chessboard[][8]);
    float  pieces_points(char _board[][8],int _mode);
    float  kings_landing(char _board[][8]);
    float  center_control(char _board[][8]);
    float  extended_center(char _board[][8]);
    float  doubled_pieces(char _board[][8]);
    float  piece_square_table(char _board[][8], int _mode);
    float square_tables(char piece, int h, int w, int _mode);
    bool myfunction(int i,int j);
    void show_board(char _board[][8]);
    void guess_black(char temp_board[][8]  , char chess_board[][8]);
    void guess_white(char white_board[8][8], char black_board[8][8], int move_index);
    QString black_move_to_screen(QPoint ex_p, QPoint new_p);

    chess_pack            pack;
    score_pack            white_pack;
    score_pack            black_pack;
    score_pack            general_pack;
    safelist<chess_pack> *list;
    safelist<chess_pack> *white_list;
    QVector<int>          list_sizes;
    int turn = 0;
    board                *b;
    char verticle[8]   = {'8','7','6','5','4','3','2','1'};
    char horizontal[8] = {'a','b','c','d','e','f','g','h'};
signals:
    void signalToBoard(message msg);
public slots:
    void findLegalBlackMoves();
};

#endif // MOVEGENERATOR_H
