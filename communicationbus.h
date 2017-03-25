#ifndef COMMUNICATIONBUS_H
#define COMMUNICATIONBUS_H
#include <QObject>
#include "defines.h"
#include "iostream"
class communicationBus : public QObject
{
    Q_OBJECT
public:
    communicationBus();
    ~communicationBus();
private:
    int ex_x(std::string  s);
    int ex_y(std::string  s);
    int new_x(std::string s);
    int new_y(std::string s);
signals:
    void signalFromComBus(QPoint ex_p, QPoint new_p,std::string message);
public slots:
    void test();
};

#endif // COMMUNICATIONBUS_H
