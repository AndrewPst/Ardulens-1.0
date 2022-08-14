using ArduLens_prism.Services;
using Plugin.BLE.Abstractions.Contracts;
using Prism.Commands;
using Prism.Mvvm;
using Prism.Navigation;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace ArduLens_prism.ViewModels
{
    public class DetectedDevicesListPageViewModel : BindableBase
    {
        private IBleService _ble;
        private INavigationService _navigation;
        private IAppPreferences _preferences;


        private IDevice _selectedDevice;
        public IDevice SelectedDevice
        {
            get => _selectedDevice;
            set
            {
                if (value == null)
                    return;
                _selectedDevice = null;
                RaisePropertyChanged();
                IsRefreshing = false;
                _ble.ConnectToDevice(value);

            }
        }

        private bool _isConnected;
        public bool IsConnected
        {
            get => _isConnected;
            set => SetProperty(ref _isConnected, value);
        }

        private bool is_refreshing;
        public bool IsRefreshing
        {
            get => is_refreshing;
            set
            {
                SetProperty(ref is_refreshing, value);
                if (value)
                    RefreshTimeOut();
            }
        }

        private bool is_auto_connect;
        public bool IsAutoConnect
        {
            get => is_auto_connect;
            set
            {
                if(SetProperty(ref is_auto_connect, value))
                {
                    _preferences.Save(is_auto_connect, "autoconnect", "ble");
                }
            }
        }

        public ObservableCollection<IDevice> DeviceList { get; }

        public DelegateCommand RefreshCommand { get; }
        public DelegateCommand DisconnectCommand { get; }

        public DetectedDevicesListPageViewModel(IBleService bleService, INavigationService navigationService, IAppPreferences preferences)
        {
            _ble = bleService;
            _navigation = navigationService;
            _preferences = preferences;

            DeviceList = _ble.FoundDevicesList;

            RefreshCommand = new(() => _ble.ScanDevices());
            DisconnectCommand = new(() =>
            {
                _ble.Disconnect();
                _ble.ScanDevices();
            });

            IsConnected = _ble.IsConnected;
            _ble.ConnectionStateChanged += Ble_DeviceConnected;
            is_auto_connect = _preferences.Get("autoconnect", "ble", false);

            IsRefreshing = true;
            _ble.ScanDevices();
        }

        private void Ble_DeviceConnected(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            if (_ble.IsConnected)
            {
                _preferences.Save(e.Device.Id.ToString(), "device_id", "ble");
                _preferences.Save(_ble.ConnectedDevice.Name, "device_name", "ble");
                Device.BeginInvokeOnMainThread(async () => await _navigation.GoBackAsync());
            } else
            {
                IsConnected = false;
            }
        }

        private async void RefreshTimeOut()
        {
            await Task.Delay(_ble.ScanTimeOut);
            IsRefreshing = false;
        }
    }
}
