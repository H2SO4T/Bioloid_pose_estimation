#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/QuaternionStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf/transform_datatypes.h>



double posX = 0;
double posY = 0;
double posZ = 0;
double x=1;
double y,z,w=0;
ros::Publisher final_pub;

void odomCallback(const nav_msgs::Odometry::ConstPtr odom){ // imu::CallBack 

	nav_msgs::Odometry final_pose;

	posX = odom->pose.pose.position.x;
	posY = odom->pose.pose.position.y;
	posZ = odom->pose.pose.position.z;


	final_pose.pose.pose.position.x = posX;
	final_pose.pose.pose.position.y = posY;
	final_pose.pose.pose.position.z = posZ;

	final_pose.pose.pose.orientation.x = x;
	final_pose.pose.pose.orientation.y = y;
	final_pose.pose.pose.orientation.z = z;
	final_pose.pose.pose.orientation.w = w;

	final_pub.publish(final_pose);
}


  void imuCallback(sensor_msgs::Imu imu_msg ) { // Odom::CallBack

	nav_msgs::Odometry final_pose;
/*
	tf::Quaternion q_orig, q_rot, q_new;
	double r=0, p=0, y=1.570795; 
	q_rot = tf::createQuaternionFromRPY(r, p, y);

	quaternionMsgToTF(imu_msg.orientation , q_orig);  // Get the original orientation of 'commanded_pose'
	q_new = q_rot*q_orig;  // Calculate the new orientation
	q_new.normalize();
	quaternionTFToMsg(q_new, imu_msg.orientation);  // Stuff the new rotation back into the pose. This requires conversion into a msg type
*/
	x= imu_msg.orientation.x;
	y= imu_msg.orientation.y;
	z= imu_msg.orientation.z;
	w= imu_msg.orientation.w;
	


	final_pose.pose.pose.orientation.x= x;
	final_pose.pose.pose.orientation.y= y;
	final_pose.pose.pose.orientation.z= z;
	final_pose.pose.pose.orientation.w= w;


	final_pose.pose.pose.position.x = posX;
	final_pose.pose.pose.position.y = posY;
	final_pose.pose.pose.position.z = posZ;

	final_pub.publish(final_pose);

}
  
int main(int argc, char **argv) {
  ros::init(argc, argv, "merger");
  ros::NodeHandle n;

  ros::Subscriber im = n.subscribe("/imu_in/data", 10, imuCallback); // 150 Hz
  ros::Subscriber vo = n.subscribe("/vo", 10, odomCallback); // 20 Hz
  final_pub = n.advertise<nav_msgs::Odometry>("/merged_data", 50);
  ros::spin();
  return 0;
}
