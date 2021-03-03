#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QThread>
#include "Screen.hpp"

namespace NScreen
{
    CScreenThread::CScreenThread(QObject *parent)
        : QThread(parent)
        , mAbort(false)
    {
        qDebug() << QThread::currentThreadId() << "CScreenThread::CScreenThread";
    }

    CScreenThread::~CScreenThread()
    {
        qDebug() << QThread::currentThreadId() << "CScreenThread::~CScreenThread";

        mMutex.lock();
        mAbort = true;
        mCondition.wakeOne();
        mMutex.unlock();
        wait();
    }

    void CScreenThread::makeScreenshot()
    {
        qDebug() << QThread::currentThreadId() << "CScreenThread::makeScreenshot";

        QMutexLocker locker(&mMutex);

        if (!isRunning())
            start(NormalPriority);
        else
            mCondition.wakeOne();
    }

    void CScreenThread::setLastScreenPixmap(const QPixmap& pixmap)
    {
        qDebug() << QThread::currentThreadId() << "CScreenThread::setLastScreenPixmap";

        mLastScreenPixmap = pixmap;
    }

    void CScreenThread::run()
    {
        qDebug() << QThread::currentThreadId() << "CScreenThread::run";

        QRect rect;
        QPixmap newScreenPixmap;
        unsigned equality;

        while(!mAbort)
        {
            auto source = QGuiApplication::screens().first();
            rect = source->geometry();
            newScreenPixmap = source->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
            equality = compareImages(newScreenPixmap.toImage(), mLastScreenPixmap.toImage());

            mLastScreenPixmap = newScreenPixmap;

            if (!mAbort)
                emit screenshotReady({newScreenPixmap, equality});

            mMutex.lock();
            mCondition.wait(&mMutex);
            mMutex.unlock();
        }
    }

    unsigned CScreenThread::compareImages(const QImage& left, const QImage& right)
    {
        qDebug() << QThread::currentThreadId() << "CScreenThread::compareImages";

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
