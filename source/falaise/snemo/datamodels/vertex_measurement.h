/// \file falaise/snemo/datamodels/vertex_measurement.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-10-24
 * Last modified: 2015-10-24
 *
 * Description: The class for vertex measurement
 */

#ifndef FALAISE_SNEMO_DATAMODEL_VERTEX_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_VERTEX_MEASUREMENT_H 1

// This project
#include <falaise/snemo/datamodels/base_topology_measurement.h>

// Third party:
// - Bayeux/geomtools:
#include <bayeux/geomtools/blur_spot.h>

namespace snemo {

  namespace datamodel {

    /// \brief The vertex measurement
    class vertex_measurement : public base_topology_measurement {

    public:

      /// Constructor
      vertex_measurement();

      /// Destructor
      ~vertex_measurement();

      /// Check vertex validity
      bool has_vertex() const;

      /// Get a non-mutable reference to vertex location
      const geomtools::blur_spot & get_vertex() const;

      /// Get a mutable reference to vertex location
      geomtools::blur_spot & grab_vertex();

      /// Check probability validity
      bool has_probability() const;

      /// Return vertex probability
      double get_probability() const;

      /// Set probability value
      void set_probability(const double probability_);

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:

      double _probability_;          //!< Chi2 probability of the vertex
      geomtools::blur_spot _vertex_; //!< 3D position and associated errors

      DATATOOLS_SERIALIZATION_DECLARATION();
    };

  } // end of namespace datamodel

} // end of namespace snemo

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::vertex_measurement,
                        "snemo::datamodel::vertex_measurement")

#endif // FALAISE_SNEMO_DATAMODEL_VERTEX_MEASUREMENT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/