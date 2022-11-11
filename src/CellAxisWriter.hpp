#ifndef CELLAXISWRITER_HPP_
#define CELLAXISWRITER_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>
#include "AbstractCellWriter.hpp"

template <unsigned ELEMENT_DIM, unsigned SPACE_DIM>
class CellAxisWriter : public AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>{
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version){
        archive & boost::serialization::base_object<AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>>(*this);
    }
public:
    CellAxisWriter();
    double GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);
    virtual void VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation);
    //double GetPerimeter(CellPtr pCell, VertexBasedCellPopulation<SPACE_DIM>* pCellPopulation);
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellAxisWriter)

#endif /* CELLAXISWRITER_HPP_ */