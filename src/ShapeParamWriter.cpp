/*

Copyright (c) 2005-2015, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "ShapeParamWriter.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
/**
 * Constructor
 * 
 * Labels .vtk data output "Shape Parameter"
*/
ShapeParamWriter<ELEMENT_DIM, SPACE_DIM>::ShapeParamWriter()
    : AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>("shapeparam.dat")
{
        this->mVtkCellDataName = "Shape Parameter";
}

//template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
//double GetPerimeter<ELEMENT>

/**
 * Gets data for each cell bordering the wound in the given cell population. If the cell is found not to border the wound return zero. 
 * Otherwise, we return perimeter/sqrt(area) for the given cell.  
 * 
 * Cells are determined to be wound-border elements through a series of checks. 
 * 
 * The first check is that the given cell falls within a range of 3.75 units
 * from the center of the mesh, as we assume the wound will not grow during the simulation. This significantly decreased the performance impact of the computation
 * in a large mesh. Then, we begin iterating over the all nodes in the mesh. If the node is a boundary node [found via the built in IsBoundaryNode() method] and the
 * position falls within the 3.75 unit range, we create a set of elements containing this node. We compare this to a singleton set of the current node, and if the
 * current element is contained within the set of containing elements we return perimeter/sqrt(area) of the current cell. Otherwise, we repeat this process for all
 * nodes until we have analyzed all nodes within the radius and then repeat the process from the start for the next element in the mesh.
 * 
 * @param pCell input cell
 * @param pCellPopulation input cell population
*/
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

/**
 * Outputs the .dat file corresponding to this cell writer. " " delinates a new cell and cells who return 0 are ignored by our independent analysis scripts. 
 * 
 * @param pCell input cell
 * @param pCellPopulation input cell population
*/
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