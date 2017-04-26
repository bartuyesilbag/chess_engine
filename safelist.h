#ifndef SAFELIST_H
#define SAFELIST_H
#include <QList>
#include <QDebug>
template<typename T>
class safelist
{
public:
    safelist()
    {

    }

    void push(T t)
    {
        list << t;
    }

    T at(int i){
        return list.at(i);
    }

    void pop(T & t)
    {
        t = list.takeFirst();
    }

    int size()
    {
        return list.size();
    }
    void clearList()
    {
        list.clear();
    }


private:
    QList<T> list;
};

#endif // SAFELIST_H

