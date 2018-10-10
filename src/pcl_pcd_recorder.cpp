  #include <ros/ros.h>
  #include "std_msgs/String.h"

  #include <pcl/io/pcd_io.h>
  #include <pcl/point_types.h>
#include <pcl_ros/point_cloud.h>
#include <boost/foreach.hpp>
#include <sstream>
#include <string> 
#include <stdio.h>

  
   int i; 

  void pcd_receive_Callback(const sensor_msgs::PointCloud2::ConstPtr& msg)
  {
          pcl::PointCloud<pcl::PointXYZ> cloud;
          pcl::fromROSMsg(*msg, cloud);
          
          

        std::string fileName = "test_pcd" + boost::lexical_cast<std::string>(i);  
        i++;   
          pcl::io::savePCDFileASCII (fileName, cloud);

  }


  int  main (int argc, char** argv)
  {

          ros::init(argc, argv, "pcd_sub");
          ros::NodeHandle n;
          i = 0;
          ros::Subscriber sub = n.subscribe("/kinect2/sd/points", 1, pcd_receive_Callback);
          ros::spin();


  }