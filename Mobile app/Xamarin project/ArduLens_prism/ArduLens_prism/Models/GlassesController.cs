using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using ArduLens_prism.Models;
using ArduLens_prism.Services;
using ArduLens_prism.ViewModels;
using Plugin.BLE.Abstractions.Contracts;
using Prism.Ioc;
using ArduLens_prism.Models.GlassesPageLogic;
using Xamarin.Essentials;
using Prism.Mvvm;
using System.Diagnostics;
using ArduLens_prism.ViewModels.GlassesPropertiesViewModels;
using System.Threading.Tasks;

namespace ArduLens_prism.Models
{

    public static class CommandsKeys
    {
        public static string Command = "0";
        public static string Request = "1";
        public static string Answer = "2";
        public static string Setup = "3";
        public static string CommandForPage = "4";
        public static string Info = "5";
        public static string OpenPage = "6";
        public static string CreateNotification = "7";
    }

    /// <summary>
    /// This class is responsible for the interaction between the application and the glasses.
    /// </summary>
    public class GlassesController
    {
        #region Fields
        private IBleService _ble;
        private IAppPreferences _preferences;
        private IReceivedDataParser _dataParser;
        private GlassesPreferences _glassesPreferences;
        private PagesController _pagesController;

        private delegate void OnReceivedData(ReceivedData data);
        private IDictionary<string, OnReceivedData> _commandsKeys;

        private Action<ReceivedData> answerCallBack_ = null;
        #endregion

        #region events
        public event EventHandler<int> BatteryLevelChanged;
        #endregion

        public static GlassesController Current { get; private set; }

        #region Properties
        public PagesController MyPagesController { get => _pagesController; }

        private int _battLevel;
        public int BatteryLevel
        {
            get => _battLevel;
            set
            {
                if(value != _battLevel)
                {
                    _battLevel = value;
                    BatteryLevelChanged?.Invoke(this, _battLevel);
                }
            }
        }

        #endregion

        #region Initialize
        public GlassesController(IBleService ble, IAppPreferences preferences, IReceivedDataParser dataParser,
            GlassesPreferences glassesPreferences, PagesController pagesController)
        {
            Current = this;
            _ble = ble;
            _preferences = preferences;
            _dataParser = dataParser;
            _glassesPreferences = glassesPreferences;
            _pagesController = pagesController;

            InitCommands();

            _ble.ValueUpdated += OnDataReceived;
            _ble.ConnectionStateChanged += OnConnectionStateChanged;
        }

        private void InitCommands()
        {
            _commandsKeys = new Dictionary<string, OnReceivedData>()
            {
                {CommandsKeys.Info, OnInfoCommandReceived },
                {CommandsKeys.Request, OnRequestReceived },
                {CommandsKeys.Answer, OnAnswerReceived },
                {CommandsKeys.OpenPage, OnNewPageOpened },
                {CommandsKeys.CommandForPage, OnCommandForPageReceived }
            };
        }
        #endregion

        #region Methods
        public void SendPreferences()
        {
            ReceivedData data = new(CommandsKeys.Setup,
                _glassesPreferences.Language.Key.ToString(),
                _glassesPreferences.Brightness.ToString(),
                _glassesPreferences.ReturnToHomePageTime.ToString(),
                _glassesPreferences.ShutdownTime.ToString(),
                _glassesPreferences.AccSensativity.ToString()
            );
            var array = _dataParser.Serializate(data);
            _ble.Send(array);
        }
        public void SendTime(string key)
        {
            var a = DateTimeOffset.Now.Offset;
            ReceivedData data = new ReceivedData(key, "settime", (DateTimeOffset.Now.Add(a).ToUnixTimeSeconds()).ToString());
            var array = _dataParser.Serializate(data);
            _ble.Send(array);
        }
        public void SendRequest(Action<ReceivedData> callback, params string[] request)
        {
            //if (answerCallBack_ != null) return;
            answerCallBack_ = callback;
            ReceivedData data = new(CommandsKeys.Request, request);
            var array = _dataParser.Serializate(data);
            _ble.Send(array);
        }

        public void CreateNotification(string name, string title, string text)
        {
            var data = new ReceivedData(CommandsKeys.CreateNotification, name, title, text);
            var array = _dataParser.Serializate(data);
            _ble.Send(array);
        }
        
        #endregion

        #region Event listeners
        private void OnConnectionStateChanged(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            if (_ble.IsConnected)
            {
                SendPreferences();
                SendTime(CommandsKeys.Command);
            }
        }
        private void OnDataReceived(object sender, ReceivedData data)
        {
            if (!data.IsEmpty())
            {
                if (_commandsKeys.ContainsKey(data.Key))
                {
                    var method = _commandsKeys[data.Key];
                    method(data);
                }
            }
        }
        private void OnNewPageOpened(ReceivedData data) => _pagesController.SetOpenedPageFromGlasses(data.Parameters[0]);
        private void OnRequestReceived(ReceivedData data)
        {
            switch (data.Parameters[0])
            {
                case "time":
                    SendTime(CommandsKeys.Answer);
                    break;
            }
        }
        private void OnAnswerReceived(ReceivedData data)
        {
            if (answerCallBack_ != null)
            {
                answerCallBack_(data);
                answerCallBack_ = null;
            }
        }
        private void OnInfoCommandReceived(ReceivedData data)
        {
            switch (data.Parameters[0])
            {
                case "butt":
                    BatteryLevel = int.Parse(data.Parameters[1]);
                    break;
                default:
                    break;
            };
        }

