#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "src/Differ/Differ.hpp"
#include "src/Model/Model.hpp"
#include "src/Storage/Storage.hpp"

namespace NController
{
    class CController
    {
    public:

    private:
        NDiffer::CDiffer mDiffer;
        NModel::CModel mModel;
        NStorage::CStorage mStorage;
    };
}

#endif // CONTROLLER_HPP
