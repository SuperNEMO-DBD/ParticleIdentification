/// \file falaise/snemo/datamodels/angle_measurement.h
/* Author(s) :    Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date: 2015-10-24
 * Last modified: 2015-10-24
 *
 * Description: The class for angle measurement
 */

#ifndef FALAISE_SNEMO_DATAMODEL_ANGLE_MEASUREMENT_H
#define FALAISE_SNEMO_DATAMODEL_ANGLE_MEASUREMENT_H 1

// This project:
#include <falaise/snemo/datamodels/base_topology_measurement.h>

namespace snemo {

  namespace datamodel {

    /// \brief The angle measurement
    class angle_measurement : public base_topology_measurement {

    public:
      /// Constructor
      explicit angle_measurement(double angle = datatools::invalid_real_double());

      /// Destructor
      ~angle_measurement();

      /// Check measurement validity
      bool is_valid() const;

      /// Return the measured angle 
      double get_angle() const;

      /// Smart print
      virtual void tree_dump(std::ostream      & out_    = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    private:
      double angle_; //!< The angle measurement

      DATATOOLS_SERIALIZATION_DECLARATION()
    };

  } // end of namespace datamodel

} // end of namespace snemo

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(snemo::datamodel::angle_measurement,
                        "snemo::datamodel::angle_measurement")

#endif // FALAISE_SNEMO_DATAMODEL_ANGLE_MEASUREMENT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
