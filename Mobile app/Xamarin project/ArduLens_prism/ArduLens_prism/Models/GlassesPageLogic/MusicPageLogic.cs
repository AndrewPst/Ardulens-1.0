using System;
using System.Collections.Generic;
using System.Text;
using Prism.Ioc;
using ArduLens_prism.Services;

namespace ArduLens_prism.Models.GlassesPageLogic
{
    public class MusicPageLogic : GlassesPageLogicBase
    {
        public override string Name => Resources.Resource.Page_Music_Name;
        public override string Key => "music_p";

        public override string Description => Resources.Resource.Page_Music_Description;

        private IAudioService _audio;

        public MusicPageLogic() : base()
        {
            _audio = App.Current.Container.Resolve<IAudioService>();
        }

        public override IList<string> OnShow()
        {
            return new List<string>() { _audio.IsMusicActive ? "1" : "0", _audio.VolumeInPercent.ToString() };
        }

        public override IList<string> OnAction(IList<string> list)
        {
            bool isMActive = _audio.IsMusicActive;
            switch (list[0])
            {
                case "1":
                    _audio.PauseResume();
                    isMActive = !isMActive;
                    break;
                case "2":
                    _audio.Previous();
                    isMActive = true;
                    break;
                case "3":
                    _audio.Next();
                    isMActive = true;
                    break;
                case "4":
                    _audio.UpVolume();
                    break;
                case "5":
                    _audio.DownVolume();
                    break;
            }
            return new List<string>() { isMActive ? "1" : "0", _audio.VolumeInPercent.ToString()};
         }
    }
}
