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

#include "SimpleVolumeBasedStochasticCellCycleModel.hpp"
#include "RandomNumberGenerator.hpp"
#include "Exception.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "WildTypeCellMutationState.hpp"
#include "ApcOneHitCellMutationState.hpp"
#include "CellLabel.hpp"
#include "Cell.hpp"
#include "SmartPointers.hpp"
#include <stdlib.h>
#include <time.h>

SimpleVolumeBasedStochasticCellCycleModel::SimpleVolumeBasedStochasticCellCycleModel()
    :mQuiescentVolumeFraction(0.812261*0.9)
{
    SetSDuration(1e-10);
    SetG2Duration(1e-10);
}

void SimpleVolumeBasedStochasticCellCycleModel::SetG1Duration()
{
    assert(mpCell != NULL);

    //RandomNumberGenerator* p_gen = RandomNumberGenerator::Instance();
    //mG1Duration = 9 + 4*p_gen->ranf();
    srand(time(NULL));
    unsigned randomN = rand() % 2 + 10;
    if(mpCell->GetCellProliferativeType()->IsType<StemCellProliferativeType>())
    {
        mG1Duration = randomN;
    }
    else if (mpCell->GetCellProliferativeType()->IsType<TransitCellProliferativeType>())
    {
        mG1Duration = randomN;
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

void SimpleVolumeBasedStochasticCellCycleModel::UpdateCellCyclePhase()
{
    double cell_area = mpCell->GetCellData()->GetItem("volume");

    mpCell->RemoveCellProperty<CellLabel>();

    MAKE_PTR(WildTypeCellMutationState, p_defaultState);
    mpCell->SetMutationState(p_defaultState);

    if (mCurrentCellCyclePhase == G_ONE_PHASE)
    {
        double dt = SimulationTime::Instance()->GetTimeStep();

        mQuiescentVolumeFraction = 0.812261*0.9;

        double equilibrium_area = 1.0;
        double quiescent_area = equilibrium_area * mQuiescentVolumeFraction;

        if(cell_area < quiescent_area)
        {
            mG1Duration += dt;

            MAKE_PTR(ApcOneHitCellMutationState, p_contInhibitedState);
            mpCell->SetMutationState(p_contInhibitedState);

            boost::shared_ptr<AbstractCellProperty> p_label =
                mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<CellLabel>();
            mpCell->AddCellProperty(p_label);
        }
    }

    double time_since_birth = GetAge();
    assert(time_since_birth >= 0);

    if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    {
        mCurrentCellCyclePhase = G_ZERO_PHASE;
    }
    else if (time_since_birth < GetMDuration())
    {
        mCurrentCellCyclePhase = M_PHASE;
    }
    else if (time_since_birth < GetMDuration() + mG1Duration)
    {
        mCurrentCellCyclePhase = G_ONE_PHASE;
    }
    else if ( time_since_birth < GetMDuration() + mG1Duration + GetSDuration())
    {
        mCurrentCellCyclePhase = S_PHASE;
    }
    else if ( time_since_birth < GetMDuration() + mG1Duration + GetSDuration() + GetG2Duration())
    {
        mCurrentCellCyclePhase = G_TWO_PHASE;
    }
}

AbstractCellCycleModel* SimpleVolumeBasedStochasticCellCycleModel::CreateCellCycleModel()
{
    SimpleVolumeBasedStochasticCellCycleModel* p_model = new SimpleVolumeBasedStochasticCellCycleModel();

    p_model->SetBirthTime(mBirthTime);
    p_model->SetDimension(mDimension);
    p_model->SetMinimumGapDuration(mMinimumGapDuration);
    p_model->SetStemCellG1Duration(mStemCellG1Duration);
    p_model->SetTransitCellG1Duration(mTransitCellG1Duration);
    p_model->SetSDuration(mSDuration);
    p_model->SetG2Duration(mG2Duration);
    p_model->SetMDuration(mMDuration);
    p_model->SetQuiescentVolumeFraction(mQuiescentVolumeFraction);

    return p_model;
}

void SimpleVolumeBasedStochasticCellCycleModel::SetQuiescentVolumeFraction(double quiescentVolumeFraction)
{
    mQuiescentVolumeFraction = quiescentVolumeFraction;
}

double SimpleVolumeBasedStochasticCellCycleModel::GetQuiescentVolumeFraction()
{
    return mQuiescentVolumeFraction;
}

#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(SimpleVolumeBasedStochasticCellCycleModel)