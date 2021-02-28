#ifndef MODEL_HPP
#define MODEL_HPP

#include <QList>
#include <QPixmap>

namespace NModel
{
    class CModel
    {
    public:
        void addScreenshot(QPixmap screenshot);

    private:
        QList<QPixmap> mScreenshots;
    };
}

#endif // MODEL_HPP
