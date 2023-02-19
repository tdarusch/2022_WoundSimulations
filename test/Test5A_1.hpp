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

/**
 * Helper Class. Acts as a new mutation state to assign to cells on the border of the wound in
 * LabelCellsAroundWound()
*/
class BorderCellMutationState : public AbstractCellMutationState
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellMutationState>(*this);
    }

public:
    BorderCellMutationState()
        : AbstractCellMutationState(4)
    {
    }
};

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
     * From A. G. Fletcher et al.
     * 
     * Helper method. Smooth out edges of a vertex mesh.
     * 
     * @param rCellPopulation a cell population
     */
    void SmoothVertexMeshEdges(AbstractCellPopulation<2>& rCellPopulation)
    {
        MutableVertexMesh<2, 2>& r_mesh = static_cast<VertexBasedCellPopulation<2>* >(&rCellPopulation)->rGetMesh();

        for (VertexMesh<2,2>::NodeIterator node_iter = r_mesh.GetNodeIteratorBegin();
             node_iter != r_mesh.GetNodeIteratorEnd();
             ++node_iter)
        {
            unsigned node_index = node_iter->GetIndex();
            std::set<unsigned> containing_element_indices = node_iter->rGetContainingElementIndices();
            if (containing_element_indices.size() == 1)
            {
                // Get this element
                unsigned elem_index = (*containing_element_indices.begin());

                VertexElement<2,2>* p_element = r_mesh.GetElement(elem_index);

                // Remove node from this element and delete the node
                p_element->DeleteNode(p_element->GetNodeLocalIndex(node_index));
                r_mesh.DeleteNodePriorToReMesh(node_index);
            }
        }
        r_mesh.ReMesh();
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
     * Method for creating a circular wound in a given mesh. Takes in a vertex-based cell population and an integer radius
     * that corresponds to the desired wound size. Removal is accomplished by normalizing the axes to center at (0,0), finding the vector
     * position of cells relative to this center, and removing all cells where sqrt(x_pos^2 + y_pos^2) < radius,
     * 
     * @param rCellPopulation given cell population
     * @param radius desired radius
     */
    void CreateCircularHoleInPopulation(AbstractCellPopulation<2>& rCellPopulation,
    double radius){
        for(AbstractCellPopulation<2>::Iterator cell_iter = rCellPopulation.Begin();
            cell_iter != rCellPopulation.End();
            ++cell_iter){
                c_vector<double, 2> cell_center = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
                double x = cell_center[0] - 7.5;
                double y = cell_center[1] - 7.5;
                double vec = sqrt((pow(x,2)) + pow(y,2));
                if(vec < radius){
                    cell_iter->Kill();
                }
            }
        dynamic_cast<VertexBasedCellPopulation<2>* >(&rCellPopulation)->RemoveDeadCells();
        dynamic_cast<VertexBasedCellPopulation<2>* >(&rCellPopulation)->Update();
    }

    /**
     * Rudimentary method; sets a mutation state for cells bordering the wound area as a function of area.
     * 
     * @param rCellPopulation given cell population
     */
    void LabelCellsAroundWound(AbstractCellPopulation<2>& rCellPopulation){
        MAKE_PTR(BorderCellMutationState, p_state);
        for(AbstractCellPopulation<2>::Iterator cell_iter = rCellPopulation.Begin(); cell_iter != rCellPopulation.End(); ++cell_iter){
            c_vector<double, 2> cell_center = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
                double x = cell_center[0] - 7.5;
                double y = cell_center[1] - 7.5;
                double vec = sqrt((pow(x,2)) + pow(y,2));
                if((vec < 3.80)){
                    {
                        cell_iter->SetMutationState(p_state);
                    }
                }
            }
        }

    /**
     * From A. G. Fletcher et al.
     * 
     * Helper method. Iterate over all cells and define the 'hole' by
     * killing those cells whose centres are located in a given region.
     * 
     * @param rCellPopulation a cell population
     * @param holeWidth the width of the hole
     * @param xMin the left boundary of the hole
     * @param xMax the right boundary of the hole
     * @param yMin the bottom boundary of the hole
     * @param yMax the top boundary of the hole
     */
    void CreateHoleInCellPopulation(AbstractCellPopulation<2>& rCellPopulation,
                                    double holeWidth,
                                    double xMin,
                                    double xMax,
                                    double yMin,
                                    double yMax)
    {
        for (AbstractCellPopulation<2>::Iterator cell_iter = rCellPopulation.Begin();
             cell_iter != rCellPopulation.End();
             ++cell_iter)
        {
            // Get the coordinates of this cell centre
            c_vector<double, 2> centre_of_cell = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
            double x = centre_of_cell[0];
            double y = centre_of_cell[1];

            if ((fabs(y-x)<holeWidth) && (x>xMin) && (x<xMax) && (y>yMin) && (y<yMax))
            {
                cell_iter->Kill();
            }
        }

        /* Need to remove the deleted cells and call update note this is usually
         * performed in the Solve() method of the simulation class.
         */
        dynamic_cast<VertexBasedCellPopulation<2>* >(&rCellPopulation)->RemoveDeadCells();
        dynamic_cast<VertexBasedCellPopulation<2>* >(&rCellPopulation)->Update();

        
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
     * Main method, responsible for initializing mesh and subsequent wound. The process for running this simulation is broken into two distinct segments: Mesh Growth and Wound Healing.
     * 
     * Mesh Growth: Using the force model developed by Nagai & Honda, the Stochastic Area-Dependent Cell Cycle Model from Fletcher et al., grow a mesh of size
     * 15 x 15, beginning with a single parent cell. Here, the parameters for the force model are unchanged from Fletcher et al.'s simulations aside from the
     * intercellular contact adhesion coefficient. This is variable and corresponds to the name of the .hpp file.
     * 
     * Wound Healing: Again using the Nagai-Honda force model and some helper methods from Fletcher et al. alongside our own helper methods, we remove a circular
     * section of cells in the center of the mesh. Moreover, we disable cell division and decrease the dT, sampling multiple, and endTime.
     * 
     * Outside helper classes such as ShapeParamWriter and the modified T1SwapWriter class are used in addition to built-in Chaste Cell/Population writers to
     * collect data. These data are then exported and analyzed via a combination of external python scripts and visualized using MATLAB.
     */
	void TestNagaiHondaMonolayerClass() throw (Exception)
	{
		try {
			HoneycombVertexMeshGenerator mesh_generator(1, 1);
			MutableVertexMesh<2,2>* p_mesh = mesh_generator.GetMesh();
			p_mesh->SetCellRearrangementThreshold(0.1);
            p_mesh->Scale(0.75, 0.75);
			
			std::vector<CellPtr> cells;
			SetUpCellsWithStochasticAreaDependentCellCycleModel(cells, 1, 0.812261*0.9);
			
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
			simulator.SetOutputDirectory("NewDT_Output/5.0A");
			simulator.SetDt(.001);
			simulator.SetEndTime(115);
			simulator.SetSamplingTimestepMultiple(1000);
			
			MAKE_PTR(VolumeTrackingModifier<2>, p_modifier);
			simulator.AddSimulationModifier(p_modifier);
			
			MAKE_PTR(MPhaseGrowthTargetAreaModifier<2>, p_growth_modifier);
			simulator.AddSimulationModifier(p_growth_modifier);

			MAKE_PTR(NagaiHondaForce<2>, p_force);
			p_force->SetNagaiHondaDeformationEnergyParameter(55);
			p_force->SetNagaiHondaMembraneSurfaceEnergyParameter(0.0);
			p_force->SetNagaiHondaCellCellAdhesionEnergyParameter(5);
			p_force->SetNagaiHondaCellBoundaryAdhesionEnergyParameter(10);
			simulator.AddForce(p_force);

            // Create boundary condition y > 0
            c_vector<double, 2> point1 = zero_vector<double>(2);
            c_vector<double, 2> normal1 = zero_vector<double>(2);
            normal1(1) = -1.0;
            MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_condition1, (&population, point1, normal1));

            // Create boundary condition x > 0
            c_vector<double, 2> point2 = zero_vector<double>(2);
            c_vector<double, 2> normal2 = zero_vector<double>(2);
            normal2(0) = -1.0;
            MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_condition2, (&population, point2, normal2));

            // Create boundary condition y < 10
            c_vector<double, 2> point3 = zero_vector<double>(2);
            point3(1) = 15.0;
            normal1(1) = 1.0;
            MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_condition3, (&population, point3, normal1));

            // Create boundary condition x < 10
            c_vector<double, 2> point4 = zero_vector<double>(2);
            point4(0) = 15.0;
            normal2(0) = 1.0;
            MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_condition4, (&population, point4, normal2));
            
            simulator.AddCellPopulationBoundaryCondition(p_condition1);
            simulator.AddCellPopulationBoundaryCondition(p_condition2);
            simulator.AddCellPopulationBoundaryCondition(p_condition3);
            simulator.AddCellPopulationBoundaryCondition(p_condition4);

			simulator.Solve();

            // Pass the boundary conditions to the simulation
            CreateCircularHoleInPopulation(population, 3.1);
            preventDivision(population);
            SmoothVertexMeshEdges(population);
            LabelCellsAroundWound(population);

            // Reset timestep, sampling timestep and end time for simulation and run for a further duration
            simulator.SetDt(0.0005);
            simulator.SetSamplingTimestepMultiple(35);
            simulator.SetEndTime(121);

            population.AddCellWriter<ShapeParamWriter>();

            simulator.Solve();
		}
		catch ( ... )
		{
			throw;
		}
	}
};

#endif /*TESTNAGAIHONDAMONOLAYER_HPP_*/