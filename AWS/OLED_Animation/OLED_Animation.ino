#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// 사용자 설정: OLED SDA = D5 (GPIO 14), SCL = D6 (GPIO 12)
#define OLED_SDA 14 // GPIO 14 (D5)
#define OLED_SCL 12 // GPIO 12 (D6)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 애니메이션 전환 주기 (6초)
#define ANIMATION_DURATION 6000
unsigned long lastSwitchTime = 0;
int currentAnimation = 0;

// ==========================================
// 1. Starfield (3D 우주 별빛 이동)
// ==========================================
#define NUM_STARS 60
struct Star {
  float x, y, z;
};
Star stars[NUM_STARS];

void initStars() {
  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].x = random(-64, 64);
    stars[i].y = random(-32, 32);
    stars[i].z = random(1, 128);
  }
}

void drawStarfield() {
  display.clearDisplay();
  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].z -= 2; // 별이 화면 앞으로 가까워짐
    if (stars[i].z <= 0) {
      stars[i].x = random(-64, 64);
      stars[i].y = random(-32, 32);
      stars[i].z = 128;
    }
    
    // 3D 투영 연산
    int sx = (stars[i].x * 64) / stars[i].z + 64;
    int sy = (stars[i].y * 64) / stars[i].z + 32;
    
    if (sx >= 0 && sx < 128 && sy >= 0 && sy < 64) {
      if (stars[i].z < 40) {
        display.fillRect(sx, sy, 2, 2, SSD1306_WHITE); // 가까운 별은 크게
      } else {
        display.drawPixel(sx, sy, SSD1306_WHITE);      // 먼 별은 점으로
      }
    }
  }
  display.display();
}

// ==========================================
// 2. Rotating 3D Cube (회전하는 3D 큐브)
// ==========================================
struct Point3D {
  float x, y, z;
};

Point3D cube[8] = {
  {-20, -20, -20}, {20, -20, -20}, {20, 20, -20}, {-20, 20, -20},
  {-20, -20, 20}, {20, -20, 20}, {20, 20, 20}, {-20, 20, 20}
};

int edges[12][2] = {
  {0, 1}, {1, 2}, {2, 3}, {3, 0}, // 후면
  {4, 5}, {5, 6}, {6, 7}, {7, 4}, // 전면
  {0, 4}, {1, 5}, {2, 6}, {3, 7}  // 연결선
};

float angleX = 0;
float angleY = 0;
float angleZ = 0;

void drawCube() {
  display.clearDisplay();
  Point3D projected[8];
  
  float radX = angleX * PI / 180.0;
  float radY = angleY * PI / 180.0;
  float radZ = angleZ * PI / 180.0;
  
  for (int i = 0; i < 8; i++) {
    // X축 회전
    float y1 = cube[i].y * cos(radX) - cube[i].z * sin(radX);
    float z1 = cube[i].y * sin(radX) + cube[i].z * cos(radX);
    
    // Y축 회전
    float x2 = cube[i].x * cos(radY) + z1 * sin(radY);
    float z2 = -cube[i].x * sin(radY) + z1 * cos(radY);
    
    // Z축 회전
    float x3 = x2 * cos(radZ) - y1 * sin(radZ);
    float y3 = x2 * sin(radZ) + y1 * cos(radZ);
    
    // 2D 평면에 투영 및 화면 센터 이동
    projected[i].x = x3 + 64;
    projected[i].y = y3 + 32;
  }
  
  // 모서리 선 그리기
  for (int i = 0; i < 12; i++) {
    display.drawLine(projected[edges[i][0]].x, projected[edges[i][0]].y,
                     projected[edges[i][1]].x, projected[edges[i][1]].y, SSD1306_WHITE);
  }
  
  // 꼭짓점 점 그리기
  for (int i = 0; i < 8; i++) {
    display.fillRect(projected[i].x - 1, projected[i].y - 1, 3, 3, SSD1306_WHITE);
  }
  
  display.display();
  
  angleX += 2.0;
  angleY += 3.0;
  angleZ += 1.0;
}

// ==========================================
// 3. Sine Wave / Oscilloscope (사인 파형 오실로스코프)
// ==========================================
float phase = 0;

void drawSineWave() {
  display.clearDisplay();
  
  for (int x = 0; x < 128; x++) {
    float y1 = 32 + 15 * sin(x * 0.05 + phase);
    float y2 = 32 + 10 * sin(x * 0.08 - phase * 1.5);
    
    display.drawPixel(x, (int)y1, SSD1306_WHITE);
    display.drawPixel(x, (int)y2, SSD1306_WHITE);
    
    // 8픽셀 간격으로 두 파형을 잇는 점선 세로축 연결
    if (x % 8 == 0) {
      display.drawLine(x, (int)y1, x, (int)y2, SSD1306_WHITE);
    }
  }
  
  display.display();
  phase += 0.08;
}

