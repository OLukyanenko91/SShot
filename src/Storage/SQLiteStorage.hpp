#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <QList>
#include <src/Screen/Screen.hpp>
#include <src/Screen/Screenshot.hpp>

namespace NStorage
{
    const QString DATABASE_FILE_NAME("data.db");

    class CSQLiteStorage : public QObject
    {
        Q_OBJECT

    public:
        CSQLiteStorage(QObject* parent = nullptr);

    public:
        void saveData(const QList<NScreen::CScreenshot>& screenshots);
        void loadData(QList<NScreen::CScreenshot>& screenshots);

    signals:
        void screenshotLoaded(NScreen::CScreenshot screenshot);
    };
}

#endif // STORAGE_HPP
