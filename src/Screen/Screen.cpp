#include <QGuiApplication>
#include <QScreen>
#include <QBitmap>
#include <QDebug>
#include "Screen.hpp"

namespace NScreen
{
    Screenshot CScreen::grab()
    {
        qDebug() << "CScreen::grab";

        Screenshot newScreenshot;
        auto source = QGuiApplication::screens().first();
        QRect rect = source->geometry();

        newScreenshot.pixmap = source->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());;
        newScreenshot.equality = compareScreenshots(newScreenshot.pixmap.toImage(),
                                                    mLastScreenshot.pixmap.toImage());

        mLastScreenshot = newScreenshot;

        return newScreenshot;
    }

    int CScreen::compareScreenshots(const QImage& left, const QImage& right)
    {
        qDebug() << "CScreen::compareScreenshots";

        double equality = 0;

        if (left.isNull() || right.isNull())
            return equality;

        for (int y = 0; y < left.height(); y++)
        {
            auto firstLine = (uint*)left.scanLine(y) ;
            auto secondLine = (uint*)right.scanLine(y) ;

            for (int x = 0; x < left.width(); x++)
                if (firstLine[x] == secondLine[x])
                    ++equality;
        }

        return equality * 100 / (left.height() * left.width());
    }
}
