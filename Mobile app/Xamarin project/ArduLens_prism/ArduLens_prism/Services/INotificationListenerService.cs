using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.Services
{
    public interface INotificationListenerService
    {
        bool IsListenerConnected { get; }

        public event EventHandler<bool> ConnectionStateChanged;

        public event EventHandler<NotificationEventArgs> NotificationPosted;
        bool CheckNotificationSetting();
        void MakePermissionRequest();
    }

    public class NotificationEventArgs
    {
        public string Title { get; private set; }
        public string Text { get; private set; }
        public string AppName { get; private set; }
        public NotificationEventArgs(string title, string text, string appName)
        {
            Title = title;
            Text = text;
            AppName = appName;
        }

        bool Equals(NotificationEventArgs args)
        {
            return args.AppName == AppName && args.Title == Title && args.Text == Text;
        }
    } 
}
