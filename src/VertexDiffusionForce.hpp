#ifndef VERTEXDIFFUSIONFORCE_HPP_
#define VERTEXDIFFUSIONFORCE_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

#include "AbstractForce.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "RandomNumberGenerator.hpp"

template<unsigned DIM>
class VertexDiffusionForce : public AbstractForce<DIM>{
private:
    double mDiffusionConstant;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version){
        archive & boost::serialization::base_object<AbstractForce<DIM>>(*this);
        archive & mDiffusionConstant;
    }
public:
    //Constructor
    /**
    *@params diffusionConstant
    */
    VertexDiffusionForce(double diffusionConstant=0.010);

    //Destructor
    ~VertexDiffusionForce();

    /**
    *@params diffusionConstant
    */
    void SetDiffusionConstant(double diffusionConstant);

    /**
    *@param rForces
    *@param rCellPopulation
    */
   void AddForceContribution(AbstractCellPopulation<DIM>& rCellPopulation);
   /**
    * @param rParamsFile
    */
   void OutputForceParameters(out_stream& rParamsFile);
};  

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(VertexDiffusionForce)

#endif /*VERTEXDIFFUSIONFORCE_HPP_*/