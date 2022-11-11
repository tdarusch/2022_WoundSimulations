#ifndef TESTVERTEXRESTRICTEDGEOMETRY_HPP_
#define TESTVERTEXRESTRICTEDGEOMETRY_HPP_

#include <cxxtest/TestSuite.h>

// Must be included before other cell_based headers
#include "CellBasedSimulationArchiver.hpp"

#include "SmartPointers.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "CellsGenerator.hpp"
#include "SimpleWntUniformDistCellCycleModel.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "OffLatticeSimulation.hpp"
#include "NagaiHondaForce.hpp"
#include "MPhaseGrowthTargetAreaModifier.hpp"
#include "ModifiedWelikyOsterForce.hpp"
#include "VertexAngleForce.hpp"
#include "PlaneBasedCellKiller.hpp"
#include "CylindricalHoneycombVertexMeshGenerator.hpp"
#include "PlaneBoundaryCondition.hpp"
#include "ObstructionBoundaryCondition.hpp"
#include "CellAgesWriter.hpp"
#include "CellAncestorWriter.hpp"
#include "CellIdWriter.hpp"
#include "CellMutationStatesWriter.hpp"
#include "CellProliferativePhasesWriter.hpp"
#include "CellProliferativeTypesWriter.hpp"
#include "CellVolumesWriter.hpp"
#include "NodeVelocityWriter.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include <stdexcept>

class TestVertexRestrictedGeometry: public AbstractCellBasedTestSuite
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
	
public:

    void TestVertexWithObstuction()
    {
        try {
            // Create a regular compressed 2D vertex-based mesh, of size 12 by 1 elements
            CylindricalHoneycombVertexMeshGenerator generator(12, 1, true, 0.6);

            // Impose periodicity at the left- and right-hand boundaries of the domain
            Cylindrical2dVertexMesh* p_mesh = generator.GetCylindricalMesh();
            p_mesh->SetCellRearrangementThreshold(0.1);

            // Set up a vector of cells with given cell-cycle model
            std::vector<CellPtr> cells;
            MAKE_PTR(TransitCellProliferativeType, p_transit_type);
            CellsGenerator<SimpleWntUniformDistCellCycleModel, 2> cells_generator;
            cells_generator.GenerateBasicRandom(cells,p_mesh->GetNumElements(), p_transit_type);
        
            // Create a vertex-based cell population and set up a cell-based simulation
            VertexBasedCellPopulation<2> population(*p_mesh, cells);
        
            // Specify what to output from the simulation
            population.AddCellWriter<CellAgesWriter>();
            population.AddCellWriter<CellAncestorWriter>();
            population.AddCellWriter<CellIdWriter>();
            population.AddCellWriter<CellMutationStatesWriter>();
            population.AddCellWriter<CellProliferativePhasesWriter>();
            population.AddCellWriter<CellProliferativeTypesWriter>();
            population.AddCellWriter<CellVolumesWriter>();
            population.AddPopulationWriter<NodeVelocityWriter>();

            // Set up cell-based simulation
            OffLatticeSimulation<2> simulator(population);
            simulator.SetOutputDirectory("TestVertexObstruction");
            simulator.SetDt(0.001);
            simulator.SetEndTime(100.0);
            simulator.SetSamplingTimestepMultiple(1000);
        
            // Create a force law and pass it to the simulation
            MAKE_PTR(NagaiHondaForce<2>, p_force);
            p_force->SetNagaiHondaDeformationEnergyParameter(55.0);          // lambda
            p_force->SetNagaiHondaMembraneSurfaceEnergyParameter(0.0);       // beta
            p_force->SetNagaiHondaCellCellAdhesionEnergyParameter(5.0);      // gamma_cell
            p_force->SetNagaiHondaCellBoundaryAdhesionEnergyParameter(10.0); // gamma_boundary
            simulator.AddForce(p_force);

            MAKE_PTR(MPhaseGrowthTargetAreaModifier<2>, p_growth_modifier);
            simulator.AddSimulationModifier(p_growth_modifier);

            // Create cell killer for y = 12 and pass to the simulation
            c_vector<double, 2> point1 = zero_vector<double>(2);
            point1(1) = 12.0;
            c_vector<double, 2> normal1 = zero_vector<double>(2);
            normal1(1) = 1.0;
            MAKE_PTR_ARGS(PlaneBasedCellKiller<2>, p_killer, (&population, point1, normal1));
            simulator.AddCellKiller(p_killer);

            // Create boundary condition y > 0
            c_vector<double, 2> point2 = zero_vector<double>(2);
            c_vector<double, 2> normal2 = zero_vector<double>(2);
            normal2(1) = -1.0;
            MAKE_PTR_ARGS(PlaneBoundaryCondition<2>, p_boundary_condition_1, (&population, point2, normal2));
            simulator.AddCellPopulationBoundaryCondition(p_boundary_condition_1);

            // Create obstruction boundary condition
            MAKE_PTR_ARGS(ObstructionBoundaryCondition<2>, p_boundary_condition_2, (&population));
            simulator.AddCellPopulationBoundaryCondition(p_boundary_condition_2);

            // Create an instance of a Wnt concentration
            WntConcentration<2>::Instance()->SetType(LINEAR);
            WntConcentration<2>::Instance()->SetCellPopulation(population);
            WntConcentration<2>::Instance()->SetCryptLength(3); // So only cells at very bottom divide
         //   WntConcentration<2>::Instance()->SetCryptLength(40); // So all cells  divide

            // Run simulation
            simulator.Solve();
        
            // Tidy up
            WntConcentration<2>::Instance()->Destroy();
        }
        catch ( ... )
		{
			throw;
		}
    }
};

#endif /*TESTVERTEXRESTRICTEDGEOMETRY_HPP_*/