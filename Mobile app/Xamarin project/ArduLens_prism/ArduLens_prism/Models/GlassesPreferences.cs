using ArduLens_prism.Services;
using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.Models
{
    public class GlassesPreferences
    {
        private IAppPreferences _appPreferences;

        private const string _directory = "glasses_pref";

        public static GlassesPreferences Instance { get; private set; }

        private int _returnToHomePageTime;
        public int ReturnToHomePageTime
        {
            get => _returnToHomePageTime;
            set
            {
                if (value >= 0)
                    _returnToHomePageTime = value;
            }
        }

        private int _shutdownTime;
        public int ShutdownTime
        {
            get => _shutdownTime;
            set
            {
                if (value >= 0)
                    _shutdownTime = value;
            }
        }

        private int _brightness;
        public int Brightness
        {
            get => _brightness;
            set
            {
                if (value >= 0 && value <= 255)
                    _brightness = value;
            }
        }

        private int _accSensativity;
        public int AccSensativity
        {
            get => _accSensativity;
            set
            {
                if (value >= 0 && value <= 100)
                {
                    _accSensativity = value;
                }
            }
        }

        private Language _language;
        public Language Language
        {
            get => _language;
            set
            {
                if(value != null)
                    _language = value;
            }
        }

        public GlassesPreferences(IAppPreferences appPreferences, ILocalize localize)
        {
            Instance = this;
            _appPreferences = appPreferences;

            _accSensativity = appPreferences.Get(nameof(AccSensativity), _directory, 80);
            _brightness = appPreferences.Get(nameof(Brightness), _directory, 80);
            _returnToHomePageTime = appPreferences.Get(nameof(ReturnToHomePageTime), _directory, 30);
            _shutdownTime = appPreferences.Get(nameof(ShutdownTime), _directory, 6);
            _language = localize.CurrentLanguage;
        }

        public void Save()
        {
            _appPreferences.Save(_accSensativity, nameof(AccSensativity), _directory);
            _appPreferences.Save(_brightness, nameof(Brightness), _directory);
            _appPreferences.Save(_returnToHomePageTime, nameof(ReturnToHomePageTime), _directory);
            _appPreferences.Save(_shutdownTime, nameof(ShutdownTime), _directory);
        }
    }
}
