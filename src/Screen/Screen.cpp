#include <QGuiApplication>
#include <QScreen>
#include "Screen.hpp"

namespace NScreen
{
    QPixmap CScreen::grab()
    {
        static int i = 0;

        auto source = QGuiApplication::screens().first();
        QRect g = source->geometry();
        QPixmap screen;

        screen = source->grabWindow(0, g.x(), g.y(), g.width(), g.height());
//        screen.save(QString::number(i++) + ".jpg", "jpg", 100);

        return screen;
    }
}
