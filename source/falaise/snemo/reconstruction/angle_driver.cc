/// \file falaise/snemo/reconstruction/angle_driver.cc

// Ourselves:
#include <snemo/reconstruction/angle_driver.h>

// Standard library:
#include <stdexcept>
#include <sstream>

// This project:
#include <falaise/snemo/datamodels/pid_utils.h>
#include <falaise/snemo/datamodels/particle_track.h>
#include <falaise/snemo/datamodels/angle_measurement.h>

namespace {
/// Return direction of particle track at source foil
/// returned vector is invalid if
/// - particle has no vertex on foil
///
geomtools::vector_3d direction_at_foil(const snemo::datamodel::particle_track & pt_)
{
  // Default invalid direction
  geomtools::vector_3d direction_;
  geomtools::invalidate(direction_);


  // Find first vertex on foil
  geomtools::vector_3d foil_vertex;
  geomtools::invalidate(foil_vertex);

  for (auto& i_vtx : pt_.get_vertices()) {
    if (snemo::datamodel::particle_track::vertex_is_on_source_foil(i_vtx.get())) {
      foil_vertex = i_vtx.get().get_position();
      break;
    }
  }

  if (! geomtools::is_valid(foil_vertex)) {
    return direction_;
  }

  // Check particle type NB: Couples angle driver to PID and PID impl
  if (snemo::datamodel::pid_utils::particle_is_gamma(pt_)) {
    // Get the first vertex on calorimeter (should be the first associated calorimeter)
    for (auto& i_vtx : pt_.get_vertices()) {
      if (snemo::datamodel::particle_track::vertex_is_on_main_calorimeter(i_vtx.get()) ||
          snemo::datamodel::particle_track::vertex_is_on_x_calorimeter(i_vtx.get())    ||
          snemo::datamodel::particle_track::vertex_is_on_gamma_veto(i_vtx.get())) {
        const geomtools::vector_3d& calo_vertex = i_vtx.get().get_position();
        direction_ = calo_vertex - foil_vertex;
        break;
      }
    }
  } else if (pt_.has_trajectory()) {
    auto a_trajectory = pt_.get_trajectory();
    auto& a_track_pattern = a_trajectory.get_pattern();
    direction_ = a_track_pattern.get_shape().get_direction_on_curve(foil_vertex);
  } else {
    return direction_;
  }

  // Return a normalized direction
  direction_ /= direction_.mag();
  return direction_;
}
}


namespace snemo {

  namespace reconstruction {

    const std::string & angle_driver::get_id()
    {
      static const std::string _id("AD");
      return _id;
    }

    // Constructor
    angle_driver::angle_driver()
    {
      //_set_defaults();
    }

    // Destructor
    angle_driver::~angle_driver()
    {
      //if (is_initialized()) {
      //  reset();
      //}
    }


    // Initialization :
    void angle_driver::initialize(const datatools::properties& /*setup_*/)
    {
    }


    double angle_driver::process(const snemo::datamodel::particle_track& pt_)
    {
      double measuredAngle {datatools::invalid_real_double()};

      if (snemo::datamodel::pid_utils::particle_is_gamma(pt_)) {
        //DT_LOG_WARNING(get_logging_priority(),
        //             "No angle can be deduced from a single gamma !");
        // BUT... In get_direction, gamma CAN have an angle deduced
        return datatools::invalid_real_double();
      }

      geomtools::vector_3d particle_dir = direction_at_foil(pt_);

      if (geomtools::is_valid(particle_dir)) {
        geomtools::vector_3d Ox(1,0,0);
        measuredAngle = std::acos(particle_dir * Ox) / M_PI * 180 * CLHEP::degree;
      }

      return measuredAngle;
    }


    double
    angle_driver::process(const snemo::datamodel::particle_track& pt1_,
                          const snemo::datamodel::particle_track& pt2_)
    {
      // Invalidate angle meas.
      double measuredAngle {datatools::invalid_real_double()};

      if (snemo::datamodel::pid_utils::particle_is_gamma(pt1_) &&
          snemo::datamodel::pid_utils::particle_is_gamma(pt2_)) {
        //DT_LOG_WARNING(get_logging_priority(), "The two particles are gammas ! No angle can be measured !");
        return measuredAngle;
      }

      geomtools::vector_3d particle_dir1 = direction_at_foil(pt1_);
      geomtools::vector_3d particle_dir2 = direction_at_foil(pt2_);

      if (geomtools::is_valid(particle_dir1) && geomtools::is_valid(particle_dir2)) {
        measuredAngle = std::acos(particle_dir1 * particle_dir2) / M_PI * 180 * CLHEP::degree;
      }

      return measuredAngle;
    }


  } // end of namespace reconstruction

} // end of namespace snemo
