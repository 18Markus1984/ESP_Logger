#include <SPI.h>
#include <SD.h>
#include <LedControl.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <esp_sntp.h>
#include <OneButton.h>

// =====================
// PIN-DEFINITIONEN
// =====================
#define BUTTON_PIN 2
#define LED_PIN    3   // Status-LED

#define DIN_PIN 4
#define CLK_PIN 5
#define CS_LED  6

#define SD_CS   9
#define SD_MOSI 20
#define SD_MISO 21
#define SD_SCK  10

// =====================
// LED-CONTROLLER
// =====================
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_LED, 1);

// =====================
// OneButton initialisieren
// =====================
OneButton button(BUTTON_PIN, true); // true = Input Pullup

// =====================
// Zähler & Log
// =====================
int counter = 0;
int counterAlle = 0;
File logfile;

// =====================
// Datum & Uhrzeit
// =====================
int editDay   = 3;
int editMonth = 2;
int editYear  = 2026;
int editHour = 12;
int editMin = 12;

enum EditField { DAY, MONTH, YEAR, HOUR, MIN, DONE };
EditField currentField = DAY;

bool blinkState = false;
unsigned long lastBlink = 0;

// =====================
// NTP-Zeit
// =====================
bool initTime() {
  configTzTime("CET-1CEST,M3.5.0/2,M10.5.0/3", "de.pool.ntp.org", "time.google.com");  Serial.print("Warte auf Netzwerkzeit");
  unsigned long start = millis();
  while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED || WiFi.status() != WL_CONNECTED) {
    if (millis() - start > 100000) {
      Serial.println("\n Keine Netzwerkzeit");
      if (loadLastDateTimeFromCSV(editDay, editMonth, editYear, editHour, editMin)) {
          Serial.println("Daten von SD geladen!"); // Test-Ausgabe 2
      } else {
          Serial.println("SD-Daten konnten nicht gelesen werden (Datei leer oder fehlt).");
      }
      Serial.println("Oeffne Offline-Editor..."); // Test-Ausgabe 3
      offlineDateEditor();
      setSystemTimeFromManual(editDay,editMonth,editYear,editHour,editMin);
      return false;
    }
    Serial.print(".");
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    digitalWrite(LED_PIN, LOW);
    delay(250); 
  }
  Serial.println("Zeit synchronisiert!");
  return true;
}

void offlineDateEditor() {
  Serial.println("Kein WLAN → Offline-Datum einstellen");
  button.reset();
  button.attachClick(editClick);
  button.attachLongPressStart(editLongPress);
  button.attachDoubleClick(editDoubleClick);
  int value = editDay+editMonth+editYear;
  showDate(editDay, editMonth, editYear);
  blinkState = true;
  lastBlink = millis();

  while(currentField != DONE)
  {
    button.tick();
    if (millis() - lastBlink > 200) {
      blinkState = !blinkState;
      lastBlink = millis();
      if(blinkState){
        if(currentField == HOUR || currentField == MIN) {
          showTime(editHour, editMin);
        } else {
          showDate(editDay, editMonth, editYear);
        }
      } else {
        switch (currentField){
        case DAY:
          showDate(0, editMonth, editYear);
          break;
        case MONTH:
          showDate(editDay, 0, editYear);
          break;
        case YEAR:
          showDate(editDay, editMonth, 0);
          break;
        case HOUR:
          showTime(0, editMin);
          break;
        case MIN:
          showTime(editHour, 0);
          break;

        }
      }
    }
  }
}

void showTime(int editHour, int editMin){
  lc.clearDisplay(0);
  if(editHour > 0) {  
    showNumberAt(editHour, 4,true);
    if(editHour<10){
    lc.setDigit(0, 5, 0, false);
    }
  }
  if(editMin > 0) {
    showNumberAt(editMin, 2,false);
    if(editMin<10){
      lc.setDigit(0, 3, 0, false);
    }
  }
}

void showDate(int editDay, int editMonth, int editYear){
  lc.clearDisplay(0);
  if(editDay > 0) {  
    showNumberAt(editDay, 6,true);
    if(editDay<10){
    lc.setDigit(0, 7, 0, false);
    }
  }
  if(editMonth > 0) {
    showNumberAt(editMonth, 4,true);
    if(editMonth<10){
      lc.setDigit(0, 5, 0, false);
    }
  }
  if(editYear > 0){
    showNumberAt(editYear,0, false);
  }
}

void showNumberAt(int number, int index, bool mark){
    while (number > 0 && index < 8) {
      if(mark) {
        lc.setDigit(0, index, number % 10, true);
        mark = false;
      }
      else {
        lc.setDigit(0, index, number % 10, false);
      }
      number /= 10;
      index++;
    }
}

