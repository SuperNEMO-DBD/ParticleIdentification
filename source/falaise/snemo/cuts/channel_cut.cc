// falaise/snemo/cuts/channel_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/channel_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/things.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/datamodels/base_topology_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(channel_cut, "snemo::cut::channel_cut");

    void channel_cut::_set_defaults()
    {
      _TD_label_ = "TD";//snemo::datamodel::data_info::default_topology_data_label();
      _cuts_.clear();
      return;
    }
    channel_cut::channel_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    channel_cut::~channel_cut()
    {
      if (is_initialized()) this->channel_cut::reset();
      return;
    }

    void channel_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void channel_cut::initialize(const datatools::properties & configuration_,
                                 datatools::service_manager  & /*service_manager_*/,
                                 cuts::cut_handle_dict_type  & cut_dict_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("TD_label")) {
        _TD_label_ = configuration_.fetch_string("TD_label");
      }

      DT_THROW_IF(! configuration_.has_key("cuts"), std::logic_error, "Missing 'cuts' list !");
      std::vector<std::string> cuts;
      configuration_.fetch("cuts", cuts);
      for (size_t i = 0; i < cuts.size(); i++) {
        const std::string & a_name = cuts.at(i);
        DT_THROW_IF(! configuration_.has_key(a_name + ".cut_label"), std::logic_error,
                    "Missing associated cut label to '" << a_name << "' cut!");
        const std::string a_cut_label = configuration_.fetch_string(a_name + ".cut_label");
        DT_THROW_IF(cut_dict_.find(a_cut_label) == cut_dict_.end(), std::logic_error,
                    "No cut '" << a_cut_label << "' has been registered !");
        DT_THROW_IF(! configuration_.has_key(a_name + ".measurement_label"), std::logic_error,
                    "Missing associated measurement label to '" << a_name << "' cut!");
        const std::string a_meas_label = configuration_.fetch_string(a_name + ".measurement_label");
        _cuts_.insert(std::make_pair(a_meas_label, cut_dict_[a_cut_label]));
      }

      this->i_cut::_set_initialized(true);
      return;
    }

    int channel_cut::_accept()
    {
      // Get event record
      const datatools::things & ER = get_user_data<datatools::things>();

      if (! ER.has(_TD_label_)) {
        DT_LOG_WARNING(get_logging_priority(), "Event record has no '" << _TD_label_ << "' bank !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::topology_data & TD = ER.get<snemo::datamodel::topology_data>(_TD_label_);
      if (! TD.has_pattern()) {
        DT_LOG_DEBUG(get_logging_priority(), "Missing topology pattern !");
        return cuts::SELECTION_INAPPLICABLE;
      }
      const snemo::datamodel::base_topology_pattern & a_pattern = TD.get_pattern();

      // Loop over cuts
      for (cuts::cut_handle_dict_type::iterator icut = _cuts_.begin();
           icut != _cuts_.end(); ++icut) {
        const std::string & a_meas_label = icut->first;
        if (! a_pattern.has_measurement(a_meas_label)) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing '" << a_meas_label << "' measurement !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::base_topology_measurement & a_meas = a_pattern.get_measurement(a_meas_label);
        cuts::i_cut & a_cut = icut->second.grab_initialized_cut_handle().grab();
        a_cut.set_user_data(a_meas);
        const int status = a_cut.process();
        if (status == cuts::SELECTION_REJECTED) {
          DT_LOG_DEBUG(get_logging_priority(), "Event rejected by '"
                       << a_cut.get_name() << "' cut !");
          return cuts::SELECTION_REJECTED;
        } else if (status == cuts::SELECTION_INAPPLICABLE) {
          DT_LOG_DEBUG(get_logging_priority(), "Cut '" << a_cut.get_name() << "' can not be applied to '"
                       << a_meas_label << "' measurement !");
          return cuts::SELECTION_INAPPLICABLE;
        }
      }

      DT_LOG_DEBUG(get_logging_priority(), "Event accepted by channel '" << get_name() << "'cut !");
      return cuts::SELECTION_ACCEPTED;
    }

  }  // end of namespace cut

}  // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
