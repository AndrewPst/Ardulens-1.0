using ArduLens_prism.Services;
using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;
using Prism.Ioc;

namespace ArduLens_prism.Models.GlassesPageLogic
{
    public class TimePageLogic : GlassesPageLogicBase
    {
        public override string Name => Resources.Resource.Page_Time_Name;
        public override string Key => "time_p";

        public int colorR { get; set; }
        public int colorG { get; set; }
        public int colorB { get; set; }

        public override string Description =>
             Resources.Resource.Page_Time_Description;
        public override string Icon => "TimeIcon.png";
        public override string ViewModelKey => "TimePage_glasses";

        public TimePageLogic()
        {
            IAppPreferences pref = App.Current.Container.Resolve<IAppPreferences>();
            Color color = pref.Get("BoxColor", "page_time", Color.SkyBlue);
            colorR = (int)map(color.R, 0, 1, 0, 255);
            colorG = (int)map(color.G, 0, 1, 0, 255);
            colorB = (int)map(color.B, 0, 1, 0, 255);
        }


        public override IList<string> OnShow()
        {
            var result = new List<string>()
            {
                "color",
                colorR.ToString(),
                colorG.ToString(),
                colorB.ToString(),
            };

            return result;
        }

        private double map(double x, double in_min, double in_max, double out_min, double out_max)
        {
            double dividend = out_max - out_min;
            double divisor = in_max - in_min;
            double delta = x - in_min;
            if (divisor == 0)
            {
                return -1;
            }
            return (delta * dividend + (divisor / 2)) / divisor + out_min;
        }

    }
}
