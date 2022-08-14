using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using ArduLens_prism.Models;
using ArduLens_prism.Services;
using Java.Util;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
using Xamarin.Forms;

[assembly: Dependency(typeof(ArduLens_prism.Droid.Localize))]
namespace ArduLens_prism.Droid
{
    public class Localize : ILocalize
    {
        private Language _currentLang = null;
        public Language CurrentLanguage
        {
            get => _currentLang;
            set
            {
                Thread.CurrentThread.CurrentUICulture = new CultureInfo(value.Coding);
                Thread.CurrentThread.CurrentCulture = new CultureInfo(value.Coding);
                //if(_currentLang != null)
                //    RelaunchActivity();
                _currentLang = value;
            }
        }

        public CultureInfo GetCurrentCultureInfo()
        {
            var androidLocale = Java.Util.Locale.Default;
            var netLanguage = androidLocale.ToString().Replace("_", "-");
            return new System.Globalization.CultureInfo(netLanguage);
        }

        private void RelaunchActivity()
        {
            Context context = Android.App.Application.Context;
            Intent intent = new Intent(context, new MainActivity().Class);
            intent.AddFlags(ActivityFlags.NewTask);
            context.StartActivity(intent);
        }
    }
}