#include "manager.h"
#include "movegenerator.h"
#include "communicationbus.h"
#include "pieces.h"
#include "board.h"
#include "defines.h"
#include <QObject>
#include <QDebug>

manager::manager(): QObject(NULL)
{
    qDebug() << "Yapay Zeka Temelli Satranç Simulasyonu v0.1";
    qDebug() << "Game Start! Play a Move!";
    start();
}

manager::~manager()
{
    qDebug() << "manager deleted!";
}

void manager::start()
{
    list = new safelist<chess_pack>();
    b = new board(list);
    c = new communicationBus();
    m = new moveGenerator(list,b);
    connect(b,SIGNAL(signal_from_board(int)),this,SLOT(turn_change(int)));
    connect(c,SIGNAL(signalFromComBus(QPoint,QPoint,std::string)),b,SLOT(slot_msg_to_board(QPoint,QPoint,std::string)));
    c->test();
}

void manager::turn_change(int turn_counter)
{
    if(turn_counter % 2 EQ 0){
        qDebug() << "White Turn!";
        c->test();
    }
    else if(turn_counter % 2 EQ 1){
        qDebug() << "Black Turn!";
        m->findLegalMoves(mode::black);
    }

}
