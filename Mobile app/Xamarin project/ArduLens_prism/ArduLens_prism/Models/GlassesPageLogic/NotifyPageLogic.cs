using System;
using System.Collections.Generic;
using System.Text;
using ArduLens_prism.Services;
using Prism.Ioc;

namespace ArduLens_prism.Models.GlassesPageLogic
{
    public class NotifyPageLogic : GlassesPageLogicBase
    {
        public override string Name => "Notification page";

        public override string Key => "notify_p";

        private DateTime _timeOfLastMessage;

        public NotifyPageLogic() 
        {
            var notifyListener = App.Current.Container.Resolve<INotificationListenerService>();
            notifyListener.NotificationPosted += OnNotifyPosted;
        }

        private void OnNotifyPosted(object sender, NotificationEventArgs e)
        {
            if (string.IsNullOrEmpty(e.Text) || string.IsNullOrEmpty(e.Title))
                return;
            if ((DateTime.Now - _timeOfLastMessage) < new TimeSpan(0, 0, 5))
                return;
            _timeOfLastMessage = DateTime.Now;
            string name = (!string.IsNullOrEmpty(e.AppName)) ? e.AppName : "Unknown";
            if (name.Length >= 13)
                name = name.Substring(0, 11) + "...";

            string title = (!string.IsNullOrEmpty(e.Title)) ? e.Title : "-";
            if (title.Length >= 10)
                title = title.Substring(0, 8) + "..."; 

            string text = (!string.IsNullOrEmpty(e.Text)) ? e.Text : "-";
            if (text.Length >= 38)
                text = text.Substring(0, 36) + "...";

            App.Current.Container.Resolve<GlassesController>().CreateNotification(name, title, text);
        }

        public override IList<string> OnShow()
        {
            return null;
        }
    }
}
