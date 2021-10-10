#include <memory>
#include <QDebug>
#include <QBuffer>
#include <algorithm>
#include "Model.hpp"


namespace NModel
{
    // --- constructors ---

    CModel::CModel(QObject *parent)
        : QObject(parent)
    {
        qInfo() << QThread::currentThreadId() << "CModel::CModel()";

        connect(&mScreenThread, &NScreen::CScreenThread::screenshotReady, this, &CModel::onScreenshotReady);
        connect(this, &CModel::loadScreenshots, &mStorage, &NStorage::CSQLiteStorage::loadData);
        connect(this, &CModel::saveScreenshots, &mStorage, &NStorage::CSQLiteStorage::saveData);
        connect(&mStorage, &NStorage::CSQLiteStorage::screenshotsLoaded, this, &CModel::onScreenshotsLoaded);
        connect(&mStorage, &NStorage::CSQLiteStorage::screenshotsSaved, this, &CModel::onScreenshotsSaved);
        connect(&mStorageThread, &QThread::finished, this, &CModel::onStorageThreadFinished);

        mStorage.moveToThread(&mStorageThread);
        mStorageThread.start();
    }

    // --- Q_INVOKABLE ---

    void CModel::load()
    {
        qInfo() << QThread::currentThreadId() << "CModel::load()";
        emit loadScreenshots();
    }

    void CModel::makeScreenshot()
    {
        qInfo() << QThread::currentThreadId() << "CModel::increase()";
        mScreenThread.takeScreenshot();
    }

    void CModel::removeScreenshot(const int index)
    {
        qInfo() << QThread::currentThreadId() << "CModel::removeScreenshot()";
        mScreenshots.removeAt(index);
    }

    void CModel::save()
    {
        qInfo() << QThread::currentThreadId() << "CModel::save()";
        emit saveScreenshots(mScreenshots);
    }

    // --- public slots ---

    void CModel::onScreenshotReady(NScreen::CScreenshot* screenshot)
    {
        qInfo() << QThread::currentThreadId() << "CModel::onScreenshotReady()";

        mScreenshots.insert(0, screenshot);
        emit addUIScreenshot(screenshot);
    }

    void CModel::onScreenshotsLoaded(QList<NScreen::CScreenshot*> screenshots)
    {
        qInfo() << QThread::currentThreadId() << "CModel::onScreenshotsLoaded()";

        if (!screenshots.empty())
        {
            mScreenshots = screenshots;
            mScreenThread.setLastScreenPixmap(screenshots.first()->getPixmap());
        }

        emit loaded(screenshots);
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
}
