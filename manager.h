#ifndef MANAGER_H
#define MANAGER_H
#include "safelist.h"
#include "defines.h"
#include "movegenerator.h"
#include "communicationbus.h"
class QObject;

class manager : public QObject
{
    Q_OBJECT
public:
    manager();
    ~manager();
    void start();
private:
    communicationBus *c;
    moveGenerator *m;
    board *b;
    safelist<chess_pack> *list;
    safelist<chess_pack> *white_list;
    int turn_counter = 1;
signals:
    void turnChangeWhite();
    void turnChangeBlack(int mode);
public slots:
    void turn_change(int turn_counter);
};

#endif // MANAGER_H
