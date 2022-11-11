#ifndef STOCHASTICAREADEPENDENTCELLCYCLEMODEL_HPP_
#define STOCHASTICAREADEPENDENTCELLCYCLEMODEL_HPP_

//Includes necessary header files
#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "SmartPointers.hpp"
#include "Exception.hpp"
#include "AbstractSimpleGenerationalCellCycleModel.hpp"
#include "AbstractCellCycleModel.hpp"
#include "AbstractSimpleCellCycleModel.hpp"

#include "HoneycombMeshGenerator.hpp"
#include "WildTypeCellMutationState.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "FakePetscSetup.hpp"
#include "RandomNumberGenerator.hpp"
#include "AbstractSimplePhaseBasedCellCycleModel.hpp"

#include <math.h>

class StochasticAreaDependentCellCycleModel : public AbstractSimplePhaseBasedCellCycleModel
{
    friend class TestSimpleCellCycleModels;
private:
    double mMinG1Duration;
    double mMaxG1Duration;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractSimplePhaseBasedCellCycleModel>(*this);

        // Make sure the RandomNumberGenerator singleton gets saved too
        SerializableSingleton<RandomNumberGenerator>* p_wrapper = RandomNumberGenerator::Instance()->GetSerializationWrapper();
        archive & p_wrapper;

        archive & mMinG1Duration;
        archive & mMaxG1Duration;
    }
protected:
    StochasticAreaDependentCellCycleModel(const StochasticAreaDependentCellCycleModel& rModel);
public: 
    StochasticAreaDependentCellCycleModel();
    void SetG1Duration();
    void SetMinG1Duration(double MinG1Duration);
    void SetMaxG1Duration(double MaxG1Duration);
    double GetMinG1Duration() const;
    double GetMaxG1Duration() const;
    AbstractCellCycleModel* CreateCellCycleModel();
    virtual void OutputCellCycleModelParameters(out_stream& rParamsFile);
};
    /*friend class TestSimpleCellCycleModels;

private:

    double mMinCellCycleDuration;
 
    double mMaxCellCycleDuration;

    double _target_area;

    double _timestep;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractSimpleCellCycleModel>(*this);

        //Ensures the RandomNumberGenerator singleton number gets saved as well
        SerializableSingleton<RandomNumberGenerator>* p_wrapper = RandomNumberGenerator::Instance()->GetSerializationWrapper();
        archive & p_wrapper;
        archive & mMinCellCycleDuration;
        archive & mMaxCellCycleDuration;
    }

protected:
    StochasticAreaDependentCellCycleModel(const StochasticAreaDependentCellCycleModel& rModel);

public:
    StochasticAreaDependentCellCycleModel();

    void SetCellCycleDuration();

    void GetCellCycleDuration();

    AbstractCellCycleModel* CreateCellCycleModel();

    double GetMinCellCycleDuration();
 
    void SetMinCellCycleDuration(double minCellCycleDuration);
 
    double GetMaxCellCycleDuration();
 
    void SetMaxCellCycleDuration(double maxCellCycleDuration);

    double Get_timestep();

    void Set_timestep(double _timestep);

    double Get_target_area();

    void Set_target_area(double _target_area);

    bool ReadyToDivide() override
    {
        //Checks to make sure passed cells exists
        assert(mpCell != NULL);
        
        //Gets the area of cell i through the GetCellData() method of the cell class,
        //Fletcher et al. defines area as length^2, so I used the C math library
        //to square the length of the cell associated with this model
        double cell_area = pow (mpCell->GetCellData()->GetItem("volume"), 2);
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

    double GetAverageTransitCellCycleTime();

    double GetAverageStemCellCycleTime();

    virtual void OutputCellCycleModelParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
//Declares identifier for serializer
CHASTE_CLASS_EXPORT(StochasticAreaDependentCellCycleModel)*/

#endif /*STOCHASTICAREADEPENDENTCELLCYCLEMODEL_HPP_*/