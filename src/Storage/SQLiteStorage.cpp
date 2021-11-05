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
        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::saveData:: screenshots count " << screenshots.length();
        QSqlQuery query(mDatabase);
        bool newImagesExist = false;

        mDatabase.open();

        if (!mDatabase.isOpen())
            qCritical() << "Database isn't open. Reason: " << mDatabase.lastError().text();
        else
        {
            query.exec("CREATE TABLE IF NOT EXISTS screenshots "
                       "(ID INTEGER PRIMARY KEY, position INTEGER, hash INTEGER, data BLOB, equality INTEGER)");


            // Remove deleted images from db
            for (auto hash : mHashes)
            {
                auto result = std::find_if(screenshots.begin(),
                                           screenshots.end(),
                                           [&](NScreen::CScreenshot* screenshot){ return hash == screenshot->getHash(); });

                if (result == std::end(screenshots))
                {
                    qInfo() << "Delete from db item with hash: " << hash;
                    query.prepare( "DELETE FROM screenshots WHERE hash=:hash" );
                    query.bindValue(":hash", hash);

                    if (!query.exec())
                        qCritical() << "Error deleting from table:\n" << query.lastError();
                }
            }

            // Add new images
            for (int index = 0; index < screenshots.length(); ++index)
            {
                auto screenshot = screenshots[index];
                auto result = std::find_if(mHashes.begin(),
                                           mHashes.end(),
                                           [&](qint64 hash){ return hash == screenshot->getHash(); });

                if (result == std::end(mHashes))
                {
                    newImagesExist = true;

                    qInfo() << "Insert new item with hash: " << screenshot->getHash();
                    query.prepare("INSERT INTO screenshots (position, hash, data, equality) VALUES (:position, :hash, :data, :equality)");
                    query.bindValue(":position", index);
                    query.bindValue(":hash", screenshot->getHash());
                    query.bindValue(":data", screenshot->toBlobData());
                    query.bindValue(":equality", screenshot->getEquality());

                    if (!query.exec())
                        qCritical() << "Error inserting image into table:\n" << query.lastError();
                }
            }

            // Update order of existing images
            if (newImagesExist) // update order of existing images
            {
                for (int index = 0; index < screenshots.length(); ++index)
                {
                    auto screenshot = screenshots[index];

                    qInfo() << "Update existing item with hash: " << screenshot->getHash();
                    query.prepare( "UPDATE screenshots SET position=:position WHERE hash=:hash" );
                    query.bindValue(":position", index);
                    query.bindValue(":hash", screenshot->getHash());

                    if (!query.exec())
                        qCritical() << "Error update image index:\n" << query.lastError();
                }
            }
        }

        query.clear();
        mDatabase.close();
        emit screenshotsSaved();
    }

    void CSQLiteStorage::loadData()
    {
        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::loadData";

        QPixmap pixmap;
        uint hash;
        QSqlQuery query(mDatabase);
        QList<NScreen::CScreenshot*> screenshots;

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
                    hash = query.value(2).toUInt();
                    pixmap.loadFromData(query.value(3).toByteArray());
                    screenshots.push_back(new NScreen::CScreenshot(pixmap,
                                                                   query.value(4).toUInt(),
                                                                   query.value(1).toUInt(),
                                                                   hash));
                    mHashes.push_back(hash);
                }
            }
        }

        std::sort(screenshots.begin(), screenshots.end(),
                  [](NScreen::CScreenshot* leftSshot, NScreen::CScreenshot* rightSshot)
                  { return leftSshot->getIndex() < rightSshot->getIndex(); });

        query.clear();
        mDatabase.close();

        qDebug() << QThread::currentThreadId() << "CSQLiteStorage::loadData:: Screenshots count " << screenshots.length();
        emit screenshotsLoaded(screenshots);
    }
}
