// falaise/snemo/cuts/channel_1e_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/channel_1e_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>
#include <falaise/snemo/datamodels/topology_1e_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(channel_1e_cut, "snemo::cut::channel_1e_cut");

    void channel_1e_cut::_set_defaults()
    {
      base_channel_cut::_set_defaults();
      return;
    }
    channel_1e_cut::channel_1e_cut(datatools::logger::priority logger_priority_)
      : base_channel_cut::base_channel_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    channel_1e_cut::~channel_1e_cut()
    {
      if (is_initialized()) this->channel_1e_cut::reset();
      return;
    }

    void channel_1e_cut::reset()
    {
      _set_defaults();
      this->base_channel_cut::_reset();
      this->base_channel_cut::_set_initialized(false);
      return;
    }

    void channel_1e_cut::initialize(const datatools::properties & configuration_,
                                    datatools::service_manager  & service_manager_,
                                    cuts::cut_handle_dict_type  & cut_dict_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");
      base_channel_cut::initialize(configuration_, service_manager_, cut_dict_);
      // this->base_channel_cut::initialize(configuration_);
      return;
    }

    int channel_1e_cut::_accept()
    {
      uint32_t cut_returned = cuts::SELECTION_INAPPLICABLE;

      // Get event record
      const datatools::things & ER = get_user_data<datatools::things>();

      if (! ER.has(_TD_label_)) {
        DT_LOG_WARNING(get_logging_priority(), "Event record has no '" << _TD_label_ << "' bank !");
        return cut_returned;
      }

      const snemo::datamodel::topology_data & TD = ER.get<snemo::datamodel::topology_data>(_TD_label_);
      if (! TD.has_pattern()) {
        DT_LOG_DEBUG(get_logging_priority(), "Missing topology pattern !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::base_topology_pattern & a_pattern = TD.get_pattern();
      const std::string & a_pattern_id = a_pattern.pattern_id();

      std::string pattern_id_1e;
      {
        snemo::datamodel::topology_1e_pattern * a_1e_pattern = new snemo::datamodel::topology_1e_pattern;
        pattern_id_1e = a_1e_pattern->pattern_id();
      }

      if (a_pattern_id != pattern_id_1e) {
        DT_LOG_DEBUG(get_logging_priority(), "This cut is only applicable to '"
                     << pattern_id_1e << "' topology !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::topology_1e_pattern & a_1e_pattern
        = dynamic_cast<const snemo::datamodel::topology_1e_pattern &>(a_pattern);

      // Check if event has angle
      bool check_has_angle = true;
      if (is_mode_has_angle()) {
        if (! a_1e_pattern.has_electron_angle()) {
            check_has_angle = false;
        }
      }

      // Check if event has required angle
      bool check_range_angle = true;
      if (is_mode_range_angle()) {
        if (! a_1e_pattern.has_electron_angle()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing angle !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const double angle = a_1e_pattern.get_electron_angle();
        if (datatools::is_valid(_angle_min_)) {
          if (angle < _angle_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Angle (" << angle << ") lower than " << _angle_min_);
            check_range_angle = false;
          }
        }
        if (datatools::is_valid(_angle_max_)) {
          if (angle > _angle_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Angle (" << angle << ") greater than " << _angle_max_);
            check_range_angle = false;
          }
        }
      }

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_has_angle &&
          check_range_angle) {
        DT_LOG_DEBUG(get_logging_priority(), "Event rejected by channel 1e cut!");
        cut_returned = cuts::SELECTION_ACCEPTED;
      }

      return cut_returned;
    }

  }  // end of namespace cut

}  // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/