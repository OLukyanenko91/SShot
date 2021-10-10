#ifndef MODEL_HPP
#define MODEL_HPP

#include <QObject>
#include "src/Screen/Screen.hpp"
#include "src/Screen/Screenshot.hpp"
#include "src/Storage/SQLiteStorage.hpp"

namespace NModel
{
    class CModel : public QObject
    {
        Q_OBJECT

    public:
        CModel(QObject* parent = nullptr);

    public:
        Q_INVOKABLE void load();
        Q_INVOKABLE void makeScreenshot();
        Q_INVOKABLE void removeScreenshot(const int index);
        Q_INVOKABLE void save();

    public slots:
        void onScreenshotReady(NScreen::CScreenshot* screenshot);
        void onScreenshotsLoaded(QList<NScreen::CScreenshot*> screenshots);
        void onScreenshotsSaved();
        void onStorageThreadFinished();

    signals:
        void addUIScreenshot(NScreen::CScreenshot* screenshot);
        void loaded(QList<NScreen::CScreenshot*> screenshots);
        void loadScreenshots();
        void saved();
        void saveScreenshots(QList<NScreen::CScreenshot*> screenshots);

    private:
        QList<NScreen::CScreenshot*> mScreenshots;
        NScreen::CScreenThread mScreenThread;
        NStorage::CSQLiteStorage mStorage;
        QThread mStorageThread;
    };
}

#endif // MODEL_HPP
