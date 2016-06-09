// falaise/snemo/cuts/energy_measurement_cut.cc

// Ourselves:
#include <falaise/snemo/cuts/energy_measurement_cut.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/clhep_units.h>

// SuperNEMO data models :
#include <falaise/snemo/datamodels/energy_measurement.h>

namespace snemo {
namespace cut {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(energy_measurement_cut, "snemo::cut::energy_measurement_cut")

    void energy_measurement_cut::_set_defaults()
    {
      energyRequired_ = false;
      energyRange_.invalidate();
    }

    bool energy_measurement_cut::energy_required() const
    {
      return energyRequired_;
    }

    energy_measurement_cut::energy_measurement_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _set_defaults();
      this->register_supported_user_data_type<snemo::datamodel::base_topology_measurement>();
      this->register_supported_user_data_type<snemo::datamodel::energy_measurement>();
    }

    energy_measurement_cut::~energy_measurement_cut()
    {
      if (is_initialized()) this->energy_measurement_cut::reset();
    }

    void energy_measurement_cut::reset()
    {
      _set_defaults();
      this->i_cut::_reset();
      this->i_cut::_set_initialized(false);
    }

    void energy_measurement_cut::initialize(const datatools::properties & configuration_,
                                           datatools::service_manager  & /* service_manager_ */,
                                           cuts::cut_handle_dict_type  & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Cut '" << get_name() << "' is already initialized ! ");

      this->i_cut::_common_initialize(configuration_);

      energyRequired_ = configuration_.has_flag("mode.has_energy");

      // Extract the energy bound, with a default, throwing for
      // unphysical values;
      auto get_range_energy = [&configuration_](const std::string& key) {
        double value {0.0*CLHEP::keV};
        if (configuration_.has_key(key)) {
          value = configuration_.fetch_real(key);
          if (!configuration_.has_explicit_unit(key)) {
            value *= CLHEP::keV;
          }
        }
        DT_THROW_IF(value < 0.0,
                    std::range_error,
                    "Unphysical energy in parameter '" << key << "': " << value);
        return value;
      };

      double emin {get_range_energy("range_energy.min")};
      energyRange_.set_lower(emin);

      double emax {get_range_energy("range_energy.max")};
      energyRange_.set_upper(emax);

      // Require normal ordered range (real_range does not enforce this?)
      DT_THROW_IF(emin > emax,
                  std::logic_error,
                  "Range is not normal ordered : range_energy.min (" << emin << ") > 'range_energy.max (" << emax << ")'!");
      this->i_cut::_set_initialized(true);
    }


    int energy_measurement_cut::_accept()
    {
      // Get energy measurement
      const snemo::datamodel::energy_measurement * ptr_meas = 0;
      if (is_user_data<snemo::datamodel::energy_measurement>()) {
        ptr_meas = &(get_user_data<snemo::datamodel::energy_measurement>());
      } else if (is_user_data<snemo::datamodel::base_topology_measurement>()) {
        auto& btm = get_user_data<snemo::datamodel::base_topology_measurement>();
        ptr_meas = dynamic_cast<const snemo::datamodel::energy_measurement *>(&btm);
      } else {
        DT_THROW_IF(true, std::logic_error, "Invalid data type !");
      }

      auto a_energy_meas = *ptr_meas;
      bool haveValidMeasurement = a_energy_meas.has_energy();

      // Check if measurement has energy
      if (this->energy_required() && !haveValidMeasurement) {
        return cuts::SELECTION_REJECTED;
      }

      // Check if measurement has correct angle
      bool energyAccepted = true;

      if (energyRange_.is_valid()) {
        if (!haveValidMeasurement) {
          return cuts::SELECTION_INAPPLICABLE;
        }
        energyAccepted = energyRange_.has(a_energy_meas.get_energy());
      }

      return energyAccepted ? cuts::SELECTION_ACCEPTED : cuts::SELECTION_REJECTED;
    }

}  // end of namespace cut
}  // end of namespace snemo

DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::cut::energy_measurement_cut, ocd_)
{
  ocd_.set_class_name("snemo::cut::energy_measurement_cut");
  ocd_.set_class_description("Cut based on criteria applied to a energy measurement");
  ocd_.set_class_library("falaise");
  // ocd_.set_class_documentation("");

  cuts::i_cut::common_ocd(ocd_);

  {
    // Description of the 'mode.has_energy' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode.has_energy")
      .set_terse_description("Mode requiring energy availability")
      .set_traits(datatools::TYPE_BOOLEAN)
      .add_example("Activate the requested energy mode:: \n"
                   "                                     \n"
                   "  mode.has_energy : boolean = true   \n"
                   "                                     \n"
                   )
      ;
  }
  {
    // Description of the 'range_energy.min' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_energy.min")
      .set_terse_description("Minimal value of the requested ranged energy")
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("energy")
      .set_unit_symbol("keV")
      .add_example("Set a specific minimal value of the energy:: \n"
                   "                                             \n"
                   "  range_energy.min : real as energy = 50 keV \n"
                   "                                             \n"
                   )
      ;
  }

  {
    // Description of the 'range_energy.max' configuration property :
    datatools::configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("range_energy.max")
      .set_terse_description("Maximal value of the requested ranged energy")
      .set_traits(datatools::TYPE_REAL)
      .set_explicit_unit(true)
      .set_unit_label("energy")
      .set_unit_symbol("keV")
      .add_example("Set a specific maximal value of the energy::   \n"
                   "                                               \n"
                   "  range_energy.max : real as energy = 1000 keV \n"
                   "                                               \n"
                   )
      ;
  }

  // Additional configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the     \n"
                               "``datatools::properties`` ASCII format::        \n"
                               "                                                \n"
                               "   mode.has_energy : boolean = true             \n"
                               "   mode.range_energy : boolean = true           \n"
                               "   range_energy.min : real as energy = 50 keV   \n"
                               "   range_energy.max : real as energy = 1000 keV \n"
                               "                                                \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
}
DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation

// Registration macro for class 'snemo::cut::simulated_data_cut' :
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::cut::energy_measurement_cut, "snemo::cut::energy_measurement_cut")

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
