using ArduLens_prism.Services;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using Xamarin.Essentials;
using Xamarin.Forms;

[assembly: Dependency(typeof(ArduLens_prism.Droid.AppPreferences))]
namespace ArduLens_prism.Droid
{
    public class AppPreferences : IAppPreferences
    {
        public void Save<T>(T value, string key, string directory)
        {
            Application.Current.Properties[directory + "|" + key] = value;
            Application.Current.SavePropertiesAsync();
        }

        public T Get<T>(string key, string directory, T defaultValue)
        {
            if (Application.Current.Properties.ContainsKey(directory + "|" + key))
            {
                try
                {
                    object result = Application.Current.Properties[directory + "|" + key];
                    T value = (T)result;
                    return value;
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.Message);
                }
            }
            return defaultValue;
        }

        public void Remove<T>(string key, string directory)
        {
            Application.Current.Properties.Remove(directory + "|" + key);
            Application.Current.SavePropertiesAsync();
        }

        public bool ConstrainKey(string key, string directory)
        {
            return Application.Current.Properties.ContainsKey(directory + "|" + key);
        }
    }
}
