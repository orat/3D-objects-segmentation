#ifndef CLOUD_MANIP_H
#define CLOUD_MANIP_H

#include "../geom_op/geom_op.h"
#include "../objects/point_xy_greyscale.h"
#include "../objects/greyscale_image.h"
#include "../objects/point_clstr.h"

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <float.h>
#include <exception>

namespace cloud_manip
{
    /** @return all of the x coordinates found in the parameter cloud */
    std::vector<float> cloud_x_coords(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_ptr);

    /** @return all of the y coordinates found in the parameter cloud */
    std::vector<float> cloud_y_coords(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_ptr);

    /** @return all of the z coordinates found in the parameter cloud */
    std::vector<float> cloud_z_coords(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_ptr);

    /**
     * @brief copy_cloud copies a cloud into another cloud
     * @param src is a pointer to the source cloud
     * @param dest is a pointer to the destination cloud
     */
    void copy_cloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr src_ptr,
                    pcl::PointCloud<pcl::PointXYZRGB>::Ptr dest_ptr);

    /**
     * @brief scale_cloud rescales a cloud by rescaling the coordinates of all of its points
     * @param cloud_ptr is a pointer to the point cloud to be modified
     * @param x_scale is the float by which the x coordinate will be multiplied
     * @param y_scale is the float by which the y coordinate will be multiplied
     * @param z_scale is the float by which the z coordinate will be multiplied
     * @param precision is used for float comparison
     */
    void scale_cloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_ptr, float x_scale, float y_scale,
                     float z_scale, float precision);

    /**
     * @brief fragment_cloud breaks a cloud down into smaller pieces along the y axis
     * @details only works on clouds using the y axis to represent depth
     * @param cloud_ptr is a pointer to the point cloud to be fragmented
     * @param max_scaled_fragment_depth is the maximum depth of a fragment taking scale into account
     * @param precision is the precision for the float comparison
     * @return a vector containing former cloud fragments
     */
    std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> fragment_cloud(
            pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_ptr,
            float max_scaled_fragment_depth, float precision);


    /**
     * @brief crop_cloud removes the points of which the coordinates are beyond a certain threshold
     * @param cloud_ptr is a pointer to the point cloud to be treated
     * @param x_thresh is the threshold for the x coordinate
     * @param y_thresh is the threshold for the y coordinate
     * @param z_thresh is the threshold for the z coordinate
     * @param precision is used for float comparison
     * @return a cropped point cloud
     */
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr crop_cloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_ptr,
                           float x_thresh, float y_thresh, float z_thresh, float precision);

    /**
     * @brief merge_clouds merges cloud fragments into one cloud
     * @param cloud_fragments is an array of cloud fragments
     * @return a pointer the cloud resulted from merging the cloud fragments
     */
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr merge_clouds(
            std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> cloud_fragments);

    /**
     * @brief cloud_to_greyscale turns a 3D colored cloud into a 2D greyscale points vector
     * @param cloud_ptr is a pointer the point cloud to be transformed to greyscale
     * @return an array of 2D greyscale points
     */
     std::vector<point_xy_greyscale> cloud_to_greyscale(
            pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_ptr);

     /**
      * @brief cloud_homogenization homogenizes the similar colors within a cloud
      * @details if two points have similar but not identical colors they will be attributed the same color
      * @param cloud a pointer to the point cloud to be homogenized
      * @param epsilon defines the 3 dimensions of the cube used to regroup colors
      */
     void cloud_homogenization(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_ptr, short epsilon);

     /**
     * @brief convertBoolToXYZRGB converts a CLSTR point_clstr cloud into a PCL RGB cloud, so its point can be written in a file
     * @param cloud_bool the CLSTR point_clstr from where the data are taken
     * @param cloud_RGB the PCL RGB cloud cloud where the data will be converted
     */
    void convertBoolToXYZRGB(pcl::PointCloud<clstr::point_clstr>::Ptr cloud_bool, pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_RGB);

    /**
     * @brief convertBoolToXYZRGB converts a PCL RGB cloud into a CLSTR point_clstr cloud, used for the clustering and bounding algorithm
     * @param cloud_bool the CLSTR point_clstr cloud where the data will be converted
     * @param cloud_RGB the PCL RGB cloud from where the data are taken
     */
    void convertXYZRGBToBool(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_RGB, pcl::PointCloud<clstr::point_clstr>::Ptr cloud_bool);

    void giveRandomColorToCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud);


}

#endif // CLOUD_MANIP_H

