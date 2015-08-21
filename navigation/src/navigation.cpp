#include "navigation.h"
#include <sensor_msgs/LaserScan.h>
#include <sstream>

#define PI 3.14159265359
#define RAD2DEG 180/PI
#define ANGLE 20*(PI/180)

std_msgs::Float64MultiArray hi;
obstacle_detection::boundingboxes b_boxes;
bool hi_received = false;
bool bb_received = false;

float Start_x = 0;
float Start_y = 0;	
float Width_x = 0;
float Width_y = 0;

ros::Publisher marker_pub_BBox;

        
  visualization_msgs::Marker  Frobit::lines(float x1,float y1, float z1,float x2,float y2, float z2, visualization_msgs::Marker lineCollection ){
            geometry_msgs::Point p;
            p.x = x2;
            p.y = y2;
            p.z = 0;

            // The line list needs two points for each line
            lineCollection.points.push_back(p);

            p.x = x1;
            p.y = y1;
            p.z = 0;
            lineCollection.points.push_back(p);

            //marker_pub.publish(line_list);
		return lineCollection;
        }


void Frobit::bboxesCallback(const obstacle_detection::boundingboxes& bboxes, int i,float r1_, float r2_, float theta)
{
	visualization_msgs::Marker line_list;
	line_list.header.frame_id = "/laser";
	 line_list.header.stamp = ros::Time::now();
     line_list.ns = "BBox of Objects";
     line_list.action = visualization_msgs::Marker::ADD;
    line_list.pose.orientation.w = 1.0;

    line_list.id = 2;

    line_list.type = visualization_msgs::Marker::LINE_LIST;

    line_list.scale.x = 0.05;

	ROS_INFO("1");

    // Line list is red
    line_list.color.g = 1.0;
	line_list.color.r = 1.0;
    line_list.color.a = 1.0;
	
		
			Start_x = bboxes.boundingboxes[i].start.x;
			Start_y = bboxes.boundingboxes[i].start.y;	
			Width_x = bboxes.boundingboxes[i].width.x;
			Width_y = bboxes.boundingboxes[i].width.y;
		    //1-2
		ROS_INFO("2");
		    line_list=lines(Start_x, Start_y, 0, Start_x+Width_x, Start_y, 0,line_list);
		    	ROS_INFO("3");
		    //2-3
		    line_list=lines(Start_x+Width_x, Start_y, 0, Start_x+Width_x, Start_y+Width_y, 0,line_list) ;
		    //3-4
		    line_list=lines(Start_x+Width_x, Start_y+Width_y, 0, Start_x, Start_y+Width_y, 0,line_list);
		    //1-4
		    line_list=lines( Start_x, Start_y, 0, Start_x, Start_y+Width_y, 0,line_list);
			float X1,X2,Y1,Y2;
				float r = r1_; 
				if ((theta > PI/4 && theta < 3*PI/4) || (theta > 5*PI/4 && theta < 7*PI/4))
				{
					X1 = -10000;
					Y1 = (double)(r - (double)X1  * cos(theta)) / sin(theta);
					X2 = 10000;
					Y2 = (double)(r - (double)X2  * cos(theta)) / sin(theta);
					//ROS_INFO("oppe = %f, %f, %d, %f, %f, %f, %f",theta,r, h, Y1, (double)(r-(double)std::floor<int>(h/2))*RRESOLUTION, cos(t*tRes * DEG2RAD));

				}
				else
				{
					Y1 = -10000;
					X1 = (double)(r - (double)Y1  * sin(theta)) / cos(theta);
					Y2 = 10000;
					X2 = (double)(r - (double)Y2  * sin(theta)) / cos(theta);
					//ROS_INFO("nede");
				}
	ROS_INFO("4");
		    line_list=lines( X1, Y1, 0, X2, Y2, 0,line_list);
				r = r2_;
				if ((theta > PI/4 && theta < 3*PI/4) || (theta > 5*PI/4 && theta < 7*PI/4))
				{
					X1 = -10000;
					Y1 = (double)(r - (double)X1  * cos(theta)) / sin(theta);
					X2 = 10000;
					Y2 = (double)(r - (double)X2  * cos(theta)) / sin(theta);
					//ROS_INFO("oppe = %f, %f, %d, %f, %f, %f, %f",theta,r, h, Y1, (double)(r-(double)std::floor<int>(h/2))*RRESOLUTION, cos(t*tRes * DEG2RAD));

				}
				else
				{
					Y1 = -10000;
					X1 = (double)(r- (double)Y1  * sin(theta)) / cos(theta);
					Y2 = 10000;
					X2 = (double)(r - (double)Y2  * sin(theta)) / cos(theta);
					//ROS_INFO("nede");
				}
	ROS_INFO("5");
		    line_list=lines( X1, Y1, 0, X2, Y2, 0,line_list);
			
	
    marker_pub_BBox.publish(line_list);
	ROS_INFO("6");
}



