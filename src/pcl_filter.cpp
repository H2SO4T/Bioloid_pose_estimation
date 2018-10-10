#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/passthrough.h>

//Use this class to contain a public member that is used in the callback function

class cloudHandler
{
private:
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::PointXYZ> cloud_filtered;
    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> statFilter;

public:
    cloudHandler()
    {
        pcl_sub = nh.subscribe("/pcl_downsampled", 10, &cloudHandler::cloudCB, this);
        pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_filtered", 1);
    }

    void cloudCB(const sensor_msgs::PointCloud2& input)
    {
        
        sensor_msgs::PointCloud2 output;
        pcl::fromROSMsg(input, cloud);
        //Algorithm provided by PCL library  that performs	an	analysis	of	the	point	cloud	and	removes	those	points	that	do	not
        //satisfy	a	specific	statistical	property,	which,	in	this	case,	is	the	average	distance	in
        //a	neighborhood,	removing	all	of	those	points	that	deviate	too	much	from	the
        //average.     
        statFilter.setInputCloud(cloud.makeShared());
        statFilter.setMeanK(5); //10
        statFilter.setStddevMulThresh(0.05); //0.5
        statFilter.filter(cloud_filtered);


        //passThrough Filter 
        pcl::PassThrough<pcl::PointXYZ> pass;
        pass.setInputCloud (cloud_filtered.makeShared());
        pass.setFilterFieldName ("z");
        pass.setFilterLimits (0.6, 1.0);
        //pass.setFilterLimitsNegative (true);
        pass.filter (cloud_filtered);

        //pcl::PassThrough<pcl::PointXYZ> pass2;
        pass.setInputCloud (cloud_filtered.makeShared());
        pass.setFilterFieldName ("x");
        pass.setFilterLimits (-0.22, 0.22);
        //pass.setFilterLimitsNegative (true);
        pass.filter (cloud_filtered);



        pcl::toROSMsg(cloud_filtered, output);
        pcl_pub.publish(output);
    }

protected:
    ros::NodeHandle nh;
    ros::Subscriber pcl_sub;
    ros::Publisher pcl_pub;
};

main(int argc, char** argv)
{
    ros::init(argc, argv, "pcl_filter");

    cloudHandler handler;

    ros::spin();

    return 0;
}
