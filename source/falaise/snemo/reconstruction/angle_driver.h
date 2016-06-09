/** \file falaise/snemo/reconstruction/angle_driver.h
 * Author(s)     : Steven Calvez <calvez@lal.in2p3.fr>
 * Creation date : 2015-04-01
 * Last modified : 2015-04-01
 *
 * Copyright (C) 2015 Steven Calvez <calvez@lal.in2p3.fr>
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
 *   A driver class that wraps the angle measurement algorithm.
 *
 * History:
 *
 */

#ifndef FALAISE_ANGLE_MEASUREMENT_PLUGIN_SNEMO_RECONSTRUCTION_ANGLE_DRIVER_H
#define FALAISE_ANGLE_MEASUREMENT_PLUGIN_SNEMO_RECONSTRUCTION_ANGLE_DRIVER_H 1

// Standard library:
#include <string>
#include <vector>

// - Bayeux/datatools:
#include <bayeux/datatools/handle.h>
// - Bayeux/geomtools:
#include <bayeux/geomtools/clhep.h>

namespace datatools {
  class properties;
}

namespace snemo {

  namespace datamodel {
    class particle_track;
    class angle_measurement;
    class base_topology_measurement;
  }

  namespace reconstruction {

    /// Driver for the angle measurement algorithms
    class angle_driver
    {
    public:
      /// Dedicated driver id
      static const std::string & get_id();

    public:
      /// Constructor
      angle_driver();

      /// Destructor
      ~angle_driver();


      /// Initialize the driver through configuration properties
      void initialize(const datatools::properties & setup_);

      /// Return angle between foil and trajectory at foil vertex
      double process(const snemo::datamodel::particle_track& pt_);

      /// Return angle between trajectories evaluated at foil vertices
      double process(const snemo::datamodel::particle_track & pt1_,
                     const snemo::datamodel::particle_track & pt2_);

    };

  }  // end of namespace reconstruction

}  // end of namespace snemo


// Declare the OCD interface of the module
//#include <datatools/ocd_macros.h>
//DOCD_CLASS_DECLARATION(snemo::reconstruction::angle_driver)

#endif // FALAISE_ANGLE_MEASUREMENT_PLUGIN_SNEMO_RECONSTRUCTION_ANGLE_DRIVER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
