using Android.App;
using Android.Content;
using Android.Content.PM;
using Android.Service.Notification;
using Android.Widget;
using System;
using System.Diagnostics;
using System.Threading.Tasks;
using Xamarin.Forms;
using ArduLens_prism.Services;
using Android.Runtime;
using Prism.Services.Dialogs;

[assembly: Dependency(typeof(ArduLens_prism.Droid.NLService))]
namespace ArduLens_prism.Droid
{
    /// <summary>
    /// Посредник между сервисом уведомлений и другими сервисами
    /// </summary>
    public class NLService : INotificationListenerService
    {
        public event EventHandler<bool> ConnectionStateChanged;
        public event EventHandler<NotificationEventArgs> NotificationPosted;
        private IDialogService _dialogServices;
        public bool IsListenerConnected
        {
            get
            {
                if (NotificationListener.Current != null)
                    return NotificationListener.Current.IsListenerConnected;
                return false;
            }
        }

        public NLService(IAppEventsService appEvents, IDialogService dService)
        {
            NotificationListener.ConnectionStateChanged += OnConnectionStateChanged;
            NotificationListener.NotificationPosted += OnNotificationPosted;
            _dialogServices = dService;

            appEvents.OnApplicationDestroyed += (o, e) =>
            {
                Android.App.Application.Context.StopService(new Intent(Android.App.Application.Context, Java.Lang.Class.FromType(typeof(NotificationListener))));
            };
        }

        private void OnConnectionStateChanged(object s, bool e) => ConnectionStateChanged?.Invoke(this, e);
        private void OnNotificationPosted(object s, NotificationEventArgs e) => NotificationPosted?.Invoke(this, e);
        public bool CheckNotificationSetting()
        {
            NotificationManager notificationManager = Android.App.Application.Context.GetSystemService(Context.NotificationService) as NotificationManager;
            return notificationManager.IsNotificationListenerAccessGranted(
                new Android.Content.ComponentName(Android.App.Application.Context.PackageName,
                Java.Lang.Class.FromType(typeof(NotificationListener)).Name));
        }

        public void MakePermissionRequest()
        {
            Device.BeginInvokeOnMainThread(async () =>
            {
                var result = await _dialogServices.ShowDialogAsync("TwoButtonsDialog", new DialogParameters()
                    {
                        {"title", "Access to notifications" },
                        {"message", "Please grant the app access to notifications!"}
                    });
                Intent intent = new Intent("android.settings.ACTION_NOTIFICATION_LISTENER_SETTINGS");
                intent.SetFlags(ActivityFlags.NewTask);
                Android.App.Application.Context.StartActivity(intent);
            });
        }
    }


    [Service(Label = "Ardulens Notification listener service", Permission = "android.permission.BIND_NOTIFICATION_LISTENER_SERVICE")]
    [IntentFilter(new[] { "android.service.notification.NotificationListenerService" })]
    class NotificationListener : Android.Service.Notification.NotificationListenerService
    {
        /// <summary>
        /// Вызывается при получении нового уведомления
        /// </summary>
        public static event EventHandler<NotificationEventArgs> NotificationPosted;

        /// <summary>
        /// Вызывается при изменении статуса подключения слушателя
        /// </summary>
        public static event EventHandler<bool> ConnectionStateChanged;

        public bool IsListenerConnected { get; private set; }
        public static NotificationListener Current { get; private set; }

        public override void OnListenerConnected()
        {
            base.OnListenerConnected();
            IsListenerConnected = true;
            ConnectionStateChanged?.Invoke(this, IsListenerConnected);
        }

        public override void OnListenerDisconnected()
        {
            base.OnListenerDisconnected();
            IsListenerConnected = false;
            ConnectionStateChanged?.Invoke(this, IsListenerConnected);
        }

        /// <summary>
        /// Создает несколько экземпляров. Причина не ясна
        /// </summary>
        public NotificationListener()
        {
            Current = this;
        }

        public override void OnCreate()
        {
            base.OnCreate();
        }

        public override void OnNotificationPosted(StatusBarNotification sbn)
        {
            if (sbn?.PackageName != "com.android.systemui")
            {
                PackageManager pm = ApplicationContext.PackageManager;
                ApplicationInfo ai;
                try
                {
                    ai = pm.GetApplicationInfo(sbn?.PackageName, 0);
                }
                catch (Exception e)
                {
                    ai = null;
                    Debug.WriteLine(e.Message);
                }
                string applicationName = (string)(ai != null ? pm.GetApplicationLabel(ai) : string.Empty);
                var text = sbn.Notification.Extras.GetCharSequence(Notification.ExtraText);
                var title = sbn.Notification.Extras.GetCharSequence(Notification.ExtraTitle);
                var args = new NotificationEventArgs(title, text, applicationName);
                NotificationPosted?.Invoke(this, args);
                Debug.WriteLine("\ntext: " + text + "\ntitle: " + title + "\nApp:" + applicationName);
            }
        }
        [return: GeneratedEnum]
        public override StartCommandResult OnStartCommand(Intent intent, [GeneratedEnum] StartCommandFlags flags, int startId)
        {
            base.OnStartCommand(intent, flags, startId);
            return StartCommandResult.NotSticky;
        }

        public override void OnDestroy()
        {
            base.OnDestroy();
        }

        /// <summary>
        /// Отключает оповещение об уведомлениях. Модет вызывать ошибки
        /// </summary>
        public void StopReceive()
        {
            if (IsListenerConnected)
            {
                RequestUnbind();
            }
        }

        /// <summary>
        /// Восстанавливает получение уведомлений
        /// </summary>
        public static void ResumeReceive() => RequestRebind(new Android.Content.ComponentName(Android.App.Application.Context.PackageName, Java.Lang.Class.FromType(typeof(NotificationListener)).Name));
    }
}
