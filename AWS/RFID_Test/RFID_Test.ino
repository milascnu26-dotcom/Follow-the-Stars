#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_PN532.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C 핀 설정 (OLED와 PN532가 공유)
#define OLED_SDA D5 // GPIO 14
#define OLED_SCL D6 // GPIO 12

// PN532 제어용 IRQ 및 RESET 핀 설정
#define PN532_IRQ   D3 // GPIO 0
#define PN532_RESET D4 // GPIO 2

// 패시브 부저 핀 설정
#define BUZZER_PIN  D7 // GPIO 13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

bool nfcDetected = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting RFID + OLED Test...");

  // 부저 핀을 출력으로 설정
  pinMode(BUZZER_PIN, OUTPUT);

  // I2C 버스 시작 (D5 = SDA, D6 = SCL)
  Wire.begin(OLED_SDA, OLED_SCL);

  // OLED 초기화
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED allocation failed");
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  // PN532 NFC 리더 초기화
  nfc.begin();
  
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Didn't find PN532 board");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("PN532 Connection");
    display.println("Status: FAILED");
    display.println("\nCheck connection &");
    display.println("DIP switches (I2C)");
    display.display();
    while (1) {
      delay(10);
    }
  }
  
  nfcDetected = true;
  
  // Got ok data, print chip response
  Serial.print("Found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); 
  Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); 
  Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // SAM(Secure Access Module) 구성 설정
  nfc.SAMConfig();
  
  // OLED 화면에 성공 메시지 표시
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("PN532 RFID TEST");
  display.println("---------------------");
  display.println("Status: READY");
  display.println("OLED: OK (D5, D6)");
  display.println("PN532: OK (I2C)");
  display.println("BUZZER: OK (D7)");
  display.println("\nPlace tag near reader");
  display.display();

  // 부팅 성공 알림음 (도-미-솔-도 멜로디)
  tone(BUZZER_PIN, 523, 80);  // C5
  delay(100);
  tone(BUZZER_PIN, 659, 80);  // E5
  delay(100);
  tone(BUZZER_PIN, 784, 80);  // G5
  delay(100);
  tone(BUZZER_PIN, 1047, 150); // C6
  delay(1700); // 이전 delay(2000)에서 사운드 재생 딜레이(300ms) 차감
}

void loop() {
  if (!nfcDetected) return;

  // OLED 기본 스캔 화면 표시
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("   PN532 RFID TEST");
  display.println("=====================");
  display.setCursor(0, 24);
  display.println("Status: Scanning...");
  display.setCursor(0, 40);
  display.println("Place RFID tag / card");
  display.println("on the reader...");
  display.display();

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // UID 저장 버퍼
  uint8_t uidLength;                        // UID 길이 (4 또는 7 바이트)
  
  // 카드가 근처에 오는지 대기 (비차단형 대기: 200ms 타임아웃)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 200);
  
  if (success) {
    // 카드 인식 성공
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    // OLED 화면에 인식 결과 표시
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("   [ TAG DETECTED ]");
    display.println("=====================");
    
    // UID 길이 표시
    display.setCursor(0, 22);
    display.print("Type: MIFARE (");
    display.print(uidLength);
    display.println("B)");
    
    // UID 16진수 문자열 변환 및 표시
    display.setCursor(0, 36);
    display.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) display.print("0");
      display.print(uid[i], HEX);
      if (i < uidLength - 1) display.print(" ");
    }
    
    // 장식선 및 성공 메시지
    display.setCursor(0, 52);
    display.println("Read Successful! (OK)");
    display.display();

    // 카드 인식 성공음 (삐빅 두 번)
    tone(BUZZER_PIN, 2093, 80); // C7 (높은 도)
    delay(120);
    tone(BUZZER_PIN, 2093, 80); // C7
    
    // 시리얼 로그 확인을 위해 부저 연결 시 시그널용 딜레이
    delay(2300); // 삐빅 소리 지연 시간 200ms를 차감하여 이전과 동일한 2500ms 유지
  }
  
  delay(100); // 루프 주기에 따른 안정적인 지연 시간
}
