using ArduLens_prism.Services;
using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

[assembly: Dependency(typeof(ArduLens_prism.Droid.AppEventsService))]
namespace ArduLens_prism.Droid
{
    public class AppEventsService : IAppEventsService
    {
        public event EventHandler OnApplicationOpened;
        public event EventHandler OnApplicationClosed;
        public event EventHandler OnApplicationDestroyed;

        public void Close()
        {
            OnApplicationClosed?.Invoke(this, null);
        }

        public void Destroy()
        {
            OnApplicationDestroyed?.Invoke(this, null);
        }

        public void Open()
        {
            OnApplicationOpened?.Invoke(this, null);
        }
    }
}
