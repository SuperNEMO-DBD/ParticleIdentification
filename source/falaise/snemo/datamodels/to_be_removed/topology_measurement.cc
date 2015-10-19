/** \file falaise/snemo/datamodels/topology_measurement.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/topology_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(TOF_measurement,
                                                      "snemo::datamodel::TOF_measurement")

    TOF_measurement::TOF_measurement()
    {
      return;
    }

    TOF_measurement::~TOF_measurement()
    {
      return;
    }

    const TOF_measurement::probability_type & TOF_measurement::get_internal_probabilities() const
    {
      return _internal_probabilities_;
    }

    TOF_measurement::probability_type & TOF_measurement::grab_internal_probabilities()
    {
      return _internal_probabilities_;
    }

    const TOF_measurement::probability_type & TOF_measurement::get_external_probabilities() const
    {
      return _external_probabilities_;
    }

    TOF_measurement::probability_type & TOF_measurement::grab_external_probabilities()
    {
      return _external_probabilities_;
    }

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(delta_vertices_measurement,
                                                      "snemo::datamodel::delta_vertices_measurement")

    delta_vertices_measurement::delta_vertices_measurement()
    {
      datatools::invalidate(_delta_vertices_y_);
      datatools::invalidate(_delta_vertices_z_);
      return;
    }

    delta_vertices_measurement::~delta_vertices_measurement()
    {
      return;
    }

    bool delta_vertices_measurement::has_delta_vertices_y() const
    {
      return datatools::is_valid(_delta_vertices_y_);
    }

    void delta_vertices_measurement::set_delta_vertices_y(double delta_)
    {
      _delta_vertices_y_ = delta_;
      return;
    }

    const double & delta_vertices_measurement::get_delta_vertices_y() const
    {
      return _delta_vertices_y_;
    }

    double & delta_vertices_measurement::grab_delta_vertices_y()
    {
      return _delta_vertices_y_;
    }

    bool delta_vertices_measurement::has_delta_vertices_z() const
    {
      return datatools::is_valid(_delta_vertices_z_);
    }

    void delta_vertices_measurement::set_delta_vertices_z(double delta_)
    {
      _delta_vertices_z_ = delta_;
      return;
    }

    const double & delta_vertices_measurement::get_delta_vertices_z() const
    {
      return _delta_vertices_z_;
    }

    double & delta_vertices_measurement::grab_delta_vertices_z()
    {
      return _delta_vertices_z_;
    }

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(angle_measurement,
                                                      "snemo::datamodel::angle_measurement")

    angle_measurement::angle_measurement()
    {
      datatools::invalidate(_angle_);
      return;
    }

    angle_measurement::~angle_measurement()
    {
      return;
    }

    bool angle_measurement::has_angle() const
    {
      return datatools::is_valid(_angle_);
    }

    void angle_measurement::set_angle(double angle_)
    {
      _angle_ = angle_;
      return;
    }

    const double & angle_measurement::get_angle() const
    {
      return _angle_;
    }

    double & angle_measurement::grab_angle()
    {
      return _angle_;
    }

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(energy_measurement,
                                                      "snemo::datamodel::energy_measurement")

    energy_measurement::energy_measurement()
    {
      datatools::invalidate(_energy_);
      return;
    }

    energy_measurement::~energy_measurement()
    {
      return;
    }

    bool energy_measurement::has_energy() const
    {
      return datatools::is_valid(_energy_);
    }

    void energy_measurement::set_energy(double energy_)
    {
      _energy_ = energy_;
      return;
    }

    const double & energy_measurement::get_energy() const
    {
      return _energy_;
    }

    double & energy_measurement::grab_energy()
    {
      return _energy_;
    }

  } // end of namespace datamodel

} // end of namespace snemo

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/