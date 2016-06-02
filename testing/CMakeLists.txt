# - List of test programs:
set(FalaiseParticleIdentificationPlugin_TESTS
  test_topology_data.cxx
  test_base_topology_pattern.cxx
  test_tof_measurement.cxx
  test_vertex_measurement.cxx
  test_energy_driver.cxx
  test_angle_driver.cxx
  test_vertex_driver.cxx
  test_tof_driver.cxx
  test_tof_measurement_cut.cxx
  )

foreach(_testsource ${FalaiseParticleIdentificationPlugin_TESTS})
  get_filename_component(_testname ${_testsource} NAME_WE)
  set(_testname "falaiseparticleidentificationplugin-${_testname}")
  add_executable(${_testname} ${_testsource})
  target_link_libraries(${_testname} Falaise_ParticleIdentification Falaise)

  add_test(NAME ${_testname} COMMAND ${_testname})
endforeach()

# end of CMakeLists.txt
