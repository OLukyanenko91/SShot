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
        Q_INVOKABLE void save();
        Q_INVOKABLE void load();

    public:
        virtual int rowCount(const QModelIndex& p) const;
        virtual QVariant data(const QModelIndex& index, int role) const;
        QHash<int, QByteArray> roleNames() const;

    private slots:
        void addScreenshot(NScreen::CScreenshot screenshot);

    private:
        QList<NScreen::CScreenshot> mScreenshots;
        NScreen::CScreenThread mScreen;
        NStorage::CSQLiteStorage mStorage;
    };
}

#endif // MODEL_HPP
