using ArduLens_prism.Models;
using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.ViewModels.GlassesPropertiesViewModels
{
    public class ButteryProperty : GlassesPropertyBase
    {
        public override string Name => Resources.Resource.ButtLevel;

        public ButteryProperty() : base()
        {
            Value = "50%";
            _ble.ConnectionStateChanged += (o, e) => IsVisible = _ble.IsConnected;
            _controller.BatteryLevelChanged += (o, e) =>
            {
                IsCritical = e < 20;
                Value = e.ToString() + '%';
            };
        }
    }
}
