using Plugin.BLE.Abstractions.Contracts;
using Plugin.BLE.Abstractions.EventArgs;
using Plugin.BLE;
using System;
using System.Collections.ObjectModel;
using System.Text;
using System.Diagnostics;
using Plugin.BLE.Abstractions;
using System.Collections.Generic;
using System.Collections.Concurrent;
using Android.App;
using Android.Content;
using Android.Bluetooth;
using Android.Runtime;
using ArduLens_prism.Models;

using Xamarin.Forms;
using ArduLens_prism.Services;
using System.Threading.Tasks;

[assembly: Dependency(typeof(ArduLens_prism.Droid.BleService))]
namespace ArduLens_prism.Droid
{
    public class BleService : IBleService
    {

        private IBluetoothLE _ble;
        private IAdapter _adapter;

        private IService _service;
        private ICharacteristic _inputCharacteristic;
        private ICharacteristic _outputCharacteristic;

        private IReceivedDataParser _dataParser;

        private bool _isConnectionAttempt;

        private const string _serviceUUID = "C6FBDD3C-7123-4C9E-86AB-005F1A7EDA01";
        private const string _outputCharacteristicUUID = "B88E098B-E464-4B54-B827-79EB2B150A9F";
        private const string _inputCharacteristicUUID = "B88E098C-E464-4B54-B827-79EB2B150A9F";
        private const string _descriptorUUID = "00002902-0000-1000-8000-00805f9b34fb";

        public event EventHandler<DeviceEventArgs> ConnectionStateChanged;
        public event EventHandler<ReceivedData> ValueUpdated;

        private ConcurrentQueue<byte[]> _messageQueue;


        public event EventHandler<BluetoothStateChangedArgs> BleStateChanged
        {
            add => _ble.StateChanged += value;
            remove => _ble.StateChanged -= value;
        }

        public ObservableCollection<IDevice> FoundDevicesList { get; }
        public IDevice ConnectedDevice { get; private set; }

        public BluetoothState BluetoothState => _ble.State;

        public int ScanTimeOut
        {
            get => _adapter.ScanTimeout;
            set => _adapter.ScanTimeout = value;
        }

        public bool IsConnected { get; private set; }

