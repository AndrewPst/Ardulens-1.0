#define SERVICE_UUID "C6FBDD3C-7123-4C9E-86AB-005F1A7EDA01" //при изменении не забудьте поменять и в приложении
#define INPUT_CHARACTERISTIC_UUID "B88E098B-E464-4B54-B827-79EB2B150A9F"
#define OUTPUT_CHARACTERISTIC_UUID "B88E098C-E464-4B54-B827-79EB2B150A9F"
// #define BATTERY_SERVICE_UUID "0000180F-0000-1000-8000-00805F9B34FB"
// #define BATTERY_CHARACTERISTIC_UUID "00002A19-0000-1000-8000-00805F9B34FB"
// #define DESCRIPTOR_2901_UUID "00002901-0000-1000-8000-00805F9B34FB"
// #define DESCRIPTOR_2902_UUID "00002902-0000-1000-8000-00805F9B34FB"

#define DEVICE_NAME "Ardulens_ESP32"

//Для изменения контактов дисплея используйте файл User_Setup.h
//в директории библиотеки TFT_eSPI

#define BATTERY_VOLTAGE_PIN 27

#define DISPLAY_LED_PIN 25

#define ON_CONNECTED_PAUSE_TIME 1600

#define CPU_FREQ_DEFAULT 80

#define INIT_BLE

#define DISPLAY_OFFSET 28

/*--------------SYSTEM COMMANDS-----------------*/
#define ON_CONNECT_COMMAND "#|5|connected|#"

#define SEND_REQUEST "#|1|"

#define CMDKEY_CMD       "0"
#define CMDKEY_REQUEST   "1"
#define CMDKEY_ANSWER    "2"
#define CMDKEY_SETUP     "3"
#define CMDKEY_CMDPAGE   "4"
#define CMDKEY_INFO      "5"
#define CMDKEY_OPEN_PAGE "6"
#define CMDKEY_CREATE_NOTIFY "7"

#define START_MESSAGE_STR   "#|"
#define END_MESSAGE_STR     "|#"

#define MESSAGE_SEPARATOR   "|"
#define MESSAGE_SYMBOL      "#"
