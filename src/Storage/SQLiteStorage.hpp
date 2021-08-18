#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <QList>
#include <QtSql>
#include <src/Screen/Screen.hpp>
#include <src/Screen/Screenshot.hpp>

namespace NStorage
{
    const QString DATABASE_FILE_NAME("data.db");
    const QString DATABASE_TYPE("QSQLITE");

    class CSQLiteStorage : public QObject
    {
        Q_OBJECT

    public:
        CSQLiteStorage(QObject* parent = nullptr);

    public slots:
        void loadData();
        void saveData(const QList<NScreen::CScreenshot> screenshots);

    signals:
        void screenshotsLoaded(QList<NScreen::CScreenshot> screenshots);
        void screenshotSaved();
        void screenshotLoaded(NScreen::CScreenshot screenshot);

    private:
        QSqlDatabase mDatabase;
    };
}

#endif // STORAGE_HPP
