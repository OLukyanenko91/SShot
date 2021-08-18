#ifndef SCREENSHOT_HPP
#define SCREENSHOT_HPP

#include <QPixmap>

namespace NScreen
{
    class CScreenshot
    {
    public:
        CScreenshot();
        CScreenshot(const QPixmap& image, const unsigned equality);

    public:
        unsigned getEquality() const;
        QPixmap getPixmap() const;
        QByteArray toBlobData() const;
        QString toStringData() const;

    private:
        QPixmap mPixmap;
        unsigned mEquality;
    };
}

#endif // SCREENSHOT_HPP
