
#include <ros/ros.h>
#include <battery/bms.h>
#include <std_msgs/Int32.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "battery");
  ros::AsyncSpinner spinner(1);
  spinner.start();

  ros::NodeHandle nh, private_nh("~");
  std::string bms_port;
  private_nh.param<std::string>("bms_port", bms_port, "/dev/ttyUSB0");
  ros::Publisher battery_pub = nh.advertise<std_msgs::Int32> ("/battery", 10);
  BMS *bms;
  bms = new BMS(bms_port);
  bms->connect();

  double control_frequency;
  private_nh.param<double>("control_frequency", control_frequency, 50.0);

  int battery_capacit;      
  int battery_status;
  int battery_capacity;
  ros::Time diagnostic_current_time, diagnostic_last_time, bms_last_time;
  while (ros::ok())
  {
    diagnostic_current_time = ros::Time::now();
    ros::Duration bms_period(0.1);
    if ((diagnostic_current_time - bms_last_time).toSec() > bms_period.toSec())
    {
      int *soc = bms->getSOC();
      if (soc != NULL)
        {
          soc[0] = (soc[0] > 100) ? 100 : soc[0];
          soc[0] = (soc[0] < 0) ? 0 : soc[0];
          battery_capacity = soc[0];
          battery_status = soc[1];
          // debug
          std_msgs::Int32 status;
          status.data = soc[0];
          battery_pub.publish(status);
        }      
        bms_last_time = diagnostic_current_time;
}
}
}
