#ifndef SCREENSHOT_HPP
#define SCREENSHOT_HPP

#include <QPixmap>

namespace NScreen
{
    class CScreenshot : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(int equality READ getEquality)
        Q_PROPERTY(QString imageStr READ toStringData)

    public:
        CScreenshot(const QPixmap& image, const unsigned equality, QObject* parent = nullptr);

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
