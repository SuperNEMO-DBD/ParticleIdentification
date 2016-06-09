// falaise/snemo/cuts/angle_measurement_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/angle_measurement_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/things.h>
#include <datatools/clhep_units.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/angle_measurement.h>

namespace snemo {

  namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(angle_measurement_cut, "snemo::cut::angle_measurement_cut")

    void angle_measurement_cut::_set_defaults()
    {
      angleRequired_ = false;
      angleRange_.invalidate();
    }

    bool angle_measurement_cut::angle_required() const
    {
      return angleRequired_;
    }


    angle_measurement_cut::angle_measurement_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      this->register_supported_user_data_type<snemo::datamodel::base_topology_measurement>();
      this->register_supported_user_data_type<snemo::datamodel::angle_measurement>();
    }

    angle_measurement_cut::~angle_measurement_cut()
    {
      if (is_initialized()) this->angle_measurement_cut::reset();
    }

    void angle_measurement_cut::reset()
    {
      this->_set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
    }

    void angle_measurement_cut::initialize(const datatools::properties & configuration_,
                                           datatools::service_manager  & /* service_manager_ */,
                                           cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      angleRequired_ = configuration_.has_flag("mode.has_angle");

      // mode PARTICLE_RANGE_ANGLE:
      datatools::real_range angleLimits(0.0*CLHEP::degree, 360.0*CLHEP::degree);

      // Extract the angle bound, with a default
      auto get_range_angle = [&configuration_, &angleLimits](const std::string& key) {
        double value {angleLimits.get_lower()};
        if (configuration_.has_key(key)) {
          value = configuration_.fetch_real(key);
          if (!configuration_.has_explicit_unit(key)) {
            value *= CLHEP::degree;
          }
        }
        return value;
      };

      double amin {get_range_angle("range_angle.min")};
      DT_THROW_IF(!angleLimits.has(amin),
                  std::range_error,
                  "Invalid minimal angle value (" << amin << ") !");
      angleRange_.set_lower(amin);

      double amax {get_range_angle("range_angle.max")};
      DT_THROW_IF(!angleLimits.has(amax),
                  std::range_error,
                  "Invalid maximal angle (" << amax << ") !");
      angleRange_.set_upper(amax);

      // Require normal ordered range (real_range does not enforce this?)
      DT_THROW_IF(amin > amax,
                  std::logic_error,
                  "Invalid 'range_angle.min' > 'range_angle.max' values !");
      this->i_cut::_set_initialized(true);
    }


    int angle_measurement_cut::_accept()
    {
      // Get angle measurement
      const snemo::datamodel::angle_measurement * ptr_meas = nullptr;

      if (is_user_data<snemo::datamodel::angle_measurement>()) {
        ptr_meas = &(get_user_data<snemo::datamodel::angle_measurement>());
      } else if (is_user_data<snemo::datamodel::base_topology_measurement>()) {
        auto& btm = get_user_data<snemo::datamodel::base_topology_measurement>();
        ptr_meas = dynamic_cast<const snemo::datamodel::angle_measurement *>(&btm);
      } else {
        DT_THROW_IF(true, std::logic_error, "Invalid data type !");
      }
      auto a_angle_meas = *ptr_meas;
      bool haveValidMeasurement = a_angle_meas.is_valid();

      // Check if measurement has angle
      if (this->angle_required() && !haveValidMeasurement) {
        return cuts::SELECTION_REJECTED;
      }

      // Check if measurement has correct angle
      bool angleAccepted = true;

      if (angleRange_.is_valid()) {
        if (!haveValidMeasurement) {
          return cuts::SELECTION_INAPPLICABLE;
        }
        angleAccepted = angleRange_.has(a_angle_meas.get_angle());
      }

      return angleAccepted ? cuts::SELECTION_ACCEPTED : cuts::SELECTION_REJECTED;
    }

  }  // end of namespace cut

}  // end of namespace snemo

DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::cut::angle_measurement_cut, ocd_)
{
  ocd_.set_class_name("snemo::cut::angle_measurement_cut");
  ocd_.set_class_description("Cut based on criteria applied to a angle measurement");
  ocd_.set_class_library("falaise");
  // ocd_.set_class_documentation("");

  cuts::i_cut::common_ocd(ocd_);

  {
    // Description of the 'mode.has_angle' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.has_angle")
      .set_terse_description("Require a valid angle measurement")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the requested angle mode:: \n"
                   "                                    \n"
                   "  mode.has_angle : boolean = true   \n"
                   "                                    \n"
                   )
      ;
  }
  {
    // Description of the 'range_angle.min' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_angle.min")
      .set_terse_description("Minimal value of the requested ranged angle")
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("angle")
      .set_unit_symbol("degree")
      .add_example("Set a specific minimal value of the angle::   \n"
                   "                                              \n"
                   "  range_angle.min : real as angle = 30 degree \n"
                   "                                              \n"
                   )
      ;
  }

  {
    // Description of the 'range_angle.max' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_angle.max")
      .set_terse_description("Maximal value of the requested ranged angle")
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("angle")
      .set_unit_symbol("degree")
      .add_example("Set a specific maximal value of the angle::   \n"
                   "                                              \n"
                   "  range_angle.max : real as angle = 90 degree \n"
                   "                                              \n"
                   )
      ;
  }

  // Additional configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the     \n"
                               "``datatools::properties`` ASCII format::        \n"
                               "                                                \n"
                               "   mode.has_angle : boolean = true              \n"
                               "   range_angle.min : real as angle = 5 degree   \n"
                               "   range_angle.max : real as angle = 100 degree \n"
                               "                                                \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation

// Registration macro for class 'snemo::cut::simulated_data_cut' :
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::cut::angle_measurement_cut, "snemo::cut::angle_measurement_cut")

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
