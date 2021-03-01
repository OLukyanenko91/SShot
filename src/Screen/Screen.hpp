#ifndef SCREEN_H
#define SCREEN_H

#include <QPixmap>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

namespace NScreen
{
    struct Screenshot
    {
        QPixmap pixmap;
        int equality;
    };

    class CScreenThread : public QThread
    {
        Q_OBJECT

    public:
        CScreenThread(QObject* parent = nullptr);
        ~CScreenThread();

    public:
        void makeScreenshot();

    signals:
        void screenshotReady(NScreen::Screenshot screenshot);

    protected:
        void run() override;

    private:
        int compareScreenshots(const QImage& left, const QImage& right);

    private:
        Screenshot mLastScreenshot;
        QMutex mMutex;
        QWaitCondition mCondition;
        bool mAbort;
    };
}

#endif // SCREEN_H
