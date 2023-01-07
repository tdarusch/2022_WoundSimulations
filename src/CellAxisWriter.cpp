#include "CellAxisWriter.hpp"
#include "AbstractCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"
#include <stdexcept>


template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
CellAxisWriter<ELEMENT_DIM, SPACE_DIM>::CellAxisWriter()
    : AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>("cellaxes.dat")
{
        this->mVtkCellDataName = "Cell axes";
}

//template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
//double GetPerimeter<ELEMENT>

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellAxisWriter<ELEMENT_DIM, SPACE_DIM>::GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation){
    double axis = 0.0;
    double volume = pCellPopulation->GetVolumeOfCell(pCell);
    unsigned index = pCellPopulation->GetLocationIndexUsingCell(pCell);
    double perimeter = pCellPopulation->rGetMesh().GetSurfaceAreaOfElement(index);
    axis = (perimeter / std::sqrt(volume));
    return axis;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellAxisWriter<ELEMENT_DIM, SPACE_DIM>::VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation){
    unsigned location_index = pCellPopulation->GetLocationIndexUsingCell(pCell);
    unsigned cell_id = pCell->GetCellId();
    c_vector<double, SPACE_DIM> centre_location = pCellPopulation->GetLocationOfCellCentre(pCell);
    double axis = GetCellDataForVtkOutput(pCell, pCellPopulation);
    *this->mpOutStream << location_index << " " << cell_id << " ";
    for(unsigned i = 0; i < SPACE_DIM; i++){
        *this->mpOutStream << centre_location[i] << " ";
    }
    *this ->mpOutStream << axis << " ";
}

template class CellAxisWriter<1,1>;
template class CellAxisWriter<1,2>;
template class CellAxisWriter<2,2>;
template class CellAxisWriter<1,3>;
template class CellAxisWriter<2,3>;
template class CellAxisWriter<3,3>;

#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellAxisWriter)