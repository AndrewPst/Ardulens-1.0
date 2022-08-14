using ArduLens_prism.Models;
using ArduLens_prism.Services;
using ArduLens_prism.ViewModels;
using ArduLens_prism.Views;
using Prism;
using Prism.Ioc;
using Prism.Plugin.Popups;
using System.Globalization;
using System.Threading;
using Xamarin.Essentials;
using Xamarin.Essentials.Implementation;
using Xamarin.Essentials.Interfaces;
using Xamarin.Forms;
using System.Linq;

namespace ArduLens_prism
{
    public partial class App
    {
        public App(IPlatformInitializer initializer)
            : base(initializer)
        {
        }

        protected override async void OnInitialized()
        {
            if (Preferences.ContainsKey("current_lang"))
            {
                var lang = Preferences.Get("current_lang", string.Empty);
                ArduLens_prism.Resources.Resource.Culture = new CultureInfo(lang);
                Current.Container.Resolve<ILocalize>().CurrentLanguage = Language.Languages.Where(x => x.Coding.Equals(lang)).FirstOrDefault();
            } else
            {
                var lang = Current.Container.Resolve<ILocalize>().GetCurrentCultureInfo();
                ArduLens_prism.Resources.Resource.Culture = lang;
                Current.Container.Resolve<ILocalize>().CurrentLanguage = Language.Languages.Where(x => x.Coding.Equals(lang.Name)).FirstOrDefault();

            }
            InitializeComponent();
            var result = await NavigationService.NavigateAsync("NavigationPage/MainTabbedPage");
            if (!result.Success)
            {
                System.Diagnostics.Debugger.Break();
            }
        }

        protected override void RegisterTypes(IContainerRegistry containerRegistry)
        {
            containerRegistry.RegisterPopupDialogService();
            containerRegistry.RegisterPopupNavigationService();

            containerRegistry.RegisterSingleton<IReceivedDataParser, E2AParser>();

            containerRegistry.RegisterSingleton<GlassesController>();
            containerRegistry.RegisterSingleton<GlassesPreferences>();
            containerRegistry.RegisterSingleton<GlassesController.PagesController>();
            containerRegistry.RegisterSingleton<IAppInfo, AppInfoImplementation>();


            containerRegistry.RegisterDialog<Dialogs.TimePageDialog, ViewModels.DialogsViewModels.TimePageDialogViewModel>("TimePage_glasses");
            containerRegistry.RegisterDialog<Dialogs.NotesPageDialog, ViewModels.DialogsViewModels.NotesPageDialogViewModel>("NotesPage_glasses");
            containerRegistry.RegisterDialog<Dialogs.TwoButtonsDialog, ViewModels.DialogsViewModels.TwoButtonsDialogViewModel>("TwoButtonsDialog");
            containerRegistry.RegisterDialog<Dialogs.LanguageChangerDialog, ViewModels.DialogsViewModels.LanguageChangerDialogViewModel>("LanguagesDialog");
            containerRegistry.RegisterDialog<Dialogs.EditNoteForNotesPageDialog, ViewModels.DialogsViewModels.EditNoteForNotesPageDialogViewModel>("EditNotesDialog");
            containerRegistry.RegisterDialog<Dialogs.WeatherPageDialog, ViewModels.DialogsViewModels.WeatherPageDialogViewModel>("WeatherPage_glasses");


            containerRegistry.RegisterForNavigation<NavigationPage>();
            containerRegistry.RegisterForNavigation<MainTabbedPage, MainTabbedPageViewModel>();
            containerRegistry.RegisterForNavigation<MainPage, MainPageViewModel>();
            containerRegistry.RegisterForNavigation<CommandPage, CommandPageViewModel>();
            containerRegistry.RegisterForNavigation<DetectedDevicesListPage, DetectedDevicesListPageViewModel>();
            containerRegistry.RegisterForNavigation<DebuggerPage, DebuggerPageViewModel>();
        }

        protected override void OnResume()
        {
            this.PopupPluginOnResume();
            base.OnResume();
        }

        protected override void OnSleep()
        {
            this.PopupPluginOnSleep();
            base.OnSleep();
        }
    }
}
