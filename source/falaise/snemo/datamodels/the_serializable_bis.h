// -*- mode: c++; -*-
/// \file falaise/snemo/datamodels/the_serializable_bis.h

#ifndef FALAISE_SNEMO_DATAMODELS_THE_SERIALIZABLE_BIS_H
#define FALAISE_SNEMO_DATAMODELS_THE_SERIALIZABLE_BIS_H 1

// Third party:
// - Boost:
#include <boost/serialization/export.hpp>
// - Bayeux/datatools:
#include <datatools/archives_instantiation.h>

/******************************************
 * snemo::datamodel::topology_measurement *
 ******************************************/
// #include <falaise/snemo/datamodels/topology_measurement.ipp>
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::tof_measurement)
// BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::tof_measurement)


/**************************************
 * snemo::datamodel::topology_pattern *
 **************************************/

#include <falaise/snemo/datamodels/base_topology_pattern.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::base_topology_pattern)
// BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::base_topology_pattern)

// #include <falaise/snemo/datamodels/base_topology_measurement.ipp>
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::base_topology_measurement)
// BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::base_topology_measurement)

#include <falaise/snemo/datamodels/tof_measurement.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::tof_measurement)

// #include <falaise/snemo/datamodels/delta_vertices_measurement.ipp>

// #include <falaise/snemo/datamodels/angle_measurement.ipp>

// #include <falaise/snemo/datamodels/energy_measurement.ipp>

/***********************************
 * snemo::datamodel::topology_data *
 ***********************************/

#include <falaise/snemo/datamodels/topology_data.ipp>
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(snemo::datamodel::topology_data)
BOOST_CLASS_EXPORT_IMPLEMENT(snemo::datamodel::topology_data)

#endif // FALAISE_SNEMO_DATAMODELS_THE_SERIALIZABLE_BIS_H
