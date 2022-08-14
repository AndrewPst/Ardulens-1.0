using ArduLens_prism.Models;
using Plugin.BLE.Abstractions.Contracts;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;

namespace ArduLens_prism.Services
{
    public interface IBleService
    {
        event EventHandler<Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs> ConnectionStateChanged;

        event EventHandler<ReceivedData> ValueUpdated;

        event EventHandler<Plugin.BLE.Abstractions.EventArgs.BluetoothStateChangedArgs> BleStateChanged;

        BluetoothState BluetoothState { get; }
        int ScanTimeOut { get; }
        bool IsConnected { get; }
        IDevice ConnectedDevice { get; }

        void ConnectToDevice(string device_id);
        void ConnectToDevice(IDevice device);
        void Disconnect();
        void ScanDevices();
        void Send(byte[] data);

        void EnebleBluetooth();

        ObservableCollection<IDevice> FoundDevicesList { get;  }
    }
}
