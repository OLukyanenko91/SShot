#ifndef SCREEN_H
#define SCREEN_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <src/Screen/Screenshot.hpp>

namespace NScreen
{
    class CScreenThread : public QThread
    {
        Q_OBJECT

    public:
        CScreenThread(QObject* parent = nullptr);
        ~CScreenThread();

    public:
        void makeScreenshot();
        void setLastScreenPixmap(const QPixmap& pixmap);

    signals:
        void screenshotReady(NScreen::CScreenshot screenshot);

    protected:
        void run() override;

    private:
        unsigned compareImages(const QImage& left, const QImage& right);

    private:
        QPixmap mLastScreenPixmap;
        QMutex mMutex;
        QWaitCondition mCondition;
        bool mAbort;
    };
}

#endif // SCREEN_H
