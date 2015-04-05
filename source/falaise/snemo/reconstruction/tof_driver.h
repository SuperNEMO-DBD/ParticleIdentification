/** \file falaise/snemo/reconstruction/tof_driver.h
 * Author(s)     : Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date : 2012-10-07
 * Last modified : 2014-02-09
 *
 * Copyright (C) 2012-2014 Xavier Garrido <garrido@lal.in2p3.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Description:
 *
 *   A driver class that wraps the GammaClustering algorithm.
 *
 * History:
 *
 */

#ifndef FALAISE_TOF_PLUGIN_SNEMO_RECONSTRUCTION_TOF_DRIVER_H
#define FALAISE_TOF_PLUGIN_SNEMO_RECONSTRUCTION_TOF_DRIVER_H 1

// Standard library:
#include <string>
#include <list>
#include <map>
#include <vector>

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>

// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/properties.h>

// This project:
#include <falaise/snemo/datamodels/particle_track.h>

namespace geomtools {
  class manager;
}

namespace snemo {

  namespace datamodel {
    class particle_track_data;
  }

  namespace datamodel {
    class topology_data;
  }

  namespace geometry {
    class locator_plugin;
  }

  namespace reconstruction {

    /// Driver for the gamma clustering algorithms
    class tof_driver
    {

    public:

      static const std::string & tof_id();

      /// Constructor
      tof_driver();

      /// Destructor
      ~tof_driver();

      /// Setting logging priority
      void set_logging_priority(const datatools::logger::priority priority_);

      /// Getting logging priority
      datatools::logger::priority get_logging_priority() const;

      //Main process
      int process(const snemo::datamodel::particle_track & pt1_,
                  const snemo::datamodel::particle_track & pt2_,
                  double & proba_int, double & proba_ext);

      /// Check if theclusterizer is initialized
      bool is_initialized() const;

      /// Initialize the gamma tracker through configuration properties
      void initialize(const datatools::properties & setup_);

      /// Reset the clusterizer
      void reset();

    protected:

      /// Set the initialization flag
      void _set_initialized(bool);

      /// Special method to process and generate particle track data
      int _process_algo(const snemo::datamodel::particle_track & particle_1_,
                        const snemo::datamodel::particle_track & particle_2_,
                        double & proba_int_, double & proba_ext_);

      /// Gives the energy of particle_
      double _get_energy(const snemo::datamodel::particle_track & particle_);

      /// Gives the theoretical time of the track
      double _get_theoretical_time(double energy_, double mass_, double track_length_);

      /// Returns the beta
      double _beta(double energy_, double mass_);

      /// Gives the times for two charged particles (single deposit)
      int _get_times(const snemo::datamodel::particle_track & particle_,
                     double & t_, double & sigma_t);

      /// Gives the times of the relevant vertices
      int _get_times(const snemo::datamodel::particle_track & particle_,
                     double & t_first_, double & sigma_t_first,
                     double & t_last_, double & sigma_t_last);

      /// Gives the mass of the particle_
      double _get_mass(const snemo::datamodel::particle_track & particle_);

      /// Gives the track length of the particles
      int _get_track_length(const snemo::datamodel::particle_track & particle1_,
                            const snemo::datamodel::particle_track & particle2_,
                            double & track_length_1_, double & track_lengh_2_);

      /// Gives the track length of an electron
      double _get_electron_track_length(const snemo::datamodel::particle_track & pt_);

      /// Gives the track length of a gamma from the electron vertex
      double _get_gamma_track_length(const snemo::datamodel::particle_track & pt_gamma_,
                                     const snemo::datamodel::particle_track & pt_electron_);

      /// Give default values to specific class members.
      void _set_defaults ();

    private:
      bool _initialized_;            //!< Initialization status
      datatools::logger::priority _logging_priority_; //!< Logging priority
      double _sigma_t_gamma_interaction_uncertainty_;     //!< The uncertainty on the track length
    };

  }  // end of namespace reconstruction

}  // end of namespace snemo


// Declare the OCD interface of the module
#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(snemo::reconstruction::tof_driver)

#endif // FALAISE_TOF_PLUGIN_SNEMO_RECONSTRUCTION_TOF_DRIVER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/