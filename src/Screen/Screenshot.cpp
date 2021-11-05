#include <QBuffer>
#include <QThread>
#include <QDebug>
#include "src/Screen/Screenshot.hpp"

namespace NScreen
{
    CScreenshot::CScreenshot(const QPixmap& image,
                             const unsigned equality,
                             const unsigned index,
                             const qint64 hash,
                             QObject* parent)
        : QObject(parent),
          mPixmap(image),
          mEquality(equality),
          mIndex(index),
          mHash(hash)
    {
        qInfo() << QThread::currentThreadId() << "CScreenshot::CScreenshot(QPixmap, unsigned, uint)";
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

    qint64 CScreenshot::getHash() const
    {
        qInfo() << QThread::currentThreadId() << "CScreenshot::getHash()";

        return mHash;
    }

    uint CScreenshot::getIndex() const
    {
        qInfo() << QThread::currentThreadId() << "CScreenshot::getIndex()";

        return mIndex;
    }

    void CScreenshot::setHash(const qint64 hash)
    {
        mHash = hash;
    }
}
