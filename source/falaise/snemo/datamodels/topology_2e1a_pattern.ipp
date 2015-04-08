// -*- mode: c++ ; -*-
/// \file falaise/snemo/datamodels/topology_2e1a_pattern.ipp

#ifndef FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E1A_PATTERN_IPP
#define FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E1A_PATTERN_IPP 1

// Ourselves:
#include <falaise/snemo/datamodels/topology_2e1a_pattern.h>

// Third party:
// - Boost:
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>

// This project:
#include <falaise/snemo/datamodels/base_topology_pattern.ipp>

namespace snemo {

  namespace datamodel {

    /// Serialization method
    template<class Archive>
    void topology_2e1a_pattern::serialize(Archive & ar, const unsigned int /* version */)
    {
      ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_topology_pattern);
      return;
    }

  } // end of namespace datamodel

} // end of namespace snemo

#endif // FALAISE_SNEMO_DATAMODEL_TOPOLOGY_2E1A_PATTERN_IPP
