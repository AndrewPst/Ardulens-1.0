#include <BleController.h>

#define ENTER_BLE xSemaphoreTake(ble.bleMutex_, portMAX_DELAY)
#define EXIT_BLE xSemaphoreGive(ble.bleMutex_)

//#define ENTER_BLE
//#define EXIT_BLE

BleController &BleController::getInstance()
{
    static BleController ble;
    return ble;
}

BleController::BleController()
{
    bleMutex_ = xSemaphoreCreateMutex();
}

void BleController::onConnect(NimBLEServer *server)
{
    isConnected_ = true;
    log_d("connected");
    OnConnectionStateChanged.Invoke(true);
}

void BleController::onDisconnect(NimBLEServer *pServer, ble_gap_conn_desc *desc)
{
    isConnected_ = false;
    log_d("Disconnected");
    NimBLEDevice::whiteListAdd(NimBLEAddress(desc->peer_ota_addr));
    OnConnectionStateChanged.Invoke(false);
}

void BleController::DataReceiver::onWrite(NimBLECharacteristic *pCharacteristic)
{
    ENTER_BLE;
    if (ble.buffer != nullptr)
        delete[] ble.buffer;
    ble.buffer = new char[pCharacteristic->getValue().length() + 1];
    strcpy(ble.buffer, pCharacteristic->getValue().c_str());
    log_i("data received: %s", ble.buffer);
    ble.dataParser_.parse(ble.buffer);
    EXIT_BLE;
    if (ble.callback_ != nullptr)
    {
        if (!strcmp(ble.dataParser_.getParameter(0), CMDKEY_ANSWER))
        {
            ble.callback_(&ble.dataParser_);
            ble.callback_ = nullptr;
        }
    }
    ble.isNewDataReceived_ = true;
    ble.OnNewDataReceived.Invoke(ble.dataParser_.getKey(), &ble.dataParser_);
}

void BleController::Initialize()
{
    ENTER_BLE;
    NimBLEDevice::init(DEVICE_NAME);
    NimBLEDevice::setPower(ESP_PWR_LVL_P3);
    pServer_ = NimBLEDevice::createServer();
    pServer_->setCallbacks(this);
    pService_ = pServer_->createService(SERVICE_UUID);
    inputCharacteristic_ = pService_->createCharacteristic(INPUT_CHARACTERISTIC_UUID,
                                                           NIMBLE_PROPERTY::READ |
                                                               NIMBLE_PROPERTY::WRITE |
                                                               NIMBLE_PROPERTY::NOTIFY);
    inputCharacteristic_->setCallbacks(new DataReceiver());
    outputCharacteristic_ = pService_->createCharacteristic(OUTPUT_CHARACTERISTIC_UUID,
                                                            NIMBLE_PROPERTY::READ |
                                                                NIMBLE_PROPERTY::NOTIFY);
    pService_->start();
    EXIT_BLE;
    macAddress_ = BLEDevice::getAddress().toString().c_str();
}

void BleController::startAdvertising()
{
    ENTER_BLE;
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    if (NimBLEDevice::getWhiteListCount() > 0)
    {
        // Allow anyone to scan but only whitelisted can connect.
        pAdvertising->setScanFilter(false, true);
    }
    // advertise with whitelist for 30 seconds
    pAdvertising->setScanResponse(true);
    pAdvertising->start(30, onAdvComplete);
    EXIT_BLE;
}
void BleController::onAdvComplete(NimBLEAdvertising *pAdvertising)
{
    Serial.println("Advertising stopped");
    if (ble.isConnected_)
    {
        return;
    }
    // If advertising timed out without connection start advertising without whitelist filter
    pAdvertising->setScanFilter(false, false);
    pAdvertising->start();
}

bool BleController::isConnected()
{
    return isConnected_;
}

bool BleController::isNewDataReceived()
{
    if (isNewDataReceived_)
    {
        isNewDataReceived_ = false;
        return true;
    }
    return false;
}

DataParser *BleController::getReceivedData()
{
    return &dataParser_;
}

void BleController::disconnect()
{
    ENTER_BLE;
    pServer_->disconnect(0);
    EXIT_BLE;
}

void BleController::write(const char *data)
{
    ENTER_BLE;
    if (outputCharacteristic_ != nullptr && isConnected_)
    {
        outputCharacteristic_->setValue((const uint8_t *)data, strlen(data));
        outputCharacteristic_->notify();
    }
    EXIT_BLE;
}

bool BleController::sendRequest(char *key, RequestCallBack callback)
{
    if (callback_ != nullptr)
        return false;
    send(SEND_REQUEST, {key});
    callback_ = callback;
    return true;
}

void BleController::send(const char *key, std::initializer_list<const char *> array)
{
    if (!isConnected_)
        return;
    uint16_t buf_size = strlen(START_MESSAGE_STR) + strlen(END_MESSAGE_STR) + 1;
    for (auto &i : array)
    {
        buf_size += strlen(i) + 1;
    }
    char *buffer = (char *)malloc(buf_size + 1);
    strcpy(buffer, START_MESSAGE_STR);
    strcat(buffer, key);
    strcat(buffer, "|");
    for (auto &i : array)
    {
        strcat(buffer, i);
        strcat(buffer, "|");
    }
    strcat(buffer, "#");
    strcat(buffer, "\0");
    write(buffer);
    log_i("Send: %S", buffer);
    free(buffer);
}

String BleController::getDeviceMacAddress()
{
    return macAddress_;
}

BleController &ble = BleController::getInstance();