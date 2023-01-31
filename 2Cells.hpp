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

    void preventDivision(AbstractCellPopulation<2>& rCellPopulation){
        MAKE_PTR(DifferentiatedCellProliferativeType, p_diff);
        for(AbstractCellPopulation<2>::Iterator cell_iter = rCellPopulation.Begin();
            cell_iter != rCellPopulation.End();
            ++cell_iter){
                cell_iter->SetCellProliferativeType(p_diff);
            }
    }

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