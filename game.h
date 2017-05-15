Key keyA(PIN_KEYA, INPUT_PULLUP);
Key keyB(PIN_KEYB, INPUT_PULLUP);

uint32_t ledTimer;
uint16_t scoreA, scoreB;
int8_t score;
uint8_t ledX, ledY;
bool playStatus, playgo;
uint8_t colorNum;
uint32_t scoreTimer;

//---------------闪烁分数差-----------------//
void ledScore(int8_t _score)
{
  setAllLed(COLOR_NONE);
  if (_score > 0)
    ledBlinkNum(_score, COLOR_COLD, 0, 300);
  else if (_score < 0)
    ledBlinkNum(abs(_score), COLOR_COLD, 1, 300);
}

//----------ColorLED和Buzzer--------------//
void soundInit()
{
  setAllLed(COLOR_RED);
  tone(PIN_BUZZER, 1000);
  delay(500);
  setAllLed(COLOR_NONE);
  noTone(PIN_BUZZER);
  delay(500);
  setAllLed(COLOR_WARM);
  tone(PIN_BUZZER, 1000);
  delay(500);
  setAllLed(COLOR_NONE);
  noTone(PIN_BUZZER);
  delay(500);
  setAllLed(COLOR_GREEN);
  tone(PIN_BUZZER, 1500);
  delay(500);
  noTone(PIN_BUZZER);
  setAllLed(COLOR_NONE);
}

//--------------闪烁新颜色-----------------//
void updateLed()
{
  if (millis() > ledTimer)
  {
    if (ledX == 0 || ledY == 0)
    {
      ledX = random(5, 9);
      ledY = random(5, 9);
      ledTimer = millis() + random(LED_INTERVAL - 200, LED_INTERVAL);//闪烁一定时间。
    }
    else
    {
      ledX = 0;
      ledY = 0;
      ledTimer = millis() + LED_INTERVAL / 2;   //熄灭一定时间。
    }
    setLed(ledX, 0);
    setLed(ledY, 1);
  }
}

//---------------计算得分-----------------//
int8_t updateScore()
{
  if (!digitalRead(PIN_KEYA))
  {
    setLed(COLOR_NONE, 1);
    if (ledX == ledY && ledX > 0)
    {
      tone(PIN_BUZZER, 500, 300);
      if (scoreA < 255)
        scoreA++;
      else
        scoreA = 255;
      scoreupload(0, scoreA);
      ledBlinkNum(1, COLOR_GREEN, 0, 600);
    }
    else
    {
      tone(PIN_BUZZER, 1000, 300);
      if (scoreB < 255)
        scoreB++;
      else
        scoreB = 255;
      scoreupload(1, scoreB);
      ledBlinkNum(1, COLOR_RED, 0, 600);
    }
    ledX = 0;
    ledY = 0;
  }
  else if (!digitalRead(PIN_KEYB))
  {
    setLed(COLOR_NONE, 0);
    if (ledX == ledY && ledX > 0)
    {
      tone(PIN_BUZZER, 500, 300);
      scoreB++;
      scoreupload(1, scoreB);
      ledBlinkNum(1, COLOR_GREEN, 1, 600);
    }
    else
    {
      tone(PIN_BUZZER, 1000, 300);
      scoreA++;
      scoreupload(0, scoreA);
      ledBlinkNum(1, COLOR_RED, 1, 600);
    }
    ledX = 0;
    ledY = 0;
  }
  return (scoreA - scoreB);
}

void playReset()
{
  if (keyA.read() == LONG_PRESS || keyB.read() == LONG_PRESS || restart == "true")
  {
    restart = "";
    if (scoreA != 0 || scoreB != 0)
    {
      scoreA = 0;
      scoreB = 0;
      scoreupload(0, scoreA);
      delay(30);
      scoreupload(1, scoreB);
      delay(30);
    }
    soundInit();
    playStatus = true;
    playgo = true;
  }
}

//------------游戏结束-------------//
void gameOver()
{
  uint32_t bTimer = millis();
  while (millis() - bTimer < 300)
  {
    if (colorNum % 2)
    {
      setAllLed(COLOR_GREEN);
      tone(PIN_BUZZER, 1000);
    }
    else
    {
      noTone(PIN_BUZZER);
      setAllLed(COLOR_NONE);
    }
    playReset();
  }
  colorNum++;
  if (colorNum > 9)
    colorNum = 1;
}
