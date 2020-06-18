#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TridentTD_LineNotify.h>
#include <ESP8266HTTPClient.h>
#define SSID "WiFi"
#define PASSWORD ""
#define LINE_TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

HTTPClient http;
String IPaddress;

//เปิดระบบส่งแผนที่ฝนเข้า Line ***ต้องมี PHP/Web Server ด้วย ถ้าไม่มี ขี้เกียจลง ยุ่งยาก ให้ Set ค่าตรงนี้เป็น 0
//ดูรายละเอียดและ Script ได้ที่ https://github.com/superogira/php---Bangkok-rain-radar-line-notify
int rainRadarMap = 1;

//ของเซนเซฮร์ตรวจจับน้ำฝน
int analogPin = A0; //ประกาศตัวแปร ให้ analogPin 
int val = 0;

//ตั้งค่าเวลา
const long utcOffsetInSeconds = 25200;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup()
{
  Serial.begin(9600);
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  Serial.println(LINE.getVersion());
  IPaddress =  WiFi.localIP().toString();
  LINE.setToken(LINE_TOKEN);
  LINE.notify("WeMos D1 R1 (ระบบตรวจจับน้ำฝนและข้อมูลเรดาร์ฝน) - ทำการเชื่อมต่อ WiFi เรียบร้อย \nIP Address : "+ IPaddress);
  timeClient.begin();
}

int median = 1024;
int littlerain = 0;
int littlecount = 0;
int raining = 0;
int countforstop = 0;
int norain = 1;
int noraincount = 0;
int rainwillhard = 0;
int hardrain = 0;
int heavyrain = 0;
int superrain = 0;
int val1min = 0;
int loop600 = 0;
 
