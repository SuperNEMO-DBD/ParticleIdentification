/** \file falaise/snemo/datamodels/topology_1e_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/topology_1e_builder.h>
#include <falaise/snemo/reconstruction/angle_driver.h>
#include <falaise/snemo/reconstruction/energy_driver.h>
#include <falaise/snemo/datamodels/topology_1e_pattern.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/energy_measurement.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(topology_1e_builder,
                                                                    "snemo::reconstruction::topology_1e_builder")

    snemo::datamodel::base_topology_pattern::handle_type topology_1e_builder::create_pattern()
    {
      snemo::datamodel::base_topology_pattern::handle_type h(new snemo::datamodel::topology_1e_pattern);
      return h;
    }

    void topology_1e_builder::make_measurements(snemo::datamodel::base_topology_pattern & pattern_)
    {
      const std::string e1_label = "e1";
      DT_THROW_IF(! pattern_.has_particle_track(e1_label), std::logic_error,
                  "No particle with label '" << e1_label << "' has been stored !");
      auto e1 = pattern_.get_particle_track(e1_label);

      auto meas = pattern_.get_measurement_dictionary();
      auto& drivers = base_topology_builder::get_measurement_drivers();

      if (drivers.AMD) {
        double electronAngle = drivers.AMD->process(e1);
        meas["angle_" + e1_label].reset(new snemo::datamodel::angle_measurement(electronAngle));
      }

      {
        snemo::datamodel::energy_measurement * ptr_energy = new snemo::datamodel::energy_measurement;
        meas["energy_" + e1_label].reset(ptr_energy);
        if (drivers.EMD) drivers.EMD->process(e1, *ptr_energy);
      }
    }

  } // end of namespace reconstruction

} // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
