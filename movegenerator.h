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
    moveGenerator(safelist<chess_pack> *_list, board *_b);
    ~moveGenerator();
private:
    void msgToBoard(QPoint ex_p, QPoint new_p);
    void calculateBoard(char board[][8], int mode);
    void sort_scores(QVector<int> _score);
    void find_legal_white_moves();
    QString black_move_to_screen(QPoint ex_p, QPoint new_p);

    chess_pack           pack;
    safelist<chess_pack> *list;
    board                *b;
    char verticle[8]   = {'8','7','6','5','4','3','2','1'};
    char horizontal[8] = {'a','b','c','d','e','f','g','h'};
signals:
    void signalToBoard(message msg);
public slots:
    void findLegalBlackMoves();
};

#endif // MOVEGENERATOR_H
