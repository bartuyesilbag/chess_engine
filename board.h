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
    board(safelist<chess_pack> *_list, QObject *parent = 0);
    void whitePawn(int x, int y);
    void blackPawn(int x, int y);
    void rook(int x, int y)     ;
    void knight(int y, int x)   ;
    void bishop(int x, int y)   ;
    void queen(int x, int y)    ;
    void king(int x, int y)     ;
    void getBoard(int mode)             ;
    int  getPiecesNumber()      ;
    void getMoves(char pieceName, int x, int y);
private:
    void setup_board();
    safelist<chess_pack> *list;
    chess_pack  pack;
    char chessBoard[8][8];
    char piece_list[14] = {'0','P','R','N','B','Q','K','p','r','n','b','q','k'};
    void getBlackMoves(char pieceName, int x, int y);
    void getWhiteMoves(char pieceName, int x, int y);
    void show_board();
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
