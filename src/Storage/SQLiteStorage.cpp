#include <QDebug>
#include "SQLiteStorage.hpp"

namespace NStorage
{
    CSQLiteStorage::CSQLiteStorage(QObject* parent)
        : QObject(parent)
    {
        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::CSQLiteStorage";

        mDatabase = QSqlDatabase::addDatabase("QSQLITE");
        mDatabase.setDatabaseName(DATABASE_FILE_NAME);
    }

    void CSQLiteStorage::saveData(const QList<NScreen::CScreenshot*> screenshots)
    {
        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::saveData";
        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::saveData:: size " << screenshots.length();
        QFile::remove(DATABASE_FILE_NAME);
        QSqlQuery query(mDatabase);

        mDatabase.open();

        if (!mDatabase.isOpen())
            qCritical() << "Database isn't open. Reason: " << mDatabase.lastError().text();
        else
        {
            query.exec("CREATE TABLE IF NOT EXISTS screenshots "
                       "(ID INTEGER PRIMARY KEY, imagedata BLOB, equality INTEGER)");

            for (auto screenshot : screenshots)
            {
                query.prepare( "INSERT INTO screenshots (imagedata, equality) VALUES (:imageData, :equality)" );
                query.bindValue(":imageData", screenshot->toBlobData());
                query.bindValue(":equality", screenshot->getEquality());

                if (!query.exec())
                    qCritical() << "Error inserting image into table:\n" << query.lastError();
            }
        }

        mDatabase.close();
        emit screenshotsSaved();
    }

    void CSQLiteStorage::loadData()
    {
        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::loadData";

        QList<NScreen::CScreenshot*> screenshots;
        QPixmap pixmap;
        QSqlQuery query(mDatabase);
        unsigned equality;

        mDatabase.open();

        if (!mDatabase.isOpen())
            qCritical() << "Database isn't open. Reason: " << mDatabase.lastError().text();
        else if (mDatabase.tables().empty())
            qWarning() << "Database is empty";
        else
        {
            if( !query.exec( "SELECT * from screenshots" ))
                    qCritical() << "Error getting data from table:\n" << query.lastError();
            else
            {
                while (query.next())
                {
                    pixmap.loadFromData(query.value(1).toByteArray());
                    equality = query.value(2).toUInt();
                    screenshots.push_back(new NScreen::CScreenshot(pixmap, equality));
                }
            }
        }

        mDatabase.close();
        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::loadData:: Screenshots count " << screenshots.length();
        emit screenshotsLoaded(screenshots);
    }
}
