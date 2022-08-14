using Android.App;
using Android.Content;
using Android.Telephony;
using System.IO;
using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using System.Diagnostics;
using System.Threading.Tasks;
using Android.Telecom;
using Android.Views;
using Java.Lang;
using Android.Provider;

using Xamarin.Forms;
using ArduLens_prism.Services;
using Android.App.Roles;
using CallState = ArduLens_prism.Services.CallState;

[assembly: Dependency(typeof(ArduLens_prism.Droid.PhoneStateListenerService))]
namespace ArduLens_prism.Droid
{
    [BroadcastReceiver]
    [IntentFilter(new[] { "android.intent.action.PHONE_STATE" })]
    public class PhoneStateListenerService : PhoneStateListener, IPhoneStateListenerService
    {
        public event IPhoneStateListenerService.OnPhoneStateChangedHundler OnPhoneStateChanged;

        private TelephonyManager _telephony;
        private Java.Lang.Object _telephonyService;
        private Java.Lang.Reflect.Method _endCallMethod;

        public PhoneStateListenerService()
        {
            _telephony = (TelephonyManager)Android.App.Application.Context.GetSystemService(Context.TelephonyService);
            _telephony.Listen(this, PhoneStateListenerFlags.CallState);


            try
            {
                Class c = Class.ForName(_telephony.Class.Name);
                _endCallMethod = c.GetDeclaredMethod("getITelephony");
                _endCallMethod.Accessible = true;
                _telephonyService = _endCallMethod.Invoke(_telephony);
                c = Class.ForName(_telephonyService.Class.Name);
                _endCallMethod = c.GetDeclaredMethod("endCall");
                _endCallMethod.Accessible = true;
            }
            catch (System.Exception e)
            {
                Debug.WriteLine(e.Message);
            }

        }

        public override void OnCallStateChanged(Android.Telephony.CallState state, string number)
        {
            base.OnCallStateChanged(state, number);
            if (!string.IsNullOrEmpty(number))
            {
                OnPhoneStateChanged?.Invoke(this, new ArduLens_prism.Services.PhoneStateEventErgs((CallState)state, number));
            }

        }

        public async Task<string> GetAbonentNameWithNumberAsync(string number)
        {
            try
            {
                var list = await Plugin.ContactService.CrossContactService.Current.GetContactListAsync();
                var result = list.Where(x => x.Number == number).FirstOrDefault().Name;
                return result;
            }
            catch (System.Exception e)
            {
                Debug.WriteLine(e.Message);
                return string.Empty;
            }
        }

        public void EndCall()
        {
            try
            {
                _endCallMethod.Invoke(_telephonyService);
            }
            catch (System.Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }
    }
}
