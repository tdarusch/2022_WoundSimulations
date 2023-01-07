#include "UniformlyDistributedStochasticDurationCellCycleModel.hpp"
#include "Exception.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"

UniformlyDistributedStochasticDurationCellCycleModel::UniformlyDistributedStochasticDurationCellCycleModel()
    : AbstractSimplePhaseBasedCellCycleModel(){}

UniformlyDistributedStochasticDurationCellCycleModel::UniformlyDistributedStochasticDurationCellCycleModel(const UniformlyDistributedStochasticDurationCellCycleModel& rModel)
    : AbstractSimplePhaseBasedCellCycleModel(rModel){}

AbstractCellCycleModel* UniformlyDistributedStochasticDurationCellCycleModel::CreateCellCycleModel(){
    return new UniformlyDistributedStochasticDurationCellCycleModel(*this);
}

void UniformlyDistributedStochasticDurationCellCycleModel::SetG1Duration(){
    RandomNumberGenerator* p_gen = RandomNumberGenerator::Instance();

    if (mpCell->GetCellProliferativeType()->IsType<StemCellProliferativeType>()){
        mG1Duration = GetStemCellG1Duration() + 4*p_gen->ranf(); //[0,4]
    }
    else if (mpCell->GetCellProliferativeType()->IsType<TransitCellProliferativeType>()){
        mG1Duration = GetTransitCellG1Duration() + 4*p_gen->ranf(); //[0,4]
    }
    else if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>()){
        mG1Duration = DBL_MAX;
    }
    else {
        NEVER_REACHED;
    }
}

void UniformlyDistributedStochasticDurationCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile){
    AbstractSimplePhaseBasedCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(UniformlyDistributedStochasticDurationCellCycleModel)