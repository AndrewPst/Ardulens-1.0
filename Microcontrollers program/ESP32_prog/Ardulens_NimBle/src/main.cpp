#include <Arduino.h>

uint16_t readBatteryVoltage();
uint16_t readBatteryPercent();

#include <BleController.h>
#include <DataParser.h>
#include <PagesController.h>
#include <MotionDetector.h>
#include <Resource.h>
#include <Settings.h>
#include <TimeParser.h>
#include <Display.h>
#include <EnergyController.h>
#include <ButtonsController.h>
#include <SPIFFS.h>
#include <FS.h>

void OnConnectionStateChanged(bool data);
void OnCmdReceived(DataParser *data);
void OnRequestReceived(DataParser *data);
void onEnergyStateChanged(bool mode);

void checkBatteryVoltage();

//Для измерения заряда батареи
#define VOLTAGE_ARRAY_SIZE 15
uint16_t *voltArr_;
uint16_t vArrCursor_ = 0; //курсор для записи в массиве

void setup()
{
  Serial.begin(115200);

  setCpuFrequencyMhz(CPU_FREQ_DEFAULT);

  log_i("Starting module initialization");

  voltArr_ = new uint16_t[VOLTAGE_ARRAY_SIZE]{0};

  SPIFFS.begin();
  settings.Initialize();
  resource.initializate();
  motionDetector.initialize();
  buttons.initialize();
  pages.Initialize();
#ifdef INIT_BLE
  ble.Initialize();
  //добавляем слушателей
  ble.OnConnectionStateChanged += OnConnectionStateChanged; 
  ble.OnNewDataReceived.subscribe(CMDKEY_CMD, OnCmdReceived);
  ble.OnNewDataReceived.subscribe(CMDKEY_REQUEST, OnRequestReceived);
  
  Serial.println(ble.getDeviceMacAddress());//если использовать log_d - выводятся кракозябры
  ble.startAdvertising();//запускаем рассылку
#endif
  log_i("Initializate done");
  energy.updateLastActiveTime();
}

void loop()
{
  energy.checkEnergySaveNeeding();
  checkBatteryVoltage();
  static uint32_t timer = 0;
  if(millis() - timer >= 10000){
    timer = millis();
    uint16_t percent = readBatteryPercent();
    log_d("voltage: %d", readBatteryVoltage());
    ble.send(CMDKEY_INFO, {"butt", String(percent).c_str()});
  }
  delay(10);
}

uint16_t readBatteryVoltage()
{
  uint32_t avr = 0;
  for(uint16_t i = 0; i < VOLTAGE_ARRAY_SIZE; i++){
    avr += voltArr_[i];
  }
  avr /= VOLTAGE_ARRAY_SIZE;
  return avr;
}

uint16_t readBatteryPercent()
{
  return constrain(map(readBatteryVoltage(), 2350, 2850, 0, 20) * 5, 0, 100);
}

void checkBatteryVoltage()
{
  vArrCursor_ = vArrCursor_ >= VOLTAGE_ARRAY_SIZE ? 0 : vArrCursor_ + 1;
  voltArr_[vArrCursor_] = analogReadMilliVolts(BATTERY_VOLTAGE_PIN);
}

void OnConnectionStateChanged(bool data) //событие подключения
{
  if (!data)
  {
    pages.createNotification(new NotificationParameters_t(resource[0], resource[1], resource[2]));
    ble.startAdvertising(); //рассылка рекламы при отключении
  }
  else
    //отправка данных об успешном подключении. Необходима задержка, иначе не отправляются
    xTaskCreateUniversal([](void *args)
                         {
                           delay(ON_CONNECTED_PAUSE_TIME);
                           ble.send(CMDKEY_OPEN_PAGE, {pages.getOpenedPage()->getKey()});
                           vTaskDelete(nullptr);
                         },
                         "send_answer", 4096, nullptr, 1, nullptr, 1);
}

void OnCmdReceived(DataParser *data)
{
  if (!strcmp(data->getParameter(0), "settime"))
  {
    timeNow.setEpochTime(atoll(data->getParameter(1)));
  }
}

void OnRequestReceived(DataParser *data)
{
  char *answer;
  if (!strcmp(data->getParameter(0), "page"))
  {
    answer = (char *)pages.getOpenedPage()->getKey();
    ble.send(CMDKEY_ANSWER, {answer});
  }
}