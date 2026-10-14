#include <ros/ros.h>
#include <battery/bms.h>

BMS::BMS(string dev)
{
  handle = -1;
  port.assign(dev);
}

BMS::~BMS()
{
  if(handle != -1)
    close(handle);

  handle = -1;
}

int BMS::connect()
{
  if(handle != -1)
    return 0;

  handle = open(port.c_str(), O_RDWR |O_NOCTTY | O_NDELAY);
  if(handle == -1)
  {
    ROS_ERROR("Error opening BMS port");
    return -1;
  }

  fcntl (handle, F_SETFL, O_APPEND | O_NONBLOCK & ~FNDELAY);
  initPort();

  cout<<"Opening BMS: '"<<port<<"'..."<<"succeeded."<<endl;
  return 0;
}


void BMS::initPort()
{
  if(handle == -1)
    return;

  int BAUDRATE = B9600;
  struct termios newtio;
  tcgetattr (handle, &newtio);

  cfsetospeed (&newtio, (speed_t)BAUDRATE);
  cfsetispeed (&newtio, (speed_t)BAUDRATE);

  newtio.c_iflag = IGNBRK;		
  newtio.c_lflag = 0;			
  newtio.c_oflag = 0;			
  newtio.c_cflag |= (CLOCAL | CREAD);	
  newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  newtio.c_iflag &= ~(IXON | IXOFF | IXANY);
  newtio.c_cflag &= ~CSIZE;		
  newtio.c_cflag |= CS8;			
  newtio.c_cflag &= ~PARENB;		
  newtio.c_cflag &= ~PARODD;		
  newtio.c_cflag &= ~CSTOPB;		
  newtio.c_cc[VMIN] = 0;
  newtio.c_cc[VTIME] = 1;

  tcflush (handle, TCIFLUSH);
  tcsetattr (handle, TCSANOW, &newtio);	
}

int* BMS::getSOC()
{
  if(handle == -1)
    return NULL;

  char command[7];
  command[0] = 0xDD;
  command[1] = 0xA5;
  command[2] = 0x03;
  command[3] = 0x00;
  command[4] = 0xFF;
  command[5] = 0xFD;
  command[6] = 0x77;
  int countSent = write(handle, command, 7);
  if(countSent < 0)
  {
    return NULL;
  }

  int countRcv;
  char buf[512] = "";
  string dataI = "";
  while((countRcv = read(handle, buf, 512)) > 0)
  {
    dataI.append(buf, countRcv);
    if(countRcv < 512)
      break;
  }
  if(countRcv < 0)
  {
    return NULL;
  }

  //for(int i=0;i<27;i++){
    // printf("%02X ",(unsigned char)dataI[i]);
     //cout<<dataI[i]<<" ";
  //}
  // printf("\n");
  float soc = 0;
  if (countRcv == 45)
  {
      //    cout<<"SOC: "<< (int)dataI[74] << " status: "<<(int)(unsigned char)dataI[137]<<endl;
      static int battery[2];
      battery[0] = (int)dataI[23];
      // uint8_t status_u8n[2] = {
      //         static_cast<uint8_t>(dataI[7]),
      //         static_cast<uint8_t>(dataI[6])
      //     };
      // int16_t status_16n;
      // memcpy(&status_16n,status_u8n,2);
      // if(status_16n > 0) {
      //   battery[1] = 0;
      // } else {
      //   battery[1] = 1;
      // }
      // printf("Soc:%d status:%d \n", battery[0], battery[1]);
      return battery;
      }
  else
  {
    return NULL;
  }
}
