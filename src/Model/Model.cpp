#include <QDebug>
#include "Model.hpp"

namespace NModel
{
    void CModel::addScreenshot(QPixmap screenshot)
    {
        qDebug() << "CModel::addScreenshot";

        mScreenshots.append(screenshot);
        qDebug() << "CModel::addScreenshot: size: " << mScreenshots.size();
    }
}
