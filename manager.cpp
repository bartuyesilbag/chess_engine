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
    qDebug() << "|---------------------------------------------|";
    qDebug() << "|Yapay Zeka Temelli Satranç Simulasyonu v0.0.1|";
    qDebug() << "|Bartu Yeşilbağ                               |";
    qDebug() << "|Tez Danışmanı: Nihan Kahraman                |";
    qDebug() << "|YTU EHM 2016-2017 Bitirme Tezi               |";
    qDebug() << "|---------------------------------------------|";
    qDebug() << "   ";
    qDebug() << "Game Start! Play a Move!";
    qDebug() << "for help, write help";
    qDebug() << "White Turn!";
    start();
}

manager::~manager()
{
    qDebug() << "Game Over!";
}

void manager::start()
{
    list = new safelist<chess_pack>();
    white_list = new safelist<chess_pack>();
    b = new board(list,white_list);
    c = new communicationBus();
    m = new moveGenerator(list,white_list,b);
    connect(b,SIGNAL(signal_from_board(int)),this,SLOT(turn_change(int)));
    connect(c,SIGNAL(signalFromComBus(QPoint,QPoint,std::string)),b,SLOT(slot_msg_to_board(QPoint,QPoint,std::string)));
    c->test();
}

void manager::turn_change(int turn_counter)
{
//    if(turn_counter % 2 EQ 0 AND turn_counter BG 2 ){
//        if(b->check_endgame()){
//            return;
//        }
//    }
    if(turn_counter % 2 EQ 0){
        qDebug() << "----> White Turn!";
        c->test();
    }
    else if(turn_counter % 2 EQ 1){
        m->findLegalBlackMoves();
    }

}
