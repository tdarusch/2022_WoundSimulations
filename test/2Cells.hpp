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

#ifndef TESTNAGAIHONDAMONOLAYER_HPP_
#define TESTNAGAIHONDAMONOLAYER_HPP_

#include <cxxtest/TestSuite.h>

#include <fstream>
#include <iostream>

#include "CellAxisWriter.hpp"
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "HoneycombVertexMeshGenerator.hpp"
#include "MutableVertexMesh.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "OffLatticeSimulation.hpp"
#include "VolumeTrackingModifier.hpp"
#include "NagaiHondaForce.hpp"

#include "FixedG1GenerationalCellCycleModel.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "CellMutationStatesCountWriter.hpp"
#include "UniformCellCycleModel.hpp"
#include "WildTypeCellMutationState.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "OutputFileHandler.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "SimpleTargetAreaModifier.hpp"
#include "SimulationTime.hpp"
#include "CellLabel.hpp"
#include "MutableMesh.hpp"
#include "PlaneBoundaryCondition.hpp"
#include "SmartPointers.hpp"
#include "CellsGenerator.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "CylindricalHoneycombVertexMeshGenerator.hpp"
#include "PlaneBasedCellKiller.hpp"
#include "ContactInhibitionCellCycleModel.hpp"
#include "GammaG1CellCycleModel.hpp"
#include "SimpleVolumeBasedStochasticCellCycleModel.hpp"
#include "DifferentiatedCellProliferativeType.hpp"

#include "CellAgesWriter.hpp"
#include "CellAncestorWriter.hpp"
#include "CellIdWriter.hpp"
#include "CellMutationStatesWriter.hpp"
#include "CellProliferativePhasesWriter.hpp"
#include "CellProliferativeTypesWriter.hpp"
#include "CellVolumesWriter.hpp"
#include "MPhaseGrowthTargetAreaModifier.hpp"
#include "CellLabelWriter.hpp"
#include "ApcOneHitCellMutationState.hpp"
#include "AbstractCellMutationState.hpp"
#include "ShapeParamWriter.hpp"

#include "VoronoiDataWriter.hpp"
#include <stdexcept>

class TestNagaiHondaMonolayer : public AbstractCellBasedTestSuite
{
private:

	double mLastStartTime;
    void setUp()
    {
        mLastStartTime = std::clock();
        AbstractCellBasedTestSuite::setUp();
    }
    void tearDown()
    {
        double time = std::clock();
        double elapsed_time = (time - mLastStartTime)/(CLOCKS_PER_SEC);
        std::cout << "Elapsed time: " << elapsed_time << std::endl;
        AbstractCellBasedTestSuite::tearDown();
    }

	/**
     * Method for turning off division after mesh growth is complete. Iterates over a given cell population and uses
     * SetCellProliferativeType() to assign a differentiated proliferative type to all cells in the passed in population. 
     * 
     * @param rCellPopulation given cell population
     */
    void preventDivision(AbstractCellPopulation<2>& rCellPopulation){
        MAKE_PTR(DifferentiatedCellProliferativeType, p_diff);
        for(AbstractCellPopulation<2>::Iterator cell_iter = rCellPopulation.Begin();
            cell_iter != rCellPopulation.End();
            ++cell_iter){
                cell_iter->SetCellProliferativeType(p_diff);
            }
    }

