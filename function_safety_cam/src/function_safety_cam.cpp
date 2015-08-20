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
#include <list>

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

		cam_pub = it.advertiseCamera("/imageWithBBox", 1);
		cam_sub = it.subscribeCamera("/usb_cam/image_raw", 1, &MyNode::onImage, this);
		

	}
	;

	~MyNode() {

	}
	;

	void onImage(const sensor_msgs::ImageConstPtr& msg,const sensor_msgs::CameraInfoConstPtr& p) {
		mesReceived.push_back(ros::Time::now());
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

		resize(cv_ptr->image, image, Size(), 0.5, 0.5);
		
		cv::Mat greyMat;
		cv::cvtColor(image, greyMat, cv::COLOR_BGR2GRAY);

		IplImage* GrayIPLimage_;
		GrayIPLimage_ = cvCreateImage(cvSize(greyMat.cols,greyMat.rows),8,1);
		IplImage GrayIPLimg=greyMat;
		cvCopy(&GrayIPLimg,GrayIPLimage_);



		//IplImage ipltemp=image1;
		//cvCopy(&ipltemp,image2);

		safety::histogram<unsigned int> histo(false);
		histo = safe_image.do_histogram((unsigned char*)GrayIPLimage_->imageData, greyMat.cols, greyMat.rows, 1	);
		//resize(cv_ptr->image, image, Size(), imageResize, imageResize);
		std::cout << "Non Empty : "<<histo.nonempty() << std::endl;
		std::cout << "Hist Size : "<<histo.size() << std::endl;
		std::cout << "hist Max : "<<histo.max() << std::endl;
		std::cout << "hist Min : "<<histo.min() << std::endl;
		//namedWindow( "Display window", WINDOW_AUTOSIZE );
				//cv::Mat Test(GrayIPLimage_);// Create a window for display.
			//	/imshow( "Display window", Test );                   // Show our image inside it.

		
		return;
	}
	std::list<ros::Time> mesReceived;
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
	ros::init(argc, argv, "function_safety_cam");
	MyNode node;
	ros::spin();
	/*std::list<ros::Time>::iterator it1;
	while(1)
	{
		while(node.mesReceived.size()>10)
		{
			it1 = node.mesReceived.begin();
			node.mesReceived.erase(it1);
			std::cout << "errased!!! Size is now = "<< node.mesReceived.size() << std::endl;
		}
		if(node.mesReceived.size()>1)
		{
			unsigned int time = 0;
			int i = 1;
			for (std::list<ros::Time>::iterator it=node.mesReceived.begin(); it != node.mesReceived.end(); ++it)
			{
				std::list<ros::Time>::iterator itt = node.mesReceived.begin();
				std::advance(itt, i);
				i++;
				time += (unsigned int)((*it).nsec-(*itt).nsec)/1000000;
				if(i == 10)
					break;
			}










			std::cout <<(double)time/(node.mesReceived.size()-1) << std::endl;

		}

		ros::spinOnce();
	}*/

	return 0;
}

