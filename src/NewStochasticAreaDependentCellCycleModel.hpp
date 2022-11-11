#ifndef NEWSTOCHASTICAREADEPENDENTCELLCYCLEMODEL_HPP_
#define NEWSTOCHASTICAREADEPENDENTCELLCYCLEMODEL_HPP_

#include "AbstractSimplePhaseBasedCellCycleModel.hpp"
#include "RandomNumberGenerator.hpp"

class NewStochasticAreaDependentCellCycleModel : public AbstractSimplePhaseBasedCellCycleModel
{
    friend class TestSimpleCellCycleModels;

private:
    
    double _targetArea;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractSimplePhaseBasedCellCycleModel>(*this);

        //Ensures the RandomNumberGenerator singleton gets saved as well
        SerializableSingleton<RandomNumberGenerator>* p_wrapper = RandomNumberGenerator::Instance()->GetSerializationWrapper();
        archive & p_wrapper;

        archive & _targetArea;
    }

protected:

    NewStochasticAreaDependentCellCycleModel(const NewStochasticAreaDependentCellCycleModel& rModel);

public:

    NewStochasticAreaDependentCellCycleModel();

    void SetG1Duration();

    AbstractCellCycleModel* CreateCellCycleModel();

    void Set_targetArea(double target);

    double Get_targetArea() const;

    virtual void OutputCellCycleModelParameters(out_stream& rParmsFile);
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(NewStochasticAreaDependentCellCycleModel)

#endif /* NEWSTOCHASTICAREADEPENDENTCELLCYCLEMODEL_HPP_ */

