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
    void msgToBoard(QPoint ex_p, QPoint new_p);
    void calculateBoard(char board[][8], int _mode, int count = 0);
    QVector<int> sort_scores(QVector<int> _score);
    void find_legal_white_moves(int count);
    void fill_board(char _board[][8]);
    QString black_move_to_screen(QPoint ex_p, QPoint new_p);

    chess_pack           pack;
    score_pack           white_score_pack;
    score_pack           black_score_pack;
    score_pack           general_score_pack;
    safelist<chess_pack> *list;
    safelist<chess_pack> *white_list;
    board                *b;
    char verticle[8]   = {'8','7','6','5','4','3','2','1'};
    char horizontal[8] = {'a','b','c','d','e','f','g','h'};
signals:
    void signalToBoard(message msg);
public slots:
    void findLegalBlackMoves();
};

#endif // MOVEGENERATOR_H
