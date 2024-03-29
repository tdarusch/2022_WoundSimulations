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

#include "CellAxisWriter.hpp"
#include "AbstractCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"
#include <stdexcept>

/**
* Gets data for each cell in a passed in cell population at every timestep. Here, our value of interest is perimeter/sqrt(area) for each cell.
* 
* Unused in final tests, but can be used to calculate shape parameter for ALL cells in a given population rather than just border cells.
*/
template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
/**
* Constructor
* 
* Names data output "Cell axes"
*/
CellAxisWriter<ELEMENT_DIM, SPACE_DIM>::CellAxisWriter()
    : AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>("cellaxes.dat")
{
        this->mVtkCellDataName = "Cell axes";
}

//template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
//double GetPerimeter<ELEMENT>

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
/**
* Gets data for each cell in a passed in cell population at every timestep. Here, our value of interest is perimeter/sqrt(area) for each cell.
* 
* Unused in final tests, but can be used to calculate shape parameter for ALL cells in a given population rather than just border cells.
* @param pCell input cell
* @param pCellPopulation input population
*/
double CellAxisWriter<ELEMENT_DIM, SPACE_DIM>::GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation){
    double axis = 0.0;
    double volume = pCellPopulation->GetVolumeOfCell(pCell);
    unsigned index = pCellPopulation->GetLocationIndexUsingCell(pCell);
    double perimeter = pCellPopulation->rGetMesh().GetSurfaceAreaOfElement(index);
    axis = (perimeter / std::sqrt(volume));
    return axis;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
/**
* Writes to data file using GetCellDataForVtkOutput to retrieve desired value
*
* @param pCell input cell
* @param pCellPopulation input population
*/
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