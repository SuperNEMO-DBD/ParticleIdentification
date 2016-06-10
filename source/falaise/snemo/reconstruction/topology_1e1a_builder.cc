/** \file falaise/snemo/datamodels/topology_1e1a_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/topology_1e1a_builder.h>
#include <falaise/snemo/reconstruction/angle_driver.h>
#include <falaise/snemo/reconstruction/vertex_driver.h>
#include <falaise/snemo/datamodels/topology_1e1a_pattern.h>
#include <falaise/snemo/datamodels/angle_measurement.h>
#include <falaise/snemo/datamodels/vertex_measurement.h>


namespace datatools {
  template <typename T, typename ...Args>
  datatools::handle<T> make_handle(Args&& ...args) {
    return datatools::handle<T>(new T(std::forward<Args>(args)...));
  }
}


namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    FL_SNEMO_RECONSTRUCTION_TOPOLOGY_BUILDER_REGISTRATION_IMPLEMENT(topology_1e1a_builder,
                                                                    "snemo::reconstruction::topology_1e1a_builder")

    snemo::datamodel::base_topology_pattern::handle_type topology_1e1a_builder::create_pattern()
    {
      snemo::datamodel::base_topology_pattern::handle_type h(new snemo::datamodel::topology_1e1a_pattern);
      return h;
    }

    void topology_1e1a_builder::make_measurements(snemo::datamodel::base_topology_pattern & pattern_)
    {
      snemo::reconstruction::topology_1e_builder::make_measurements(pattern_);

      const std::string e1_label = "e1";
      DT_THROW_IF(! pattern_.has_particle_track(e1_label), std::logic_error,
                  "No particle with label '" << e1_label << "' has been stored !");
      auto e1 = pattern_.get_particle_track(e1_label);

      const std::string a1_label = "a1";
      DT_THROW_IF(! pattern_.has_particle_track(a1_label), std::logic_error,
                  "No particle with label '" << a1_label << "' has been stored !");
      auto a1 = pattern_.get_particle_track(a1_label);

      auto meas = pattern_.get_measurement_dictionary();
      auto& drivers = base_topology_builder::get_measurement_drivers();

      if (drivers.AMD) {
        double alphaFoilAngle = drivers.AMD->process(a1);
        meas["angle_" + a1_label].reset(new snemo::datamodel::angle_measurement(alphaFoilAngle));

        double alphaElectronAngle = drivers.AMD->process(e1, a1);
        meas["angle_" + e1_label + "_" + a1_label].reset(new snemo::datamodel::angle_measurement(alphaElectronAngle));
      }


      {
        snemo::datamodel::vertex_measurement * ptr_vertex_measurement = new snemo::datamodel::vertex_measurement;
        meas["vertex_" + e1_label + "_" + a1_label].reset(ptr_vertex_measurement);
        if (drivers.VD) drivers.VD->process(e1, a1, *ptr_vertex_measurement);
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
