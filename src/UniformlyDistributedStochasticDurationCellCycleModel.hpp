#ifndef UNIFORMLYDISTRIBUTEDSTOCHASTICDURATIONCELLCYCLEMODEL_HPP_
#define UNIFORMLYDISTRIBUTEDSTOCHASTICDURATIONCELLCYCLEMODEL_HPP_

#include "AbstractSimplePhaseBasedCellCycleModel.hpp"
#include "RandomNumberGenerator.hpp"

/**
 * A stochastic cell-cyclemodel where cells divide with a stochastic G1 phase duration
 * 
 * If the cell has DifferentiatedCellProliferativeType, then the G1 phase duration is
 * set to be infinite, so that the cell will never divide.
 */

class UniformlyDistributedStochasticDurationCellCycleModel : public AbstractSimplePhaseBasedCellCycleModel{
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & archive, const unsigned int version){
        archive & boost::serialization::base_object<AbstractSimplePhaseBasedCellCycleModel>(*this);

        SerializableSingleton<RandomNumberGenerator>* p_wrapper = RandomNumberGenerator::Instance()->GetSerializationWrapper();
        archive & p_wrapper;
    }
protected:
    UniformlyDistributedStochasticDurationCellCycleModel(const UniformlyDistributedStochasticDurationCellCycleModel& model);
public:
    UniformlyDistributedStochasticDurationCellCycleModel();
    void SetG1Duration();

    /**
     * @return new cell-cycle model
     */
    AbstractCellCycleModel* CreateCellCycleModel();

    /**
     * @param rParamsFile the file stream to which the parameters are output
     */
    virtual void OutputCellCycleModelParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(UniformlyDistributedStochasticDurationCellCycleModel)

#endif /*UNIFORMLYDISTRIBUTEDSTOCHASTICDURATIONCELLCYCLEMODEL_HPP_*/