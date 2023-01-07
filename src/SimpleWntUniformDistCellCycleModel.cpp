#include "SimpleWntUniformDistCellCycleModel.hpp"
#include "Exception.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"

#include "CellLabel.hpp"
#include "Cell.hpp" 

SimpleWntUniformDistCellCycleModel::SimpleWntUniformDistCellCycleModel(){}

AbstractCellCycleModel* SimpleWntUniformDistCellCycleModel::CreateCellCycleModel()
{
    // Create a new cell-cycle model
    SimpleWntUniformDistCellCycleModel* p_model = new SimpleWntUniformDistCellCycleModel();

    /*
     * Set each member variable of the new cell-cycle model that inherits
     * its value from the parent.
     *
     * Note 1: some of the new cell-cycle model's member variables (namely
     * mBirthTime, mCurrentCellCyclePhase, mReadyToDivide) will already have been
     * correctly initialized in its constructor.
     *
     * Note 2: one or more of the new cell-cycle model's member variables
     * may be set/overwritten as soon as InitialiseDaughterCell() is called on
     * the new cell-cycle model.
     */
    p_model->SetBirthTime(mBirthTime);
    p_model->SetDimension(mDimension);
    p_model->SetMinimumGapDuration(mMinimumGapDuration);
    p_model->SetStemCellG1Duration(mStemCellG1Duration);
    p_model->SetTransitCellG1Duration(mTransitCellG1Duration);
    p_model->SetSDuration(mSDuration);
    p_model->SetG2Duration(mG2Duration);
    p_model->SetMDuration(mMDuration);
    p_model->SetUseCellProliferativeTypeDependentG1Duration(mUseCellProliferativeTypeDependentG1Duration);
    p_model->SetWntStemThreshold(mWntStemThreshold);
    p_model->SetWntTransitThreshold(mWntTransitThreshold);
    p_model->SetWntLabelledThreshold(mWntLabelledThreshold);

    return p_model;
}
	
void SimpleWntUniformDistCellCycleModel::SetUseCellProliferativeTypeDependentG1Duration(bool useCellProliferativeTypeDependentG1Duration)
{
    mUseCellProliferativeTypeDependentG1Duration = useCellProliferativeTypeDependentG1Duration;
}

void SimpleWntUniformDistCellCycleModel::SetG1Duration()
{
    assert(mpCell != NULL);

    RandomNumberGenerator* p_gen = RandomNumberGenerator::Instance();

    if (mpCell->GetCellProliferativeType()->IsType<StemCellProliferativeType>())
    {
        if (mUseCellProliferativeTypeDependentG1Duration)
         {
             mG1Duration = GetStemCellG1Duration() + 4*p_gen->ranf(); //  U[0,4] so U[10,14] overall with default params
         }
         else
         {
             // Normally stem cells should behave just like transit cells in a Wnt simulation
             mG1Duration = GetTransitCellG1Duration() + 4*p_gen->ranf(); // U[0,4] so U[10,14] overall with default params
         }
    }
    else if (mpCell->GetCellProliferativeType()->IsType<TransitCellProliferativeType>())
    {
        mG1Duration = GetTransitCellG1Duration() + 4*p_gen->ranf(); //  U[0,4] so U[10,14] overall with default params
    }
    else if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    {
        mG1Duration = DBL_MAX;
    }
    else
    {
        NEVER_REACHED;
    }

    // Check that the normal random deviate has not returned a small or negative G1 duration
    if (mG1Duration < mMinimumGapDuration)
    {
        mG1Duration = mMinimumGapDuration;
    }
}

	
double SimpleWntUniformDistCellCycleModel::GetWntLevel()
{
    assert(mpCell != NULL);
    double level = 0;
	
    switch (mDimension)
    {
        case 1:
        {
            const unsigned DIM = 1;
            level = WntConcentration<DIM>::Instance()->GetWntLevel(mpCell);
            break;
        }
        case 2:
        {
            const unsigned DIM = 2;
            level = WntConcentration<DIM>::Instance()->GetWntLevel(mpCell);
            break;
        }
        case 3:
        {
            const unsigned DIM = 3;
            level = WntConcentration<DIM>::Instance()->GetWntLevel(mpCell);
            break;
        }
        default:
            NEVER_REACHED;
    }
    return level;
}


