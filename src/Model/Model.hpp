#ifndef MODEL_HPP
#define MODEL_HPP

#include <QList>
#include <QPixmap>
#include <QAbstractListModel>
#include "src/Screen/Screen.hpp"
#include "src/Storage/Storage.hpp"

namespace NModel
{
    class CModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        enum Roles {
                ImageData = Qt::UserRole + 1,
                PrevItemIdentity
            };

        CModel(QObject* parent = nullptr);

    public:
        Q_INVOKABLE void makeScreenshot();

    public:
        virtual int rowCount(const QModelIndex& p) const;
        virtual QVariant data(const QModelIndex& index, int role) const;
        QHash<int, QByteArray> roleNames() const;

    private:
        QList<QPixmap> mScreenshots;
        NScreen::CScreen mScreen;
        NStorage::CStorage mStorage;
    };
}

#endif // MODEL_HPP
