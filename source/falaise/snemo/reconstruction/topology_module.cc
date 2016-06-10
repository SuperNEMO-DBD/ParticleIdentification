/// \file falaise/snemo/reconstruction/topology_module.cc

// Ourselves:
#include <snemo/reconstruction/topology_module.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/service_manager.h>
// - Bayeux/cuts:
#include <cuts/cut_service.h>
#include <cuts/cut_manager.h>

// This project:
#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/particle_track_data.h>
#include <falaise/snemo/datamodels/topology_data.h>
#include <falaise/snemo/processing/services.h>

#include <snemo/reconstruction/particle_identification_driver.h>
#include <snemo/reconstruction/topology_driver.h>

namespace snemo {

  namespace reconstruction {

    // Registration instantiation macro :
    DPP_MODULE_REGISTRATION_IMPLEMENT(topology_module,
                                      "snemo::reconstruction::topology_module")


    /// Private struct holding the implementation details
    struct topology_module::TopologyModuleImpl {
      std::string inputBank; //!< The label of the input data bank
      std::string outputBank;  //!< The label of the output data bank
      snemo::reconstruction::particle_identification_driver pidDriver; //! pid driver instance
      snemo::reconstruction::topology_driver topoDriver; //! topology driver instance
    };

    // Constructor :
    topology_module::topology_module(datatools::logger::priority p)
    : dpp::base_module(p), tpmImpl_(new TopologyModuleImpl)
    {
      _set_defaults();
    }

    // Destructor :
    topology_module::~topology_module()
    {
      if (is_initialized()) topology_module::reset();
    }



    void topology_module::_set_defaults()
    {
      tpmImpl_->inputBank= snemo::datamodel::data_info::default_particle_track_data_label();
      tpmImpl_->outputBank = "TD";//snemo::datamodel::data_info::default_topology_data_label();
      tpmImpl_->pidDriver.reset();
      tpmImpl_->topoDriver.reset();
    }

    // Initialization :
    void topology_module::initialize(const datatools::properties  & setup_,
                                     datatools::service_manager   & service_manager_,
                                     dpp::module_handle_dict_type & /* module_dict_ */)
    {
      DT_THROW_IF (is_initialized(),
                   std::logic_error,
                   "Module '" << get_name() << "' is already initialized ! ");

      dpp::base_module::_common_initialize(setup_);

      if (setup_.has_key("PTD_label")) {
        tpmImpl_->inputBank = setup_.fetch_string("PTD_label");
      }

      if (setup_.has_key("TD_label")) {
        tpmImpl_->outputBank = setup_.fetch_string("TD_label");
      }

      // Cut manager :
      std::string cut_label = snemo::processing::service_info::default_cut_service_label();
      if (setup_.has_key("Cut_label")) {
        cut_label = setup_.fetch_string("Cut_label");
      }
      DT_THROW_IF(cut_label.empty(), std::logic_error,
                  "Module '" << get_name() << "' has no valid '" << "Cut_label" << "' property !");
      DT_THROW_IF(! service_manager_.has(cut_label) ||
                  ! service_manager_.is_a<cuts::cut_service>(cut_label),
                  std::logic_error,
                  "Module '" << get_name() << "' has no '" << cut_label << "' service !");
      auto Cut = service_manager_.grab<cuts::cut_service>(cut_label);

      // Drivers
      tpmImpl_->pidDriver.set_cut_manager(Cut.grab_cut_manager());
      datatools::properties PID_config;
      setup_.export_and_rename_starting_with(PID_config, particle_identification_driver::get_id() + ".", "");
      tpmImpl_->pidDriver.initialize(PID_config);
      tpmImpl_->topoDriver.initialize(setup_);

      _set_initialized(true);
    }

    void topology_module::reset()
    {
      DT_THROW_IF (! is_initialized(), std::logic_error,
                   "Module '" << get_name() << "' is not initialized !");
      _set_initialized(false);
      _set_defaults();
    }


