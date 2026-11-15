//  menu.h

#ifndef MENU_H
#define MENU_H

#include <QObject>

class Menu : public QObject
{
    Q_OBJECT
public:
    Menu(QObject *parent = nullptr);

};

#endif // MENU_H
