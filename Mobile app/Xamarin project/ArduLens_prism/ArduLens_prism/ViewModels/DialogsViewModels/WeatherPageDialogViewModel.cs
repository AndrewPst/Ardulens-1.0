using ArduLens_prism.Models.GlassesPageLogic;
using ArduLens_prism.Services;
using Prism.Services.Dialogs;
using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.ViewModels.DialogsViewModels
{
    public class WeatherPageDialogViewModel : DialogViewModelBase
    {
        private string _city;
        public string City
        {
            get => _city;
            set => SetProperty(ref _city, value);
        }

        private string _apiKey;
        public string ApiKey
        {
            get => _apiKey;
            set => SetProperty(ref _apiKey, value);
        }

        private IAppPreferences _preferences;

        private WeatherPageLogic _logicPage;

        public WeatherPageDialogViewModel(IAppPreferences appPreferences)
        {
            _preferences = appPreferences;
        }

        public override void Close()
        {
            _logicPage.ApiKey = ApiKey;
            _logicPage.City = City;
            _preferences.Save(ApiKey, "apikey", "weat_p");
            _preferences.Save(City, "city", "weat_p");
            base.Close();
        }

        public override void OnDialogOpened(IDialogParameters parameters)
        {
            _logicPage = parameters["glassesPageLogic"] as WeatherPageLogic;
            ApiKey = _logicPage.ApiKey;
            City = _logicPage.City;
        }
    }
}
