#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QTimer>
#include "src/Screen/Screen.hpp"
#include "src/Model/Model.hpp"
#include "src/Storage/Storage.hpp"

namespace NController
{
    class CController : public QObject
    {
        Q_OBJECT

    public:
        CController(QObject* parent = nullptr);

    public:
        Q_INVOKABLE void startTimer();
        Q_INVOKABLE void stopTimer();

    private slots:
        void timeout();

    private:
        NScreen::CScreen mScreen;
        NModel::CModel mModel;
        NStorage::CStorage mStorage;
        QTimer mTimer;
    };
}

#endif // CONTROLLER_HPP
