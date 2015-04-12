#include <iostream>
#include "ros/ros.h"
#include <boost/thread.hpp>
#include <std_msgs/String.h>

using namespace std;

void threada()
{
	ros::NodeHandle n;
	ros::Publisher pub=n.advertise<std_msgs::String>("topic1",1000);
	ros::Rate loop_rate(10);  //rate of publishing of threada
	std_msgs::String msg;
	std::stringstream ss;
	ss << "Hello \n";
	msg.data = ss.str();
	while(1)
	{
		pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
}

void threadb()
{
	ros::NodeHandle n;
	ros::Publisher pub=n.advertise<std_msgs::String>("topic1",1000);
	ros::Rate loop_rate1(30);  //rate of publishing of threadb
	std_msgs::String msg;
	std::stringstream ss;
	ss << "World \n";
	msg.data = ss.str();
	while(1)
	{
		pub.publish(msg);
		ros::spinOnce();
		loop_rate1.sleep();
	}
}

int main(int argc, char **argv)
{

	ros::init(argc,argv,"boost");

	//initializing threads
	boost::thread Thread1(threada);
	boost::thread Thread2(threadb);

	//starting threads
	Thread1.join();
	Thread2.join();

	return 0;
}