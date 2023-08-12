#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc/types_c.h>
#include <Eigen/Core>
#include <opencv2/core/eigen.hpp>

#include "yaml-cpp/yaml.h"
#include "cameracalibration.h"
#include "apriltags/TagDetector.h"
#include "apriltags/Tag16h5.h"


// using namespace std;
using namespace cv;
using namespace Eigen;

int main()
{

    // CameraCalibration *cameraclib;
    // cameraclib = new CameraCalibration();
    std::vector<cv::Point2f> image_det;
    AprilTags::TagDetector tagDetector(AprilTags::tagCodes16h5,1);
    cv::Mat image = cv::imread("/home/wyj/software/apriltag_calibration_YAML-x86/11.jpeg",1);
    cv::Mat Extractcorner = image.clone();
    cv::Mat imageGray;
    cv::Mat imageGray1;
    cv::cvtColor(image,imageGray,cv::COLOR_BGR2GRAY);
    // cv::cvtColor(image,imageGray1,CV_BGR2GRAY);
    // cv::Mat diff = imageGray != imageGray1;
    // bool eq = cv::countNonZero(diff) == 0;
    // std::cout << eq << std::endl;
    // std::vector<cv::Mat> imageData;
    std::vector<std::string> src_name ;
    std::string src_path = "/home/wyj/software/apriltag_calibration_YAML-x86/";
    std::string path = src_path + "*.jpg";
    cv::glob(path, src_name, false);//调用opncv中的glob函数，将遍历路径path，将该路径下的全部文件名的绝对路径存进src_name
    if (src_name.size() == 0)
    {
        std::cerr << "There is no file in " << path << std::endl;
        exit(1);
    }
    
    std::string detections_filename = "/home/wyj/software/apriltag_calibration_YAML-x86/build_apriltag/detections";
    
    
    
    
    // cv::FileStorage fs(detections_filename,cv::FileStorage::WRITE);//APPEND,WRITE
 	// time_t rawtime;
  	// time(&rawtime);
 	// fs << "Time" << asctime(localtime(&rawtime));//calibrationDate
  	// fs << "Detections" << "[";
    vector<AprilTags::TagDetection> detections = tagDetector.extractTags(imageGray);
    // vector<cv::Point2f> imp;

    // for(size_t i = 0; i < detections.size(); ++i)
    // {
    //     const auto& det = detections[i];
    //     // std::cout << cv::Point(det.p[0].second,det.p[0].first) << std::endl;
    //     // std::cout << cv::Point2d(det.p[1].second,det.p[1].first) << std::endl;
    //     // std::cout << cv::Point2f(det.p[2].second,det.p[2].first) << std::endl;
    //     std::cout << det.id << std::endl;
    //     for (int j = 0; j < 4; ++j)
    //         imp.push_back(cv::Point2f(det.p[j].second,det.p[j].first));

    // }
    // for(size_t i = 0; i < imp.size(); ++i)
    // {
    //     fs << imp[i];
    //     image_det.push_back(imp[i]);

    // }
  	// fs << "]";
  	// fs.release();
  	// printf("The detetion results have been read and written to the project directory~\n");
    
    for(size_t i = 0; i < detections.size(); ++i)
    {
        std::cout << detections[i].id << std::endl;
        // const auto& det = detections[i];
        for (int j = 0; j < 4; ++j)
            cv::circle(Extractcorner,cv::Point2f(detections[i].p[j].first, detections[i].p[j].second),10,cv::Scalar(255,0,255),2);

    }
    cv::namedWindow("test1",cv::WINDOW_GUI_NORMAL);
    cv::imshow("test1",Extractcorner);
    // for(size_t i = 0; i < n_det.size(); ++i)
    // {
    //     const auto& det = n_det[i];
    //     for (int j = 0; j < 4; ++j)
    //         cv::circle(image2,cv::Point2f(det.p[j].first,det.p[j].second),5,cv::Scalar(255,0,255),2);

    // }
    // cv::namedWindow("test2",cv::WINDOW_GUI_NORMAL);
    // cv::imshow("test2",image2);
    
    cv::waitKey(0);

    return 0;


}