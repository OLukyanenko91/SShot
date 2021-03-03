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
        load();
    }

    void CModel::increase()
    {
        qDebug() << QThread::currentThreadId() << "CModel::increase";

        mScreen.makeScreenshot();
    }

    void CModel::save()
    {
        qDebug() << QThread::currentThreadId() << "CModel::save";

        mStorage.saveData(mScreenshots);
    }

    void CModel::load()
    {
        qDebug() << QThread::currentThreadId() << "CModel::load";

        mStorage.loadData(mScreenshots);

        if (!mScreenshots.empty())
        {
            for (auto& val : mScreenshots)
            {
                beginInsertRows(QModelIndex(), 0, 0);
                endInsertRows();
            }

            mScreen.setLastScreenPixmap(mScreenshots.first().getPixmap());
        }
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
            return QVariant(mScreenshots[index.row()].toStringData());
        else if (role == ImageEquality)
            return QVariant(QString::number(mScreenshots[index.row()].getEquality()) + "%");
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

    void CModel::addScreenshot(NScreen::CScreenshot screenshot)
    {
        qDebug() << QThread::currentThreadId() << "CModel::addScreenshot";

        beginInsertRows(QModelIndex(), 0, 0);
        mScreenshots.insert(0, screenshot);
        endInsertRows();
    }
}
