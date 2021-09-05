#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QThread>
#include "Screen.hpp"

namespace NScreen
{
    // --- constructors ---

    CScreenThread::CScreenThread(QObject *parent)
        : QThread(parent)
        , mAbort(false)
    {
        qInfo() << QThread::currentThreadId() << "CScreenThread::CScreenThread()";
    }

    // --- destructors ---

    CScreenThread::~CScreenThread()
    {
        qInfo() << QThread::currentThreadId() << "CScreenThread::~CScreenThread()";

        mMutex.lock();
        mAbort = true;
        mCondition.wakeOne();
        mMutex.unlock();
        wait();
    }

    // --- public ---

    void CScreenThread::makeScreenshot()
    {
        qInfo() << QThread::currentThreadId() << "CScreenThread::makeScreenshot()";

        QMutexLocker locker(&mMutex);

        if (!isRunning())
            start(NormalPriority);
        else
            mCondition.wakeOne();
    }

    void CScreenThread::setLastScreenPixmap(const QPixmap& pixmap)
    {
        qInfo() << QThread::currentThreadId() << "CScreenThread::setLastScreenPixmap()";
        mLastScreenPixmap = pixmap;
    }

    // --- protected ---

    void CScreenThread::run()
    {
        qInfo() << QThread::currentThreadId() << "CScreenThread::run()";

        QRect rect;
        QPixmap newScreenPixmap;
        unsigned equality;

        while(!mAbort)
        {
            auto screens = QGuiApplication::screens();

            if (screens.length())
            {
                auto source = screens.first();
                rect = source->geometry();
                newScreenPixmap = source->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
                equality = compareImages(newScreenPixmap.toImage(), mLastScreenPixmap.toImage());
                mLastScreenPixmap = newScreenPixmap;

                if (!mAbort)
                    emit screenshotReady({newScreenPixmap, equality});
            }

            mMutex.lock();
            mCondition.wait(&mMutex);
            mMutex.unlock();
        }
    }

    // --- private ---

    unsigned CScreenThread::compareImages(const QImage& left, const QImage& right)
    {
        qInfo() << QThread::currentThreadId() << "CScreenThread::compareImages()";

        double equality = 0;

        if (left.isNull() || right.isNull())
        {
            qWarning() << QThread::currentThreadId() << "CScreenThread::compareImages -> One of the images is null";
            return equality;
        }
        else if (left.width() != right.width() || left.height() != right.height())
        {
            qWarning() << QThread::currentThreadId() << "CScreenThread::compareImages -> The resolution of the images doesn't match";
            return equality;
        }

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
