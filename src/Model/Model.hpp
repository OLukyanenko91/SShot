#ifndef MODEL_HPP
#define MODEL_HPP

#include <QList>
#include <QAbstractListModel>
#include "src/Screen/Screen.hpp"
#include "src/Screen/Screenshot.hpp"
#include "src/Storage/SQLiteStorage.hpp"

namespace NModel
{
    class CModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        enum Roles {
                ImageData = Qt::UserRole + 1,
                ImageEquality
            };

        CModel(QObject* parent = nullptr);

    public:
        Q_INVOKABLE void increase();
        Q_INVOKABLE void load();
        Q_INVOKABLE void save();

    public slots:
        void addScreenshot(NScreen::CScreenshot screenshot);
        void onScreenshotsLoaded(QList<NScreen::CScreenshot> screenshots);
        void onScreenshotsSaved();
        void onStorageThreadFinished();

    signals:
        void loadScreenshots();
        void ready();
        void saved();
        void saveScreenshots(QList<NScreen::CScreenshot> screenshots);

    public:
        virtual QVariant data(const QModelIndex& index, int role) const;
        QHash<int, QByteArray> roleNames() const;
        virtual int rowCount(const QModelIndex& p) const;

    private:
        QList<NScreen::CScreenshot> mScreenshots;
        NScreen::CScreenThread mScreenThread;
        NStorage::CSQLiteStorage mStorage;
        QThread mStorageThread;
    };
}

#endif // MODEL_HPP
