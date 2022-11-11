/*
2	
3	Copyright (c) 2005-2015, University of Oxford.
4	All rights reserved.
5	
6	University of Oxford means the Chancellor, Masters and Scholars of the
7	University of Oxford, having an administrative office at Wellington
8	Square, Oxford OX1 2JD, UK.
9	
10	This file is part of Chaste.
11	
12	Redistribution and use in source and binary forms, with or without
13	modification, are permitted provided that the following conditions are met:
14	 * Redistributions of source code must retain the above copyright notice,
15	   this list of conditions and the following disclaimer.
16	 * Redistributions in binary form must reproduce the above copyright notice,
17	   this list of conditions and the following disclaimer in the documentation
18	   and/or other materials provided with the distribution.
19	 * Neither the name of the University of Oxford nor the names of its
20	   contributors may be used to endorse or promote products derived from this
21	   software without specific prior written permission.
22	
23	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
24	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
25	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
26	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
27	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
28	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
29	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
30	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
31	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
32	OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
33	
34	*/
	
/**********************************************
* THIS CODE WORKS WITH RELEASE 3.3 OF CHASTE *
**********************************************/
	
#ifndef SIMPLEVOLUMEBASEDSTOCHASTICCELLCYCLEMODEL_HPP_
#define SIMPLEVOLUMEBASEDSTOCHASTICCELLCYCLEMODEL_HPP_
	
#include "AbstractSimplePhaseBasedCellCycleModel.hpp"
	

class SimpleVolumeBasedStochasticCellCycleModel : public AbstractSimplePhaseBasedCellCycleModel
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractSimplePhaseBasedCellCycleModel>(*this);
        archive & mQuiescentVolumeFraction;
    }
	
    double mQuiescentVolumeFraction;

    void SetG1Duration();

public:
    SimpleVolumeBasedStochasticCellCycleModel();

    void UpdateCellCyclePhase();

    AbstractCellCycleModel* CreateCellCycleModel();

    void SetQuiescentVolumeFraction(double quiescentVolumeFraction);

    double GetQuiescentVolumeFraction();
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(SimpleVolumeBasedStochasticCellCycleModel)

#endif /*SIMPLEVOLUMEBASEDSTOCHASTICCELLCYCLEMODEL_HPP_*/