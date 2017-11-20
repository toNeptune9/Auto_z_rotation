#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/PoseStamped.h"
#include <fstream>

using namespace std;
std::ofstream file;
void callback_april(const geometry_msgs::PoseArrayConstPtr& pose_array);
void callback_robot(const std_msgs::Float64& msg);
ros::NodeHandle *p;
const float PI = 3.14159265;
int rad2angle(float i);
bool *to_b;
int main(int argc, char**argv)
{
   bool b = false;
   ros::init(argc,argv,"compare_node");
   file.open("test_april2.txt"); //create  a file
   ros::NodeHandle nh;
   p = &nh;
   to_b = &b;
   //ros::Publisher pub;
   ros::Subscriber sub_april = nh.subscribe("/tag_detections_pose",1,callback_april);
   ros::Subscriber sub_robot = nh.subscribe("/tag_rotate_robot_aroundz/joint1_position_controller/command",1,callback_robot);
  bool hit;

  ros::Rate loop_rate(1);
  int count_positive = 0;
  int count_negative = 0;
  int sum = 0;
  int count_plus = 0;
  int count_minus = 0;
  while(ros::ok() && sum<1000)
  {
    //int angle1 = 0;
    //nh.getParam("current_angle_april",angle1);
    int angle2 = 0;
    nh.getParam("current_angle_robot",angle2);


    if (b == true) {
      ROS_INFO_STREAM("[true] at angle="<<angle2);
      file <<"[true] at angle ="<<angle2;
      file << endl;
      count_positive++;
    }

    else {
      ROS_INFO_STREAM("[false] at angle="<<angle2);
      file <<"[false] at angle ="<<angle2;
      file <<endl;
      count_negative++;
    }

    sum ++;
    ROS_INFO_STREAM("sum ="<<sum);
    if (angle2== 86)
      count_plus++;
    if (angle2 == -86)
      count_minus++;
    ros::spinOnce();
    loop_rate.sleep();
    file<<endl;

  }

  file<<"Finally positive results (number of success recognition)="<<count_positive;
  file<<endl;
  file<<"Finally negative results (number of fault recognition)="<<count_negative;
  file<<endl;
  file<<"Number of positive iteration (to 86 degrees)="<<count_plus;
  file<<endl;
  file<<"Number of negative iteration (to -86 degrees)="<<count_minus;
  file.close();
  return 0;
}

int rad2angle(float rad)
{ int angle;
  float formula = (rad *180) / PI;
  //from ragians to degrees
  angle = (int)formula;
  //then to int
  return angle;
}



void callback_april(const geometry_msgs::PoseArrayConstPtr &pose_array) {

  if (pose_array->poses.size()==1) {
   // ROS_INFO_STREAM("PoseArray Size from AprilTag ="<<pose_array->poses.size());
    //ROS_INFO_STREAM("Data from AprilTag =["<<pose_array->poses.data()->orientation<<"]");
   // ROS_INFO_STREAM("Z orientation from AprilTag ={"<<pose_array->poses.data()->orientation.z<<"}");
  //  int angle = rad2angle(pose_array->poses.data()->orientation.z);
    ROS_INFO_STREAM("Z is exist");
    *to_b = true;
  }
  else {
    ROS_INFO_STREAM("----Size is null");
   *to_b = false;
  }

}

void callback_robot(const std_msgs::Float64 &msg) {
  int angle = rad2angle(msg.data);
 p->setParam("current_angle_robot",angle);

}
