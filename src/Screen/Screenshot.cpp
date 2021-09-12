#include <QBuffer>
#include <QThread>
#include <QDebug>
#include "src/Screen/Screenshot.hpp"

namespace NScreen
{
    CScreenshot::CScreenshot()
        : mEquality(0)
    {
        qInfo() << QThread::currentThreadId() << "CScreenshot::CScreenshot()";
    }

    CScreenshot::CScreenshot(const QPixmap& image, const unsigned equality)
        : mPixmap(image)
        , mEquality(equality)
    {
        qInfo() << QThread::currentThreadId() << "CScreenshot::CScreenshot(QPixmap, unsigned)";
    }

    QString CScreenshot::toStringData() const
    {
        qInfo() << QThread::currentThreadId() << "CScreenshot::toStringData()";

        QByteArray byteArray;
        QBuffer buffer(&byteArray);

        buffer.open(QIODevice::WriteOnly);
        mPixmap.save(&buffer, "PNG");

        return "data:image/jpg;base64," + QString::fromLatin1(byteArray.toBase64().data());
    }

    QByteArray CScreenshot::toBlobData() const
    {
        qInfo() << QThread::currentThreadId() << "CScreenshot::toBlobData()";

        QByteArray byteArray;
        QBuffer buffer(&byteArray);

        buffer.open(QIODevice::WriteOnly);
        mPixmap.save(&buffer, "PNG");

        return byteArray;
    }

    QPixmap CScreenshot::getPixmap() const
    {
        qInfo() << QThread::currentThreadId() << "CScreenshot::getPixmap()";

        return mPixmap;
    }

    unsigned CScreenshot::getEquality() const
    {
        qInfo() << QThread::currentThreadId() << "CScreenshot::getEquality()";

        return mEquality;
    }
}
