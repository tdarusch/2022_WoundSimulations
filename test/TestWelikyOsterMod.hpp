#ifndef TESTWELIKYOSTERMOD_HPP_
#define TESTWELIKYOSTERMOD_HPP_

#include <cxxtest/TestSuite.h>

#include "AbstractCellBasedTestSuite.hpp"

#include "VolumeTrackingModifier.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "CellsGenerator.hpp"
#include "SimpleVolumeBasedStochasticCellCycleModel.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "OffLatticeSimulation.hpp"
#include "SmartPointers.hpp"
#include "MPhaseGrowthTargetAreaModifier.hpp"
#include "WelikyOsterForce.hpp"
#include "ModifiedWelikyOsterForce.hpp"
#include "VertexAngleForce.hpp"
#include "CellAgesWriter.hpp"
#include "CellAncestorWriter.hpp"
#include "CellIdWriter.hpp"
#include "CellMutationStatesWriter.hpp"
#include "CellProliferativePhasesWriter.hpp"
#include "CellProliferativeTypesWriter.hpp"
#include "CellVolumesWriter.hpp"
#include "CellLabelWriter.hpp"
#include "CellRadiusWriter.hpp"
#include "CellAppliedForceWriter.hpp"


class testModWelikyOster: public AbstractCellBasedTestSuite
{
private:

	double mLastStartTime;
	void setUp(){
		mLastStartTime = std::clock();
		AbstractCellBasedTestSuite::setUp();
	}
	void tearDown(){
		double time = std::clock();
		double elapsed_time = (time-mLastStartTime)/(CLOCKS_PER_SEC);
		std::cout << "Elapsed time: " << elapsed_time << std::endl;
		
		AbstractCellBasedTestSuite::tearDown();
	}
	
	void SetUpCellsWithStochasticAreaDependentCellCycleModel(std::vector<CellPtr>& rCells, unsigned numCells, double quiescentVolumeFraction){
        rCells.clear();
        rCells.reserve(numCells);

        MAKE_PTR(WildTypeCellMutationState, p_state);
        MAKE_PTR(TransitCellProliferativeType, p_transit_type);
        for (unsigned i=0; i<numCells; i++)
        {
            // Create a cell-cycle model and set contact inhibition parameter
        	SimpleVolumeBasedStochasticCellCycleModel* p_cell_cycle_model = new SimpleVolumeBasedStochasticCellCycleModel;
            p_cell_cycle_model->SetDimension(2);
            p_cell_cycle_model->SetQuiescentVolumeFraction(quiescentVolumeFraction);
            p_cell_cycle_model->SetSDuration(1e-10);
            p_cell_cycle_model->SetG2Duration(1e-10);

            // Create a cell using the cell-cycle model and a 'wild-type' cell mutation state
            CellPtr p_cell(new Cell(p_state, p_cell_cycle_model));
            p_cell->SetCellProliferativeType(p_transit_type);

            // Give the cell a random birth time
            double birth_time = -p_cell_cycle_model->GetAverageTransitCellCycleTime()*RandomNumberGenerator::Instance()->ranf();
            p_cell->SetBirthTime(birth_time);

            rCells.push_back(p_cell);
        }
    }
	
public:

	void TestModifiedWelikyOsterMonolayerWithVertexAngleForce() throw (Exception){
		HoneycombVertexMeshGenerator mesh_generator(1, 1);
		MutableVertexMesh<2,2>* p_mesh = mesh_generator.GetMesh();
		p_mesh->SetCellRearrangementThreshold(0.1);
		
		std::vector<CellPtr> cells;
		
		SetUpCellsWithStochasticAreaDependentCellCycleModel(cells, 1, .812261 * .731);
		
		VertexBasedCellPopulation<2> population(*p_mesh, cells);
		
		population.AddCellWriter<CellAgesWriter>();
		population.AddCellWriter<CellAncestorWriter>();
		population.AddCellWriter<CellIdWriter>();
		//population.AddCellWriter<CellMutationStatesWriter>();
		population.AddCellWriter<CellProliferativePhasesWriter>();
		//population.AddCellWriter<CellProliferativeTypesWriter>();
		population.AddCellWriter<CellVolumesWriter>();
		population.AddCellWriter<CellLabelWriter>();

		
		OffLatticeSimulation<2> simulator(population);
		simulator.SetOutputDirectory("TestWelikyOster");
		simulator.SetDt(0.001);
		simulator.SetEndTime(115);
		simulator.SetSamplingTimestepMultiple(1000);
		
		MAKE_PTR(VolumeTrackingModifier<2>, p_modifier);
		simulator.AddSimulationModifier(p_modifier);
		
		MAKE_PTR(ModifiedWelikyOsterForce<2>, p_force);
		p_force->SetWelikyOsterAreaParameter(1.0); //beta
		p_force->SetWelikyOsterPerimeterParameter(1.0); //kappa
		simulator.AddForce(p_force);
		
		MAKE_PTR(VertexAngleForce<2>, p_force2);
		p_force2->SetAngleRestrainingParameter(10.0); //gamma
		simulator.AddForce(p_force2);
		
		simulator.Solve();
	}
};

#endif /*TESTWELIKYOSTERMOD_HPP_*/