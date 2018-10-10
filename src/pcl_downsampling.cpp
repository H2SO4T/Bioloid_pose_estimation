#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/filters/voxel_grid.h>

class cloudHandler
{
private:
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::PointXYZ> cloud_downsampled;
    sensor_msgs::PointCloud2 output;
    pcl::VoxelGrid<pcl::PointXYZ> voxelSampler;

public:
    cloudHandler()
    {
        pcl_sub = nh.subscribe("/pcl_background_segmentation", 10, &cloudHandler::cloudCB, this);
        pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("/pcl_downsampled", 1);
    }

    void cloudCB(const sensor_msgs::PointCloud2 &input)
    {
        pcl::fromROSMsg(input, cloud);
        voxelSampler.setInputCloud(cloud.makeShared());
        voxelSampler.setLeafSize(0.01f, 0.01f, 0.01f);
        voxelSampler.filter(cloud_downsampled);

        pcl::toROSMsg(cloud_downsampled, output);
        pcl_pub.publish(output);

    }
protected:
    ros::NodeHandle nh;
    ros::Subscriber pcl_sub;
    ros::Publisher pcl_pub;
};

main(int argc, char **argv)
{
    ros::init(argc, argv, "pcl_voxel");

    cloudHandler handler;

    ros::spin();

    return 0;
}

