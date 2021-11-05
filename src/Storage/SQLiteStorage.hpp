#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <QList>
#include <QtSql>
#include <src/Screen/Screenshot.hpp>

namespace NStorage
{
    const QString DATABASE_FILE_NAME("data.db");

    class CSQLiteStorage : public QObject
    {
        Q_OBJECT

    public:
        CSQLiteStorage(QObject* parent = nullptr);

    public slots:
        void saveData(const QList<NScreen::CScreenshot*> screenshots);
        void loadData();

    signals:
        void screenshotsLoaded(QList<NScreen::CScreenshot*> screenshots);
        void screenshotsSaved();

    private:
        QSqlDatabase mDatabase;
        QList<qint64> mHashes;
    };
}

#endif // STORAGE_HPP
