#include "NewStochasticAreaDependentCellCycleModel.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "StemCellProliferativeType.hpp"
#include <cstdlib>
#include <iostream>

NewStochasticAreaDependentCellCycleModel::NewStochasticAreaDependentCellCycleModel()
    : AbstractSimplePhaseBasedCellCycleModel(),
    _targetArea(0.9 * 0.812261)
{
}

NewStochasticAreaDependentCellCycleModel::NewStochasticAreaDependentCellCycleModel(const NewStochasticAreaDependentCellCycleModel& rModel)
    :AbstractSimplePhaseBasedCellCycleModel(rModel),
    _targetArea(rModel._targetArea)
{
}

AbstractCellCycleModel* NewStochasticAreaDependentCellCycleModel::CreateCellCycleModel()
{
    return new NewStochasticAreaDependentCellCycleModel(*this);
}

void NewStochasticAreaDependentCellCycleModel::SetG1Duration()
{
    if (mpCell->GetCellProliferativeType()->IsType<StemCellProliferativeType>()
        || mpCell->GetCellProliferativeType()->IsType<TransitCellProliferativeType>() )
    {
        //Sets G1 Duration to be a random value between 10 & 14
        mG1Duration = rand() % 4 + 3;
    }
    else if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    {
        mG1Duration = DBL_MAX;
    }
    else
    {
        NEVER_REACHED;
    }
}

void NewStochasticAreaDependentCellCycleModel::Set_targetArea(double area)
{
    _targetArea = area;
}

void NewStochasticAreaDependentCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<TargetArea>" << _targetArea << "</TargetArea>\n";

    AbstractSimplePhaseBasedCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

//Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(NewStochasticAreaDependentCellCycleModel)