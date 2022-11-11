#include "SimpleVolumeBasedStochasticCellCycleModel.hpp"
#include "RandomNumberGenerator.hpp"
#include "Exception.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "WildTypeCellMutationState.hpp"
#include "ApcOneHitCellMutationState.hpp"
#include "CellLabel.hpp"
#include "Cell.hpp"
#include "SmartPointers.hpp"

SimpleVolumeBasedStochasticCellCycleModel::SimpleVolumeBasedStochasticCellCycleModel()
    :mQuiescentVolumeFraction(0.812261*0.9)
{
    SetSDuration(1e-10);
    SetG2Duration(1e-10);
}

void SimpleVolumeBasedStochasticCellCycleModel::SetG1Duration()
{
    assert(mpCell != NULL);

    RandomNumberGenerator* p_gen = RandomNumberGenerator::Instance();
    if(mpCell->GetCellProliferativeType()->IsType<StemCellProliferativeType>())
    {
        mG1Duration = 9+4*p_gen->ranf();
    }
    else if (mpCell->GetCellProliferativeType()->IsType<TransitCellProliferativeType>())
    {
        mG1Duration = 9+4*p_gen->ranf();
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

void SimpleVolumeBasedStochasticCellCycleModel::UpdateCellCyclePhase()
{
    double cell_area = mpCell->GetCellData()->GetItem("volume");

    mpCell->RemoveCellProperty<CellLabel>();

    MAKE_PTR(WildTypeCellMutationState, p_defaultState);
    mpCell->SetMutationState(p_defaultState);

    if (mCurrentCellCyclePhase == G_ONE_PHASE)
    {
        double dt = SimulationTime::Instance()->GetTimeStep();

        mQuiescentVolumeFraction = 0.812261*0.9;

        double equilibrium_area = 1.0;
        double quiescent_area = equilibrium_area * mQuiescentVolumeFraction;

        if(cell_area < quiescent_area)
        {
            mG1Duration += dt;

            MAKE_PTR(ApcOneHitCellMutationState, p_contInhibitedState);
            mpCell->SetMutationState(p_contInhibitedState);

            boost::shared_ptr<AbstractCellProperty> p_label =
                mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<CellLabel>();
            mpCell->AddCellProperty(p_label);
        }
    }

    double time_since_birth = GetAge();
    assert(time_since_birth >= 0);

    if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    {
        mCurrentCellCyclePhase = G_ZERO_PHASE;
    }
    else if (time_since_birth < GetMDuration())
    {
        mCurrentCellCyclePhase = M_PHASE;
    }
    else if (time_since_birth < GetMDuration() + mG1Duration)
    {
        mCurrentCellCyclePhase = G_ONE_PHASE;
    }
    else if ( time_since_birth < GetMDuration() + mG1Duration + GetSDuration())
    {
        mCurrentCellCyclePhase = S_PHASE;
    }
    else if ( time_since_birth < GetMDuration() + mG1Duration + GetSDuration() + GetG2Duration())
    {
        mCurrentCellCyclePhase = G_TWO_PHASE;
    }
}

AbstractCellCycleModel* SimpleVolumeBasedStochasticCellCycleModel::CreateCellCycleModel()
{
    SimpleVolumeBasedStochasticCellCycleModel* p_model = new SimpleVolumeBasedStochasticCellCycleModel();

    p_model->SetBirthTime(mBirthTime);
    p_model->SetDimension(mDimension);
    p_model->SetMinimumGapDuration(mMinimumGapDuration);
    p_model->SetStemCellG1Duration(mStemCellG1Duration);
    p_model->SetTransitCellG1Duration(mTransitCellG1Duration);
    p_model->SetSDuration(mSDuration);
    p_model->SetG2Duration(mG2Duration);
    p_model->SetMDuration(mMDuration);
    p_model->SetQuiescentVolumeFraction(mQuiescentVolumeFraction);

    return p_model;
}

void SimpleVolumeBasedStochasticCellCycleModel::SetQuiescentVolumeFraction(double quiescentVolumeFraction)
{
    mQuiescentVolumeFraction = quiescentVolumeFraction;
}

double SimpleVolumeBasedStochasticCellCycleModel::GetQuiescentVolumeFraction()
{
    return mQuiescentVolumeFraction;
}

#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(SimpleVolumeBasedStochasticCellCycleModel)