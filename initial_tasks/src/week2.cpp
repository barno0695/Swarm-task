#include <iostream>
#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"
#include "nav_msgs/Odometry.h"

using namespace std;


float PI = 22/7;

float goal_px;
float goal_py;

float u;
float w;
int flag=0;
float k1=1, k2=1, k3=1;

void goalCallback(const nav_msgs::Odometry::ConstPtr& msg)
 {
    //set the goal
    if(flag==0)
    {
      goal_px = msg->pose.pose.position.x;
      goal_py = msg->pose.pose.position.y;

      flag++;
    }
 }

//compute linear and angular velocities based on the equations
void currentCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
  float rho,gamma,delta,theta;
  theta = atan2(msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
  if (theta > PI)
  {
    theta = theta - (2 * PI);
  }
  rho = sqrt(pow(msg->pose.pose.position.x - goal_px,2) + pow(msg->pose.pose.position.y - goal_py,2));
  gamma = atan2(msg->pose.pose.position.y - goal_py, msg->pose.pose.position.x - goal_px ) - theta + PI;
  if (gamma > PI)
  {
    gamma = gamma - (2 * PI);
  }
  delta = gamma + theta;
  if (delta > PI)
  {
    delta = delta - (2 * PI);
  }
  u = k1 * rho * cos(gamma);
  w = (k2 * gamma) + (k1 * (sin(gamma)*cos(gamma)/gamma) * (gamma + (k3 * delta)));
}



int main(int argc, char **argv)
{

  ros::init(argc, argv, "SwarmSimu");

  ros::NodeHandle n;

  ros::Publisher vel_pub_0 = n.advertise<geometry_msgs::Twist>("/swarmbot0/cmd_vel", 1);
  ros::Rate loop_rate(5);

  ros::Subscriber sub = n.subscribe("/swarmbot0/odom", 1, currentCallback);
  ros::Subscriber sub1 = n.subscribe("/swarmbot1/odom", 1, goalCallback);

  int count = 0;

  while (ros::ok())
  {
    geometry_msgs::Twist cmd_vel;

    cmd_vel.linear.x = u;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = w;

    //publish the velocities
    vel_pub_0.publish(cmd_vel);
    ros::spinOnce();

    loop_rate.sleep();

    ++count;

  }

  return 0;
}
