#include <iostream>
#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"
#include "nav_msgs/Odometry.h"

using namespace std;

float xy = 2; //angular velocity
float a;
float b;
float e = 0.5; //error distance
int flag=0; //to check first flip
long int i;
int flip = 0; //to check if already flipped

void chatterCallback(const nav_msgs::Odometry::ConstPtr& msg)
 {
    //starting positions
    if(flag==0)
    {
      a = msg->pose.pose.position.x;
      b = msg->pose.pose.position.y;
      ROS_INFO("%f\n",a);
      ROS_INFO("%f\n",b);
      flag++;
    }
    else
    {
      int count = 0;
      nav_msgs::Odometry odom;
      odom = *msg;

      //check if bot is almost at the starting position
      if(odom.pose.pose.position.x > a-e && odom.pose.pose.position.x < a+e)
        count++;
      if(odom.pose.pose.position.y > b-e && odom.pose.pose.position.y < b+e)
        count++;
 
      if(count == 2)
      {
        ROS_INFO("yoyo");
        //change the sign of the angular velocity
        if(flip==0)
        {
          xy = xy * (-1);
          a = odom.pose.pose.position.x;
          b = odom.pose.pose.position.y;

          ROS_INFO("%f\n",a);
          ROS_INFO("%f\n",b);
          ROS_INFO("yo");
          flip++;
          i =0;
        }

      }
      else
      {
        flip=0;
      }


    }    

    
 }

int main(int argc, char **argv)
{

  ros::init(argc, argv, "SwarmSimu");

  ros::NodeHandle n;

  ros::Publisher vel_pub_0 = n.advertise<geometry_msgs::Twist>("/swarmbot0/cmd_vel", 1);

  ros::Rate loop_rate(5);

  ros::Subscriber sub = n.subscribe("/swarmbot0/odom", 1, chatterCallback);

  int count = 0;

  while (ros::ok())
  {
    geometry_msgs::Twist cmd_vel;
  
    cmd_vel.linear.x = 3;
    cmd_vel.linear.y = 0;
    cmd_vel.linear.z = 0;
    cmd_vel.angular.x = 0;
    cmd_vel.angular.y = 0;
    cmd_vel.angular.z = xy;

    //publish the velocities
    vel_pub_0.publish(cmd_vel);
    ros::spinOnce();

    loop_rate.sleep();

    ++count;

  }
  return 0;
}
