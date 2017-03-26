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
    chess_pack           pack;
    safelist<chess_pack> *list;
    board                *b;
    void calculateBoard(char board[][8]);

signals:
    void signalToBoard(message msg);
public slots:
    void findLegalMoves(int mode);
};

#endif // MOVEGENERATOR_H
