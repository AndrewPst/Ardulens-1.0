using System;

namespace ArduLens_prism.Services
{
    public interface IAppEventsService
    {
        event EventHandler OnApplicationOpened;
        event EventHandler OnApplicationClosed;
        event EventHandler OnApplicationDestroyed;

        void Open();
        void Close();
        void Destroy();
    }
}
