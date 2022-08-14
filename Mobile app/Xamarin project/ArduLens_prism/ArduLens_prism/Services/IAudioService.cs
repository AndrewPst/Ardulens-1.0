using ArduLens_prism.Models;
using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.Services
{
    public interface IAudioService
    {
        public int Volume { get; }
        public int VolumeInPercent { get; }
        public int MaxVolume { get; }
        public int MinVolume { get; }

        public void PauseResume();
        public void UpVolume();
        public void DownVolume();
        public void Next();
        public void Previous();

        public void SetSilentMode();
        public bool IsMusicActive { get; }

    }
}
