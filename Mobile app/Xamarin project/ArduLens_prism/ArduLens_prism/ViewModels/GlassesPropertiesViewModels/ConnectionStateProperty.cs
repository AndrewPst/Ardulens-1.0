using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.ViewModels.GlassesPropertiesViewModels
{
    public class ConnectionStateProperty : GlassesPropertyBase
    {
        public override string Name => Resources.Resource.ConnectionState;

        public ConnectionStateProperty() : base()
        {
            IsVisible = true;
            OnConnectionStateChanged(null, null);
            _ble.ConnectionStateChanged += OnConnectionStateChanged;
        }

        private void OnConnectionStateChanged(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            if (_ble.IsConnected)
            {
                Value = Resources.Resource.Connected;
                IsCritical = false;
            }
            else
            {
                Value = Resources.Resource.Disconnected;
                IsCritical = true;
            }
        }
    }
}