    // Processing :
    dpp::base_module::process_status topology_module::process(datatools::things & data_record_)
    {
      // Validate status/input
      DT_THROW_IF (!this->is_initialized(),
                   std::logic_error,
                   "Module '" << get_name() << "' is not initialized !");

      DT_THROW_IF(!data_record_.has(tpmImpl_->inputBank),
                  std::runtime_error,
                  "Missing particle track data to be processed !");


      // Grab the 'particle_track_data' entry from the data model :
      auto particleTrackData = data_record_.grab<snemo::datamodel::particle_track_data>(tpmImpl_->inputBank);

      // Prepare process by running the PID driver
      tpmImpl_->pidDriver.process(particleTrackData);

      // Prepare output bank
      if (!data_record_.has(tpmImpl_->outputBank)) {
        data_record_.add<snemo::datamodel::topology_data>(tpmImpl_->outputBank);
      }
      auto topologyData = data_record_.grab<snemo::datamodel::topology_data>(tpmImpl_->outputBank);
      topologyData.reset();

      // Main processing method via the topology driver
      tpmImpl_->topoDriver.process(particleTrackData, topologyData);

      return dpp::base_module::PROCESS_SUCCESS;
    }

  } // end of namespace reconstruction

} // end of namespace snemo

/* OCD support */
#include <datatools/object_configuration_description.h>
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(snemo::reconstruction::topology_module, ocd_)
{
  ocd_.set_class_name("snemo::reconstruction::topology_module");
  ocd_.set_class_description("A module that considers the event topology");
  ocd_.set_class_library("Falaise_ParticleIdentification");
  ocd_.set_class_documentation("This module uses the ``snemo::datamodel::particle_track_data`` bank               \n"
                               "and, given the particles identified, computes relevant topology quantities before \n"
                               "storing them in ``snemo::datamodel::topology_data.``                              \n");

  dpp::base_module::common_ocd(ocd_);

  {
    // Description of the 'PTD_label' configuration property :
    datatools::configuration_property_description & cpd
      = ocd_.add_property_info();
    cpd.set_name_pattern("PTD_label")
      .set_terse_description("The label/name of the 'particle track data' bank")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("This is the name of the input bank to be used as           \n"
                            "the source of calorimeter hits and reconstructed vertices. \n")
      .set_default_value_string(snemo::datamodel::data_info::default_particle_track_data_label())
      .add_example("Use an alternative name for the 'particle track data' bank:: \n"
                   "                                                             \n"
                   "  PTD_label : string = \"PTD2\"                              \n"
                   "                                                             \n"
                   );
  }

  {
    // Description of the 'TD_label' configuration property :
    datatools::configuration_property_description & cpd
      = ocd_.add_property_info();
    cpd.set_name_pattern("TD_label")
      .set_terse_description("The label/name of the 'topology data' bank")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("This is the name of the output bank to be used \n"
                            "to select events based on their topology.      \n")
      .set_default_value_string("TD")//snemo::datamodel::data_info::default_topology_data_label())
      .add_example("Use an alternative name for the 'topology data' bank:: \n"
                   "                                                       \n"
                   "  TD_label : string = \"TD2\"                          \n"
                   "                                                       \n"
                   );
  }

  {
    datatools::configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("drivers")
      .set_terse_description("The driver ids to be used")
      .set_traits(datatools::TYPE_STRING,
                  datatools::configuration_property_description::ARRAY)
      .set_mandatory(false)
      .add_example("Use TOF driver only::            \n"
                   "                                 \n"
                   "  drivers : string[1] = \"TOFD\" \n"
                   "                                 \n"
                   );
  }

  // Invoke specific OCD support from the driver class:
  ::snemo::reconstruction::topology_driver::init_ocd(ocd_);

  // Additionnal configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the      \n"
                               "``datatools::properties`` ASCII format::         \n"
                               "                                                 \n"
                               "  PTD_label : string = \"PTD\"                   \n"
                               "  TD_label  : string = \"TD\"                    \n"
                               "  drivers   : string[3] = \"TOFD\" \"VD\" \"AD\" \n"
                               "  TOFD.logging.priority : string = \"error\"     \n"
                               "  VD.logging.priority  : string = \"error\"      \n"
                               "  AMD.logging.priority  : string = \"error\"     \n"
                               "                                                 \n"
                               );

  ocd_.set_validation_support(true);
  ocd_.lock();
}

DOCD_CLASS_IMPLEMENT_LOAD_END() // Closing macro for implementation
DOCD_CLASS_SYSTEM_REGISTRATION(snemo::reconstruction::topology_module,
                               "snemo::reconstruction::topology_module")
