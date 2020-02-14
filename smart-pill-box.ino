
#include <time.h>
#include <core_build_options.h>
#include <swRTC.h>


int sensor = A2;                        
time_t     tm_st; // 사용자입력
time_t     tm_nd; // 현재시간
int        tm_day, tm_hour, tm_min, tm_sec;
double     d_diff;
struct tm  user_stime;
struct tm  user_stime2;

int light_chk = 0;
int angle_chk = 0;

int  led = 13;

swRTC rtc;  //클래스 개체 선언

//현재시간
int nhour=0;
int nminu=0;

void setup(){
  setUserTime(0, 0, 0, 2019, 7, 1);
  tm_st = mktime( &user_stime); 

  
  pinMode(led, OUTPUT);    

  //현재시간
  rtc.stopRTC();            //정지
  rtc.setTime(0, 0, 0);  //시, 분, 초
  rtc.setDate(1, 7, 2019); //일, 월, 년
  rtc.startRTC();    
  
  Serial.begin(9600);                
}

void setUserTime(int h, int i, int s, int y, int m, int d) {
  user_stime.tm_year   = y   -1900;   // 주의 :년도는 1900년부터 시작
  user_stime.tm_mon    = m      -1;      // 주의 :월은 0부터 시작
  user_stime.tm_mday   = d;
  user_stime.tm_hour   = h;
  user_stime.tm_min    = i;
  user_stime.tm_sec    = s;
  user_stime.tm_isdst  = 0;    

}

void loop(){
  int val = analogRead(sensor);             //압전 센서의 데이터 받아오기
  int angle = map(val, 0, 1023, 0, 180);    //압력센서 값을 모터값으로 변환

  setUserTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getYear(), rtc.getMonth(), rtc.getDay());
  tm_nd = mktime( &user_stime); 

  // 시간차
  d_diff  = difftime( tm_nd, tm_st);
  tm_min   = d_diff / 60; // 초기값이랑 몇분 차이 나는지

  //  압력 센서가 처음 들어갔으면 time 바꾸기
  if(angle_chk == 0 && angle > 0) {
    angle_chk = 1; // 지금 압력 들어가고 있다는거 체크
    Serial.println("angle_chk=1");

    // 기준 시간 바꿔주기
    setUserTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getYear(), rtc.getMonth(), rtc.getDay());
    tm_st = mktime( &user_stime);
  }


  // 알약 가져갔으면
  if(angle_chk == 1 && angle == 0) {
    angle_chk = 0; 
    light_chk = 0; // light도 끄기
    digitalWrite(led,LOW);

    Serial.println("angle_chk=0, light=0");

    // 기준 시간 바꿔주기
    setUserTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getYear(), rtc.getMonth(), rtc.getDay());
    tm_st = mktime( &user_stime);
    
  }

  // 10분동안 알약 안가져가면
  if(angle_chk == 1 && tm_min >= 1) { // 나중에 10으로 숫자바꾸기
    digitalWrite(led, HIGH); 
    light_chk = 1;
    Serial.println("light=1");
  }


  Serial.println(tm_nd);
  Serial.println(tm_st);
  Serial.println(tm_min);
  Serial.println(angle);   

  Serial.print(rtc.getHours(), DEC);
  Serial.print(":");
  Serial.print(rtc.getMinutes(), DEC);
  Serial.print(":");
  Serial.print(rtc.getSeconds(), DEC);
  Serial.print("\t"); 

  Serial.print(rtc.getYear(), DEC);
  Serial.print("/");
  Serial.print(rtc.getMonth(), DEC);
  Serial.print("/");
  Serial.println(rtc.getDay(), DEC); 
  
  Serial.println();           
  delay(1000);
}
