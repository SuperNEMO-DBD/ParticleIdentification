/** \file falaise/snemo/datamodels/vertex_measurement.cc
 */

// Ourselves:
#include <falaise/snemo/datamodels/vertex_measurement.h>

namespace snemo {

  namespace datamodel {

    // Serial tag for datatools::i_serializable interface :
    DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(vertex_measurement,
                                                      "snemo::datamodel::vertex_measurement")

    vertex_measurement::vertex_measurement()
    {
      _vertex_.invalidate();
      datatools::invalidate(_probability_);
    }

    vertex_measurement::~vertex_measurement()
    {
    }

    bool vertex_measurement::has_vertex() const
    {
      // Only test if placement is valid (blur_spot validation implies a valid
      // base_hit i.e. valid geom_id + valid errors...)
      // return _vertex_.get_placement().is_valid();
      return has_probability();
    }

    const geomtools::blur_spot & vertex_measurement::get_vertex() const
    {
      return _vertex_;
    }

    geomtools::blur_spot & vertex_measurement::get_vertex()
    {
      return _vertex_;
    }

    bool vertex_measurement::has_probability() const
    {
      return datatools::is_valid(_probability_);
    }

    double vertex_measurement::get_probability() const
    {
      return _probability_;
    }

    void vertex_measurement::set_probability(const double probability_)
    {
      _probability_ = probability_;
    }

    bool vertex_measurement::has_vertices_distance() const
    {
      return (datatools::is_valid(_vertex_.get_x_error()) &&
              datatools::is_valid(_vertex_.get_y_error()) &&
              datatools::is_valid(_vertex_.get_z_error()));
      // return (datatools::is_valid(_vertex_.get_y_error()) &&
      //         datatools::is_valid(_vertex_.get_z_error()));
    }

    double vertex_measurement::get_vertices_distance_x() const
    {
      return _vertex_.get_x_error();
    }

    double vertex_measurement::get_vertices_distance_y() const
    {
      return _vertex_.get_y_error();
    }

    double vertex_measurement::get_vertices_distance_z() const
    {
      return _vertex_.get_z_error();
    }

    void vertex_measurement::tree_dump(std::ostream      & out_,
                                       const std::string & title_,
                                       const std::string & indent_,
                                       bool inherit_) const
    {
      std::string indent;
      if (! indent_.empty ()) indent = indent_;
      base_topology_measurement::tree_dump(out_, title_, indent_, true);

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Probability: ";
      if (! has_probability()) {
        out_ << "<no value>" << std::endl;
      } else {
        out_ << _probability_/CLHEP::perCent << "%" << std::endl;
      }

      out_ << indent << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Vertex: ";
      if (! has_vertex()) {
        out_ << "<no value>" << std::endl;
      } else {
        out_ << std::endl;
        std::ostringstream indent_oss;
        indent_oss << indent_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
        _vertex_.tree_dump(out_, "", indent_oss.str(), true);
      }
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
