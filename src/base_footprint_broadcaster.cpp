#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>



void poseCallback(const nav_msgs::Odometry::ConstPtr msg){
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->pose.pose.position.x,msg->pose.pose.position.y,0));
  tf::Quaternion q;
  q.setRPY(0, 0, 0);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world_frame", "base_footprint"));
}

int main(int argc, char** argv){
  ros::init(argc, argv, "base_footprint");
  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("/vo",10, &poseCallback);

  ros::spin();
  return 0;
};