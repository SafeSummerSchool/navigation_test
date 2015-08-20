#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include "Image_Safety.h"
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/Image.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/MultiArrayLayout.h>
#include <image_transport/image_transport.h>
#include <camera_info_manager/camera_info_manager.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

using namespace std;
using namespace cv;

class MyNode {
public:
	MyNode() :
			nh("~"), it(nh) {
		//ROS_ERROR("NodeBeingCreated");
		//nh.param<double>("FOV_verticalDeg",FOV_verticalDeg,47.0);
		//nh.param<double>("FOV_horizontal",FOV_horizontal,83.0);
		//nh.param<double>("angleTiltDegrees",angleTiltDegrees,7.0);
		//nh.param<double>("cameraHeight",cameraHeight,1.9);
		//nh.param<double>("imageResize",imageResize,0.5);

		//nh.param<std::string>("model_dir",model_dir,"model");
		//ROS_ERROR("%s\n",model_dir.c_str());
		//cam_pub = it.advertiseCamera("imageWithBBox", 1);

		//cinfor_ = boost::shared_ptr<camera_info_manager::CameraInfoManager>(new camera_info_manager::CameraInfoManager(nh, "test", ""));

		//array_pub = nh.advertise<std_msgs::Float64MultiArray>("DistanceAngle", 1);
		//array_pub2 = nh.advertise<std_msgs::Float64MultiArray>("BBox", 1);
		//cam_sub = it.subscribeCamera("image_raw", 1, &MyNode::onImage, this);
std::cout << "test" << std::endl;
		cam_pub = it.advertiseCamera("/imageWithBBox", 1);
		cam_sub = it.subscribeCamera("/usb_cam/image_raw", 1, &MyNode::onImage, this);
		


	}
	;

	~MyNode() {

	}
	;

	void onImage(const sensor_msgs::ImageConstPtr& msg,const sensor_msgs::CameraInfoConstPtr& p) {
std::cout << "receiving " << std::endl;
		// do all the stuff here
		//ROS_ERROR("GOT Image");
		//convert  image to opencv
		//ROS_ERROR("ImageHasBeenReceived"); 
		cv_bridge::CvImagePtr cv_ptr;
		try {
			cv_ptr = cv_bridge::toCvCopy(msg,
					sensor_msgs::image_encodings::BGR8);
		} catch (cv_bridge::Exception& e) {
			ROS_ERROR("cv_bridge exception: %s", e.what());
			return;
		}

		
		cv::Mat greyMat;
		cv::cvtColor(cv_ptr->image, greyMat, cv::COLOR_BGR2GRAY);


		sensor_msgs::CameraInfoPtr cc(
				new sensor_msgs::CameraInfo(cinfor_->getCameraInfo()));
		sensor_msgs::ImagePtr msg_out = cv_bridge::CvImage(std_msgs::Header(),"bgr8", greyMat).toImageMsg();
		msg_out->header.stamp = ros::Time::now();
		IplImage* GrayIPLimage_;
		GrayIPLimage_ = cvCreateImage(cvSize(greyMat.cols,greyMat.rows),8,1);

		//cam_pub.publish(msg_out, cc);
		//IplImage ipltemp=image1;
		//cvCopy(&ipltemp,image2);

		safety::histogram<unsigned int> histo(false);
		histo = safe_image.do_histogram((unsigned char*)GrayIPLimage_->imageData, greyMat.cols, greyMat.rows, 16);
		//resize(cv_ptr->image, image, Size(), imageResize, imageResize);
		std::cout << "Non Empty : "<<histo.nonempty() << std::endl;
		std::cout << "Hist Size : "<<histo.size() << std::endl;
		std::cout << "hist Max : "<<histo.max() << std::endl;
		std::cout << "hist Min : "<<histo.min() << std::endl;
		
		return;
	}
private:
	cv::Mat image;
	ros::NodeHandle nh;
	image_transport::ImageTransport it;
	image_transport::CameraSubscriber cam_sub;
	image_transport::CameraPublisher cam_pub;
	boost::shared_ptr<camera_info_manager::CameraInfoManager> cinfor_;
	safety::safe safe_image;


};

int main(int argc, char** argv) {
	std::cout << "halloooo" << std::endl;
	ros::init(argc, argv, "function_safety_cam");

	MyNode node;

	ros::spin();


	return 0;
}
