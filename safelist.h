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
        safe_list << t;
    }

    T at(int i){
        return safe_list.at(i);
    }

    void pop(T & t)
    {
        t = safe_list.takeFirst();
    }

    int size()
    {
        return safe_list.size();
    }

    void clearList()
    {
        safe_list.clear();
    }


private:
    QList<T> safe_list;
};

#endif // SAFELIST_H

