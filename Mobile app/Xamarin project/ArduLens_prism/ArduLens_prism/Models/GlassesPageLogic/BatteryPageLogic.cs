using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.Models.GlassesPageLogic
{
    public class BatteryPageLogic : GlassesPageLogicBase
    {
        public override string Name => "Battery page";

        public override string Key => "batt_p";

        public BatteryPageLogic()
        {
            Xamarin.Essentials.Battery.BatteryInfoChanged += Battery_BatteryInfoChanged;
        }

        private void Battery_BatteryInfoChanged(object sender, Xamarin.Essentials.BatteryInfoChangedEventArgs e)
        {
            if(e.ChargeLevel < 0.15)
            {
                GlassesController.Current.CreateNotification("Phone", (e.ChargeLevel*100d).ToString() + "%", "Сharge your phone");
            }
        }

        public override IList<string> OnShow()
        {
            int level = (int)(Xamarin.Essentials.Battery.ChargeLevel * 100d);
            return new List<string>() { level.ToString() };
        }
    }
}
