
using ArduLens_prism.Services;
using Plugin.Permissions.Abstractions;
using Prism.Commands;
using Prism.Mvvm;
using Prism.Navigation;
using Prism.Services;
using Prism.Services.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace ArduLens_prism.ViewModels
{
    public class MainTabbedPageViewModel : ViewModelBase
    {
        private IPermissionService _permissionService;
        private INavigationService _navigation;
        private IDialogService _dialogServices;
        private IBleService _bleService;
        private INotificationListenerService _notificationListener;

        private Permission[] _permissions = new Permission[]
        {
            Permission.Contacts,
            Permission.Location,
            Permission.Phone,
            Permission.Sensors,
            Permission.Storage
        };

        public MainTabbedPageViewModel(INavigationService navigationService, IPermissionService permissionService,
                IDialogService dialogService, IBleService bleService, INotificationListenerService notificationListenerService,
                IPhoneStateListenerService phoneState)
                : base(navigationService)
        {
            _permissionService = permissionService;
            _navigation = navigationService;
            _dialogServices = dialogService;
            _bleService = bleService;
            _notificationListener = notificationListenerService;

            Title = "ArduLens";

            if (_bleService.BluetoothState != Plugin.BLE.Abstractions.Contracts.BluetoothState.On)
                RequestBtOn();

            _bleService.BleStateChanged += (o, e) =>
            {
                if (e.NewState == Plugin.BLE.Abstractions.Contracts.BluetoothState.Off)
                    RequestBtOn();
                if (e.NewState == Plugin.BLE.Abstractions.Contracts.BluetoothState.On)
                    _bleService.ConnectToDevice(_bleService.ConnectedDevice);
            };

            CheckPermissons();
            CheckAccessListenNotifications();
        }

        private async void CheckPermissons()
        {
            bool isRequestNeeding = await _permissionService.CheckAllPermissonsGranted(_permissions);

            if (!isRequestNeeding)
            {
                Device.BeginInvokeOnMainThread(async () =>
                {
                    bool isAllPermGranted = false;
                    do
                    {
                        bool permResult = false;
                        do
                        {
                            var result = await _dialogServices.ShowDialogAsync("TwoButtonsDialog",
                                                    new DialogParameters()
                                                    {
                                                        {"title",  "Grant permissions!"},
                                                        {"message", "Please, provide permissions for the full operation of the app" }
                                                    });
                            permResult = (bool)result.Parameters["result"];

                        } while (!permResult);
                        isAllPermGranted = await _permissionService.RequestPermissions(_permissions);
                    } while (!isAllPermGranted);
                });
            }
        }

        private void RequestBtOn()
        {
            Device.BeginInvokeOnMainThread(async () =>
            {
                var result = await _dialogServices.ShowDialogAsync("TwoButtonsDialog", new DialogParameters()
                {
                    {"title", "Bluetooth is disabled!" },
                    {"message", "Enable bluetooth?"}
                });
                if (((bool)result.Parameters["result"]))
                    _bleService.EnebleBluetooth();
            });
        }

        private void CheckAccessListenNotifications()
        {
            if (!_notificationListener.CheckNotificationSetting())
            {
                _notificationListener.MakePermissionRequest();
            }
        }

    }
}
