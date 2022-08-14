#ifndef BLE_CONTROLLER_
#define BLE_CONTROLLER_

#include <Arduino.h>
#include <Setup.h>

#include <NimBLEDevice.h>

#include <DataParser.h>
#include <Events.h>

typedef std::function<void(DataParser *data)> RequestCallBack;

class BleController : public NimBLEServerCallbacks
{
private:
    BleController();
    BleController(const BleController &) = delete;
    BleController &operator=(BleController &) = delete;

    SemaphoreHandle_t bleMutex_;

    NimBLECharacteristic *inputCharacteristic_ = nullptr;
    NimBLECharacteristic *outputCharacteristic_ = nullptr;
    NimBLEService *pService_ = nullptr;
    NimBLEServer *pServer_ = nullptr;

    DataParser dataParser_ = DataParser('|');

    bool isConnected_ = false;
    bool isNewDataReceived_ = false;

    char *buffer = nullptr;

    String macAddress_;

    void onConnect(NimBLEServer *pServer) override;
    void onDisconnect(NimBLEServer *pServer, ble_gap_conn_desc *desc) override;

    class DataReceiver : public NimBLECharacteristicCallbacks
    {
    public:
        void onWrite(NimBLECharacteristic *pCharacteristic) override;
    };

    static void onAdvComplete(NimBLEAdvertising *pAdvertising);

    RequestCallBack callback_ = nullptr;

public:
    static BleController &getInstance();

    EventsMap<String, DataParser *> OnNewDataReceived;
    Events<bool> OnConnectionStateChanged;

    void Initialize();

    bool isConnected();
    bool isNewDataReceived();

    DataParser *getReceivedData();

    void disconnect();
    void startAdvertising();

    void write(const char *data);
    bool sendRequest(char *key, RequestCallBack callback);
    void send(const char *key, std::initializer_list<const char *> array);
    String getDeviceMacAddress();
    void setButtPercent(uint16_t value);
};

extern BleController &ble;

#endif /*BLE_CONTROLLER_*/