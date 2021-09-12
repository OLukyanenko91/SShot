#include <QDebug>
#include <QBuffer>
#include <algorithm>
#include "Model.hpp"


namespace NModel
{
    // --- constructors ---

    CModel::CModel(QObject *parent)
        : QAbstractListModel(parent)
    {
        qInfo() << QThread::currentThreadId() << "CModel::CModel()";

        connect(&mScreenThread, &NScreen::CScreenThread::screenshotReady, this, &CModel::addScreenshot);
        connect(this, &CModel::loadScreenshots, &mStorage, &NStorage::CSQLiteStorage::loadData);
        connect(this, &CModel::saveScreenshots, &mStorage, &NStorage::CSQLiteStorage::saveData);
        connect(&mStorage, &NStorage::CSQLiteStorage::screenshotsLoaded, this, &CModel::onScreenshotsLoaded);
        connect(&mStorage, &NStorage::CSQLiteStorage::screenshotSaved, this, &CModel::onScreenshotsSaved);
        connect(&mStorageThread, &QThread::finished, this, &CModel::onStorageThreadFinished);

        mStorage.moveToThread(&mStorageThread);
        mStorageThread.start();
    }

    // --- Q_INVOKABLE ---

    void CModel::increase()
    {
        qInfo() << QThread::currentThreadId() << "CModel::increase()";
        mScreenThread.makeScreenshot();
    }

    void CModel::load()
    {
        qInfo() << QThread::currentThreadId() << "CModel::load()";
        emit loadScreenshots();
    }

    void CModel::save()
    {
        qInfo() << QThread::currentThreadId() << "CModel::save()";
        emit saveScreenshots(mScreenshots);
    }

    // --- public slots ---

    void CModel::addScreenshot(NScreen::CScreenshot screenshot)
    {
        qInfo() << QThread::currentThreadId() << "CModel::addScreenshot()";

        beginInsertRows(QModelIndex(), 0, 0);
        mScreenshots.insert(0, screenshot);
        endInsertRows();
    }

    void CModel::onScreenshotsLoaded(QList<NScreen::CScreenshot> screenshots)
    {
        qInfo() << QThread::currentThreadId() << "CModel::onScreenshotsLoaded()";

        if (!screenshots.empty())
        {
            std::for_each(screenshots.begin(), screenshots.end(), [&](const NScreen::CScreenshot&){
                beginInsertRows(QModelIndex(), 0, 0);
                endInsertRows();
            });

            mScreenshots = screenshots;
            mScreenThread.setLastScreenPixmap(screenshots.first().getPixmap());
        }

        emit ready();
    }

    void CModel::onScreenshotsSaved()
    {
        qInfo() << QThread::currentThreadId() << "CModel::onScreenshotsSaved()";
        mStorageThread.quit();
    }

    void CModel::onStorageThreadFinished()
    {
        qInfo() << QThread::currentThreadId() << "CModel::onStorageThreadFinished()";
        emit saved();
    }

    // --- public ---

    int CModel::rowCount(const QModelIndex& p) const
    {
        Q_UNUSED(p)
        qInfo() << QThread::currentThreadId() << "CModel::rowCount()";

        return mScreenshots.size();
    }

    QVariant CModel::data(const QModelIndex &index, int role) const
    {
        qInfo() << QThread::currentThreadId() << "CModel::data()";

        if (!index.isValid())
            return QVariant();

        if (role == ImageData)
            return QVariant(mScreenshots[index.row()].toStringData());
        else if (role == ImageEquality)
            return QVariant(QString::number(mScreenshots[index.row()].getEquality()) + "%");
        else
            return QVariant();
    }

    QHash<int, QByteArray> CModel::roleNames() const
    {
        qInfo() << QThread::currentThreadId() << "CModel::roleNames()";

        QHash<int, QByteArray> roles;
        roles[ImageData] = "imageData";
        roles[ImageEquality] = "imageEquality";

        return roles;
    }

}
