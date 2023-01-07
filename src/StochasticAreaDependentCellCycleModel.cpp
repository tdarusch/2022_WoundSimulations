/*Includes necessary header files
#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "SmartPointers.hpp"
#include "Exception.hpp"
#include "AbstractSimpleGenerationalCellCycleModel.hpp"
#include "AbstractCellCycleModel.hpp"

#include "DifferentiatedCellProliferativeType.hpp"
#include "StochasticAreaDependentCellCycleModel.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "WildTypeCellMutationState.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "FakePetscSetup.hpp"
#include "AbstractPhaseBasedCellCycleModel.hpp"
#include <math.h>

StochasticAreaDependentCellCycleModel::StochasticAreaDependentCellCycleModel()
     : AbstractSimplePhaseBasedCellCycleModel(),
       mMinG1Duration(10), // Passed to .hpp file, given time parameters from Fletcher paper
       mMaxG1Duration(14)  
{
}

StochasticAreaDependentCellCycleModel::StochasticAreaDependentCellCycleModel(const StochasticAreaDependentCellCycleModel& rModel)
    : AbstractSimplePhaseBasedCellCycleModel(rModel),
      mMinG1Duration(rModel.mMinG1Duration),
      mMaxG1Duration(rModel.mMaxG1Duration)
{
}

AbstractCellCycleModel* StochasticAreaDependentCellCycleModel::CreateCellCycleModel()
{
    return new StochasticAreaDependentCellCycleModel(*this);
}

void StochasticAreaDependentCellCycleModel::SetG1Duration()
{
    RandomNumberGenerator* p_gen = RandomNumberGenerator::Instance();

    if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    {
        mG1Duration = DBL_MAX;
    }
    else
    {
        //generates a random number [10,14] and passes it to the parent .hpp file,
        //this is done because the root class (AbstractSimpleCellCycleModel) has
        //mCellCycleDuration as a protected variable, meaning we cannot edit it in the private
        //or public namespace in a .cpp file
        mG1Duration = mMinG1Duration + (mMaxG1Duration - mMinG1Duration) * p_gen->ranf();
    }
}

void StochasticAreaDependentCellCycleModel::SetMinG1Duration(double MinG1)
{
    mMinG1Duration = MinG1;    
}

void StochasticAreaDependentCellCycleModel::SetMaxG1Duration(double MaxG1)
{
    mMaxG1Duration = MaxG1;
}

/*double StochasticAreaDependentCellCycleModel::GetMinG1Duration()
{
    return mMinG1Duration;
}

void StochasticAreaDependentCellCycleModel::SetMinCellCycleDuration(double minCellCycleDuration){
    mMinCellCycleDuration = minCellCycleDuration;
}
double StochasticAreaDependentCellCycleModel::GetMaxG1Duration()
{
    return mMaxG1Duration;
}

void StochasticAreaDependentCellCycleModel::SetMinCellCycleDuration(double minCellCycleDuration){
    mMinCellCycleDuration = minCellCycleDuration;
}

bool StochasticAreaDependentCellCycleModel::ReadyToDivide()
{
    //Checks to make sure passed cells exists
    assert(mpCell != NULL);
    
    //Gets the area of cell i through the GetCellData() method of the cell class,
    //Fletcher et al. defines area as length^2, so I used the C math library
    //to square the length of the cell associated with this model
    double cell_area = pow (mpCell->GetCellData()->GetItem("length"), 2);
    //double tempTime = (GetCellCycleDuration() + timestep);
    if(!mReadyToDivide){
        //Checks if the current area is less than the desired target area
        //If true, we increment the value of Tcycle by dt by means of 
        //subtracting dt from the birthtime of the cell, since the cell-cycle
        //variable is a const value and cannot be altered once defined above
        if(cell_area < _target_area){
            SetBirthTime(GetBirthTime()-_timestep);
        }
        else if((cell_area > _target_area) && (GetAge() >= mCellCycleDuration))
        {
            mReadyToDivide = true;
        }
        else
        {
            return mReadyToDivide;
        }
    }
    return mReadyToDivide;
}

double StochasticAreaDependentCellCycleModel::GetAverageTransitCellCycleTime()
{
    return 0.5*(mMinCellCycleDuration + mMaxCellCycleDuration);
}
 
double StochasticAreaDependentCellCycleModel::GetAverageStemCellCycleTime()
{
    return 0.5*(mMinCellCycleDuration + mMaxCellCycleDuration);
}

void StochasticAreaDependentCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<MinG1Duration>" << mMinG1Duration << "</MinG1Duration>\n";
    *rParamsFile << "\t\t\t<MaxG1Duration>" << mMaxG1Duration << "</MaxG1Duration>\n";

    AbstractSimplePhaseBasedCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

//Serialization for Boost
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(StochasticAreaDependentCellCycleModel);*/