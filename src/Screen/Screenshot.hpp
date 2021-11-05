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
        CScreenshot(const QPixmap& image,
                    const unsigned equality = 0,
                    const unsigned index = 0,
                    const qint64 hash = 0,
                    QObject* parent = nullptr);

    public:
        unsigned getEquality() const;
        QPixmap getPixmap() const;
        qint64 getHash() const;
        uint getIndex() const;
        void setHash(const qint64 hash);
        QByteArray toBlobData() const;
        QString toStringData() const;

    private:
        QPixmap mPixmap;
        unsigned mEquality = 0;
        qint32 mIndex = 0;
        qint64 mHash = 0;
    };
}

#endif // SCREENSHOT_HPP
