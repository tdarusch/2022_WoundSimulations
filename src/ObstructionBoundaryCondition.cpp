#include "ObstructionBoundaryCondition.hpp"
#include "VertexBasedCellPopulation.hpp"

template <unsigned DIM>
ObstructionBoundaryCondition<DIM>::ObstructionBoundaryCondition(AbstractCellPopulation<DIM>* pCellPopulation)
    : AbstractCellPopulationBoundaryCondition<DIM>(pCellPopulation)
{
    // This class is implemented only for 2D vertex-based cell populations
    assert(dynamic_cast<VertexBasedCellPopulation<DIM>*>(this->mpCellPopulation));
    assert(DIM == 2);
}

template<unsigned DIM>
void ObstructionBoundaryCondition<DIM>::ImposeBoundaryCondition(const std::map<Node<DIM>*, c_vector<double, DIM> >& rOldLocations)
{
    VertexBasedCellPopulation<DIM>* mpStaticCastCellPopulation = static_cast<VertexBasedCellPopulation<DIM>*>(this->mpCellPopulation);

    // Iterate over all nodes and update their positions according to the boundary conditions
    unsigned num_nodes = mpStaticCastCellPopulation->GetNumNodes();
    for (unsigned node_index=0; node_index<num_nodes; node_index++)
    {
        Node<DIM>* p_node = mpStaticCastCellPopulation->GetNode(node_index);
        c_vector<double, DIM> node_location = p_node->rGetLocation();

        double obsruction_radius = 3.0;

        c_vector<double, 2> obstruction_centre;
        obstruction_centre[0] = 6.0;
        obstruction_centre[1] = 5.0;
        double radius = norm_2(node_location-obstruction_centre);
        if (radius < obsruction_radius)
        {
            p_node->rGetModifiableLocation() = obstruction_centre + obsruction_radius/radius*(node_location-obstruction_centre);
        }
    }
}

template<unsigned DIM>
bool ObstructionBoundaryCondition<DIM>::VerifyBoundaryCondition()
{
    return true;
}

template<unsigned DIM>
void ObstructionBoundaryCondition<DIM>::OutputCellPopulationBoundaryConditionParameters(out_stream& rParamsFile)
{
    // No new parameters to output, so call method on direct parent class
    AbstractCellPopulationBoundaryCondition<DIM>::OutputCellPopulationBoundaryConditionParameters(rParamsFile);
}

// Explicit instantiation
template class ObstructionBoundaryCondition<1>;
template class ObstructionBoundaryCondition<2>;
template class ObstructionBoundaryCondition<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(ObstructionBoundaryCondition)