void editDoubleClick() {
  if (currentField == DAY) {
    editDay += 4;
    if (editDay > 31) editDay = 1;
  }
  else if (currentField == MONTH) {
    editMonth += 2;
    if (editMonth > 12) editMonth = 1;
  }
  else if (currentField == YEAR) {
    editYear++;
    if (editYear > 2099) editYear = 2020;
  }
  else if (currentField == HOUR) {
    editHour += 2;
    if (editHour > 24) editHour = 0;
  }
  else if (currentField == MIN) {
    editMin += 5;
    if (editMin > 60) editMin = 0;
  }
  digitalWrite(LED_PIN, HIGH);
  delay(120);
  digitalWrite(LED_PIN, LOW);
  delay(120);
}

void editClick() {
  if (currentField == DAY) {
    editDay++;
    if (editDay > 31) editDay = 1;
  }
  else if (currentField == MONTH) {
    editMonth++;
    if (editMonth > 12) editMonth = 1;
  }
  else if (currentField == YEAR) {
    editYear++;
    if (editYear > 2099) editYear = 2020;
  }
  else if (currentField == HOUR) {
    editHour++;
    if (editHour > 24) editHour = 0;
  }
  else if (currentField == MIN) {
    editMin++;
    if (editMin > 60) editMin = 0;
  }
  digitalWrite(LED_PIN, HIGH);
  delay(120);
  digitalWrite(LED_PIN, LOW);
  delay(120);
}

void editLongPress() {
  currentField = (EditField)((int)currentField + 1);
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(120);
    digitalWrite(LED_PIN, LOW);
    delay(120);
  }
}

void setSystemTimeFromManual(
  int day, int month, int year,
  int hour, int minute
) {
  struct tm t;
  t.tm_year = year - 1900;   // WICHTIG
  t.tm_mon  = month - 1;     // 0–11
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min  = minute;
  t.tm_sec  = 0;
  t.tm_isdst = -1;           // Sommerzeit automatisch

  time_t now = mktime(&t);

  struct timeval tv;
  tv.tv_sec = now;
  tv.tv_usec = 0;

  settimeofday(&tv, NULL);

  Serial.println("Systemzeit manuell gesetzt");
}

String getTimestamp() {
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);

  char buffer[32];
  char hourStr[3];
  sprintf(hourStr, "%d", t->tm_hour);
  sprintf(buffer, "%02d.%02d.%04d;%s:%02d",
          t->tm_mday,
          t->tm_mon + 1,
          t->tm_year + 1900,
          hourStr,
          t->tm_min);

  return String(buffer);
}

// =====================
// Display-Funktion
// =====================
void showNumber(int number) {
  lc.clearDisplay(0);
  if (number == 0) {
    lc.setDigit(0, 0, 0, false);
    return;
  }

  int index = 0;
  while (number > 0 && index < 8) {
    lc.setDigit(0, index, number % 10, false);
    number /= 10;
    index++;
  }
}

// =====================
// CSV-Funktionen
// =====================
int loadCounterFromCSV() {
  if (!SD.exists("/log.csv")) return 0;

  File f = SD.open("/log.csv");
  if (!f) return 0;

  String lastLine = "";
  while (f.available()) lastLine = f.readStringUntil('\n');
  f.close();

  if (lastLine.startsWith("Datum")) return 0;

  int lastSemicolon = lastLine.lastIndexOf(';');
  if (lastSemicolon < 0) return 0;

  String numberPart = lastLine.substring(lastSemicolon + 1);
  numberPart.trim();

  return numberPart.toInt();
}

void getTodayString(char* buffer) {
  time_t now = time(NULL);
  struct tm* t = localtime(&now);

  sprintf(buffer, "%02d.%02d.%04d",
          t->tm_mday,
          t->tm_mon + 1,
          t->tm_year + 1900);
}

int countVisitorsToday() {
  if (!SD.exists("/log.csv")) return 0;

  File f = SD.open("/log.csv");
  if (!f) return 0;

  char today[16];
  getTodayString(today);

  int count = 0;

  while (f.available()) {
    String line = f.readStringUntil('\n');
    if (line.startsWith(today)) {
      count++;
    }
  }

  f.close();
  return count+1;
}

bool removeLastCSVEntry() {
  if (!SD.exists("/log.csv")) return false;

  File f = SD.open("/log.csv", FILE_READ);
  if (!f) return false;

  std::vector<String> lines;
  while (f.available()) {
    String line = f.readStringUntil('\n');
    if (line.endsWith("\r")) line.remove(line.length() - 1);
    lines.push_back(line);
  }
  f.close();

  if (lines.size() <= 1) return false; // nur Header vorhanden

  lines.pop_back(); // letzte Zeile entfernen

  File w = SD.open("/log.csv", FILE_WRITE);
  if (!w) return false;

  for (auto &l : lines) w.println(l);
  w.close();
  return true;
}

