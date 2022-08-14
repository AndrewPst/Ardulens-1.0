
using ArduLens_prism.Models;
using ArduLens_prism.Services;
using ArduLens_prism.ViewModels.GlassesPropertiesViewModels;
using Prism.Commands;
using Prism.Mvvm;
using Prism.Navigation;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
using Xamarin.Essentials;
using Xamarin.Forms;

namespace ArduLens_prism.ViewModels
{
    public class MainPageViewModel : BindableBase
    {
        //private const string Device_id = "00000000-0000-0000-0000-ac67b23693ae";

        private IBleService _ble;
        private INavigationService _navigation;
        private GlassesController _glassesController;
        private IAppPreferences _preferences;

        private ObservableCollection<GlassesPropertyBase> properties;
        public ObservableCollection<GlassesPropertyBase> Properties
        {
            get => properties;
            set => SetProperty(ref properties, value);
        }

        private bool is_connected;
        public bool IsConnected
        {
            get => is_connected;
            set
            {
                SetProperty(ref is_connected, value);
            }
        }

        public DelegateCommand ConnectCommand { get; }
        public DelegateCommand OpenListCommand { get; }

        public MainPageViewModel(IBleService ble, INavigationService navigationService,
                        GlassesController glassesController, IAppPreferences preferences, 
                        ILocalize localize, IAudioService audioService)
        {
            _ble = ble;
            _navigation = navigationService;
            _glassesController = glassesController;
            _preferences = preferences;

            ConnectCommand = new DelegateCommand(ConnectLogic);
            Properties = new(GlassesPropertyBase.Properties);

            _ble.ConnectionStateChanged += (o, e) =>
            {
                IsConnected = ble.IsConnected;
            };

            OpenListCommand = new DelegateCommand(() =>
            {
                Device.BeginInvokeOnMainThread(async () =>
                {
                    await _navigation.NavigateAsync("DetectedDevicesListPage", null, true, true);
                });
            });
            var v = _preferences.Get("TryConnectWhenOpenApp", "ardulens", false);
            if (v)
                ConnectLogic();
            IsConnected = ble.IsConnected;
        }

        private async void ConnectLogic()
        {
            if (_ble.IsConnected)
            {
                _ble.Disconnect();
                return;
            }

            if (_preferences.Get("autoconnect", "ble", false))
            {
                _ble.ConnectToDevice(_preferences.Get("device_id", "ble", string.Empty));
            }
            else
            {
                await _navigation.NavigateAsync("DetectedDevicesListPage", null, true, true);
            }
        }
    }
}
