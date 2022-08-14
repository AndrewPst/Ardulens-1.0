using System;
using System.Collections.Generic;
using System.Text;
using ArduLens_prism.Services;
using Prism.Ioc;
using Xamarin.Forms;

namespace ArduLens_prism.Models.GlassesPageLogic
{
    public class PhonePageLogic : GlassesPageLogicBase
    {
        public override string Name => "Phone page";

        public override string Key => "phone_p";

        private IPhoneStateListenerService _phoneListener;
        private IAudioService _audioService;

        private string _parameter;
        private bool _isCalling = false;

        public PhonePageLogic() : base()
        {
            _audioService = App.Current.Container.Resolve<IAudioService>();
            _phoneListener = App.Current.Container.Resolve<IPhoneStateListenerService>();
            _phoneListener.OnPhoneStateChanged += OnCallReceived;
        }

        private async void OnCallReceived(object sender, PhoneStateEventErgs e)
        {
            if (e.CallState == CallState.Ringing)
            {
                string name = await _phoneListener.GetAbonentNameWithNumberAsync(e.IncomingNumber);
                _parameter = string.IsNullOrEmpty(name) ? e.IncomingNumber : name;
                _isCalling = true;
                GlassesController.Current.MyPagesController.OpenNewPage(this);
            }
            else
            {
                _isCalling = false;
            }
        }

        public override IList<string> OnAction(IList<string> list)
        {
/*            if(Android.OS.Build.VERSION.SdkInt >= Android.OS.BuildVersionCodes.Q)
            {
                _audioService.SetSilentMode();
            }*/
            switch (list[0])
            {
                case "0":
                    _audioService.SetSilentMode();
                    break;
                case "1":
                    _phoneListener.EndCall();
                    break;
            }
            _isCalling = false;
            GlassesController.Current.MyPagesController.OpenNewPage(GlassesController.Current.MyPagesController.HomePage);
            return null;
        }

        public override IList<string> OnShow()
        {
            List<string> result = new();
            if (_isCalling)
            {
                result.Add("1");
                result.Add(_parameter);
            }
            else
            {
                result.Add("0");
            }
            return result;
        }
    }
}