// ==========================================
// 4. Bouncing Particles / Plexus (움직이는 점과 연결선)
// ==========================================
#define NUM_PARTICLES 12
struct Particle {
  float x, y;
  float vx, vy;
};
Particle particles[NUM_PARTICLES];

void initParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particles[i].x = random(5, 123);
    particles[i].y = random(5, 59);
    particles[i].vx = random(-150, 150) / 100.0;
    particles[i].vy = random(-150, 150) / 100.0;
    if (particles[i].vx == 0) particles[i].vx = 1.0;
    if (particles[i].vy == 0) particles[i].vy = 1.0;
  }
}

void drawParticles() {
  display.clearDisplay();
  
  // 파티클 위치 업데이트 및 점 그리기
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particles[i].x += particles[i].vx;
    particles[i].y += particles[i].vy;
    
    // 벽 반사 처리
    if (particles[i].x <= 0 || particles[i].x >= 127) particles[i].vx *= -1;
    if (particles[i].y <= 0 || particles[i].y >= 63) particles[i].vy *= -1;
    
    display.fillRect((int)particles[i].x - 1, (int)particles[i].y - 1, 3, 3, SSD1306_WHITE);
  }
  
  // 일정 거리 이내의 파티클들 사이에 선 연결
  for (int i = 0; i < NUM_PARTICLES; i++) {
    for (int j = i + 1; j < NUM_PARTICLES; j++) {
      float dx = particles[i].x - particles[j].x;
      float dy = particles[i].y - particles[j].y;
      float dist = sqrt(dx*dx + dy*dy);
      
      if (dist < 32) {
        display.drawLine((int)particles[i].x, (int)particles[i].y,
                         (int)particles[j].x, (int)particles[j].y, SSD1306_WHITE);
      }
    }
  }
  
  display.display();
}

// ==========================================
// 5. Radar Scanner (동심원 레이더 스캔 효과)
// ==========================================
float radarAngle = 0;

void drawRadar() {
  display.clearDisplay();
  
  int cx = 64;
  int cy = 32;
  int maxR = 30;
  
  // 동심원 점선 그리기
  for (int r = 10; r <= maxR; r += 10) {
    for (float a = 0; a < 2 * PI; a += 0.3) {
      int px = cx + r * cos(a);
      int py = cy + r * sin(a);
      display.drawPixel(px, py, SSD1306_WHITE);
    }
  }
  
  // 회전하는 스캔 선
  float rad = radarAngle * PI / 180.0;
  int lx = cx + maxR * cos(rad);
  int ly = cy + maxR * sin(rad);
  display.drawLine(cx, cy, lx, ly, SSD1306_WHITE);
  
  // 탐지되는 가상 타겟 점 (반짝임)
  int blip1_x = cx + 18 * cos(1.2);
  int blip1_y = cy + 18 * sin(1.2);
  int blip2_x = cx + 25 * cos(4.0);
  int blip2_y = cy + 25 * sin(4.0);
  
  // 스캔 선이 지나가는 각도 주변에 있을 때 타겟을 강조하여 그림
  float angleDiff1 = abs(radarAngle - (1.2 * 180.0 / PI));
  float angleDiff2 = abs(radarAngle - (4.0 * 180.0 / PI));
  
  if (angleDiff1 < 40 || angleDiff1 > 320) {
    display.fillRect(blip1_x - 1, blip1_y - 1, 3, 3, SSD1306_WHITE);
  }
  if (angleDiff2 < 40 || angleDiff2 > 320) {
    display.fillRect(blip2_x - 1, blip2_y - 1, 3, 3, SSD1306_WHITE);
  }
  
  display.display();
  
  radarAngle += 4;
  if (radarAngle >= 360) radarAngle = 0;
}

// ==========================================
// 메인 설정 및 루프
// ==========================================
void setup() {
  Serial.begin(115200);
  
  // 사용자가 정의한 D5(SDA), D6(SCL) 통신 시작
  Wire.begin(OLED_SDA, OLED_SCL);
  
  // SSD1306 초기화
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.display();
  
  // 난수 시드 초기화 및 요소 생성
  randomSeed(analogRead(0));
  initStars();
  initParticles();
  
  lastSwitchTime = millis();
}

void loop() {
  // 주기적으로 다음 애니메이션으로 변경
  if (millis() - lastSwitchTime > ANIMATION_DURATION) {
    currentAnimation = (currentAnimation + 1) % 5;
    lastSwitchTime = millis();
    
    // 상태 전환 시 재초기화
    if (currentAnimation == 0) initStars();
    if (currentAnimation == 3) initParticles();
  }
  
  switch(currentAnimation) {
    case 0:
      drawStarfield();
      break;
    case 1:
      drawCube();
      break;
    case 2:
      drawSineWave();
      break;
    case 3:
      drawParticles();
      break;
    case 4:
      drawRadar();
      break;
  }
  
  delay(10); // 약 60~100 FPS 수준의 딜레이
}
