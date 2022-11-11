#ifndef MPHASEGROWTHTARGETAREAMODIFIER_HPP_
#define MPHASEGROWTHTARGETAREAMODIFIER_HPP_

#include "ChasteSerialization.hpp"
#include "AbstractTargetAreaModifier.hpp"

template<unsigned DIM>
class MPhaseGrowthTargetAreaModifier : public AbstractTargetAreaModifier<DIM>
{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractTargetAreaModifier<DIM> >(*this);
    }

public:

    MPhaseGrowthTargetAreaModifier();

    virtual ~MPhaseGrowthTargetAreaModifier();

    virtual void UpdateTargetAreaOfCell(const CellPtr pCell);

    void OutputSimulationModifierParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(MPhaseGrowthTargetAreaModifier)

#endif /*MPHASEGROWTHTARGETAREAMODIFIER_HPP_*/