#ifndef LINEFINDING_H
#define LINEFINDING_H

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/shared_ptr.hpp>

namespace lineFinding{
    boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > lineColoring(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud);

    void coloringOneLine(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud, std::vector<int> inliers, std::vector<int> color);

    std::vector<int> findBestPlane(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud);

    boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > removeSetOfIndices(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud, std::vector<int> indices);

    boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > findOnePlane(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud, std::vector<int> indices);

    void colorEntirePlane(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud, std::vector<int> color);

    std::vector<int> colorRandomizer();
}

#endif // LINEFINDING_H
