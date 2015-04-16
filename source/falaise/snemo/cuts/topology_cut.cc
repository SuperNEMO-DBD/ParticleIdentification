// falaise/snemo/cuts/topology_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/topology_cut.h>

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
#include <falaise/snemo/datamodels/topology_2e_pattern.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(topology_cut, "snemo::cut::topology_cut");

    void topology_cut::_set_defaults()
    {
      _mode_ = MODE_UNDEFINED;
      _TD_label_ = "TD";//snemo::datamodel::data_info::default_topology_data_label();
      datatools::invalidate(_prob_int_min_);
      datatools::invalidate(_prob_int_max_);
      datatools::invalidate(_prob_ext_min_);
      datatools::invalidate(_prob_ext_max_);
      datatools::invalidate(_delta_vertices_y_min_);
      datatools::invalidate(_delta_vertices_y_max_);
      datatools::invalidate(_delta_vertices_z_min_);
      datatools::invalidate(_delta_vertices_z_max_);
      datatools::invalidate(_angle_min_);
      datatools::invalidate(_angle_max_);
      return;
    }

    uint32_t topology_cut::get_mode() const
    {
      return _mode_;
    }

    bool topology_cut::is_mode_pattern_id() const
    {
      return _mode_ & MODE_PATTERN_ID;
    }

    bool topology_cut::is_mode_has_internal_probability() const
    {
      return _mode_ & MODE_HAS_INTERNAL_PROBABILITY;
    }

    bool topology_cut::is_mode_has_external_probability() const
    {
      return _mode_ & MODE_HAS_EXTERNAL_PROBABILITY;
    }

    bool topology_cut::is_mode_range_internal_probability() const
    {
      return _mode_ & MODE_RANGE_INTERNAL_PROBABILITY;
    }

    bool topology_cut::is_mode_range_external_probability() const
    {
      return _mode_ & MODE_RANGE_EXTERNAL_PROBABILITY;
    }

    bool topology_cut::is_mode_has_delta_vertices_y() const
    {
      return _mode_ & MODE_HAS_DELTA_VERTICES_Y;
    }

    bool topology_cut::is_mode_range_delta_vertices_y() const
    {
      return _mode_ & MODE_RANGE_DELTA_VERTICES_Y;
    }

    bool topology_cut::is_mode_has_delta_vertices_z() const
    {
      return _mode_ & MODE_HAS_DELTA_VERTICES_Z;
    }

    bool topology_cut::is_mode_range_delta_vertices_z() const
    {
      return _mode_ & MODE_RANGE_DELTA_VERTICES_Z;
    }

    bool topology_cut::is_mode_has_angle() const
    {
      return _mode_ & MODE_HAS_ANGLE;
    }

    bool topology_cut::is_mode_range_angle() const
    {
      return _mode_ & MODE_RANGE_ANGLE;
    }

    topology_cut::topology_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      return;
    }

    topology_cut::~topology_cut()
    {
      if (is_initialized()) this->topology_cut::reset();
      return;
    }

    void topology_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
      return;
    }

    void topology_cut::initialize(const datatools::properties & configuration_,
                                        datatools::service_manager  & /* service_manager_ */,
                                        cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      if (configuration_.has_key("TD_label")) {
        _TD_label_ = configuration_.fetch_string("TD_label");
      }

      if (configuration_.has_flag("mode.pattern_id")) {
        _mode_ |= MODE_PATTERN_ID;
      }
      if (configuration_.has_flag("mode.has_internal_probability")) {
        _mode_ |= MODE_HAS_INTERNAL_PROBABILITY;
      }
      if (configuration_.has_flag("mode.has_external_probability")) {
        _mode_ |= MODE_HAS_EXTERNAL_PROBABILITY;
      }
      if (configuration_.has_flag("mode.range_internal_probability")) {
        _mode_ |= MODE_RANGE_INTERNAL_PROBABILITY;
      }
      if (configuration_.has_flag("mode.range_external_probability")) {
        _mode_ |= MODE_RANGE_EXTERNAL_PROBABILITY;
      }
      if (configuration_.has_flag("mode.has_delta_vertices_y")) {
        _mode_ |= MODE_HAS_DELTA_VERTICES_Y;
      }
      if (configuration_.has_flag("mode.has_delta_vertices_z")) {
        _mode_ |= MODE_HAS_DELTA_VERTICES_Z;
      }
      if (configuration_.has_flag("mode.range_delta_vertices_y")) {
        _mode_ |= MODE_RANGE_DELTA_VERTICES_Y;
      }
      if (configuration_.has_flag("mode.range_delta_vertices_z")) {
        _mode_ |= MODE_RANGE_DELTA_VERTICES_Z;
      }
      if (configuration_.has_flag("mode.has_angle")) {
        _mode_ |= MODE_HAS_ANGLE;
      }
      if (configuration_.has_flag("mode.range_angle")) {
        _mode_ |= MODE_RANGE_ANGLE;
      }
      DT_THROW_IF(_mode_ == MODE_UNDEFINED, std::logic_error,
                  "Missing at least a 'mode.XXX' property !");

      if (is_mode_pattern_id()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using PATTERN_ID mode...");
        DT_THROW_IF(! configuration_.has_key("pattern_id.label"), std::logic_error,
                    "Missing 'pattern_id.label' !");
        _pattern_id_label_ = configuration_.fetch_string("pattern_id.label");
      }

      if (is_mode_range_internal_probability()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_INTERNAL_PROBABILITY mode...");
        size_t count = 0;
        if (configuration_.has_key("range_internal_probability.min")) {
          const double pmin = configuration_.fetch_real("range_internal_probability.min");
          DT_THROW_IF(pmin < 0.0 || pmin > 1.0, std::range_error,
                      "Invalid minimal internal probability (" << pmin << ") !");
          _prob_int_min_ = pmin;
          count++;
        }
        if (configuration_.has_key("range_internal_probablity.max")) {
          const double pmax = configuration_.fetch_real("range_internal_probability.max");
          DT_THROW_IF(pmax < 0.0 || pmax > 1.0, std::range_error,
                      "Invalid maximal internal probability (" << pmax << ") !");
          _prob_int_max_ = pmax;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_internal_probability.min' or 'range_internal_probability.max' property !");
        if (count == 2 && _prob_int_min_ >= 0 && _prob_int_max_ >= 0) {
          DT_THROW_IF(_prob_int_min_ > _prob_int_max_, std::logic_error,
                      "Invalid 'range_internal_probability.min' > 'range_internal_probability.max' values !");
        }
      }

      if (is_mode_range_external_probability()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_EXTERNAL_PROBABILITY mode...");
        size_t count = 0;
        if (configuration_.has_key("range_external_probability.min")) {
          const double pmin = configuration_.fetch_real("range_external_probability.min");
          DT_THROW_IF(pmin < 0.0 || pmin > 1.0, std::range_error,
                      "Invalid minimal external probability (" << pmin << ") !");
          _prob_ext_min_ = pmin;
          count++;
        }
        if (configuration_.has_key("range_external_probablity.max")) {
          const double pmax = configuration_.fetch_real("range_external_probability.max");
          DT_THROW_IF(pmax < 0.0 || pmax > 1.0, std::range_error,
                      "Invalid maximal external probability (" << pmax << ") !");
          _prob_ext_max_ = pmax;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_external_probability.min' or 'range_external_probability.max' property !");
        if (count == 2 && _prob_ext_min_ >= 0 && _prob_ext_max_ >= 0) {
          DT_THROW_IF(_prob_ext_min_ > _prob_ext_max_, std::logic_error,
                      "Invalid 'range_external_probability.min' > 'range_external_probability.max' values !");
        }
      }


      if (is_mode_range_delta_vertices_y()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_DELTA_VERTICES_Y mode...");
        size_t count = 0;
        if (configuration_.has_key("range_delta_vertices_y.min")) {
          const double delta_y_min = configuration_.fetch_real("range_delta_vertices_y.min");
          DT_THROW_IF(delta_y_min < 0.0 || delta_y_min > 1.0, std::range_error,
                      "Invalid minimal delta vertices y (" << delta_y_min << ") !");
          _delta_vertices_y_min_ = delta_y_min;
          count++;
        }
        if (configuration_.has_key("range_external_probablity.max")) {
          const double delta_y_max = configuration_.fetch_real("range_delta_vertices_y.max");
          DT_THROW_IF(delta_y_max < 0.0 || delta_y_max > 1.0, std::range_error,
                      "Invalid maximal delta vertices y (" << delta_y_max << ") !");
          _delta_vertices_y_max_ = delta_y_max;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_delta_y_vertices.min' or 'range_delta_y_vertices.max' property !");
        if (count == 2 && _delta_vertices_y_min_ >= 0 && _delta_vertices_y_max_ >= 0) {
          DT_THROW_IF(_delta_vertices_y_min_ > _delta_vertices_y_max_, std::logic_error,
                      "Invalid 'range_delta_y_vertices.min' > 'range_delta_y_vertices.max' values !");
        }
      }

      if (is_mode_range_delta_vertices_z()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_DELTA_VERTICES_Z mode...");
        size_t count = 0;
        if (configuration_.has_key("range_delta_vertices_z.min")) {
          const double delta_z_min = configuration_.fetch_real("range_delta_vertices_z.min");
          DT_THROW_IF(delta_z_min < 0.0 || delta_z_min > 1.0, std::range_error,
                      "Invalid minimal delta vertices z (" << delta_z_min << ") !");
          _delta_vertices_z_min_ = delta_z_min;
          count++;
        }
        if (configuration_.has_key("range_external_probablity.max")) {
          const double delta_z_max = configuration_.fetch_real("range_delta_vertices_z.max");
          DT_THROW_IF(delta_z_max < 0.0 || delta_z_max > 1.0, std::range_error,
                      "Invalid maximal delta vertices z (" << delta_z_max << ") !");
          _delta_vertices_z_max_ = delta_z_max;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_delta_z_vertices.min' or 'range_delta_z_vertices.max' property !");
        if (count == 2 && _delta_vertices_z_min_ >= 0 && _delta_vertices_z_max_ >= 0) {
          DT_THROW_IF(_delta_vertices_z_min_ > _delta_vertices_z_max_, std::logic_error,
                      "Invalid 'range_delta_z_vertices.min' > 'range_delta_z_vertices.max' values !");
        }
      }

      if (is_mode_range_angle()) {
        DT_LOG_DEBUG(get_logging_priority(), "Using RANGE_ANGLE mode...");
        size_t count = 0;
        if (configuration_.has_key("range_angle.min")) {
          const double angle_min = configuration_.fetch_real("range_angle.min");
          DT_THROW_IF(angle_min < 0.0 || angle_min > 1.0, std::range_error,
                      "Invalid minimal angle (" << angle_min << ") !");
          _angle_min_ = angle_min;
          count++;
        }
        if (configuration_.has_key("range_angle.max")) {
          const double angle_max = configuration_.fetch_real("range_angle.max");
          DT_THROW_IF(angle_max < 0.0 || angle_max > 1.0, std::range_error,
                      "Invalid maximal angle (" << angle_max << ") !");
          _angle_max_ = angle_max;
          count++;
        }
        DT_THROW_IF(count == 0, std::logic_error,
                    "Missing 'range_angle.min' or 'range_angle.max' property !");
        if (count == 2 && _angle_min_ >= 0 && _angle_max_ >= 0) {
          DT_THROW_IF(_angle_min_ > _angle_max_, std::logic_error,
                      "Invalid 'range_angle.min' > 'range_angle.max' values !");
        }
      }

      this->i_cut::_set_initialized(true);
      return;
    }


    int topology_cut::_accept()
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
      const std::string & a_pattern_id = a_pattern.get_pattern_id();

      // Check if event has the correct pattern id
      bool check_pattern_id = true;
      if (is_mode_pattern_id()) {
        DT_LOG_DEBUG(get_logging_priority(), "Running PATTERN_ID mode...");
        if (a_pattern_id != _pattern_id_label_) check_pattern_id = false;
      }

      // Check if event has internal probability
      bool check_has_internal_probability = true;
      if (is_mode_has_internal_probability()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "Internal probability cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_internal_probability()) {
          check_has_internal_probability = false;
        }
      }

      // Check if event has correct internal probability
      bool check_range_internal_probability = true;
      if (is_mode_range_internal_probability()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "Internal probability cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_internal_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing internal probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        const double pint = ptr_2e_pattern->get_internal_probability();
        if (datatools::is_valid(_prob_int_min_)) {
          if (pint < _prob_int_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Internal probability (" << pint << ") lower than " << _prob_int_min_);
            check_range_internal_probability = false;
          }
        }
        if (datatools::is_valid(_prob_int_max_)) {
          if (pint > _prob_int_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Internal probability (" << pint << ") greater than " << _prob_int_max_);
            check_range_internal_probability = false;
          }
        }
      }

      // Check if event has external probability
      bool check_has_external_probability = true;
      if (is_mode_has_external_probability()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "External probability cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_external_probability()) {
          check_has_external_probability = false;
        }
      }

      // Check if event has correct external probability
      bool check_range_external_probability = true;
      if (is_mode_range_external_probability()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "External probability cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_external_probability()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing external probability !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        const double pext = ptr_2e_pattern->get_external_probability();
        if (datatools::is_valid(_prob_ext_min_)) {
          if (pext < _prob_ext_min_) check_range_external_probability = false;
        }
        if (datatools::is_valid(_prob_ext_max_)) {
          if (pext > _prob_ext_max_) check_range_external_probability = false;
        }
      }

      // Check if event has delta vertices y
      bool check_has_delta_vertices_y = true;
      if (is_mode_has_delta_vertices_y()) {
        if (a_pattern_id == "2e") {
          const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
            = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
          if (! ptr_2e_pattern->has_delta_vertices_y()) {
            check_has_delta_vertices_y = false;
          }
        }
        else {
          DT_LOG_DEBUG(get_logging_priority(), "Delta vertices y cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
      }

      // Check if event has required delta vertices y
      bool check_range_delta_vertices_y = true;
      if (is_mode_range_delta_vertices_y()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "Delta vertices y cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_delta_vertices_y()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing delta vertices y !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        const double delta_vertices_y = ptr_2e_pattern->get_delta_vertices_y();
        if (datatools::is_valid(_delta_vertices_y_min_)) {
          if (delta_vertices_y < _delta_vertices_y_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Delta vertices y (" << delta_vertices_y << ") lower than " << _delta_vertices_y_min_);
            check_range_delta_vertices_y = false;
          }
        }
        if (datatools::is_valid(_delta_vertices_y_max_)) {
          if (delta_vertices_y > _delta_vertices_y_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Delta vertices y (" << delta_vertices_y << ") greater than " << _delta_vertices_y_max_);
            check_range_delta_vertices_y = false;
          }
        }
      }

      // Check if event has delta vertices z
      bool check_has_delta_vertices_z = true;
      if (is_mode_has_delta_vertices_z()) {
        if (a_pattern_id == "2e") {
          const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
            = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
          if (! ptr_2e_pattern->has_delta_vertices_z()) {
            check_has_delta_vertices_z = false;
          }
        }
        else {
          DT_LOG_DEBUG(get_logging_priority(), "Delta vertices z cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
      }

      // Check if event has required delta vertices
      bool check_range_delta_vertices_z = true;
      if (is_mode_range_delta_vertices_z()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "Delta vertices z cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_delta_vertices_z()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing delta vertices z !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        const double delta_vertices_z = ptr_2e_pattern->get_delta_vertices_z();
        if (datatools::is_valid(_delta_vertices_z_min_)) {
          if (delta_vertices_z < _delta_vertices_z_min_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Delta vertices z (" << delta_vertices_z << ") lower than " << _delta_vertices_z_min_);
            check_range_delta_vertices_z = false;
          }
        }
        if (datatools::is_valid(_delta_vertices_z_max_)) {
          if (delta_vertices_z > _delta_vertices_z_max_) {
            DT_LOG_DEBUG(get_logging_priority(),
                         "Delta vertices z (" << delta_vertices_z << ") greater than " << _delta_vertices_z_max_);
            check_range_delta_vertices_z = false;
          }
        }
      }

      // Check if event has angle
      bool check_has_angle = true;
      if (is_mode_has_angle()) {
        if (a_pattern_id == "2e") {
          const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
            = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
          if (! ptr_2e_pattern->has_angle()) {
            check_has_angle = false;
          }
        }
        else {
          DT_LOG_DEBUG(get_logging_priority(), "Angle cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
      }

      // Check if event has required angle
      bool check_range_angle = true;
      if (is_mode_range_angle()) {
        if (a_pattern_id != "2e") {
          DT_LOG_DEBUG(get_logging_priority(), "Angle cut only applicable to '2e' topology !");
          return cuts::SELECTION_INAPPLICABLE;
        }
        const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
          = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
        if (! ptr_2e_pattern->has_angle()) {
          DT_LOG_DEBUG(get_logging_priority(), "Missing angle !");
          return cuts::SELECTION_INAPPLICABLE;
        }

        const double angle = ptr_2e_pattern->get_angle();
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

      // double internal_prob = datatools::invalid_real();
      // double external_prob = datatools::invalid_real();
      // double delta_vertices_y = datatools::invalid_real();
      // double delta_vertices_z = datatools::invalid_real();

      // if(a_pattern_id == snemo::datamodel::topology_2e_pattern::pattern_id()) {
      //   const snemo::datamodel::topology_2e_pattern * ptr_2e_pattern
      //     = dynamic_cast<const snemo::datamodel::topology_2e_pattern *>(&a_pattern);
      //   // std::cout << "--------------pattern id " << a_pattern.get_pattern_id() << std::endl;

      //   internal_prob    = ptr_2e_pattern->get_internal_probability();
      //   external_prob    = ptr_2e_pattern->get_external_probability();
      //   delta_vertices_y = std::fabs(ptr_2e_pattern->get_delta_vertices_y());
      //   delta_vertices_z = std::fabs(ptr_2e_pattern->get_delta_vertices_z());
      // }

      // std::cout << _prob_int_ << " " << _prob_ext_ << " " << _delta_vertices_y_ << " " << _delta_vertices_z_ << std::endl;
      // std::cout << internal_prob << " " << external_prob << " " << delta_vertices_y << " " << delta_vertices_z << std::endl;
      // if (internal_prob < _prob_int_)
      //   check = false;
      // if (external_prob > _prob_ext_)
      //   check = false;
      // if (delta_vertices_y > _delta_vertices_y_)
      //   check = false;
      // if (delta_vertices_z > _delta_vertices_z_)
      //   check = false;

      cut_returned = cuts::SELECTION_REJECTED;
      if (check_pattern_id &&
          check_has_internal_probability &&
          check_has_external_probability &&
          check_range_internal_probability &&
          check_range_external_probability) {
        DT_LOG_DEBUG(get_logging_priority(), "Event rejected by topology cut!");
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
