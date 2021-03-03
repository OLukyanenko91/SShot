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
        QString toStringData() const;
        QByteArray toBlobData() const;
        QPixmap getPixmap() const;
        unsigned getEquality() const;

    private:
        QPixmap mPixmap;
        unsigned mEquality;
    };
}

#endif // SCREENSHOT_HPP
