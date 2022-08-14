
using Android.App;
using Android.Content;
using Android.Media;
using Android.Media.Session;
using Android.Views;
using ArduLens_prism.Models;
using System;
using Xamarin.Forms;
using ArduLens_prism.Services;
using System.Threading;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Collections.Generic;

[assembly: Dependency(typeof(ArduLens_prism.Droid.AudioMediaService))]
namespace ArduLens_prism.Droid
{
    public class AudioMediaService : IAudioService
    {

        private AudioManager _manager;

        public int Volume
        {
            get => _manager.GetStreamVolume(Stream.Music);
        }
        public int MaxVolume { get; private set; }
        public int MinVolume { get; private set; }

        public int VolumeInPercent
        {
            get
            {
                return (int)((double)Volume / (double)MaxVolume * 100d);
            }
        }


        public void PauseResume()
        {

            Keycode code = _manager.IsMusicActive ? Keycode.MediaPause : Keycode.MediaPlay;
            _manager.DispatchMediaKeyEvent(new KeyEvent(KeyEventActions.Down, code));
            _manager.DispatchMediaKeyEvent(new KeyEvent(KeyEventActions.Up, code));

        }

        //private void Pause()
        //{
        //    _manager.DispatchMediaKeyEvent(new KeyEvent(KeyEventActions.Down, Keycode.MediaPause));
        //    _manager.DispatchMediaKeyEvent(new KeyEvent(KeyEventActions.Up, Keycode.MediaPause));
        //}

        //private void Resume()
        //{
        //    _manager.DispatchMediaKeyEvent(new KeyEvent(KeyEventActions.Down, Keycode.MediaPlay));
        //    _manager.DispatchMediaKeyEvent(new KeyEvent(KeyEventActions.Up, Keycode.MediaPlay));
        //}

        public bool IsMusicActive => _manager.IsMusicActive;

        public void UpVolume()
        {
            int volume = Volume;
            volume = _manager.GetStreamVolume(Stream.Music);
            volume = Volume + 1 > MaxVolume ? Volume : Volume + 1;
            _manager.SetStreamVolume(Stream.Music, volume, 0);
        }

        public void DownVolume()
        {
            int volume = Volume;
            volume = _manager.GetStreamVolume(Stream.Music);
            volume = Volume - 1 < MinVolume ? Volume : Volume - 1;
            _manager.SetStreamVolume(Stream.Music, volume, 0);
        }

        public void Next()
        {
            _manager.DispatchMediaKeyEvent(new KeyEvent(KeyEventActions.Down, Keycode.MediaNext));
            _manager.DispatchMediaKeyEvent(new KeyEvent(KeyEventActions.Up, Keycode.MediaNext));
        }

        public void Previous()
        {
            _manager.DispatchMediaKeyEvent(new KeyEvent(KeyEventActions.Down, Keycode.MediaPrevious));
            _manager.DispatchMediaKeyEvent(new KeyEvent(KeyEventActions.Up, Keycode.MediaPrevious));
        }

        public void SetSilentMode()
        {
            _manager.RingerMode = RingerMode.Silent;
        }


        public AudioMediaService()
        {
            _manager = (AudioManager)Android.App.Application.Context.GetSystemService(Context.AudioService);
            MaxVolume = _manager.GetStreamMaxVolume(Stream.Music);
            MinVolume = _manager.GetStreamMinVolume(Stream.Music);
        }
    }
}
