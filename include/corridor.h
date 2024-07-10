#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <random>
#include <vector>

#include <Eigen/Eigen>

#include "planner_config.h"
#include "discretized_trajectory.h"
#include "environment.h"
#include "math/line_segment2d.h"

namespace planning {

using ConvexPolygon = std::vector<Eigen::Vector2d>;
using ConvexPolygons = std::vector<ConvexPolygon>;
// ax + by < c, Vector3d << a, b, c
using Constraints = std::vector<Eigen::Vector3d>;
using CorridorConstraints = std::vector<Constraints>;

using LeftLaneConstraints = std::vector<std::pair<Eigen::Vector3d, math::LineSegment2d>>;
using RightLaneConstraints = std::vector<std::pair<Eigen::Vector3d, math::LineSegment2d>>;

class Corridor {
 public:
  Corridor(const CorridorConfig& config, const Env& env);
  
  void Init(const CorridorConfig& config, const Env& env);

  bool Plan(
      const DiscretizedTrajectory& trajectory, 
      CorridorConstraints* const corridor_constraints,
      ConvexPolygons* const convex_polygons,
      LeftLaneConstraints* const left_lane_constraints,
      RightLaneConstraints* const right_lane_constraints);
  
  std::vector<std::vector<math::Vec2d>> points_for_corridors() {
    return points_for_corridors_;
  }

 private:
  bool BuildCorridorConstraints(
      const DiscretizedTrajectory& trajectory, 
      CorridorConstraints* const corridor_constraints,
      ConvexPolygons* const convex_polygons);
  
  bool BuildCorridor(
      const double origin_x, const double origin_y,  
      const std::vector<math::Vec2d>& points,  
      Constraints* const constraints,
      ConvexPolygon* const polygon);
  
  bool CalLeftLaneConstraints(
      LeftLaneConstraints* const left_lane_constraints);

  bool CalRightLaneConstraints(
      RightLaneConstraints* const right_lane_constraints);

  bool CalConstraintFromPoints(
      const math::Vec2d& start_pt,
      const math::Vec2d& end_pt,
      Eigen::Vector3d* const constraint);
  
  void AddCorridorPoints(
      const TrajectoryPoint& pt, 
      std::vector<math::Vec2d>* const points);

  std::vector<math::Vec2d> LaneBoundarySample(
      const std::vector<math::Vec2d>& lane_boundary);

  Eigen::Vector3d HalfPlaneConstraint(
      const math::Vec2d& start_pt,
      const math::Vec2d& end_pt);
 
 private:
  CorridorConfig config_;
  Env env_;
  std::vector<std::vector<math::Vec2d>> points_for_corridors_;
};

} // namespace planning