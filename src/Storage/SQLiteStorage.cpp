#include <QDebug>
#include <QtSql>
#include "SQLiteStorage.hpp"

namespace NStorage
{
    CSQLiteStorage::CSQLiteStorage(QObject* parent)
        : QObject(parent)
    {
        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::CSQLiteStorage";
    }

    void CSQLiteStorage::saveData(const QList<NScreen::CScreenshot>& screenshots)
    {
        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::saveData";

        QFile::remove(DATABASE_FILE_NAME);

        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        QSqlQuery query(database);

        database.setDatabaseName(DATABASE_FILE_NAME);
        database.open();

        if (!database.isOpen())
            qCritical() << "Database isn't open. Reason: " << database.lastError().text();
        else
        {
            query.exec("CREATE TABLE IF NOT EXISTS screenshots "
                       "(ID INTEGER PRIMARY KEY, imagedata BLOB, equality INTEGER)");

            for (auto& screenshot : screenshots)
            {
                query.prepare( "INSERT INTO screenshots (imagedata, equality) VALUES (:imageData, :equality)" );
                query.bindValue(":imageData", screenshot.toBlobData());
                query.bindValue(":equality", screenshot.getEquality());

                if (!query.exec())
                    qCritical() << "Error inserting image into table:\n" << query.lastError();
            }
        }

        database.close();
    }

    void CSQLiteStorage::loadData(QList<NScreen::CScreenshot>& screenshots)
    {
        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::loadData";

        QPixmap pixmap;
        unsigned equality;
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        QSqlQuery query(database);

        database.setDatabaseName(DATABASE_FILE_NAME);
        database.open();

        if (!database.isOpen())
            qCritical() << "Database isn't open. Reason: " << database.lastError().text();
        else if (database.tables().empty())
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
                    screenshots.push_back({pixmap, equality});
                }
            }
        }

        database.close();
    }
}
