/** \file falaise/snemo/datamodels/base_topology_builder.cc
 */

// Ourselves:
#include <falaise/snemo/reconstruction/base_topology_builder.h>

// - Falaise:
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/datamodels/pid_utils.h>

namespace snemo {

  namespace reconstruction {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_topology_builder,
                                                     "snemo::reconstruction::base_topology_builder/__system__")


    base_topology_builder::base_topology_builder()
    {
      _drivers = 0;
    }

    base_topology_builder::~base_topology_builder()
    {
    }


    bool base_topology_builder::has_measurement_drivers() const
    {
      return _drivers != 0;
    }

    void base_topology_builder::set_measurement_drivers(const measurement_drivers & drivers_)
    {
      _drivers = &drivers_;
    }

    const measurement_drivers & base_topology_builder::get_measurement_drivers() const
    {
      return *_drivers;
    }


    snemo::datamodel::base_topology_pattern::handle_type
    base_topology_builder::build(const snemo::datamodel::particle_track_data& tracks)
    {
      DT_THROW_IF(! has_measurement_drivers(), std::logic_error, "Missing measurement drivers !");
      auto builtPattern = this->create_pattern();
      this->make_track_dictionary(tracks, builtPattern.grab());
      this->make_measurements(builtPattern.grab());
      return builtPattern;
    }

    void base_topology_builder::make_track_dictionary(const snemo::datamodel::particle_track_data & ptd_,
                                                                  snemo::datamodel::base_topology_pattern& pattern_)
    {
      size_t n_electrons = 0;
      size_t n_positrons = 0;
      size_t n_alphas    = 0;
      size_t n_gammas    = 0;
      auto the_particles = ptd_.get_particles();

      for (auto& i_particle : the_particles) {
        auto a_particle = i_particle.get();
        std::ostringstream key;
        if (snemo::datamodel::pid_utils::particle_is_electron(a_particle)) {
          key << "e" << ++n_electrons;
        }
        else if (snemo::datamodel::pid_utils::particle_is_positron(a_particle)) {
          key << "p" << ++n_positrons;
        }
        else if (snemo::datamodel::pid_utils::particle_is_alpha(a_particle)) {
          key << "a" << ++n_alphas;
        }
        else if (snemo::datamodel::pid_utils::particle_is_gamma(a_particle)) {
          key << "g" << ++n_gammas;
        }
        else {
          continue; // no undefined particles for now
        }
        pattern_.get_particle_track_dictionary()[key.str()] = i_particle;
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
