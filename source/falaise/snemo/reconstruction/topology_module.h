// -*- mode: c++ ; -*-
/** \file falaise/snemo/reconstruction/topology_module.h
 * Author(s) :    Xavier Garrido <garrido@lal.in2p3.fr>
 * Creation date: 2015-02-03
 * Last modified: 2015-02-03
 *
 * Copyright (C) 2015 Xavier Garrido <garrido@lal.in2p3.fr>
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
 *
 * Description:
 *
 *   Module for particle identification within SuperNEMO
 *
 * History:
 *
 */

#ifndef FALAISE_TOPOLOGY_PLUGIN_SNEMO_RECONSTRUCTION_TOPOLOGY_MODULE_H
#define FALAISE_TOPOLOGY_PLUGIN_SNEMO_RECONSTRUCTION_TOPOLOGY_MODULE_H 1

// Third party:
#include <dpp/base_module.h>


namespace snemo {
namespace reconstruction {
    /// \brief The data processing module for the gamma tracking
    class topology_module : public dpp::base_module
    {

    public:
      /// Constructor
      topology_module(datatools::logger::priority = datatools::logger::PRIO_FATAL);

      /// Destructor
      virtual ~topology_module();

      /// Initialization
      virtual void initialize(const datatools::properties  & setup_,
                              datatools::service_manager   & service_manager_,
                              dpp::module_handle_dict_type & module_dict_);

      /// Reset
      virtual void reset();

      /// Data record processing
      virtual process_status process(datatools::things & data_);

    protected:
      /// Give default values to specific class members.
      void _set_defaults();


    private:
      struct TopologyModuleImpl;
      std::unique_ptr<TopologyModuleImpl> tpmImpl_;
      // Macro to automate the registration of the module :
      DPP_MODULE_REGISTRATION_INTERFACE(topology_module)
    };
} // end of namespace reconstruction
} // end of namespace snemo

#include <datatools/ocd_macros.h>

// Declare the OCD interface of the module
DOCD_CLASS_DECLARATION(snemo::reconstruction::topology_module)

#endif // FALAISE_TOPOLOGY_PLUGIN_SNEMO_RECONSTRUCTION_TOPOLOGY_MODULE_H
