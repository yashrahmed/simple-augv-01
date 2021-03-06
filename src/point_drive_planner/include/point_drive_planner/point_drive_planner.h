
#ifndef POINT_DRIVE_PLANNER_POINT_DRIVE_PLANNER_ROS_H_
#define POINT_DRIVE_PLANNER_POINT_DRIVE_PLANNER_ROS_H_

#include <ros/ros.h>

// abstract class from which our plugin inherits
#include <nav_core/base_local_planner.h>

// local planner specific classes which provide some macros
#include <base_local_planner/goal_functions.h>

// time
#include <time.h>
#include <math.h>

using namespace std;

// msgs
#include <nav_msgs/Path.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h>

// transforms
#include <angles/angles.h>
#include <tf2_ros/buffer.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

// costmap & geometry
#include <costmap_2d/costmap_2d_ros.h>

// other
#include <array>
#include <vector>
#include <typeinfo>

// definitions
#define PI 3.14159265
#define D2R 0.0174532925 // = 3.14159265/180

namespace point_drive_planner
{

   /**
   * @class PointDrivePlannerROS
   * @brief Plugin to the ros base_local_planner. Implements a wrapper for the Elastic Band Method
   */

   class PointDrivePlannerROS : public nav_core::BaseLocalPlanner
   {

   public:
      /**
       * @brief Default constructor for the ros wrapper
       */
      PointDrivePlannerROS();

      /**
       * @brief Constructs the ros wrapper
       * @param name The name to give this instance of the elastic band local planner
       * @param tf A pointer to a transform listener
       * @param costmap The cost map to use for assigning costs to trajectories
       */
      PointDrivePlannerROS(std::string name, tf2_ros::Buffer *tf_buffer,
                           costmap_2d::Costmap2DROS *costmap_ros);

      /**
       * @brief  Destructor for the wrapper
       */
      ~PointDrivePlannerROS();

      /**
       * @brief Initializes the ros wrapper
       * @param name The name to give this instance of the trajectory planner
       * @param tf A pointer to a transform listener
       * @param costmap The cost map to use for assigning costs to trajectories
       */
      void initialize(std::string name, tf2_ros::Buffer *tf_buffer,
                      costmap_2d::Costmap2DROS *costmap_ros);

      /**
       * @brief Set the plan that the controller is following; also reset Simple-planner
       * @param orig_global_plan The plan to pass to the controller
       * @return True if the plan was updated successfully, false otherwise
       */
      bool setPlan(const std::vector<geometry_msgs::PoseStamped> &orig_global_plan);

      /**
       * @brief Given the current position, orientation, and velocity of the robot, compute velocity commands to send to the base
       * @param cmd_vel Will be filled with the velocity command to be passed to the robot base
       * @return True if a valid trajectory was found, false otherwise
       */
      bool computeVelocityCommands(geometry_msgs::Twist &cmd_vel);

      /**
       * @brief  Check if the goal pose has been achieved
       * @return True if achieved, false otherwise
       */
      bool isGoalReached();

   private:
      //Pointer to external objects (do NOT delete object)
      costmap_2d::Costmap2DROS *costmap_ros_; ///<@brief pointer to costmap
      tf2_ros::Buffer *tf_buffer;

      // Topics & Services
      ros::Subscriber odom_sub;

      // Data
      std::vector<geometry_msgs::PoseStamped> plan; // contains the global plan
      geometry_msgs::Twist cmd;                     // contains the velocity
      visualization_msgs::Marker points;
      geometry_msgs::Pose current_pose;

      static const string ODOM_INPUT_TOPIC;
      static const double DEFAULT_THETA_Z_TOLERANCE;
      static const double DEFAULT_POS_TOLERANCE;
      static const double DEFAULT_DRIVE_MODE_THETA_Z_THRESHOLD;
      static const double DEFAULT_PLAN_WINDOW_DIST_THRESHOLD;

      double theta_z_tolerance;
      double pos_tolerance;
      double drive_mode_theta_z_threshold;
      double plan_window_dist_threshold;
      // Flags
      bool goal_reached_;
      bool initialized_;

      double getDistance(geometry_msgs::PoseStamped endPose);
      double getTurnAngle(geometry_msgs::PoseStamped endPose);
      void odomCallback(const nav_msgs::Odometry::ConstPtr &msg);
   };
}; // namespace point_drive_planner

#endif
