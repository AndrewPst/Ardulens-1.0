using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using Newtonsoft.Json;
using System.Text;
using Prism.Ioc;
using ArduLens_prism.Services;

namespace ArduLens_prism.Models.GlassesPageLogic
{
    public class WeatherPageLogic : GlassesPageLogicBase
    {
        public override string Name => "Weather page";

        public override string ViewModelKey => "WeatherPage_glasses";

        public override string Key => "weat_p";


        private bool _isRequest = false;
        private string _lang = "en";

        private string _city;
        public string City
        {
            get => _city;
            set
            {
                if (_city != value)
                    result = null;
                _city = value;
            }
        }
        public string ApiKey { get; set; }

        private WeatherResponse result = null;

        private object _locker = new object();

        public WeatherPageLogic()
        {
            var pref = App.Current.Container.Resolve<IAppPreferences>();
            City = pref.Get("city", "weat_p", "London");
            ApiKey = pref.Get("apikey", "weat_p", string.Empty);
        }

        public override IList<string> OnShow()
        {
            _lang = ArduLens_prism.Resources.Resource.Culture.Parent.Name;
            return null;
        }

        public override IList<string> OnAction(IList<string> list)
        {
            if (list[0].Equals("1"))
            {
                if (result == null)
                    RequestWeather();
                else
                    SendCurrentData();
            }
            return null;
        }

        private async void RequestWeather()
        {
            if (_isRequest) return;
            _isRequest = true;
            try
            {
                HttpWebRequest httprequest = (HttpWebRequest)WebRequest.Create($"http://api.openweathermap.org/data/2.5/weather?q={City}&appid={ApiKey}&units=metric&lang={_lang}");
                HttpWebResponse httpresponse = (HttpWebResponse)await httprequest.GetResponseAsync();
                string response;
                using (StreamReader reader = new(httpresponse.GetResponseStream()))
                {
                    response = reader.ReadToEnd();
                }
                lock (_locker)
                {
                    result = JsonConvert.DeserializeObject<WeatherResponse>(response);
                }
            }
            catch
            {

            }
            SendCurrentData();
            _isRequest = false;
        }

        private void SendCurrentData()
        {
            lock (_locker)
            {
                if (result != null)
                {
                    GlassesController.Current.MyPagesController.SendCmdToPage(Key, result.Main.Temp.ToString(), result.Name + ":\n" + result.Weather[0].Description);
                }
            }
        }

        public class WeatherResponse
        {
            public WeatherDescription[] Weather { get; set; }
            public TemperatureInfo Main { get; set; }
            public string Name { get; set; }
        }

        public class WeatherDescription
        {
            public string Main { get; set; }

            private string _desctiption;
            public string Description
            {
                get => _desctiption;
                set
                {
                    _desctiption = value;
                    if (value.Length > 15)
                        _desctiption = value.Substring(0, 14) + "...";
                }
            }
        }
        public class TemperatureInfo
        {
            public float Temp { get; set; }
        }
    }
}
