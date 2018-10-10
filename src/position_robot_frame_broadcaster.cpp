#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>



void poseCallback(const nav_msgs::Odometry::ConstPtr msg){
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(0, 0, msg->pose.pose.position.z) );
  tf::Quaternion q;
  q.setRPY(0, 0, 0);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_footprint", "position_robot_frame"));
}

int main(int argc, char** argv){
  ros::init(argc, argv, "position_robot_frame");
  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("/vo",10, &poseCallback);

  ros::spin();
  return 0;
};