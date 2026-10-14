#ifndef __BMS_H_
#define __BMS_H_

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>
#include <sstream>
#include <unistd.h>

using namespace std;

class BMS {
  private:
    int handle;
    string port;

  protected:
    void initPort();

  public:
    int connect();
    int * getSOC();
    BMS(string dev);
    ~BMS();
    int battery_capacit;      
    int battery_status;
    int battery_capacity;
    ros::Time diagnostic_current_time, diagnostic_last_time, bms_last_time;
};

#endif