// constructor for the Frobit
Frobit::Frobit(){	
	stopCounter=0;
	deadman_button.data = false;
	z_axis = 0;
	x_axis = 0;
    v_max = 0.3;
    d_max = 4;

	sub_BB = n.subscribe("/obstacle_detection/boundingboxes", 5, &Frobit::boxesCallback, this);
	sub_lines = n.subscribe("/measHoughKalman", 5, &Frobit::houghCallback,this);
	twist_pub_ = n.advertise<geometry_msgs::TwistStamped>("/cmd_vel2", 1000);
	deadman_pub_ = n.advertise<std_msgs::Bool>("/fmCommand/deadman", 1000);
	error_pub_ = n.advertise<std_msgs::Float64>("/frobyte/error", 1000);

}

void Frobit::houghCallback(const std_msgs::Float64MultiArray& houghInfo){
	hi = houghInfo;	
	hi_received=true;
	if(bb_received)
		updateVel(hi,b_boxes);
}

void Frobit::boxesCallback(const obstacle_detection::boundingboxes& bboxes){
	b_boxes = bboxes;
	bb_received=true;	
	if(hi_received)
		updateVel(hi,b_boxes);
}

void Frobit::updateVel(const std_msgs::Float64MultiArray houghInfo, const obstacle_detection::boundingboxes bboxes){
	//get bounding boxes
	float Start_x = 999999;
	float Start_y = 0;	
	float Width_x = 0;
	float Width_y = 0;
	float distance = 99;
	int foundPos = 0;
	ROS_INFO("updateVel");
	// search through bounding boxes list, delete all the ones that are behind the robot (x is negative) and save the one that is closest to the robot (smallest x)
	for (size_t i=0;i<bboxes.boundingboxes.size();i++)
	{
		if((bboxes.boundingboxes[i].start.x>0 && bboxes.boundingboxes[i].start.x < distance) || (bboxes.boundingboxes[i].start.x>0 && bboxes.boundingboxes[i].start.x+bboxes.boundingboxes[i].width.x < distance))
		{
			distance = bboxes.boundingboxes[i].start.x;
			Start_x = bboxes.boundingboxes[i].start.x;
			Start_y = bboxes.boundingboxes[i].start.y;	
			Width_x = bboxes.boundingboxes[i].width.x;
			Width_y = bboxes.boundingboxes[i].width.y;
			foundPos = i;
		}

	}
		
	twist.header.stamp = ros::Time::now();
	twist.twist.angular.z = z_axis;

	// Get values from the hough2line node (theta,r1,r2)
	float theta = houghInfo.data[0]; // theta angle
	float r1_ = houghInfo.data[1]; //left line
	float r2_ = houghInfo.data[2]; //right line
	
	// check that r1_ is always smallest and on the left side
	if (r2_ < r1_)
	{
		float tmp = r1_;
		r1_ = r2_;
		r2_ = tmp;
	}

	// compute distances to the two lines and the desired "center lane"
	float r_dist_ = r1_+r2_;  
	float center_range = fabs(r1_)+fabs(r2_);  
	

	// compute the angle error	
	double err_ = 0;
	if(fabs(r_dist_)>center_range/7)
	{
		err_ = 0.35*r_dist_;
	}
	// adjust the angle with the error
	twist.twist.angular.z = -0.4*theta*DEG2RAD+err_;
	
	//check if the BBox is close to the robot	
	if(Start_x < d_max)
	{	
		if(Start_y > 0)
		{
			r2_ = Start_y; // because r1_ should be negative
		}
		else
		{
			r1_ = Start_y+Width_y;
		}
	}
	// handle the velocity, in case robot is running close to an obstacle 
	float R=std::min(fabs(r1_),fabs(r2_)); //smallest r distance

	// determine the frontal distance to the obstacle	
	float phi = fabs(90-fabs(theta))*DEG2RAD;
	float front_dist_ = R/cos(phi);
	if(bboxes.boundingboxes.size()>0)
		bboxesCallback(bboxes,foundPos,r1_,r2_,theta);

	// change the velocity
	twist.twist.linear.x = std::min((float)fabs(front_dist_/d_max), 1.0f)*v_max;	
	//std::cout << "theta: "<< theta <<  " and phi: = " << phi << std::endl;
	//std::cout << "err: "<< err_ << std::endl;
	//std::cout << "r1: "<< r1_ <<  " and r2: = " << r2_ << std::endl;
	//std::cout << "R: "<< R << std::endl;
	//std::cout << front_dist_ <<  " and division = " << front_dist_/d_max << std::endl;
	//twist.twist.linear.x = 0.2; //constant speed
	// send command to motors
	twist_pub_.publish(twist);
	//deadman_button.data = true;	
	//deadman_pub_.publish(deadman_button);
}
int main(int argc, char** argv){	
	ros::init(argc, argv, "navigation_node");
	ros::NodeHandle nh;
	
	Frobit *frobo = new Frobit();
    ros::Rate loop_rate(10); // publish 10 messages per second
	// while (ros::ok()){			
		//frobo->updateVel(); //activate motors
		marker_pub_BBox = nh.advertise<visualization_msgs::Marker>("BBbox_Nav_Viz", 10);
		ros::spin();
		//loop_rate.sleep();
        //}*/
 return 0;
}