        public async void ConnectToDevice(string device_id)
        {
            if (_isConnectionAttempt) return;
            try
            {
                if (string.IsNullOrEmpty(device_id))
                    return;
                _isConnectionAttempt = true;
                if (ConnectedDevice == null || ConnectedDevice.State != DeviceState.Connected)
                {
                    await _adapter.StopScanningForDevicesAsync();
                    await _adapter.ConnectToKnownDeviceAsync(Guid.Parse(device_id), new ConnectParameters(false, false));
                }
                _isConnectionAttempt = false;
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        public async void ConnectToDevice(IDevice device)
        {
            if (_isConnectionAttempt) return;
            try
            {
                if (device == null)
                    return;
                _isConnectionAttempt = true;
                if (ConnectedDevice == null || ConnectedDevice.State != DeviceState.Connected)
                {
                    await _adapter.StopScanningForDevicesAsync();
                    await _adapter.ConnectToDeviceAsync(device, new ConnectParameters(false, false));
                }
                _isConnectionAttempt = false;
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        public async void Disconnect()
        {
            try
            {
                await _adapter.DisconnectDeviceAsync(ConnectedDevice);

                _service.Dispose();
                _service = null;
                _inputCharacteristic = null;
                _outputCharacteristic = null;

                ConnectedDevice.Dispose();
                ConnectedDevice = null;
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        public async void ScanDevices()
        {
            try
            {
                if (!_adapter.IsScanning)
                {
                    FoundDevicesList.Clear();
                    await _adapter.StartScanningForDevicesAsync();
                }
                else
                {
                    await _adapter.StopScanningForDevicesAsync();
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        public void Send(byte[] data)
        {
            if (!IsConnected) return;
            _messageQueue.Enqueue(data);
            if (_messageQueue.Count <= 1)
                Task.Run(SendAllMessages);
        }

        public void EnebleBluetooth()
        {
            var btAdapter = BluetoothAdapter.DefaultAdapter;
            if (btAdapter != null)
            {
                btAdapter.Enable();
            }
        }

        public BleService(IReceivedDataParser dataParser, IAppEventsService appEventService)
        {
            _ble = CrossBluetoothLE.Current;
            _adapter = CrossBluetoothLE.Current.Adapter;

            _messageQueue = new ConcurrentQueue<byte[]>();

            _dataParser = dataParser;

            ScanTimeOut = 5000;
            _adapter.ScanTimeout = ScanTimeOut;

            FoundDevicesList = new ObservableCollection<IDevice>();
            appEventService.OnApplicationDestroyed += (o, e) =>
            {
                _adapter.StopScanningForDevicesAsync();
                if (IsConnected)
                    Disconnect();
            };

            _service = null;
            _inputCharacteristic = null;
            ConnectedDevice = null;

            _adapter.DeviceDiscovered += Adapter_DeviceDiscovered;
            _adapter.DeviceConnected += Adapter_DeviceConnected;
            _adapter.DeviceConnectionLost += Adapter_DeviceConnectionLost;
            _adapter.DeviceDisconnected += Adapter_DeviceDisconnected;
        }

        private void Adapter_DeviceDisconnected(object sender, DeviceEventArgs e)
        {
            IsConnected = false;
            ConnectionStateChanged?.Invoke(sender, e);
        }

        private void Adapter_DeviceConnectionLost(object sender, DeviceErrorEventArgs e)
        {
            if (e.Device.State == DeviceState.Disconnected)
            {
                IsConnected = false;
                ConnectionStateChanged?.Invoke(sender, new DeviceEventArgs() { Device = e.Device });
            }
        }

        ~BleService()
        {
            Disconnect();
        }

        private async void SendAllMessages()
        {
            while (!_messageQueue.IsEmpty)
            {
                byte[] arr;
                if (_messageQueue.TryDequeue(out arr))
                {
                    try
                    {
                        await _outputCharacteristic.WriteAsync(arr);
                    }
                    catch (Exception e)
                    {
                        Debug.WriteLine(e.Message);
                    }
                }
            }
        }

        private void Adapter_DeviceDiscovered(object sender, DeviceEventArgs e)
        {
            FoundDevicesList.Add(e.Device);
        }

        private void Characteristic_ValueUpdated(object sender, CharacteristicUpdatedEventArgs e)
        {
            Debug.WriteLine(e.Characteristic.StringValue);
            var data = _dataParser.Deserializate(e.Characteristic.Value);
            ValueUpdated?.Invoke(this, data);
        }

        private async void Adapter_DeviceConnected(object sender, DeviceEventArgs e)
        {
            try
            {
                int result = await e.Device.RequestMtuAsync(200 + 3);

                _service = await e.Device.GetServiceAsync(Guid.Parse(_serviceUUID));
                _inputCharacteristic = await _service?.GetCharacteristicAsync(Guid.Parse(_inputCharacteristicUUID));
                await _inputCharacteristic.GetDescriptorAsync(Guid.Parse(_descriptorUUID));
                _inputCharacteristic.ValueUpdated += Characteristic_ValueUpdated;

                _outputCharacteristic = await _service.GetCharacteristicAsync(Guid.Parse(_outputCharacteristicUUID));
                await _outputCharacteristic.GetDescriptorAsync(Guid.Parse(_descriptorUUID));

                await _inputCharacteristic.StartUpdatesAsync();
                await _outputCharacteristic.StartUpdatesAsync();

                ConnectedDevice = e.Device;
                Debug.WriteLine(ConnectedDevice.Name);

                IsConnected = true;
                ConnectionStateChanged?.Invoke(this, e);
            }
            catch (Exception err)
            {
                Debug.WriteLine(err.Message);
                Disconnect();
            }
        }
    }
}
