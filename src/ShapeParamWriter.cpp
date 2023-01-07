#include "ShapeParamWriter.hpp"



template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
ShapeParamWriter<ELEMENT_DIM, SPACE_DIM>::ShapeParamWriter()
    : AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>("shapeparam.dat")
{
        this->mVtkCellDataName = "Shape Parameter";
}

//template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
//double GetPerimeter<ELEMENT>

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double ShapeParamWriter<ELEMENT_DIM, SPACE_DIM>::GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation){
    double shape_param = 0.0;
    double mesh_width = 15.0;
    double volume = pCellPopulation->GetVolumeOfCell(pCell);
    unsigned index = pCellPopulation->GetLocationIndexUsingCell(pCell);
    double perimeter = pCellPopulation->rGetMesh().GetSurfaceAreaOfElement(index);
    c_vector<double, 2> cell_center = pCellPopulation->GetLocationOfCellCentre(pCell);
    double x = cell_center[0] - (mesh_width/2.0);
    double y = cell_center[1] - (mesh_width/2.0);
    double vec = sqrt((pow(x,2)) + pow(y,2));
    double radius = 3.75;
    std::set<unsigned> neighbor_elements = pCellPopulation->rGetMesh().GetNeighbouringElementIndices(index);
    if(SPACE_DIM == 2){
        if (vec < radius){
            for (typename AbstractMesh<ELEMENT_DIM, SPACE_DIM>::NodeIterator node_iter = pCellPopulation->rGetMesh().GetNodeIteratorBegin();
                node_iter != pCellPopulation->rGetMesh().GetNodeIteratorEnd();
                ++node_iter)
            {
                if(node_iter->IsBoundaryNode()){
                    unsigned node_index = node_iter->GetIndex();
                    c_vector<double, SPACE_DIM> nPosition = node_iter->rGetLocation();
                    double nX = nPosition[0] - (mesh_width/2.0);
                    double nY = nPosition[1] - (mesh_width/2.0);
                    double nVec = sqrt((pow(nX, 2)) + pow(nY, 2));
                    if(nVec < radius){
                        std::set<unsigned> containing_elements = pCellPopulation->rGetMesh().GetNode(node_index)->rGetContainingElementIndices();
                        if(!containing_elements.size() == 0){
                            std::set<unsigned> current_element; current_element.insert(index);
                            std::set<unsigned> overlap;
                            std::set_intersection(
                                containing_elements.begin(),
                                containing_elements.end(),
                                current_element.begin(),
                                current_element.end(),
                                std::inserter(overlap, overlap.begin()));
                            if(overlap.size() != 0){
                                shape_param = (perimeter / std::sqrt(volume));
                                nVec = radius;
                            }
                        }
                    }
                }
            }
        }
    }
    return shape_param;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void ShapeParamWriter<ELEMENT_DIM, SPACE_DIM>::VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation){
    double shape = GetCellDataForVtkOutput(pCell, pCellPopulation);
    *this ->mpOutStream << shape << " ";
}

template class ShapeParamWriter<1,1>;
template class ShapeParamWriter<1,2>;
template class ShapeParamWriter<2,2>;
template class ShapeParamWriter<1,3>;
template class ShapeParamWriter<2,3>;
template class ShapeParamWriter<3,3>;

#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(ShapeParamWriter)