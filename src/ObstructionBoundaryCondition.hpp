#ifndef OBSTRUCTIONBOUNDARYCONDITION_HPP_
#define OBSTRUCTIONBOUNDARYCONDITION_HPP_

#include "AbstractCellPopulationBoundaryCondition.hpp"

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

/**
 * An obstruction boundary condition class.
 */
template<unsigned DIM>
class ObstructionBoundaryCondition : public AbstractCellPopulationBoundaryCondition<DIM>
{
private:

    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Serialize the object and its member variables.
     *
     * @param archive the archive
     * @param version the current version of this class 
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellPopulationBoundaryCondition<DIM> >(*this);
    }
	
public:
    /**
     * Constructor.
     *
     * @param pCellPopulation pointer to the cell population
     */
    ObstructionBoundaryCondition(AbstractCellPopulation<DIM>* pCellPopulation);

    /**
     * Overridden ImposeBoundaryCondition() method.
     *
     * Apply the cell population boundary conditions.
     *
     * @param rOldLocations the node locations before any boundary conditions are applied
     */
    void ImposeBoundaryCondition(const std::map<Node<DIM>*, c_vector<double, DIM> >& rOldLocations);

    /**
     * Overridden VerifyBoundaryCondition() method.
     * Verify the boundary conditions have been applied.
     * This is called after ImposeBoundaryCondition() to ensure the condition is still satisfied.
     *
     * @return whether the boundary conditions are satisfied.
     */
    bool VerifyBoundaryCondition();

    /**
     * Overridden OutputCellPopulationBoundaryConditionParameters() method.
     * Output cell population boundary condition parameters to file.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    void OutputCellPopulationBoundaryConditionParameters(out_stream& rParamsFile);
};
	
#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(ObstructionBoundaryCondition)
	
namespace boost
{
namespace serialization
{
/**
 * De-serialize constructor parameters and initialize a ObstructionBoundaryCondition.
 */
template<class Archive, unsigned DIM>
inline void load_construct_data(
    Archive & ar, ObstructionBoundaryCondition<DIM> * t, const unsigned int file_version)
{
    // Retrieve data from archive required to construct new instance
    AbstractCellPopulation<DIM>* p_cell_population;
    ar >> p_cell_population;
	
    // Invoke inplace constructor to initialise instance
    ::new(t)ObstructionBoundaryCondition<DIM>(p_cell_population);
}
}
} // namespace ...

#endif /*OBSTRUCTIONBOUNDARYCONDITION_HPP_*/