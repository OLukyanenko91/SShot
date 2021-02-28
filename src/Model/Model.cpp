#include <QDebug>
#include "Model.hpp"

namespace NModel
{
    CModel::CModel(QObject *parent)
        : QAbstractListModel(parent)
    {
        qDebug() << "CModel::CModel";
    }

    void CModel::makeScreenshot()
    {
        qDebug() << "CModel::makeScreenshot";

        QPixmap screenshot = mScreen.grab();

        beginInsertRows(QModelIndex(), 0, 0);
        mScreenshots.insert(0, screenshot);
        endInsertRows();
    }


    int CModel::rowCount(const QModelIndex &p) const
    {
        qDebug() << "CModel::rowCount: size: " << mScreenshots.size();

        return mScreenshots.size();
    }

    QVariant CModel::data(const QModelIndex &index, int role) const
    {
        qDebug() << "CModel::data";

        if (!index.isValid())
            return QVariant();

        if (role == PrevIdentity)
            return QVariant(0);
        else
            return QVariant();
    }

    QHash<int, QByteArray> CModel::roleNames() const
    {
        qDebug() << "CModel::roleNames";

        QHash<int, QByteArray> roles;
        roles[PrevIdentity] = "prevIdentity";

        return roles;
    }
}
