using ArduLens_prism.Services;
using System;
using System.Collections.Generic;
using System.Text;
using Prism.Ioc;

namespace ArduLens_prism.ViewModels.GlassesPropertiesViewModels
{
    public class LastConnectedDeviceProperty : GlassesPropertyBase
    {
        private IAppPreferences _preferences;
        public override string Name => Resources.Resource.LastConnectedDevice;

        public LastConnectedDeviceProperty() : base()
        {
            _preferences = App.Current.Container.Resolve<IAppPreferences>();
            Value = _preferences.Get("device_name", "ble", string.Empty);
            _ble.ConnectionStateChanged += (o, e) =>
            { 
                IsVisible = !_ble.IsConnected;
                Value = _preferences.Get("device_name", "ble", string.Empty);
            };
            IsVisible = true;
        }
    }
}
