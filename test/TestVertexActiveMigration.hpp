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

/**********************************************
 * THIS CODE WORKS WITH RELEASE 3.3 OF CHASTE *
 **********************************************/

#ifndef TESTVERTEXACTIVEMIGRATION_HPP_
#define TESTVERTEXACTIVEMIGRATION_HPP_

#include <cxxtest/TestSuite.h>

// Must be included before other cell_based headers
#include "CellBasedSimulationArchiver.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "SmartPointers.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "CellsGenerator.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "OffLatticeSimulation.hpp"
#include "NagaiHondaForce.hpp"
#include "SimpleTargetAreaModifier.hpp"
#include "ModifiedWelikyOsterForce.hpp"
#include "VertexAngleForce.hpp"
#include "PlaneBoundaryCondition.hpp"
#include "CellLabel.hpp"
#include "MotileCellForce.hpp"
#include "CellAgesWriter.hpp"
#include "CellAncestorWriter.hpp"
#include "CellIdWriter.hpp"
#include "CellMutationStatesWriter.hpp"
#include "CellProliferativePhasesWriter.hpp"
#include "CellProliferativeTypesWriter.hpp"
#include "CellVolumesWriter.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "CellLabelWriter.hpp"

class TestVertexActiveMigration: public AbstractCellBasedTestSuite
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
     * Helper method. Label all cells in a given disc as motile.
     * 
     * @param rCellPopulation a cell population
     * @param radius radius of the disc
     * @param xCoordinate x coordinate of the centre of the disc
     * @param yCoordinate y coordinate of the centre of the disc
     */
    void LabelMotileRegionInCellPopulation(AbstractCellPopulation<2>& rCellPopulation,
            double radius,
            double xCoordinate,
            double yCoordinate)
    {   
        for (AbstractCellPopulation<2>::Iterator cell_iter = rCellPopulation.Begin();
             cell_iter != rCellPopulation.End();
             ++cell_iter)
        {
            c_vector<double, 2> centre_of_cell = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
            double x = centre_of_cell[0];
            double y = centre_of_cell[1];
            if ((x-xCoordinate)*(x-xCoordinate) + (y-yCoordinate)*(y-yCoordinate) < radius*radius)
            {
                /*
                 * This method is usually called within a CellBasedSimulation, after the CellPopulation
                 * has called CellPropertyRegistry::TakeOwnership(). This means that were we to call
                 * CellPropertyRegistry::Instance() here when adding the CellLabel, we would be creating
                 * a new CellPropertyRegistry. In this case the CellLabel's cell count would be incorrect.
                 * We must therefore access the CellLabel via the cell's CellPropertyCollection.
                 */
                boost::shared_ptr<AbstractCellProperty> p_label =
                    cell_iter->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<CellLabel>();
                cell_iter->AddCellProperty(p_label);
            }
        }
    }
    
public:

    void TestNHVertexMonolayerWithActiveMigration() throw (Exception)
    {
        // Create a simple 2D MutableVertexMesh
        HoneycombVertexMeshGenerator generator(24, 14);
        MutableVertexMesh<2,2>* p_mesh = generator.GetMesh();
        p_mesh->Scale(0.8,0.8);
        p_mesh->SetCellRearrangementThreshold(0.1);

        // Set up cells, one for each VertexElement; specify cells as differentiated to prevent cell division
        std::vector<CellPtr> cells;
        MAKE_PTR(DifferentiatedCellProliferativeType, p_diff_type);
        CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumElements(), p_diff_type);

        // Create pointer to a labelled cell state
        boost::shared_ptr<AbstractCellProperty> p_label(CellPropertyRegistry::Instance()->Get<CellLabel>());

        // Create cell population
        VertexBasedCellPopulation<2> population(*p_mesh, cells);

        // Specify what to output from the simulation
        population.AddCellWriter<CellAgesWriter>();
        population.AddCellWriter<CellAncestorWriter>();
        population.AddCellWriter<CellIdWriter>();
        population.AddCellWriter<CellMutationStatesWriter>();
        population.AddCellWriter<CellProliferativePhasesWriter>();
        population.AddCellWriter<CellProliferativeTypesWriter>();
        population.AddCellWriter<CellVolumesWriter>();
        population.AddCellWriter<CellLabelWriter>();

        // Now label cells in a given region as motile using a helper method
        LabelMotileRegionInCellPopulation(population, 0.4472, 1.5, 5.0);

        // Set up cell-based simulation
        OffLatticeSimulation<2> simulator(population);
        simulator.SetOutputDirectory("TestVertexCellMoving");
        simulator.SetDt(0.01);
        simulator.SetEndTime(30.0);
        simulator.SetSamplingTimestepMultiple(100);

        // Create a force law and pass it to the simulation
        MAKE_PTR(NagaiHondaForce<2>, p_force);
        p_force->SetNagaiHondaDeformationEnergyParameter(55.0);
        p_force->SetNagaiHondaMembraneSurfaceEnergyParameter(0.0);
        p_force->SetNagaiHondaCellCellAdhesionEnergyParameter(5.0);
        p_force->SetNagaiHondaCellBoundaryAdhesionEnergyParameter(10.0);
        simulator.AddForce(p_force);

        MAKE_PTR(SimpleTargetAreaModifier<2>, p_growth_modifier);
        simulator.AddSimulationModifier(p_growth_modifier);

        double box_height = 10.0;
        double box_width = 20.0;

        // Create boundary condition y > 0
        c_vector<double, 2> point = zero_vector<double>(2);
        c_vector<double, 2> normal = zero_vector<double>(2);
        normal(1) = -1.0;
        MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_boundary_condition_1, (&population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_boundary_condition_1);

        // Create boundary condition y < box_height
        point(1) = box_height;
        normal(1) = 1.0;
        MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_boundary_condition_2, (&population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_boundary_condition_2);

        // Create boundary condition x > 0
        point(1) = 0.0;
        normal(0) = -1.0;
        normal(1) = 0.0;
        MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_boundary_condition_3, (&population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_boundary_condition_3);

        // Create boundary condition x < box_width
        point(0) = box_width;
        normal(0) = 1.0;
        MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_boundary_condition_4, (&population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_boundary_condition_4);
        
        // Smooth out edges to get nice box domain, then run simulation
        SmoothVertexMeshEdges(population);
        simulator.Solve();

        // Now flag a cell as motile by adding a special force object to the simulation
        MAKE_PTR(MotileCellForce<2>, p_motile_cell_force);
        simulator.AddForce(p_motile_cell_force);

        // Reset end time for simulation and run for a further duration
        simulator.SetEndTime(40.0);
        simulator.SetSamplingTimestepMultiple(10);
        simulator.Solve();
    }
};

#endif /*TESTVERTEXACTIVEMIGRATION_HPP_*/
