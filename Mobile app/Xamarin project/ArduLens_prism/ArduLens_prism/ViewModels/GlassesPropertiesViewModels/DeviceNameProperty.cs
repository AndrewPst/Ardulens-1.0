using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.ViewModels.GlassesPropertiesViewModels
{
    public class DeviceNameProperty : GlassesPropertyBase
    {
        public override string Name => Resources.Resource.DeviceName;

        public DeviceNameProperty() : base()
        {
            IsVisible = false;
            IsCritical = false;
            _ble.ConnectionStateChanged += OnConnectionStateChanged;
        }

        private void OnConnectionStateChanged(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            if (_ble.IsConnected)
            {
                Value = _ble.ConnectedDevice.Name;
                IsVisible = true;
            }
            else
            {
                IsVisible = false;
            }
        }
    }
}
