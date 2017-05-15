#include <Microduino_Key.h>
#include "colorLed.h"
#include "userDef.h"
#include "eeprom.h"
#include "RefleJson.h"
#include "game.h"

void setup() {
  Serial.begin(57600);
  read_eeprom();
  if (winThreshold < 1 || winThreshold > 10 || lightmax < 10 || lightmax > 255 )
  {
    winThreshold = 3;
    lightmax = 50;
  }
  strip.begin();
  strip.setBrightness(lightmax);
#if DEBUG
  Serial.println("**************START************");
  Serial.println("--------------");
  Serial.print("WINT DATA:");
  Serial.println(winThreshold);
  Serial.print("LIGHT MAX DATA:");
  Serial.println(lightmax);
  Serial.println("--------------");
#endif
}

void loop() {
  reflejson_data();
  if (playStatus)           //游戏是否在进行
  {
    int8_t scoreCache = score;
    score = updateScore();  //计算分数差值。
    playReset();
    if (abs(score) >= winThreshold)  //游戏结束。
    {
      playStatus = false;
      playgo = true;
    }
    else if (score != scoreCache)
      ledScore(score);//闪烁分数差。
    updateLed();//闪烁新颜色。
  }
  else if (!playgo)
  {
    ledBreath(COLOR_INDIGO, 3);
    playReset();
  }
  else if (!playStatus)
    gameOver();
  delay(15);
}
