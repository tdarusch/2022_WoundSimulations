#ifndef WOUNDT1WRITER_HPP_
#define WOUNDT1WRITER_HPP_

#include "AbstractCellPopulationWriter.hpp"
#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
class WoundT1Writer : public AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM>
{
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM> >(*this);
    }
 
public:
 
    WoundT1Writer();
 
    virtual void Visit(MeshBasedCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);
 
    virtual void Visit(CaBasedCellPopulation<SPACE_DIM>* pCellPopulation);
 
    virtual void Visit(NodeBasedCellPopulation<SPACE_DIM>* pCellPopulation);
 
    virtual void Visit(PottsBasedCellPopulation<SPACE_DIM>* pCellPopulation);
 
    virtual void Visit(VertexBasedCellPopulation<SPACE_DIM>* pCellPopulation);
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(WoundT1Writer)

#endif /* WOUNDT1WRITER_HPP_ */