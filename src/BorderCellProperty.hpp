//#include <cxxtest/TestSuite.h>
//#include "CheckpointArchiveTypes.hpp"
//#include "AbstractCellBasedTestSuite.hpp"

#include "AbstractCellProperty.hpp"
#include "AbstractForce.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "NodesOnlyMesh.hpp"
#include "WildTypeCellMutationState.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "CellLabel.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "CellMutationStatesCountWriter.hpp"
#include "OffLatticeSimulation.hpp"
#include "SmartPointers.hpp"

//#include "FakePetscSetup.hpp"

class BorderCellProperty : public AbstractCellProperty{
    private:
        unsigned mColor;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & archive, const unsigned int version){
            archive & boost::serialization::base_object<AbstractCellProperty>(*this);
            archive & mColor;
        }
    public:
        BorderCellProperty(unsigned color=5) : AbstractCellProperty(), mColor(color){}
        ~BorderCellProperty(){}
        unsigned GetColor() const
            {return mColor;}
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(BorderCellProperty)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(BorderCellProperty)