bool loadLastDateTimeFromCSV(int &day, int &month, int &year, int &hour, int &minute) {
  if (!SD.exists("/log.csv")) return false;

  File f = SD.open("/log.csv", FILE_READ);
  if (!f) return false;

  String lastValidLine = "";
  
  // Wir lesen die Datei, merken uns aber immer nur die aktuelle Zeile,
  // wenn sie nicht leer ist. So landet die letzte gefüllte Zeile in lastValidLine.
  while (f.available()) {
    String line = f.readStringUntil('\n');
    line.trim(); // Entfernt \r und Leerzeichen
    if (line.length() > 10) { 
      lastValidLine = line;
    }
  }
  f.close();

  // Debug: Was wurde wirklich gefunden?
  Serial.print("Gefundene Zeile: "); Serial.println(lastValidLine);

  if (lastValidLine.length() < 10) return false;

  // Zerlegen: 14.01.2026;17:47;120
  int p1 = lastValidLine.indexOf(';');
  int p2 = lastValidLine.indexOf(';', p1 + 1);

  if (p1 == -1 || p2 == -1) return false;

  // Datum parsen
  day   = lastValidLine.substring(0, 2).toInt();
  month = lastValidLine.substring(3, 5).toInt();
  year  = lastValidLine.substring(6, p1).toInt();

  // Zeit parsen
  String timePart = lastValidLine.substring(p1 + 1, p2);
  int colon = timePart.indexOf(':');
  if (colon != -1) {
    hour   = timePart.substring(0, colon).toInt();
    minute = timePart.substring(colon + 1).toInt();
  }
  

  return true;
}

// =====================
// Button Events
// =====================
void onClick() {
  counter++;
  showNumber(countVisitorsToday());

  String ts = getTimestamp();
  String line = ts + ";" + String(counter);

  logfile = SD.open("/log.csv", FILE_APPEND);
  if (logfile) {
    logfile.println(line);
    logfile.close();
  }

  Serial.printf("Short press -> Counter: %d\n", counter);
}

void onLongPress() {
  if (counter > 0) {
    counter--;
    showNumber(countVisitorsToday());
  }

  bool ok = removeLastCSVEntry();
  if (ok) Serial.println("Letzter Eintrag entfernt");
  else Serial.println("Kein Eintrag zum Entfernen");

  // LED blinkt als Feedback
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(120);
    digitalWrite(LED_PIN, LOW);
    delay(120);
  }
}

// =====================
// Setup
// =====================
void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  delay(50);

  bool portalRequested = (digitalRead(BUTTON_PIN) == LOW);

  WiFi.mode(WIFI_STA);
  Serial.print("Aktuelle MAC: ");
  Serial.println(WiFi.macAddress());
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  WiFi.setAutoReconnect(true);
  WiFi.setSleep(false);
  WiFi.persistent(true);
  
  WiFiManager wm;
  wm.setConfigPortalTimeout(180);
  wm.setBreakAfterConfig(true);
  // Zeigt die Eingabefelder für statische IP, Gateway und DNS im Portal an
  wm.setShowStaticFields(true); 
  // Optional: Standardwerte vorausfüllen

  if (portalRequested) {
    Serial.println("Button gedrückt → WiFiManager Portal");
    digitalWrite(LED_PIN, HIGH);

    if (!wm.startConfigPortal("Besucherzaehler-Setup")) {
      Serial.println("Portal Timeout → Reboot");
      ESP.restart();
    }
  } else {
    Serial.println("Normaler WLAN-Start");
  }

  Serial.print("Warte auf Netzwerkzeit");
  wm.autoConnect();
  unsigned long start = millis();
  if (!wm.autoConnect("Besucherzaehler-Setup")) {
    Serial.println("WLAN fehlgeschlagen");
  } else {
    Serial.println("WLAN verbunden");
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
  }

  digitalWrite(LED_PIN, LOW);

  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, SPI)) Serial.println("SD-Karte Fehler!");
  else {
    Serial.println("SD-Karte OK!");
    if (!SD.exists("/log.csv")) {
      logfile = SD.open("/log.csv", FILE_WRITE);
      logfile.println("Datum;Uhrzeit;Zaehler");
      logfile.close();
      counter = 0;
    } else {
      counter = loadCounterFromCSV();
      Serial.printf("Letzter Zählerwert: %d\n", counter);
    }
  }

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  showNumber(counter);

  initTime();
  lc.clearDisplay(0);
  showNumber(countVisitorsToday()-1);
  delay(2000);

  // OneButton Events verbinden
  button.attachClick(onClick);
  button.attachLongPressStart(onLongPress);

  Serial.println("System bereit");
}

// =====================
// Loop
// =====================
void loop() {
  button.tick(); // muss sehr oft aufgerufen werden
  if(HIGH == digitalRead(BUTTON_PIN)){
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, HIGH);
  }
}
