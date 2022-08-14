using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Text;
using Prism.Plugin.Popups;
using Prism.Services.Dialogs;
using ArduLens_prism.Services;
using Prism.Commands;
using Xamarin.Forms;
using static ArduLens_prism.Models.GlassesController;
using ArduLens_prism.Models.GlassesPageLogic;

namespace ArduLens_prism.ViewModels.DialogsViewModels
{
    public class TimePageDialogViewModel : DialogViewModelBase
    {
        private TimePageLogic _pageLogic = null;

        private Color _boxColor;
        public Color BoxColor
        {
            get => _boxColor;
            set => SetProperty(ref _boxColor, value);
        }

        public int Red
        {
            get => _pageLogic == null ? 0 : _pageLogic.colorR;
            set
            {
                _pageLogic.colorR = value;
                RaisePropertyChanged();
                BoxColor = Color.FromRgb(value, Green, Blue);
            }
        }
        public int Green
        {
            get => _pageLogic == null ? 0 : _pageLogic.colorG;
            set
            {
                _pageLogic.colorG = value;
                RaisePropertyChanged();
                BoxColor = Color.FromRgb(Red, value, Blue);
            }
        }
        public int Blue
        {
            get => _pageLogic == null ? 0 : _pageLogic.colorB;
            set
            {
                _pageLogic.colorB = value;
                RaisePropertyChanged();
                BoxColor = Color.FromRgb(Red, Green, value);
            }
        }

       
        public DelegateCommand SendColorCommands { get; }

        public TimePageDialogViewModel(IAppPreferences _appPref, PagesController pagesController)
        {
            SendColorCommands = new(() =>
            {
                pagesController.SendCmdToPage(_pageLogic.Key,  "color", Red.ToString(), Green.ToString(), Blue.ToString());
                _appPref.Save(BoxColor, "BoxColor", "page_time");
            });   
        }

        public override void OnDialogOpened(IDialogParameters parameters)
        {
            _pageLogic = (parameters["glassesPageLogic"] as TimePageLogic);
            Red = _pageLogic.colorR;
            Green = _pageLogic.colorG;
            Blue = _pageLogic.colorB;
        }
    }
}
