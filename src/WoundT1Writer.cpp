#include "WoundT1Writer.hpp"
#include "AbstractCellPopulation.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "CaBasedCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "PottsBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
WoundT1Writer<ELEMENT_DIM, SPACE_DIM>::WoundT1Writer()
    : AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM>("WoundT1Swaps.dat")
{
}
 
template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void WoundT1Writer<ELEMENT_DIM, SPACE_DIM>::Visit(MeshBasedCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void WoundT1Writer<ELEMENT_DIM, SPACE_DIM>::Visit(CaBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
}
 
template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void WoundT1Writer<ELEMENT_DIM, SPACE_DIM>::Visit(NodeBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
}
 
template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void WoundT1Writer<ELEMENT_DIM, SPACE_DIM>::Visit(PottsBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
}
 
template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void WoundT1Writer<ELEMENT_DIM, SPACE_DIM>::Visit(VertexBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    double mesh_width = 15.0;
    std::vector< c_vector<double, SPACE_DIM> > t1_swap_locations = pCellPopulation->rGetMesh().GetLocationsOfT1Swaps();
    int swaps = 0;
    for (unsigned index = 0;  index < t1_swap_locations.size(); index++)
    {
        double x = t1_swap_locations[index][0] - (mesh_width/2.0);
        double y = t1_swap_locations[index][1] - (mesh_width/2.0);
        double vec = sqrt((pow(x,2)) + pow(y,2));
        if(vec < 3.15){
            swaps++;
        }
    }
    *this->mpOutStream << swaps << "\t";
    pCellPopulation->rGetMesh().ClearLocationsOfT1Swaps();
}

// Explicit instantiation
template class WoundT1Writer<1,1>;
template class WoundT1Writer<1,2>;
template class WoundT1Writer<2,2>;
template class WoundT1Writer<1,3>;
template class WoundT1Writer<2,3>;
template class WoundT1Writer<3,3>;
 
#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(WoundT1Writer)