#ifndef SCREEN_H
#define SCREEN_H

#include <QPixmap>

namespace NScreen
{
    struct Screenshot
    {
        QPixmap pixmap;
        int equality;
    };

    class CScreen
    {
    public:
        Screenshot grab();

    private:
        int compareScreenshots(const QImage& left, const QImage& right);

    private:
        Screenshot mLastScreenshot;
    };
}

#endif // SCREEN_H
