#include "MPhaseGrowthTargetAreaModifier.hpp"
#include "ApoptoticCellProperty.hpp"

template<unsigned DIM>
MPhaseGrowthTargetAreaModifier<DIM>::MPhaseGrowthTargetAreaModifier()
    :AbstractTargetAreaModifier<DIM>()
{
}

template<unsigned DIM>
MPhaseGrowthTargetAreaModifier<DIM>::~MPhaseGrowthTargetAreaModifier()
{
}

template<unsigned DIM>
void MPhaseGrowthTargetAreaModifier<DIM>::UpdateTargetAreaOfCell(CellPtr pCell)
{
    double cell_target_area = this->mReferenceTargetArea;

    double m_duration = 1;

    if(pCell->HasCellProperty<ApoptoticCellProperty>())
    {
        if (pCell->GetStartOfApoptosisTime() - pCell->GetBirthTime() < m_duration)
        {
            cell_target_area *= 0.5*(1 + (pCell->GetStartOfApoptosisTime() - pCell->GetBirthTime())/m_duration);
        }

        cell_target_area = cell_target_area - 0.5*cell_target_area/(pCell->GetApoptosisTime())*(SimulationTime::Instance()->GetTime()-pCell->GetStartOfApoptosisTime());

        if (cell_target_area < 0)
        {
            cell_target_area = 0;
        }
    }
    else
    {
        double cell_age = pCell->GetAge();

        if(cell_age < m_duration)
        {
            cell_target_area *= 0.5*(1 + cell_age/m_duration);
        }
    }

    pCell->GetCellData()->SetItem("target area", cell_target_area);
}

template<unsigned DIM>
void MPhaseGrowthTargetAreaModifier<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    AbstractTargetAreaModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

template class MPhaseGrowthTargetAreaModifier<1>;
template class MPhaseGrowthTargetAreaModifier<2>;
template class MPhaseGrowthTargetAreaModifier<3>;

#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(MPhaseGrowthTargetAreaModifier)