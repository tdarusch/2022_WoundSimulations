#ifndef TESTVERTEXCELLSORTING_HPP_
#define TESTVERTEXCELLSORTING_HPP_

#include <cxxtest/TestSuite.h>

// Must be included before other cell_based headers
#include "CellBasedSimulationArchiver.hpp"
#include "SmartPointers.hpp"
#include "UniformlyDistributedStochasticDurationCellCycleModel.hpp"

#include "OffLatticeSimulation.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "CellsGenerator.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "NagaiHondaDifferentialAdhesionForce.hpp"
#include "SimpleTargetAreaModifier.hpp"
#include "VertexDiffusionForce.hpp"
#include "CellLabel.hpp"
#include "CellAgesWriter.hpp"
#include "CellAncestorWriter.hpp"
#include "CellIdWriter.hpp"
#include "CellMutationStatesWriter.hpp"
#include "CellProliferativePhasesWriter.hpp"
#include "CellProliferativeTypesWriter.hpp"
#include "CellVolumesWriter.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "SimpleVolumeBasedStochasticCellCycleModel.hpp"
#include "VolumeTrackingModifier.hpp"
#include "CellLabelWriter.hpp"

class TestVertexCellSorting: public AbstractCellBasedTestSuite
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

    void RandomlyLabelCells(std::vector<CellPtr>& rCells, boost::shared_ptr<AbstractCellProperty> pLabel, double labelledRatio)
    {
        for (unsigned i=0; i<rCells.size(); i++)
        {
            if (RandomNumberGenerator::Instance()->ranf() < labelledRatio)
            {
                rCells[i]->AddCellProperty(pLabel);
            }
        }
    }

public:
	
    /**
     * Simulate a population of cells exhibiting cell sorting using the
     * vertex dynamics model proposed by T. Nagai and H. Honda ("A dynamic
     * cell model for the formation of epithelial tissues", Philosophical
     * Magazine Part B 81:699-719).
     *
     * Each of the vertex dynamics model parameter member variables are
     * rescaled such that mDampingConstantNormal takes the default value 1,
     * whereas Nagai and Honda (who denote the parameter by nu) take the
     * value 0.01.
     */
   void TestVertexMonolayerCellSorting() throw (Exception)
    {
        // Create a simple 2D MutableVertexMesh
        HoneycombVertexMeshGenerator generator(10, 10);
        MutableVertexMesh<2,2>* p_mesh = generator.GetMesh();
        p_mesh->SetCellRearrangementThreshold(0.1);

        // Set up cells, one for each VertexElement
        std::vector<CellPtr> cells;
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        CellsGenerator<UniformlyDistributedStochasticDurationCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells,p_mesh->GetNumElements(), p_transit_type);
        // Randomly label some cells
        boost::shared_ptr<AbstractCellProperty> p_state(CellPropertyRegistry::Instance()->Get<CellLabel>());
        RandomlyLabelCells(cells, p_state, 0.5);
	
        // Create cell population
        VertexBasedCellPopulation<2> population(*p_mesh, cells);

        // Specify what to output from the simulation
        population.AddCellWriter<CellAgesWriter>();
        population.AddCellWriter<CellAncestorWriter>();
        population.AddCellWriter<CellIdWriter>();
        population.AddCellWriter<CellMutationStatesWriter>();
        population.AddCellWriter<CellProliferativePhasesWriter>();
        population.AddCellWriter<CellProliferativeTypesWriter>();
        population.AddCellWriter<CellLabelWriter>();
        population.AddCellWriter<CellVolumesWriter>();
	
        // Set up cell-based simulation and output directory
        OffLatticeSimulation<2> simulator(population);
        simulator.SetOutputDirectory("TestVertexCellSorting2");
	
        // Set time step and end time for simulation
        simulator.SetDt(0.001);
        simulator.SetEndTime(70.0);

        // Only record results every 100 time steps
        simulator.SetSamplingTimestepMultiple(1000);
	
        // Set up force law and pass it to the simulation
        MAKE_PTR(NagaiHondaDifferentialAdhesionForce<2>, p_force);
        p_force->SetNagaiHondaDeformationEnergyParameter(55.0);
        p_force->SetNagaiHondaMembraneSurfaceEnergyParameter(0.0);
        p_force->SetNagaiHondaCellCellAdhesionEnergyParameter(1.0);
        p_force->SetNagaiHondaLabelledCellCellAdhesionEnergyParameter(6.0);
        p_force->SetNagaiHondaLabelledCellLabelledCellAdhesionEnergyParameter(3.0);
        p_force->SetNagaiHondaCellBoundaryAdhesionEnergyParameter(12.0);
        p_force->SetNagaiHondaLabelledCellBoundaryAdhesionEnergyParameter(40.0);
        simulator.AddForce(p_force);
	
        MAKE_PTR(VolumeTrackingModifier<2>, p_modifier);
		simulator.AddSimulationModifier(p_modifier);

        MAKE_PTR(SimpleTargetAreaModifier<2>, p_growth_modifier);
        simulator.AddSimulationModifier(p_growth_modifier);
	
        //Add some random motion to vertices
        MAKE_PTR_ARGS(VertexDiffusionForce<2>, p_random_force, (0.02));
        simulator.AddForce(p_random_force);

        // Run simulation
        simulator.Solve();
   }
};
#endif /*TESTVERTEXCELLSORTING_HPP_*/