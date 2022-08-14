using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace ArduLens_prism.Models.GlassesPageLogic
{
    public abstract class GlassesPageLogicBase : BindableBase
    {
        public static List<GlassesPageLogicBase> Pages = new()
        {
            new TimePageLogic(),
            new MusicPageLogic(),
            new NotifyPageLogic(),
            new PhonePageLogic(),
            new NotesPageLogic(),
            new WeatherPageLogic(),
            new BatteryPageLogic(),
            new StepsPageLogic(),
            new StopwatchPageLogic()
        };

        private bool ishomepage = false;
        public bool IsHomePage
        {
            get => ishomepage;
            set => SetProperty(ref ishomepage, value);
        }

        public abstract string Name { get; }
        public abstract string Key { get; }

        public virtual string Description
        {
            get => string.Empty;
        }

        public virtual string ViewModelKey
        {
            get => string.Empty;
        }
        public virtual string Icon { get => "IconGlass.png"; }

        public abstract IList<string> OnShow();

        public virtual IList<string> OnAction(IList<string> list)
        {
            return null;
        }

        public virtual void OnInfoReceived(ReceivedData data)
        {

        }

        public virtual void OnHide()
        {

        }

        public GlassesPageLogicBase()
        {
        }
    }
}
