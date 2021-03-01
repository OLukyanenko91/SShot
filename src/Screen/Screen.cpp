#include <QGuiApplication>
#include <QScreen>
#include <QBitmap>
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

    void CScreenThread::run()
    {
        qDebug() << QThread::currentThreadId() << "CScreenThread::run";

        while(!mAbort)
        {
            Screenshot newScreenshot;
            auto source = QGuiApplication::screens().first();
            QRect rect = source->geometry();

            newScreenshot.pixmap = source->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());;
            newScreenshot.equality = compareScreenshots(newScreenshot.pixmap.toImage(),
                                                        mLastScreenshot.pixmap.toImage());

            mLastScreenshot = newScreenshot;

            emit screenshotReady(newScreenshot);

            mMutex.lock();
            mCondition.wait(&mMutex);
            mMutex.unlock();
        }
    }

    int CScreenThread::compareScreenshots(const QImage& left, const QImage& right)
    {
        qDebug() << QThread::currentThreadId() << "CScreenThread::compareScreenshots";

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
