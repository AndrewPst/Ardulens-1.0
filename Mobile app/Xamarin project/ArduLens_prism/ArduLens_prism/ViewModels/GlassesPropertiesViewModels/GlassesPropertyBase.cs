using ArduLens_prism.Services;
using Prism.Mvvm;
using Prism.Ioc;
using System;
using System.Collections.Generic;
using System.Text;
using ArduLens_prism.Models;

namespace ArduLens_prism.ViewModels.GlassesPropertiesViewModels
{
    public abstract class GlassesPropertyBase : BindableBase
    {
        public static List<GlassesPropertyBase> Properties { get; } = new()
        {
            new ConnectionStateProperty(),
            new DeviceNameProperty(),
            new LastConnectedDeviceProperty(),
            new ButteryProperty()
        };

        protected IBleService _ble;
        protected GlassesController _controller;
        
        public abstract string Name { get; }

        private string _value;
        public string Value
        {
            get => _value;
            set => SetProperty(ref _value, value);
        }

        private bool _isCritical;
        public bool IsCritical
        {
            get => _isCritical;
            set => SetProperty(ref _isCritical, value);
        }

        private bool _isVisible;
        public bool IsVisible
        {
            get => _isVisible;
            set => SetProperty(ref _isVisible, value);
        }

        public GlassesPropertyBase()
        {
            _ble = App.Current.Container.Resolve<IBleService>();
            _controller = App.Current.Container.Resolve<GlassesController>();
        }
    }
}
