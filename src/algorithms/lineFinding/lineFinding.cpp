#include "lineFinding.h"

#include <pcl/common/io.h>
#include <pcl/common/common.h>
#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_line.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/common/transforms.h>
#include <Eigen/StdVector>
#include <pcl/cloud_iterator.h>
#include <iterator>
#include <pcl/filters/extract_indices.h>
#include <stdlib.h>
#include <time.h>
#include <pcl/sample_consensus/rransac.h>


boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > lineFinding::lineColoring(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud){

    srand(time(NULL));

    boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > temp (new pcl::PointCloud<pcl::PointXYZRGB>);
    boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > colored (new pcl::PointCloud<pcl::PointXYZRGB>);
    *temp = *cloud;
    colored->clear();

    int i = 0;
    while( i<1 && temp->size()>1000){
        std::vector<int> inliers;
        Eigen::VectorXf coef;
        Plane* p = findBestPlane(temp, inliers, coef);
        inliers = p->getInliers();
        coef = p->getCoefficients();
        std::vector<int> color = colorRandomizer();
        boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > tempPlane (new pcl::PointCloud<pcl::PointXYZRGB>);

        tempPlane = findOnePlane(temp, inliers);
        temp = removeSetOfIndices(temp, inliers);

        std::cout << "step #" <<i+1<< " | # of points in temp:"<<temp->size() << std::endl;
        std::cout << "# of inliers:" << inliers.size() << std::endl;

        if(inliers.size()>1000){
            colorEntirePlane(tempPlane, color);
            *colored += *tempPlane;

        }


        i++;
    }

    return colored;
}

Plane*  lineFinding::findBestPlane(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud, std::vector<int> inliers, Eigen::VectorXf coef){

    pcl::SampleConsensusModelPlane<pcl::PointXYZRGB>::Ptr model (new pcl::SampleConsensusModelPlane<pcl::PointXYZRGB> (cloud));
    pcl::RandomSampleConsensus<pcl::PointXYZRGB> ransac (model);
    ransac.setDistanceThreshold(0.01);
    ransac.computeModel();
    ransac.getInliers(inliers);
    ransac.getModelCoefficients(coef);

    Plane* p = new Plane(inliers, coef);


    std::cout << "plane coef: " << coef[0] << " | " << coef[1] << " | " << coef[2] << " | " << coef[3] << " | " << std::endl;

    return p;
}

boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > lineFinding::removeSetOfIndices(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud, std::vector<int> indices){

    boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > temp (new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::IndicesPtr indicesPtr (new std::vector<int>(indices));
    pcl::ExtractIndices<pcl::PointXYZRGB> filter;
    filter.setNegative(true);
    filter.setInputCloud(cloud);
    filter.setIndices(indicesPtr);
    filter.filter(*temp);

    return temp;
}

boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > lineFinding::findOnePlane(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud, std::vector<int> indices){

    boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > res (new pcl::PointCloud<pcl::PointXYZRGB>);
    for(uint i=0; i<indices.size(); i++){
        res->points.push_back(cloud->at(indices[i]));
    }
    return res;
}

void lineFinding::colorEntirePlane(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud, std::vector<int> color){
    pcl::PointCloud<pcl::PointXYZRGB>::iterator it;

    for(it=cloud->begin(); it<cloud->end(); it++){
        (*it).r = color[0];
        (*it).g = color[1];
        (*it).b = color[2];
    }
}

std::vector<int> lineFinding::colorRandomizer(){

    int r = rand()%256;
    int g = rand()%256;
    int b = rand()%256;

    std::vector<int> color;
    color.push_back(r);
    color.push_back(g);
    color.push_back(b);

    std::cout <<"r:"<<color[0]<<" g:"<<color[1]<<" b:"<<color[2]<< std::endl;
    return color;

}

void lineFinding::coloringOneLine(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud, std::vector<int> inliers, std::vector<int> color){

    for(uint i=0; i<inliers.size(); i++){
        cloud->points[inliers[i]].r=color[0];
        cloud->points[inliers[i]].g=color[1];
        cloud->points[inliers[i]].b=color[2];
    }
}

void lineFinding::findLinesInYDirection(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud){
    srand(time(NULL));

    boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > temp (new pcl::PointCloud<pcl::PointXYZRGB>);
    boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > colored (new pcl::PointCloud<pcl::PointXYZRGB>);
    *temp = *cloud;
    colored->clear();

    int i = 0;
    while( i<10 && temp->size()>1000){
        std::vector<int> inliers;
        Eigen::VectorXf coef;
        Line* l = findALineInYDirection(temp);
        if(l!=NULL){
            inliers = l->getInliers();
            coef = (*l->getCoefficients());
            l->getAnglesToOrigin();
            std::vector<int> color = colorRandomizer();
            boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > tempLine (new pcl::PointCloud<pcl::PointXYZRGB>);

            tempLine = findOnePlane(temp, inliers);
            temp = removeSetOfIndices(temp, inliers);

            std::cout << "step #" <<i+1<< " | # of points in temp:"<<temp->size() << std::endl;
            std::cout << "# of inliers:" << inliers.size() << std::endl;

            if(inliers.size()>0){
                colorEntirePlane(tempLine, color);
                *colored += *tempLine;

            }
        }
        i++;
    }

    cloud->clear();
    *cloud = *colored;
}

Line* lineFinding::findALineInYDirection(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud){

    pcl::SampleConsensusModelLine<pcl::PointXYZRGB>::Ptr model (new pcl::SampleConsensusModelLine<pcl::PointXYZRGB> (cloud));
    pcl::RandomSampleConsensus<pcl::PointXYZRGB> ransac (model);
    //pcl::RandomizedRandomSampleConsensus<pcl::PointXYZRGB> ransac (model);
    std::vector<int> inliers;
    Eigen::VectorXf coefficients;
    ransac.setDistanceThreshold(0.01);
    Line* l = NULL;
    bool exit = false;
    int i = 0;
    while(!exit){
        ransac.computeModel();
        ransac.getInliers(inliers);
        ransac.getModelCoefficients(coefficients);

        float x = fabs(coefficients[3]);
        float y = fabs(coefficients[4]);
        float z = fabs(coefficients[5]);
        i++;
        if((y-x)>0 && (y-z)>0){
            l = new Line(inliers, &coefficients);
            exit = true;
        }else{
            if(i >= 8){
                exit = true;
            }
        }
    }

    return l;
}


pcl::PointCloud<pcl::PointXYZRGB>::Ptr lineFinding::findLinesInClusters(std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> clusters){
    std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr>::iterator it = clusters.begin();
    std::vector<Line*> lines;
    std::vector<Line*> temp;

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr res (new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr temp2 (new pcl::PointCloud<pcl::PointXYZRGB>);

    for(it = clusters.begin(); it != clusters.end(); it++){

        temp2 = findLines(*it);
        *res += *temp2;
//        temp = findLines(*it);

//        lines.insert(lines.end(), temp.begin(), temp.end());
    }

    return res;
}

pcl::PointCloud<pcl::PointXYZRGB>::Ptr lineFinding::findLines(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud){

    srand(time(NULL));

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr temp (new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr colored (new pcl::PointCloud<pcl::PointXYZRGB>);
    std::vector<Line*> lines;
    *temp = *cloud;

    int i = 0;
    while( i<25 && temp->size()>1000){
        std::vector<int> inliers;
        Eigen::VectorXf* coef;
        Line* l = findBestLine(temp);
        if(l!=NULL){
            inliers = l->getInliers();
            coef = l->getCoefficients();
            Line* ltemp = new Line(l->getInliers(),l->getCoefficients());


            lines.push_back(ltemp);

            std::vector<int> color = colorRandomizer();
            boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > tempPlane (new pcl::PointCloud<pcl::PointXYZRGB>);

            tempPlane = findOnePlane(temp, inliers);

            colorEntirePlane(tempPlane, color);
            *colored += *tempPlane;

            temp = removeSetOfIndices(temp, inliers);

        }
        i++;
    }

    cloud->clear();
//    return lines;
    return colored;
}

/*
bool lineFinding::keepLine(std::vector<Line*> lines, Line* line){

    std::vector<Line*>::iterator it;
    bool keep = true;
    Eigen::VectorXf* coefficients1 = line->getCoefficients();

    for(it=lines.begin(); it!=lines.end(); it++){
        Eigen::VectorXf* coefficients2 = (*it)->getCoefficients();
        double x,y,z;
        x = fabs((*coefficients1)[0]-(*coefficients2)[0]);
        y = fabs((*coefficients1)[1]-(*coefficients2)[1]);
        z = fabs((*coefficients1)[2]-(*coefficients2)[2]);
        double dirX,dirY,dirZ;
        dirX = fabs((*coefficients1)[3]-(*coefficients2)[3]);
        dirY = fabs((*coefficients1)[4]-(*coefficients2)[4]);
        dirZ = fabs((*coefficients1)[5]-(*coefficients2)[5]);

        double test, test2;
        test = (*coefficients1)[0]; //dirX+dirY+dirZ
        test2 = x+y+z; //x+y+z
        std::cout << "  test:" << test << " | test2:" << test2 << std::endl;

        if(test<0.2 && test2<0.2){
            keep = false;
        }
    }

    return keep;
}
*/


Line* lineFinding::findBestLine(boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud){

    pcl::SampleConsensusModelLine<pcl::PointXYZRGB>::Ptr model (new pcl::SampleConsensusModelLine<pcl::PointXYZRGB> (cloud));
    pcl::RandomSampleConsensus<pcl::PointXYZRGB> ransac (model);
    std::vector<int> inliers;
    Eigen::VectorXf coefficients;
    ransac.setDistanceThreshold(0.01);
    Line* l = NULL;
    ransac.computeModel();
    ransac.getInliers(inliers);
    ransac.getModelCoefficients(coefficients);
    l = new Line(inliers, &coefficients);

    return l;
}

std::vector<float> lineFinding::findIntersections(std::vector<Line*> lines){

    int length = lines.size();
    int i, j;
    std::vector<float> angles;
    Line* l = new Line();
    float angle;

    for(i = 0; i<length-1; i++){
        for(j = i; j<length; j++){
            angle = l->angleBetweenLines(lines.at(i), lines.at(j));
            if(angle != 0) angles.push_back(angle);
        }
    }

    return angles;
}
