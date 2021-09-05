#include <QDebug>
#include "SQLiteStorage.hpp"

namespace NStorage
{
    // --- constructors ---

    CSQLiteStorage::CSQLiteStorage(QObject* parent)
        : QObject(parent)
    {
        qInfo() << QThread::currentThreadId() << "CSQLiteStorage::CSQLiteStorage()";

        mDatabase = QSqlDatabase::addDatabase(DATABASE_TYPE);
        mDatabase.setDatabaseName(DATABASE_FILE_NAME);
    }

    // --- public slots ---

    void CSQLiteStorage::loadData()
    {
        qInfo() << QThread::currentThreadId() << "CSQLiteStorage::loadData()";

        QList<NScreen::CScreenshot> screenshots;
        QPixmap pixmap;
        QSqlQuery query(mDatabase);
        unsigned equality;

        mDatabase.open();

        if (!mDatabase.isOpen())
            qCritical() << "CSQLiteStorage::loadData -> Database isn't open. Reason: " << mDatabase.lastError().text();
        else if (mDatabase.tables().empty())
            qWarning() << "CSQLiteStorage::loadData -> Database is empty";
        else
        {
            if( !query.exec( "SELECT * from screenshots" ))
                    qCritical() << "CSQLiteStorage::loadData -> Error getting data from table:\n" << query.lastError();
            else
            {
                while (query.next())
                {
                    pixmap.loadFromData(query.value(1).toByteArray());
                    equality = query.value(2).toUInt();
                    screenshots.push_back({pixmap, equality});
                }
            }
        }

        mDatabase.close();

        qInfo() << QThread::currentThreadId() << "CSQLiteStorage::loadData -> Loaded";
        qInfo() << QThread::currentThreadId() << "CSQLiteStorage::loadData -> Screenshots count: " << screenshots.length();
        emit screenshotsLoaded(screenshots);
    }

    void CSQLiteStorage::saveData(const QList<NScreen::CScreenshot> screenshots)
    {
        qInfo() << QThread::currentThreadId() << "CSQLiteStorage::saveData()";
        qInfo() << QThread::currentThreadId() << "CSQLiteStorage::saveData -> Screenshots count: " << screenshots.length();

        QSqlQuery query(mDatabase);

        QFile::remove(DATABASE_FILE_NAME);
        mDatabase.open();

        if (!mDatabase.isOpen())
            qCritical() << "CSQLiteStorage::saveData -> Database isn't open. Reason: " << mDatabase.lastError().text();
        else
        {
            query.exec("CREATE TABLE IF NOT EXISTS screenshots "
                       "(ID INTEGER PRIMARY KEY, imagedata BLOB, equality INTEGER)");

            for (const auto& screenshot : screenshots)
            {
                query.prepare( "INSERT INTO screenshots (imagedata, equality) VALUES (:imageData, :equality)" );
                query.bindValue(":imageData", screenshot.toBlobData());
                query.bindValue(":equality", screenshot.getEquality());

                if (!query.exec())
                    qCritical() << "CSQLiteStorage::saveData -> Error inserting image into table:\n" << query.lastError();
            }
        }

        mDatabase.close();

        qInfo() << QThread::currentThreadId() << "CSQLiteStorage::loadData -> Saved";
        emit screenshotSaved();
    }
}
