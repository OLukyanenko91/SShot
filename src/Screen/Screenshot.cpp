#include <QBuffer>
#include <QThread>
#include <QDebug>
#include "src/Screen/Screenshot.hpp"

namespace NScreen
{
    CScreenshot::CScreenshot()
        : mEquality(0)
    {
        qDebug() << QThread::currentThreadId() << "CScreenshot::CScreenshot";
    }

    CScreenshot::CScreenshot(const QPixmap& image, const unsigned equality)
        : mPixmap(image)
        , mEquality(equality)
    {
        qDebug() << QThread::currentThreadId() << "CScreenshot::CScreenshot";
    }

    QString CScreenshot::toStringData() const
    {
        qDebug() << QThread::currentThreadId() << "CScreenshot::toStringData";

        QByteArray byteArray;
        QBuffer buffer(&byteArray);

        buffer.open(QIODevice::WriteOnly);
        mPixmap.save(&buffer, "PNG");

        return "data:image/jpg;base64," + QString::fromLatin1(byteArray.toBase64().data());
    }

    QByteArray CScreenshot::toBlobData() const
    {
        qDebug() << QThread::currentThreadId() << "CScreenshot::toBlobData";

        QByteArray byteArray;
        QBuffer buffer(&byteArray);

        buffer.open(QIODevice::WriteOnly);
        mPixmap.save(&buffer, "PNG");

        return byteArray;
    }

    QPixmap CScreenshot::getPixmap() const
    {
        qDebug() << QThread::currentThreadId() << "CScreenshot::getPixmap";

        return mPixmap;
    }

    unsigned CScreenshot::getEquality() const
    {
        qDebug() << QThread::currentThreadId() << "CScreenshot::getEquality";

        return mEquality;
    }
}
