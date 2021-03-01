#include <QDebug>
#include <QBuffer>
#include "Model.hpp"

namespace NModel
{
    CModel::CModel(QObject *parent)
        : QAbstractListModel(parent)
    {
        qDebug() << QThread::currentThreadId() << "CModel::CModel";

        connect(&mScreen, &NScreen::CScreenThread::screenshotReady,
                this, &CModel::addScreenshot);
    }

    void CModel::increase()
    {
        qDebug() << QThread::currentThreadId() << "CModel::makeScreenshot";

        mScreen.makeScreenshot();
    }


    int CModel::rowCount(const QModelIndex& p) const
    {
        Q_UNUSED(p)
        qDebug() << QThread::currentThreadId() << "CModel::rowCount";

        return mScreenshots.size();
    }

    QVariant CModel::data(const QModelIndex &index, int role) const
    {
        qDebug() << QThread::currentThreadId() << "CModel::data";

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
        qDebug() << QThread::currentThreadId() << "CModel::roleNames";

        QHash<int, QByteArray> roles;
        roles[ImageData] = "imageData";
        roles[ImageEquality] = "imageEquality";

        return roles;
    }

    void CModel::addScreenshot(NScreen::Screenshot screenshot)
    {
        qDebug() << QThread::currentThreadId() << "CModel::addScreenshot";

        beginInsertRows(QModelIndex(), 0, 0);
        mScreenshots.insert(0, screenshot);
        endInsertRows();
    }
}