        private void OnCommandForPageReceived(ReceivedData data)
        {
            _pagesController.PassCommandForPage(data.Parameters);
        }
        #endregion


        public class PagesController
        {
            #region Fields
            private IAppPreferences _preferences;
            private IBleService _ble;
            private IReceivedDataParser _dataParser;
            #endregion

            #region Events
            public event EventHandler<GlassesPageLogicBase> OpenedNewPage;
            public event EventHandler<GlassesPageLogicBase> SelectedNewHomePage;
            #endregion

            #region Properties
            private GlassesPageLogicBase _homepage;
            public GlassesPageLogicBase HomePage
            {
                get => _homepage;
                set
                {
                    if (value != null && value != _homepage)
                    {
                        if (_homepage != null)
                            _homepage.IsHomePage = false;
                        value.IsHomePage = true;
                        _preferences.Save(value.Name, "home_page_name", "ardulens");
                        _homepage = value;

                        ReceivedData data = new(CommandsKeys.OpenPage, new List<string>() { "sethome", value.Key });
                        var array = _dataParser.Serializate(data);
                        _ble.Send(array);

                        SelectedNewHomePage?.Invoke(null, _homepage);
                    }
                }
            }

            private GlassesPageLogicBase _openedpage;
            public GlassesPageLogicBase OpenedPage
            {
                get => _openedpage;
                private set
                {
                    if (value != null)
                    {
                        _openedpage?.OnHide();
                        _openedpage = value;
                        if (_ble.IsConnected)
                        {
                            List<string> list = new() { _openedpage.Key };
                            var args = _openedpage.OnShow();
                            if (args != null)
                            {
                                list.AddRange(args);
                                ReceivedData data = new(CommandsKeys.CommandForPage, list);
                                var array = _dataParser.Serializate(data);
                                _ble.Send(array);
                            }
                        }
                        OpenedNewPage?.Invoke(this, _openedpage);
                    }
                }
            }

            private int _indexOfOpenedPage = 0;
            public int IndexOfOpenedPage
            {
                get => _indexOfOpenedPage;
                set
                {
                    if (value > 0 && value < GlassesPages.Count)
                    {
                        OpenedPage = GlassesPages[value];
                        _indexOfOpenedPage = value;
                    }
                }
            }

            public List<GlassesPageLogicBase> GlassesPages => GlassesPageLogicBase.Pages;
            #endregion

            #region Initialize
            public PagesController(IBleService ble, IReceivedDataParser dataParser,
                IAppPreferences appPreferences)
            {
                _ble = ble;
                _dataParser = dataParser;
                _preferences = appPreferences;

                InitPages();
            }

            private void InitPages()
            {
                string HomePageName = _preferences.Get("home_page_name", "ardulens", string.Empty);

                GlassesPageLogicBase page = GlassesPages.Where(x => x.Name == HomePageName).FirstOrDefault();
                if (page != null)
                {
                    HomePage = page;
                    _indexOfOpenedPage = GlassesPages.IndexOf(HomePage);
                }
                else
                {
                    HomePage = GlassesPages[0];
                }
                OpenedPage = HomePage;
            }
            #endregion

            #region Methods
            public void SendCmdToPage(string key, params string[] args)
            {
                List<string> param = new() { key };
                param.AddRange(args);
                ReceivedData data = new(CommandsKeys.CommandForPage, param);
                var array = _dataParser.Serializate(data);
                _ble.Send(array);
            }
            public void SetOpenedPageFromGlasses(string key)
            {
                var newPage = GlassesPages.Where(x => x.Key == key)
                                          .FirstOrDefault();
                if (newPage != null)
                    OpenedPage = newPage;
            }
            
            public void OpenNewPage(GlassesPageLogicBase page)
            {
                if (page == null) return;
                ReceivedData data = new(CommandsKeys.OpenPage, page.Key);
                var array = _dataParser.Serializate(data);
                _ble.Send(array);
                _openedpage = page;
            }

            public void PassCommandForPage(IList<string> param_list)
            {
                GlassesPageLogicBase page = GlassesPages.Find(x => x.Key == param_list[0]);
                if (page == null)
                    return;
                List<string> list = new() { page.Key };
                var args = page.OnAction(param_list.Skip(1).ToList());
                if (args != null && args?.Count > 0)
                {
                    list.AddRange(args);
                    ReceivedData data = new(CommandsKeys.CommandForPage, list);
                    var array = _dataParser.Serializate(data);
                    _ble.Send(array);
                }
            }
            #endregion
        }
    } 
}