	/**
     * From A. G. Fletcher et al.
     * 
     * Helper Method. Assigns the Stochastic Area-Dependent Cell Cycle Model to the initial cells in a new population.
     * Takes in a std::vector<CellPtr> of cells, the number of cells, and the contact-dependent coefficient to assign to cells.
     * 
     * @param rCells A std::vector<CellPtr>& of cells
     * @param numCells an int >0 of the number of cells passed in
     * @param quiescentVolumeFraction a double corresponding to the contact-dependent coefficient
     */
	void SetUpCellsWithStochasticAreaDependentCellCycleModel(std::vector<CellPtr>& rCells, unsigned numCells, double quiescentVolumeFraction)
	{
		rCells.clear();
		rCells.reserve(numCells);

		MAKE_PTR(WildTypeCellMutationState, p_state);
		MAKE_PTR(TransitCellProliferativeType, p_transit_type);
		for (unsigned i=0; i<numCells; i++)
		{
			SimpleVolumeBasedStochasticCellCycleModel* p_cell_cycle_model = new SimpleVolumeBasedStochasticCellCycleModel;
			p_cell_cycle_model->SetDimension(2);
			p_cell_cycle_model->SetQuiescentVolumeFraction(quiescentVolumeFraction);
			p_cell_cycle_model->SetSDuration(1e-10);
			p_cell_cycle_model->SetG2Duration(1e-10);

			CellPtr p_cell(new Cell(p_state, p_cell_cycle_model));
			p_cell->SetCellProliferativeType(p_transit_type);

			double birth_time = -p_cell_cycle_model->GetAverageTransitCellCycleTime()*RandomNumberGenerator::Instance()->ranf();
			p_cell->SetBirthTime(birth_time);

			rCells.push_back(p_cell);
		}
	}

public:
	/**
     * From A. G. Fletcher et al with modifications.
     * 
     * Main method, responsible for initializing mesh and running simulation. This is an isolated test of the affect of increasing/decreasing the intercellular
	 * contact adhesion coefficient on a mesh constrainted to two cells who cannot divide. Our tested values in this simulation were Yc-c = 3.5 & 6.5, in accordance
	 * with the range of trials we conducted where wound healing was analyzed.
	 * 
	 * To run this simulation, we initialize a mesh of two cells using Fletcher's SetUpCellsWithStochastic[...]() method to assign the contact-dependent cell model
	 * to the cells, although this does not impact the simulation as division is prevented. However, to preserve the same conditions present in our wound-healing simulations
	 * we opted to initialize the simulation in an identical fashion. PreventDivision() differentiates the two cells so they cannot divide after creation, and this
	 * simulation proceeds for 2h with dT = .001h and a 1:1 sampling multiple. 
	 * */
	void TestNagaiHondaMonolayerClass() throw (Exception)
	{
		try {
			HoneycombVertexMeshGenerator mesh_generator(2, 1);
			MutableVertexMesh<2,2>* p_mesh = mesh_generator.GetMesh();
			p_mesh->SetCellRearrangementThreshold(0.1);
            p_mesh->Scale(0.75, 0.75);
			
			std::vector<CellPtr> cells;
			SetUpCellsWithStochasticAreaDependentCellCycleModel(cells, 2, 0.812261*0.9);
			
			VertexBasedCellPopulation<2> population(*p_mesh, cells);

			population.AddCellWriter<CellAgesWriter>();
			population.AddCellWriter<CellAncestorWriter>();
			population.AddCellWriter<CellIdWriter>();
			population.AddCellWriter<CellMutationStatesWriter>();
			population.AddCellWriter<CellProliferativePhasesWriter>();
			population.AddCellWriter<CellProliferativeTypesWriter>();
			population.AddCellWriter<CellVolumesWriter>();
			population.AddCellWriter<CellLabelWriter>();
            //population.AddCellWriter<CellAxisWriter>();
            

			OffLatticeSimulation<2> simulator(population);
			simulator.SetOutputDirectory("FinalOutput/2CellsLow");
			simulator.SetDt(.001);
			simulator.SetEndTime(2);
			simulator.SetSamplingTimestepMultiple(1);
			
			MAKE_PTR(VolumeTrackingModifier<2>, p_modifier);
			simulator.AddSimulationModifier(p_modifier);
			
			MAKE_PTR(MPhaseGrowthTargetAreaModifier<2>, p_growth_modifier);
			simulator.AddSimulationModifier(p_growth_modifier);

			MAKE_PTR(NagaiHondaForce<2>, p_force);
			p_force->SetNagaiHondaDeformationEnergyParameter(55);
			p_force->SetNagaiHondaMembraneSurfaceEnergyParameter(0.0);
			p_force->SetNagaiHondaCellCellAdhesionEnergyParameter(3.0);
			p_force->SetNagaiHondaCellBoundaryAdhesionEnergyParameter(10);
			simulator.AddForce(p_force);

            preventDivision(population);
			simulator.Solve();


		}
		catch ( ... )
		{
			throw;
		}
	}
};

#endif /*TESTNAGAIHONDAMONOLAYER_HPP_*/