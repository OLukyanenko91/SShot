#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include "Screen.hpp"

namespace NScreen
{
    QPixmap CScreen::grab()
    {
        qDebug() << "CScreen::grab";

        QScreen* source = QGuiApplication::screens().first();
        QRect rect = source->geometry();
        QPixmap screen = source->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());;

        return screen;
    }
}
