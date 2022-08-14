using ArduLens_prism.Models;
using ArduLens_prism.Services;
using Prism.Commands;
using Prism.Mvvm;
using System.Collections.ObjectModel;
using Xamarin.Forms;
using ArduLens_prism.Models.GlassesPageLogic;
using Prism.Navigation;
using System.Text;
using Prism.Services.Dialogs;
using System.Globalization;
using System.Threading;
using System.Threading.Tasks;
using Xamarin.Essentials;

namespace ArduLens_prism.ViewModels
{
    public class CommandPageViewModel : BindableBase
    {
        private IBleService _ble;
        private INavigationService _navigation;
        private GlassesController _glassesController;
        private GlassesPreferences _glassesPreferences;
        private IDialogService _dialogService;
        private ILocalize _localizeService;
        private IAppPreferences _appPreferences;
        public ObservableCollection<GlassesPageLogicBase> GlassesPages { get; }

        public GlassesPageLogicBase SelectedPage
        {
            get => _glassesController.MyPagesController.OpenedPage;
            set
            {
                if (value != _glassesController.MyPagesController.OpenedPage && value != null)
                {
                    _glassesController.MyPagesController.OpenNewPage(value);
                    RaisePropertyChanged();
                }
            }
        }

        private bool _isConnected;
        public bool IsConnected
        {
            get => _isConnected;
            set => SetProperty(ref _isConnected, value);
        }

        #region glasses preferences

        private bool _isPreferencesUpdated = false;
        public bool IsPreferencesUpdated
        {
            get => _isPreferencesUpdated;
            set => SetProperty(ref _isPreferencesUpdated, value);
        }

        public int ReturnToHomePageTime
        {
            get => _glassesPreferences.ReturnToHomePageTime;
            set
            {
                if (value != _glassesPreferences.ReturnToHomePageTime)
                {
                    _glassesPreferences.ReturnToHomePageTime = value;
                    RaisePropertyChanged();
                    IsPreferencesUpdated = true;
                }
            }
        }


        public int Brightness
        {
            get => _glassesPreferences.Brightness;
            set
            {
                if (_glassesPreferences.Brightness != value)
                {
                    _glassesPreferences.Brightness = value;
                    IsPreferencesUpdated = true;
                    RaisePropertyChanged();
                }
            }
        }

        public int DisplayShutdownTime
        {
            get => _glassesPreferences.ShutdownTime;
            set
            {
                if (value != _glassesPreferences.ShutdownTime)
                {
                    _glassesPreferences.ShutdownTime = value;
                    RaisePropertyChanged();
                    IsPreferencesUpdated = true;
                }
            }
        }

        public int AccSensativity
        {
            get => _glassesPreferences.AccSensativity;
            set
            {
                if (_glassesPreferences.AccSensativity != value)
                {
                    _glassesPreferences.AccSensativity = value;
                    IsPreferencesUpdated = true;
                    RaisePropertyChanged();
                }
            }
        }

        private bool _tryConnectWhenOpenApp;
        public bool TryConnectWhenOpenApp
        {
            get => _tryConnectWhenOpenApp;
            set
            {
                if (SetProperty(ref _tryConnectWhenOpenApp, value))
                {
                    _appPreferences.Save(value, "TryConnectWhenOpenApp", "ardulens");
                }
            }
        }


        public DelegateCommand<string> SetReturnToHomePageTimeCommand { get; }
        public DelegateCommand<string> SetShutdownTimeCommand { get; }

        #endregion

        #region Commands
        public DelegateCommand<GlassesPageLogicBase> SettingsCommand { get; }

        public DelegateCommand OpenDebugCommand { get; }

        public DelegateCommand SetHomePageCommand { get; }

        public DelegateCommand OpenDevicesListCommand { get; }

        public DelegateCommand SetNewLanguageCommand { get; }

        public DelegateCommand SavePreferencesCommand { get; }
        #endregion

        public CommandPageViewModel(IBleService service, INavigationService navig, GlassesPreferences glassesPreferences,
            GlassesController glassesController, IDialogService dialogService, ILocalize localize, IAppPreferences appPreferences)
        {
            _ble = service;
            _navigation = navig;
            _glassesController = glassesController;
            _dialogService = dialogService;
            _localizeService = localize;
            _glassesPreferences = glassesPreferences;
            _appPreferences = appPreferences;

            _ble.ConnectionStateChanged += (o, e) => IsConnected = _ble.IsConnected;
            _glassesController.MyPagesController.OpenedNewPage += (o, e) => RaisePropertyChanged(nameof(SelectedPage));

            SettingsCommand = new(SettingsCommandLogic);
            OpenDebugCommand = new(OpenDebugLogic);
            SetHomePageCommand = new(() => _glassesController.MyPagesController.HomePage = SelectedPage);
            OpenDevicesListCommand = new(OpenDeviceListCommandLogic);
            SetNewLanguageCommand = new(SetNewLanguageLogic);

            SetReturnToHomePageTimeCommand = new(value =>
            {
                ReturnToHomePageTime = (value.Equals("+") ? _glassesPreferences.ReturnToHomePageTime + 5 : _glassesPreferences.ReturnToHomePageTime - 5);
            });

            SetShutdownTimeCommand = new(value =>
            {
                DisplayShutdownTime = (value.Equals("+") ? _glassesPreferences.ShutdownTime + 1 : _glassesPreferences.ShutdownTime - 1);
            });

            SavePreferencesCommand = new(() =>
            {
                IsPreferencesUpdated = false;
                _glassesPreferences.Save();
                _glassesController.SendPreferences();
            });
            _tryConnectWhenOpenApp = _appPreferences.Get("TryConnectWhenOpenApp", "ardulens", false);
            GlassesPages = new(_glassesController.MyPagesController.GlassesPages);
            IsConnected = _ble.IsConnected;
            SelectedPage = _glassesController.MyPagesController.OpenedPage;
        }

        private void SettingsCommandLogic(GlassesPageLogicBase args)
        {
            if (!string.IsNullOrEmpty(args.ViewModelKey))
            {
                Device.BeginInvokeOnMainThread(() =>
                {
                    _dialogService.ShowDialog(args.ViewModelKey, new DialogParameters()
                    {
                        {"glassesPageLogic", args }
                    });
                });
            }
        }

        private void SetNewLanguageLogic()
        {
            Device.BeginInvokeOnMainThread(() =>
            {
                _dialogService.ShowDialog("LanguagesDialog", value =>
                {
                    if (value.Parameters.ContainsKey("Language"))
                    {
                        Language lang = (value.Parameters["Language"] as Language);
                        if (lang == _localizeService.CurrentLanguage)
                            return;
                        Preferences.Set("current_lang", lang.Coding);
                        //Toast.MakeText(Android.App.Application.Context, "Please, restart application!", ToastLength.Long).Show();
                    }
                });
            });
        }

        private void OpenDebugLogic()
        {
            Device.BeginInvokeOnMainThread(async () =>
            {
                await _navigation.NavigateAsync("DebuggerPage", null, false, true);
            });
        }

        private void OpenDeviceListCommandLogic()
        {
            Device.BeginInvokeOnMainThread(async () =>
            {
                await _navigation.NavigateAsync("DetectedDevicesListPage", null, true, true);
            });
        }
    }
}