void loop()
{
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  Serial.println(timeClient.getFormattedTime());
  
  for (int i = 0; i <= 99;i++) {
    val = analogRead(analogPin);  //อ่านค่าสัญญาณ analog 
    val1min = val1min+val;
    loop600++;
    delay(100);
  }
  Serial.print("ค่าปัจจุบันของเซนเซอร์น้ำฝน = ");
  Serial.println(val);
  
  if (loop600 == 600) {
    median = val1min / 600;
    Serial.print("ค่าเฉลี่ย 1 นาที = ");
    Serial.println(median);
    loop600 = 0;
    val1min = 0;
  }

  if (median < 980 && raining == 0) {
	LINE.notify("ตรวจพบน้ำฝน คาดว่าฝนกำลังจะตกหรือตกแล้ว");
	raining = 1;
    littlerain = 1;
    norain = 0;
	
    if (rainRadarMap == 1) {
		HTTPClient http;  //Declare an object of class HTTPClient
		http.begin("http://localhost/bangkok_rain_radar_to_line.php");  //Specify request destination
		http.GET();
		Serial.println("ทำการเรียก Url เพื่อดึงข้อมูลเรดาร์ฝนส่งเข้า Line");                     //Print the response payload
		http.end();   //Close connection
	}
  }
  
  if (median < 1024 && raining == 0 && littlerain == 0) {
    littlecount++;
    if (littlecount == 18) {
		LINE.notify("ค่าของเซนเซอร์ตรวจจับน้ำฝนมีการเปลี่ยนแปลง ฝนอาจจะกำลังตกเบา ๆ ปอย ๆ หรือมีบางสิ่งตกค้างอยู่บนเซนเซอร์");
		littlerain = 1;
		norain = 0;

		if (rainRadarMap == 1) {
			HTTPClient http;  //Declare an object of class HTTPClient
			http.begin("http://localhost/bangkok_rain_radar_to_line.php");  //Specify request destination
			http.GET();
			Serial.println("ทำการเรียก Url เพื่อดึงข้อมูลเรดาร์ฝนส่งเข้า Line");                     //Print the response payload
			http.end();   //Close connection
		}
    }
  } else {
    littlecount = 0;
  }
  
  if (median < 860 && raining == 1 && hardrain == 0) {
      LINE.notify("ตรวจพบปริมาณน้ำฝนเพิ่มขึ้น คาดว่าฝนเริ่มตกหนักขึ้น");
      hardrain = 1;
    }
  if (median < 680 && raining == 1 && hardrain == 1 && heavyrain == 0) {
      LINE.notify("ตรวจพบน้ำฝนปริมาณมาก คาดว่าฝนกำลังตกหนักมาก");
      heavyrain = 1;
    }
  if (median < 470 && raining == 1 && hardrain == 1 && heavyrain == 1 && superrain == 0) {
      LINE.notify("ตรวจพบน้ำฝนปริมาณเยอะมากกก คาดว่าฝนกำลังตกโคตรหนัก");
      superrain = 1;
    }
  
  if (median > 600 && raining == 1 && superrain == 1) {
      LINE.notify("ตรวจพบปริมาณน้ำฝนลดลง จากที่ฝนตกโคตรหนัก คาดว่าฝนเริ่มตกเบาลงจากเดิม");
      superrain = 0;
    }
  if (median > 790 && raining == 1 && heavyrain == 1) {
      LINE.notify("ตรวจพบปริมาณน้ำฝนลดลง จากที่ฝนตกหนัก คาดว่าฝนเริ่มตกเบาลงจากเดิม");
      heavyrain = 0;
    }
  if (median > 940 && raining == 1 && hardrain == 1) {
      LINE.notify("ตรวจพบปริมาณน้ำฝนน้อยลง คาดว่าฝนเริ่มซา");
      hardrain = 0;
    rainwillhard = 1;
    }
  if (median < 850 && raining == 1 && hardrain == 0 && rainwillhard ==1) {
      LINE.notify("ฝนกลับมาตกแรงขึ้น");
    rainwillhard = 0;
    }
  if (median > 1005 && raining == 1) {
      countforstop++;
    if (countforstop >= 18) {
      raining = 0;
      countforstop = 0;
      LINE.notify("ตรวจพบน้ำฝนน้อยลง คาดว่าฝนเริ่มตกเบา ๆ ปอย ๆ อาจจะตกแบบนี้ไปเรื่อย ๆ หรืออาจจะหยุดตกในอีกไม่นาน หรือหยุดตกแล้ว");
    }
    } else {
    countforstop = 0;
  }
  if (median > 1023 && norain == 0) {
    noraincount++;
    if (noraincount == 18) {
		littlerain = 0;
		norain = 1;
		LINE.notify("ไม่พบน้ำฝนเพิ่มเติมอีก ฝนได้หยุดตกโดยสมบูรณ์มาซักพักแล้ว");
    
		if (rainRadarMap == 1) {
			HTTPClient http;  //Declare an object of class HTTPClient
			http.begin("http://localhost/bangkok_rain_radar_to_line.php");  //Specify request destination
			http.GET();
			Serial.println("ทำการเรียก Url เพื่อดึงข้อมูลเรดาร์ฝนส่งเข้า Line");                     //Print the response payload
			http.end();   //Close connection
		}
    }
  } else {
    noraincount = 0;
    }

  if ((timeClient.getMinutes()  == 0 || timeClient.getMinutes()  == 30) && timeClient.getSeconds()  < 10) {
    LINE.notify("ค่าน้ำฝนปัจจุบัน = " + String (val) + "\nค่าเฉลี่ยจากการคำนวนครั้งล่าสุด = " + String (median));
  }
  
  if (rainRadarMap == 1 && timeClient.getMinutes()  == 2  && timeClient.getSeconds()  < 10) {
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://localhost/bangkok_rain_radar_to_line.php");  //Specify request destination
    http.GET();
    Serial.println("ทำการเรียก Url เพื่อดึงข้อมูลเรดาร์ฝนส่งเข้า Line");
    http.end();   //Close connection
  }
  
  Serial.println("");
}
