using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace ArduLens_prism.Services
{
    public interface IPhoneStateListenerService
    {

        public delegate void OnPhoneStateChangedHundler(object sender, PhoneStateEventErgs e);
        public event OnPhoneStateChangedHundler OnPhoneStateChanged;

        public Task<string> GetAbonentNameWithNumberAsync(string number);

        public void EndCall();
    }

    public enum CallState
    {
        Idle = 0,
        Offhook = 2,
        Ringing = 1
    }

    public class PhoneStateEventErgs
    {
        public string IncomingNumber { get; }
        public CallState CallState { get; }
        public PhoneStateEventErgs(CallState state, string incoming)
        {
            IncomingNumber = incoming;
            CallState = state;
        }
    }
}
