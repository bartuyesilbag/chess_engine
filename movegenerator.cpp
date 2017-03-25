#include "movegenerator.h"
#include "board.h"
#include "safelist.h"
#include "pack.h"
#include "pieces.h"
#include "iostream"

moveGenerator::moveGenerator(safelist<chess_pack> *_list, board *_b): QObject(NULL)
{
    list = _list;
    b    = _b;
}

moveGenerator::~moveGenerator()
{
    qDebug() << "moveGenerator deleted!";
}

void moveGenerator::findLegalMoves(int mode)
{
    b->getBoard(mode);
    int list_size = list->size();
    for(int i = 0; i < list_size; i++){
        list->pop(pack);
        qDebug() << "pack.name:" << pack.name << "pack.x:" << pack.X << "pack.y:" << pack.Y
                 << "pack.destX:" << pack.destX << "pack.destY:" << pack.destY << i;
    }
    b->slot_msg_to_board(QPoint(1,3),QPoint(2,3),"from ai");
}
