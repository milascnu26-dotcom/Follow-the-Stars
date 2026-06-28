#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C 핀 설정 (OLED)
#define OLED_SDA D5 // GPIO 14
#define OLED_SCL D6 // GPIO 12

// 패시브 부저 핀 설정 (선택 사항)
#define BUZZER_PIN D7 // GPIO 13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 와이파이 설정 (사용자 정보 입력 필요)
const char* ssid = "YOUR_SSID";         // 연결할 와이파이 SSID로 변경하세요
const char* password = "YOUR_PASSWORD"; // 와이파이 비밀번호로 변경하세요

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  // I2C 및 OLED 초기화
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED allocation failed");
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  
  // 1. 와이파이 연결 시도
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Connecting to WiFi...");
    display.println("SSID: " + String(ssid));
    display.println("\nSearching...");
    display.display();
    
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 와이파이 연결 성공음 (삐빅)
  tone(BUZZER_PIN, 1500, 80);
  delay(120);
  tone(BUZZER_PIN, 1500, 80);
  delay(100);

  // 2. NTP 서버 시간 동기화 설정 (한국 표준시 UTC+9: 9 * 3600 = 32400초)
  configTime(32400, 0, "pool.ntp.org", "time.nist.gov");
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected!");
  display.println("IP: " + WiFi.localIP().toString());
  display.println("\nSyncing NTP Time...");
  display.display();

  // 최초 동기화 대기 (시간 값이 유효해질 때까지)
  time_t now = time(nullptr);
  while (now < 24 * 3600 * 365) { // 1971년 이전 타임스탬프면 대기
    delay(200);
    now = time(nullptr);
  }
  Serial.println("NTP Time Synced!");
}

void loop() {
  // 현재 시간 가져오기
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);

  // 시간 버퍼 생성
  char dateStr[20];
  char timeStr[20];
  strftime(dateStr, sizeof(dateStr), "%Y-%m-%d (%a)", timeinfo); // 2026-06-24 (Wed)
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", timeinfo);        // 16:30:15

  // OLED 화면 출력
  display.clearDisplay();
  
  // 1. 헤더 (와이파이 정보 및 연결 상태)
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("    WIFI NTP CLOCK");
  display.println("---------------------");
  
  // 2. 날짜 출력 (텍스트 크기 1)
  display.setCursor(12, 22);
  display.println(dateStr);
  
  // 3. 시간 출력 (텍스트 크기 2로 크게 표시)
  display.setTextSize(2);
  display.setCursor(16, 36);
  display.println(timeStr);
  
  // 4. 하단 IP 주소 안내 (텍스트 크기 1)
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("IP: ");
  display.println(WiFi.localIP());

  display.display();
  
  delay(200); // 0.2초 주기로 화면 갱신
}
