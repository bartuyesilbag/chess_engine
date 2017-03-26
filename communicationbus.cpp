#include "communicationbus.h"
#include "iostream"
#include <QDebug>
#include <QObject>
#include "defines.h"

communicationBus::communicationBus(): QObject(NULL)
{

}

communicationBus::~communicationBus()
{
    qDebug() << "communication Bus deleted!";
}

void communicationBus::test()
{
    std::string message;
    std::cin >> message;
    emit signalFromComBus(QPoint(ex_x(message),ex_y(message)),QPoint(new_x(message),new_y(message)),message);
}

int communicationBus::ex_x(std::__cxx11::string s)
{
    return '8' - (int)s.at(1);
}

int communicationBus::ex_y(std::__cxx11::string s)
{
    switch (s.at(0)) {
    case 'a':
        return 0;
        break;
    case 'b':
        return 1;
        break;
    case 'c':
        return 2;
        break;
    case 'd':
        return 3;
        break;
    case 'e':
        return 4;
        break;
    case 'f':
        return 5;
        break;
    case 'g':
        return 6;
        break;
    case 'h':
        return 7;
        break;
    default:
        break;
    }
}

int communicationBus::new_x(std::__cxx11::string s)
{
    return '8' - (int)s.at(3);
}

int communicationBus::new_y(std::__cxx11::string s)
{
    switch (s.at(2)) {
    case 'a':
        return 0;
        break;
    case 'b':
        return 1;
        break;
    case 'c':
        return 2;
        break;
    case 'd':
        return 3;
        break;
    case 'e':
        return 4;
        break;
    case 'f':
        return 5;
        break;
    case 'g':
        return 6;
        break;
    case 'h':
        return 7;
        break;
    default:
        break;
    }
}
