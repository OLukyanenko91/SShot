#include <QDebug>
#include <QBuffer>
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

        NScreen::Screenshot screenshot = mScreen.grab();

        beginInsertRows(QModelIndex(), 0, 0);
        mScreenshots.insert(0, screenshot);
        endInsertRows();
    }


    int CModel::rowCount(const QModelIndex& p) const
    {
        Q_UNUSED(p)
        qDebug() << "CModel::rowCount: size: " << mScreenshots.size();

        return mScreenshots.size();
    }

    QVariant CModel::data(const QModelIndex &index, int role) const
    {
        qDebug() << "CModel::data";

        if (!index.isValid())
            return QVariant();

        if (role == ImageData)
        {
            QByteArray bArray;
            QBuffer buffer(&bArray);
            buffer.open(QIODevice::WriteOnly);
            mScreenshots[index.row()].pixmap.save(&buffer, "JPEG");

            return QVariant("data:image/jpg;base64," +
                            QString::fromLatin1(bArray.toBase64().data()));
        }
        else if (role == ImageEquality)
            return QVariant(QString::number(mScreenshots[index.row()].equality) + "%");
        else
            return QVariant();
    }

    QHash<int, QByteArray> CModel::roleNames() const
    {
        qDebug() << "CModel::roleNames";

        QHash<int, QByteArray> roles;
        roles[ImageData] = "imageData";
        roles[ImageEquality] = "imageEquality";

        return roles;
    }
}
