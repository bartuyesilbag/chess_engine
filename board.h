#ifndef BOARD_H
#define BOARD_H
#include <QObject>
#include "pack.h"
#include "safelist.h"
#include "defines.h"

#include <QVector>

class board: public QObject
{
    Q_OBJECT
public:
    board(safelist<chess_pack> *_list, safelist<chess_pack> *_white_list, QObject *parent = 0);

    void whitePawn(int x, int y, safelist<chess_pack> *_list, char board[][8]);
    void blackPawn(int x, int y, safelist<chess_pack> *_list, char board[][8]);
    void rook     (int x, int y, safelist<chess_pack> *_list, int _mode, char board[][8]);
    void knight   (int y, int x, safelist<chess_pack> *_list, int _mode, char board[][8]);
    void bishop   (int x, int y, safelist<chess_pack> *_list, int _mode, char board[][8]);
    void queen    (int x, int y, safelist<chess_pack> *_list, int _mode, char board[][8]);
    void king     (int x, int y, safelist<chess_pack> *_list, int _mode, char board[][8]);
    void getBoard (int mode, char board[][8], safelist<chess_pack> *_list);
    bool boardSimulate(QPoint ex_p, QPoint new_p, char outStr[8][8], int casling_status = 0);
    void getMoves(char pieceName, int x, int y, int mode, char board[][8], safelist<chess_pack> *_list);

    bool checkmate(int mode, char board[][8], QPoint ex_p = QPoint(10,10) , QPoint new_p = QPoint(10,10));
    bool check_endgame();
    QPoint getKingPoint(char _board[][8], char piece);
    int  getPiecesNumber()      ;
    char chessBoard[8][8];
private:
    void setup_board();
    void getBlackMoves(char pieceName, int x, int y, char board[][8], safelist<chess_pack> *_list);
    void getWhiteMoves(char pieceName, int x, int y, char board[][8], safelist<chess_pack> *_list);
    void show_board();
    void help();
    void castling_conditions_check(QPoint ex_p);

    safelist<chess_pack> *list;
    safelist<chess_pack> *white_list;
    chess_pack  pack;


    char piece_list[14] = {'0','P','R','N','B','Q','K','p','r','n','b','q','k'};

    bool check(QPoint ex_p, QPoint new_p, int _mode);

    int ex_x(std::__cxx11::string s);
    int ex_y(std::__cxx11::string s);
    int new_x(std::__cxx11::string s);
    int new_y(std::__cxx11::string s);
    int turn_counter = 0;
public slots:
    void slot_msg_to_board(QPoint ex_p, QPoint new_p, std::string message);
signals:
    void signal_from_board(int t);

};
#endif // BOARD_H
