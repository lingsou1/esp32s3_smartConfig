/*
接线说明:无

程序说明:通过smartConfig来实现配网,可以通过微信小程序使设备联网

注意事项:smartconfig完成配网主要分以下3个步骤：
          1):设备进入初始化状态，开始监听附近的WiFi数据包。
          2):手机/平板设置WiFi名称和密码后，发送UDP广播包。
          3):设备通过UDP包（长度）获取配置信息，切换网络模式，连接上家里WiFi，配置完成

函数示例:无

作者:灵首

时间:2023_4_7

*/

#include <WiFi.h>

#define MAX_RETRY 10    //用来设置重连的时间



/**
* @brief SmartConfig智能配网用到的主要函数为WiFi.beginSmartConfig()。
          通过查询SmartConfig连接状态判断WiFi是否连接成功，
          用到的主要函数为WiFi.smartConfigDone()。
*
* @param 无
* @return  无
*/
void smart_config(void)
{ 
  // 将WiFi设置为 WIFI_AP_STA 模式,并进行配网(应该是尝试接受UDP报文联网)
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();

  //串口输出连接状态,并在此持续尝试连接
  Serial.println("Waiting for SmartConfig.\n");
  while (!WiFi.smartConfigDone())
  {
    delay(500);
    Serial.print(".");
  }

  //成功连接后输出信息
  Serial.println("");
  Serial.println("SmartConfig received.\n");

  // Wait for WiFi to connect to AP
  Serial.println("Waiting for WiFi\n");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

   WiFi.setAutoConnect(true); // 设置自动连接
}



/**
* @brief  开机自动连接WiFi
*
* @param 无
* @return 布尔值,返回一个连接状态
*/
bool connect_wifi(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(); //启动WIFI连接
  Serial.println("Connection WIFI");

  int retry_count = 0;
  while (retry_count < MAX_RETRY)
  {
    delay(500);
    Serial.print(".");
    retry_count++;
    if (WiFi.status() == WL_CONNECTED) //检查连接状态
    {
      return true;
    }
  }

  return false;
}


/**
* @brief 开机判断联网方式，初次联网，则进入SmartConfig智能配网
         联网成功则输出WiFi名称及IP地址
*
* @param 无
* @return 无
*/
void setup_wifi(void)
{

  if (!connect_wifi())
  {
    smart_config();
  }

  Serial.println("");
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}



void setup()
{
  //串口设置
  Serial.begin(9600);
  setup_wifi();
}



void loop()
{
  Serial.println("hello world!");
  delay(1000);
}

