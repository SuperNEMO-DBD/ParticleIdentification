/// \file falaise/snemo/datamodels/angle_measurement_cut.h
/* Author(s) :    Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date: 2015-11-02
 * Last modified: 2015-11-02
 *
 * Description:
 *
 *   The cut on the Time-Of-Flight measurement
 */

#ifndef FALAISE_SNEMO_CUT_ANGLE_MEASUREMENT_CUT_H
#define FALAISE_SNEMO_CUT_ANGLE_MEASUREMENT_CUT_H 1

// Third party:
// - Boost:
// - Bayeux/cuts:
#include <bayeux/cuts/i_cut.h>

// - Bayeux/datatools
#include <bayeux/datatools/real_range.h>

namespace snemo {

  namespace cut {

    /// \brief A cut performed on individual 'angle measurement'
    class angle_measurement_cut : public cuts::i_cut
    {
    public:
      /// Constructor
      angle_measurement_cut(datatools::logger::priority logging_priority_ = datatools::logger::PRIO_FATAL);

      /// Destructor
      virtual ~angle_measurement_cut();

      /// Return true if cut requires angle to be present
      bool angle_required() const;


      /// Initilization
      virtual void initialize(const datatools::properties & configuration_,
                              datatools::service_manager & service_manager_,
                              cuts::cut_handle_dict_type & cut_dict_);

      /// Reset
      virtual void reset();

    protected:

      /// Default values
      void _set_defaults();

      /// Selection
      virtual int _accept();

    private:
      bool angleRequired_;          //!< True if angle measurement required
      datatools::real_range angleRange_; //!< Allowed angle range

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(angle_measurement_cut)
    };

  }  // end of namespace cut

}  // end of namespace snemo

// OCD support::
#include <datatools/ocd_macros.h>

// @arg snemo::cut::angle_measurement_cut the name the registered class in the OCD system
DOCD_CLASS_DECLARATION(snemo::cut::angle_measurement_cut)

#endif // FALAISE_SNEMO_CUT_ANGLE_MEASUREMENT_CUT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
