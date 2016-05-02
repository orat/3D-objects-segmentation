#include "e_normal_estimation.h"

pcl::PointCloud<pcl::PointXYZRGB>::Ptr e_normal_estimation(pcl::PointCloud<pcl::PointXYZRGB>::Ptr pt_cl,
                                                    float radius,
                                                    int max_neighbs,
                                                    float x_scale,
                                                    float y_scale,
                                                    float z_scale,
                                                    float precision)
{
    // the cloud colored by its normal vectors; return value
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr colored_cloud;


    try
    {
        cloud_manip::scale_cloud(pt_cl, x_scale, y_scale, z_scale, precision); // scaling cloud
        std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> cloud_fragments =
                cloud_manip::fragment_cloud(pt_cl, y_scale, precision); // fragmenting cloud for faster treatment

        // estimating the normals for each cloud fragment in parallel
        #pragma omp parallel for schedule(static)
        for (unsigned int i = 0; i < cloud_fragments.size(); i++)
        {
            normal_estimation(cloud_fragments[i], radius, max_neighbs);
        }

        colored_cloud = cloud_manip::merge_clouds(cloud_fragments); // merging fragments to build original cloud

        cloud_manip::scale_cloud(colored_cloud,
                                     (1.0/x_scale),
                                     (1.0/y_scale),
                                     (1.0/z_scale),
                                     precision);    // restoring widop scale

        return colored_cloud;
    }

    catch (std::logic_error logic_error)
    {
        throw std::logic_error(logic_error.what());
    }
}
