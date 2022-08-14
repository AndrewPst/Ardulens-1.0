using Android.App;
using Android.Content.PM;
using Android.OS;
using Android.Runtime;
using Prism;
using Prism.Ioc;
using PanCardView.Droid;
using Plugin.Permissions;
using ArduLens_prism.Services;
using Android.App.Roles;
using Android.Content;

namespace ArduLens_prism.Droid
{
    [Activity(Theme = "@style/MainTheme", ScreenOrientation = ScreenOrientation.Portrait, 
              ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize)]
    public class MainActivity : global::Xamarin.Forms.Platform.Android.FormsAppCompatActivity
    {

        protected override void OnCreate(Bundle savedInstanceState)
        {

            TabLayoutResource = Resource.Layout.Tabbar;
            ToolbarResource = Resource.Layout.Toolbar;

            base.OnCreate(savedInstanceState);
            global::Xamarin.Forms.Forms.Init(this, savedInstanceState);
            global::Rg.Plugins.Popup.Popup.Init(this);
            CardsViewRenderer.Preserve();
            Xamarin.Essentials.Platform.Init(this, savedInstanceState);
            //RoleManager roleManager = (RoleManager)Android.App.Application.Context.GetSystemService(Context.RoleService);
            //Intent intent = roleManager.CreateRequestRoleIntent(RoleManager.RoleCallScreening);
            //StartActivityForResult(intent, 0);
            LoadApplication(new App(new AndroidInitializer()));
        }

        public override void OnRequestPermissionsResult(int requestCode, string[] permissions, [GeneratedEnum] Android.Content.PM.Permission[] grantResults)
        {
            PermissionsImplementation.Current.OnRequestPermissionsResult(requestCode, permissions, grantResults);
            base.OnRequestPermissionsResult(requestCode, permissions, grantResults);
        }

        public override void OnBackPressed()
        {
            base.OnBackPressed();
            //Prism.Plugin.Popups.PopupPlugin.OnBackPressed();
        }
        protected override void OnDestroy()
        {
            App.Current.Container.Resolve<IAppEventsService>().Destroy();
            base.OnDestroy();
            System.Diagnostics.Process.GetCurrentProcess().Kill();
        }

    }

    public class AndroidInitializer : IPlatformInitializer
    {
        public void RegisterTypes(IContainerRegistry containerRegistry)
        {
            // Register any platform specific implementations
            containerRegistry.RegisterSingleton<ILocalize, Localize>();
            containerRegistry.RegisterSingleton<IBleService, BleService>();
            containerRegistry.RegisterSingleton<IAudioService, AudioMediaService>();
            containerRegistry.RegisterSingleton<IPhoneStateListenerService, PhoneStateListenerService>();
            containerRegistry.RegisterSingleton<INotificationListenerService, NLService>();
            //containerRegistry.RegisterSingleton<IStepCounterService, StepCounter>();
            containerRegistry.RegisterSingleton<IAppPreferences, AppPreferences>();
            containerRegistry.RegisterSingleton<IPermissionService, PermissionService>();
            containerRegistry.RegisterSingleton<IAppEventsService, AppEventsService>();
        }
    }
}

