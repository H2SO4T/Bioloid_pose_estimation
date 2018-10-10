#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl/filters/extract_indices.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
#include <iostream>
#include <pcl/filters/statistical_outlier_removal.h>
#include <math.h>
#include <Eigen/Geometry> 

using namespace std;
//Use this class to contain a public member that is used in the callback function

float delta = 0.08;
pcl::PointCloud<pcl::PointXYZ>::Ptr background (new pcl::PointCloud<pcl::PointXYZ>);
pcl::PointCloud<pcl::PointXYZ>::Ptr actualImage(new pcl::PointCloud<pcl::PointXYZ	>);



class cloudHandler{
	public:
    	cloudHandler(){
        pcl_sub = nh.subscribe("/kinect2/sd/points", 10, &cloudHandler::cloudCB, this);
        pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_background_segmentation", 1);
    }

    void cloudCB(const sensor_msgs::PointCloud2& input){  //or const sensor_msgs::PointCloud2ConstPtr& input
        
        sensor_msgs::PointCloud2 output;
        pcl::fromROSMsg(input, *actualImage);
   			
        for (size_t i = 0; i < actualImage->points.size (); ++i){
      		if(abs(actualImage->points[i].z - background->points[i].z) < delta){
        		actualImage->points[i].x = NAN;
        		actualImage->points[i].y = NAN;
        		actualImage->points[i].z = NAN;
      		}
  		}
  		 
        pcl::toROSMsg(*actualImage, output);
        pcl_pub.publish(output);
    }

protected:
    ros::NodeHandle nh;
    ros::Subscriber pcl_sub;
    ros::Publisher pcl_pub;

};

main(int argc, char** argv)
{
    ros::init(argc, argv, "pcl_background_segmentation");

    cloudHandler handler;
    if (pcl::io::loadPCDFile<pcl::PointXYZ> ("/home/bioloid/Desktop/catkin_ws/src/bioloid_pose_estimation/src/environment.pcd", *background) == -1) {
    	PCL_ERROR ("Couldn't read file \n");
    	return (-1);
  	}
    ros::spin();

    return 0;
}
