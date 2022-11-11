#include "VertexDiffusionForce.hpp"

template<unsigned DIM>
VertexDiffusionForce<DIM>::VertexDiffusionForce(double diffusionConstant)
    : AbstractForce<DIM>(),
    mDiffusionConstant(diffusionConstant)
{
}

template<unsigned DIM>
VertexDiffusionForce<DIM>::~VertexDiffusionForce()
{
}

template<unsigned DIM>
void VertexDiffusionForce<DIM>::SetDiffusionConstant(double diffusionConstant){
    mDiffusionConstant = diffusionConstant;
}

template<unsigned DIM>
void VertexDiffusionForce<DIM>::AddForceContribution(AbstractCellPopulation<DIM>& rCellPopulation){
    if (dynamic_cast<VertexBasedCellPopulation<DIM>*>(&rCellPopulation) == NULL){
            EXCEPTION("VertexDiffusionForce is to be used with a VertexBasedCellPopulation only");
    }

    VertexBasedCellPopulation<DIM>* p_cell_population = static_cast<VertexBasedCellPopulation<DIM>*>(&rCellPopulation);

    double dt = SimulationTime::Instance()->GetTimeStep();

    for(unsigned node_index=0; node_index<p_cell_population->GetNumNodes(); node_index++){
        c_vector<double, DIM> force_contribution;
        for(unsigned i = 0; i < DIM; i++){
            double nu = p_cell_population->GetDampingConstant(node_index);
            double xi = RandomNumberGenerator::Instance()->StandardNormalRandomDeviate();
            force_contribution[i] = (nu*sqrt(2.0*mDiffusionConstant/dt)*xi);
        }
        rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force_contribution);
    }
}

template<unsigned DIM>
void VertexDiffusionForce<DIM>::OutputForceParameters(out_stream& rParamsFile){
    *rParamsFile << "\t\t\t<DiffusionConstant>" << mDiffusionConstant << "</DiffusionConstant> \n";
    AbstractForce<DIM>::OutputForceParameters(rParamsFile);
}

template class VertexDiffusionForce<1>;
template class VertexDiffusionForce<2>;
template class VertexDiffusionForce<3>;

#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(VertexDiffusionForce)