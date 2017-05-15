#include <aJSON.h>
aJsonStream serial_stream(&Serial);

String restart = "";
String winT = "";
String lightS = "";
String projtctid = "";

String ID = "{\"ID\":\"590bc5e87bb3e300016e2da3\"}";

//{"leftScore":0}
//{"rightScore":1}
aJsonObject *createMessage(uint8_t x, uint8_t y)
{
  aJsonObject *msg = aJson.createObject();
  aJsonObject *analog = aJson.createItem(y);
  if (x == 0)
    aJson.addItemToObject(msg, "leftScore", analog);
  else if (x == 1)
    aJson.addItemToObject(msg, "rightScore", analog);
  return msg;
}

//{"ID":"project"}
void IDupload()
{
  Serial.print(ID.substring(0, 17));
  delay(30);
  Serial.println(ID.substring(17, 33));
  delay(30);
}


void Ctrldata(String _st, int _data)
{
  String send_data;
  int number;
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"\}";
  number = send_data.length() / 17;

  if (number == 0)
  {
    Serial.println(send_data);
    delay(30);
  }
  else
  {
    while (number >= 0)
    {
      Serial.print(send_data.substring(0, 17));
      send_data = send_data.substring(17, send_data.length());
      delay(30);
      number--;
    }
    Serial.print("\n");
  }
}

//{"restart":"true"}
//{"winThreshold":"3"}
//{"lightness":"20"}

void ComExecution(aJsonObject *msg)
{
  aJsonObject *IDval = aJson.getObjectItem(msg, "ID");
  if (IDval) {
    projtctid = IDval->valuestring;
    if (projtctid == "project")
    {
      IDupload();
      Ctrldata("winThreshold", winThreshold);
      Ctrldata("lightness", lightmax);
      Ctrldata("leftScore", 0);
      Ctrldata("rightScore", 0);
    }
  }
  else {
#if DEBUG
    Serial.println("no ID data");
#endif
  }

  aJsonObject *resetval = aJson.getObjectItem(msg, "restart");
  if (resetval) {
    restart = resetval->valuestring;
  }
  else {
#if DEBUG
    Serial.println("no restart data");
#endif
  }
  aJsonObject *winTval = aJson.getObjectItem(msg, "winThreshold");
  if (winTval) {
    winT = winTval->valuestring;
    winThreshold = winT.toInt();
    tone(PIN_BUZZER, 800, 300);
    allLedBlinkNum(1, COLOR_GREEN, 300);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no winTval data");
#endif
  }

  aJsonObject *lightval = aJson.getObjectItem(msg, "lightness");
  if (lightval) {
    lightS = lightval->valuestring;
    lightmax = lightS.toInt();
    strip.setBrightness(lightmax);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no lightness data");
#endif
  }
#if DEBUG
  Serial.println("--------------");
  Serial.print("RESET DATA:");
  Serial.println(restart);
  Serial.print("WINT DATA:");
  Serial.println(winThreshold);
  Serial.print("LIGHTMAX DATA:");
  Serial.println(lightmax);
  Serial.println("--------------");
#endif
}

void reflejson_data()
{
  if (serial_stream.available()) {
    serial_stream.skip();
  }
  if (serial_stream.available()) {
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}

void scoreupload(uint8_t x, uint8_t y)
{
  aJsonObject *msg = createMessage(x, y);
  aJson.print(msg, &serial_stream);
  Serial.println();
  aJson.deleteItem(msg);
}