WntConcentrationType SimpleWntUniformDistCellCycleModel::GetWntType()
{
    WntConcentrationType wnt_type;
    switch (mDimension)
    {
        case 1:
        {
            const unsigned DIM = 1;
            wnt_type = WntConcentration<DIM>::Instance()->GetType();
            break;
        }
        case 2:
        {
            const unsigned DIM = 2;
            wnt_type = WntConcentration<DIM>::Instance()->GetType();
            break;
        }
        case 3:
        {
            const unsigned DIM = 3;
            wnt_type = WntConcentration<DIM>::Instance()->GetType();
            break;
        }
        default:
            NEVER_REACHED;
    }
    return wnt_type;
}

	
void SimpleWntUniformDistCellCycleModel::UpdateCellCyclePhase()
{
    // The cell can divide if the Wnt concentration >= wnt_division_threshold
    double wnt_division_threshold = DBL_MAX;

    // Set up under what level of Wnt stimulus a cell will divide
    if (mpCell->GetMutationState()->IsType<WildTypeCellMutationState>())
    {
        wnt_division_threshold = mWntTransitThreshold;
    }
    else if (mpCell->GetMutationState()->IsType<ApcOneHitCellMutationState>())
    {
        // should be less than healthy values
        wnt_division_threshold = 0.77*mWntTransitThreshold;
    }
    else if (mpCell->GetMutationState()->IsType<BetaCateninOneHitCellMutationState>())
    {
        // less than above value
        wnt_division_threshold = 0.155*mWntTransitThreshold;
    }
    else if (mpCell->GetMutationState()->IsType<ApcTwoHitCellMutationState>())
    {
        // Should be zero (no Wnt-dependence)
        wnt_division_threshold = 0.0;
    }
    else
    {
        NEVER_REACHED;
    }

    if (mpCell->HasCellProperty<CellLabel>())
    {
        wnt_division_threshold = mWntLabelledThreshold;
    }

    double wnt_level = GetWntLevel();
    WntConcentrationType wnt_type = GetWntType();

    // Set the cell type to transit if the Wnt stimulus exceeds wnt_division_threshold
    if (wnt_level >= wnt_division_threshold)
    {
        /*
         * This method is usually called within a CellBasedSimulation, after the CellPopulation
         * has called CellPropertyRegistry::TakeOwnership(). This means that were we to call
         * CellPropertyRegistry::Instance() here when setting the CellProliferativeType, we
         * would be creating a new CellPropertyRegistry. In this case the cell proliferative
         * type counts, as returned by AbstractCellPopulation::GetCellProliferativeTypeCount(),
         * would be incorrect. We must therefore access the CellProliferativeType via the cell's
         * CellPropertyCollection.
         */
        boost::shared_ptr<AbstractCellProperty> p_transit_type =
            mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<TransitCellProliferativeType>();
        mpCell->SetCellProliferativeType(p_transit_type);

        // For a RADIAL Wnt type, override the cell type to stem if the Wnt stimulus exceeds a higher threshold
        if ((wnt_type == RADIAL) && (wnt_level > mWntStemThreshold))
        {
            boost::shared_ptr<AbstractCellProperty> p_stem_type =
                mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<StemCellProliferativeType>();
            mpCell->SetCellProliferativeType(p_stem_type);
        }
    }
    else
    {
        // The cell is differentiated and so in G0 phase
        boost::shared_ptr<AbstractCellProperty> p_diff_type =
            mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<DifferentiatedCellProliferativeType>();
        mpCell->SetCellProliferativeType(p_diff_type);
    }
    AbstractSimplePhaseBasedCellCycleModel::UpdateCellCyclePhase();
}
	
void SimpleWntUniformDistCellCycleModel::InitialiseDaughterCell()
{
    WntConcentrationType wnt_type = GetWntType();

    if (wnt_type == RADIAL)
    {
        boost::shared_ptr<AbstractCellProperty> p_transit_type =
            mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<TransitCellProliferativeType>();
        mpCell->SetCellProliferativeType(p_transit_type);
    }

    AbstractSimplePhaseBasedCellCycleModel::InitialiseDaughterCell();
}

bool SimpleWntUniformDistCellCycleModel::CanCellTerminallyDifferentiate()
{
    return false;
}
	
double SimpleWntUniformDistCellCycleModel::GetWntStemThreshold()
{
    return mWntStemThreshold;
}

void SimpleWntUniformDistCellCycleModel::SetWntStemThreshold(double wntStemThreshold)
{
    assert(wntStemThreshold <= 1.0);
    assert(wntStemThreshold >= 0.0);
    mWntStemThreshold = wntStemThreshold;
}
	
double SimpleWntUniformDistCellCycleModel::GetWntTransitThreshold()
{
    return mWntTransitThreshold;
}

void SimpleWntUniformDistCellCycleModel::SetWntTransitThreshold(double wntTransitThreshold)
{
    //assert(wntTransitThreshold <= 1.0);
    //assert(wntTransitThreshold >= 0.0);
    mWntTransitThreshold = wntTransitThreshold;
}

double SimpleWntUniformDistCellCycleModel::GetWntLabelledThreshold()
{
    return mWntLabelledThreshold;
}

void SimpleWntUniformDistCellCycleModel::SetWntLabelledThreshold(double wntLabelledThreshold)
{
//    assert(wntLabelledThreshold <= 1.0);
//    assert(wntLabelledThreshold >= 0.0);
    mWntLabelledThreshold = wntLabelledThreshold;
}

void SimpleWntUniformDistCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<UseCellProliferativeTypeDependentG1Duration>" << mUseCellProliferativeTypeDependentG1Duration << "</UseCellProliferativeTypeDependentG1Duration>\n";
    *rParamsFile << "\t\t\t<WntStemThreshold>" << mWntStemThreshold << "</WntStemThreshold>\n";
    *rParamsFile << "\t\t\t<WntTransitThreshold>" << mWntTransitThreshold << "</WntTransitThreshold>\n";
    *rParamsFile << "\t\t\t<WntLabelledThreshold>" << mWntLabelledThreshold << "</WntLabelledThreshold>\n";

    // Call method on direct parent class
    AbstractSimplePhaseBasedCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(SimpleWntUniformDistCellCycleModel)