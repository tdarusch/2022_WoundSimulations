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

/**
* Unused method, used as a temporary stand-in prior to reaching out to A. G. Fletcher while attempting to recreate his 2013 simulations. 
*/
#include "NewStochasticAreaDependentCellCycleModel.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "StemCellProliferativeType.hpp"
#include <cstdlib>
#include <iostream>

/**
* Constructor A
*/
NewStochasticAreaDependentCellCycleModel::NewStochasticAreaDependentCellCycleModel()
    : AbstractSimplePhaseBasedCellCycleModel(),
    _targetArea(0.9 * 0.812261)
{
}

/**
* Constructor B
* 
* @param rModel input model
*/
NewStochasticAreaDependentCellCycleModel::NewStochasticAreaDependentCellCycleModel(const NewStochasticAreaDependentCellCycleModel& rModel)
    :AbstractSimplePhaseBasedCellCycleModel(rModel),
    _targetArea(rModel._targetArea)
{
}

/**
* Constructor C
*/
AbstractCellCycleModel* NewStochasticAreaDependentCellCycleModel::CreateCellCycleModel()
{
    return new NewStochasticAreaDependentCellCycleModel(*this);
}

/**
* Sets the G1 duration for a given cell to a stochastic unsigned integer [10,14] if the cell is either a transit or stem cell. If the cell is differentiated, 
* we set the duration to infinity.
* 
* @param rModel input model
*/
void NewStochasticAreaDependentCellCycleModel::SetG1Duration()
{
    if (mpCell->GetCellProliferativeType()->IsType<StemCellProliferativeType>()
        || mpCell->GetCellProliferativeType()->IsType<TransitCellProliferativeType>() )
    {
        //Sets G1 Duration to be a random value between 10 & 14
        mG1Duration = rand() % 4 + 3;
    }
    else if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    {
        mG1Duration = DBL_MAX;
    }
    else
    {
        NEVER_REACHED;
    }
}

/**
* Setter for target area
* 
* @param area input target area
*/
void NewStochasticAreaDependentCellCycleModel::Set_targetArea(double area)
{
    _targetArea = area;
}

/**
* Outputs parameters
* 
* @param rParamsFile input parameter file
*/
void NewStochasticAreaDependentCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<TargetArea>" << _targetArea << "</TargetArea>\n";

    AbstractSimplePhaseBasedCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

//Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(NewStochasticAreaDependentCellCycleModel)