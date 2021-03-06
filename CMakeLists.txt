cmake_minimum_required(VERSION 3.3)
project(FalaiseParticleIdentificationPlugin)

include(GNUInstallDirs)

find_package(Falaise 2.1.0 REQUIRED)
###########################################################################################
# - GammaTracking modules:

# - Headers:
list(APPEND FalaiseParticleIdentificationPlugin_HEADERS
  source/falaise/snemo/reconstruction/topology_module.h
  source/falaise/snemo/reconstruction/particle_identification_driver.h
  source/falaise/snemo/reconstruction/topology_driver.h
  source/falaise/snemo/reconstruction/tof_driver.h
  source/falaise/snemo/reconstruction/vertex_driver.h
  source/falaise/snemo/reconstruction/angle_driver.h
  source/falaise/snemo/reconstruction/energy_driver.h
  source/falaise/snemo/reconstruction/base_topology_builder.h
  source/falaise/snemo/reconstruction/topology_1e_builder.h
  source/falaise/snemo/reconstruction/topology_1e1a_builder.h
  source/falaise/snemo/reconstruction/topology_1e1p_builder.h
  source/falaise/snemo/reconstruction/topology_2e_builder.h
  source/falaise/snemo/reconstruction/topology_2p_builder.h
  source/falaise/snemo/reconstruction/topology_1eNg_builder.h
  source/falaise/snemo/reconstruction/topology_2eNg_builder.h
  source/falaise/snemo/cuts/pid_cut.h
  source/falaise/snemo/cuts/topology_data_cut.h
  source/falaise/snemo/cuts/tof_measurement_cut.h
  source/falaise/snemo/cuts/vertices_measurement_cut.h
  source/falaise/snemo/cuts/angle_measurement_cut.h
  source/falaise/snemo/cuts/energy_measurement_cut.h
  source/falaise/snemo/cuts/channel_cut.h
  source/falaise/snemo/datamodels/topology_data.h
  source/falaise/snemo/datamodels/topology_data.ipp
  source/falaise/snemo/datamodels/the_serializable_bis.h
  source/falaise/snemo/datamodels/base_topology_pattern.h
  source/falaise/snemo/datamodels/topology_1e_pattern.h
  source/falaise/snemo/datamodels/topology_2e_pattern.h
  source/falaise/snemo/datamodels/topology_2p_pattern.h
  source/falaise/snemo/datamodels/topology_1eNg_pattern.h
  source/falaise/snemo/datamodels/topology_2eNg_pattern.h
  source/falaise/snemo/datamodels/topology_1e1a_pattern.h
  source/falaise/snemo/datamodels/topology_1e1p_pattern.h
  source/falaise/snemo/datamodels/base_topology_measurement.h
  source/falaise/snemo/datamodels/tof_measurement.h
  source/falaise/snemo/datamodels/vertex_measurement.h
  source/falaise/snemo/datamodels/angle_measurement.h
  source/falaise/snemo/datamodels/energy_measurement.h
  source/falaise/snemo/datamodels/pid_utils.h
  )

# - Sources:
list(APPEND FalaiseParticleIdentificationPlugin_SOURCES
  source/falaise/snemo/reconstruction/topology_module.cc
  source/falaise/snemo/reconstruction/particle_identification_driver.cc
  source/falaise/snemo/reconstruction/topology_driver.cc
  source/falaise/snemo/reconstruction/tof_driver.cc
  source/falaise/snemo/reconstruction/vertex_driver.cc
  source/falaise/snemo/reconstruction/angle_driver.cc
  source/falaise/snemo/reconstruction/energy_driver.cc
  source/falaise/snemo/reconstruction/base_topology_builder.cc
  source/falaise/snemo/reconstruction/topology_1e_builder.cc
  source/falaise/snemo/reconstruction/topology_1e1a_builder.cc
  source/falaise/snemo/reconstruction/topology_1e1p_builder.cc
  source/falaise/snemo/reconstruction/topology_1e_builder.cc
  source/falaise/snemo/reconstruction/topology_2e_builder.cc
  source/falaise/snemo/reconstruction/topology_2p_builder.cc
  source/falaise/snemo/reconstruction/topology_1eNg_builder.cc
  source/falaise/snemo/reconstruction/topology_2eNg_builder.cc
  source/falaise/snemo/cuts/pid_cut.cc
  source/falaise/snemo/cuts/topology_data_cut.cc
  source/falaise/snemo/cuts/tof_measurement_cut.cc
  source/falaise/snemo/cuts/vertices_measurement_cut.cc
  source/falaise/snemo/cuts/angle_measurement_cut.cc
  source/falaise/snemo/cuts/energy_measurement_cut.cc
  source/falaise/snemo/cuts/channel_cut.cc
  source/falaise/snemo/datamodels/topology_data.cc
  source/falaise/snemo/datamodels/the_serializable_bis.cc
  source/falaise/snemo/datamodels/base_topology_pattern.cc
  source/falaise/snemo/datamodels/topology_1e_pattern.cc
  source/falaise/snemo/datamodels/topology_2e_pattern.cc
  source/falaise/snemo/datamodels/topology_2p_pattern.cc
  source/falaise/snemo/datamodels/topology_1eNg_pattern.cc
  source/falaise/snemo/datamodels/topology_2eNg_pattern.cc
  source/falaise/snemo/datamodels/topology_1e1a_pattern.cc
  source/falaise/snemo/datamodels/topology_1e1p_pattern.cc
  source/falaise/snemo/datamodels/base_topology_measurement.cc
  source/falaise/snemo/datamodels/tof_measurement.cc
  source/falaise/snemo/datamodels/vertex_measurement.cc
  source/falaise/snemo/datamodels/angle_measurement.cc
  source/falaise/snemo/datamodels/energy_measurement.cc
  source/falaise/snemo/datamodels/pid_utils.cc
  )

###########################################################################################

# Build a dynamic library from our sources
add_library(Falaise_ParticleIdentification SHARED
  ${FalaiseParticleIdentificationPlugin_HEADERS}
  ${FalaiseParticleIdentificationPlugin_SOURCES})
target_include_directories(Falaise_ParticleIdentification
  PUBLIC
    ${PROJECT_SOURCE_DIR}/source
    ${PROJECT_SOURCE_DIR}/source/falaise
  )
target_link_libraries(Falaise_ParticleIdentification FalaiseModule)

# Install it:
install(TARGETS Falaise_ParticleIdentification DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Test support:
enable_testing()
add_subdirectory(testing)
