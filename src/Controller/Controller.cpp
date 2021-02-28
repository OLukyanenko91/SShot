#include <QDebug>
#include "Controller.hpp"

namespace NController
{
    CController::CController(QObject* parent)
        : QObject(parent)
    {
        qDebug() << "CController::CController";

        mTimer.setInterval(100);
        connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    }

    void CController::startTimer()
    {
        qDebug() << "CController::startTimer";

        mTimer.start();
    }

    void CController::stopTimer()
    {
        qDebug() << "CController::stopTimer";

        mTimer.stop();
    }

    void CController::timeout()
    {
        qDebug() << "CController::timeout";
    }
}
