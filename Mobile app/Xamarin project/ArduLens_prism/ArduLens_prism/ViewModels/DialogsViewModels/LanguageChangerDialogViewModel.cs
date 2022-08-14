using ArduLens_prism.Models;
using ArduLens_prism.Services;
using Prism.Commands;
using Prism.Mvvm;
using Prism.Services.Dialogs;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace ArduLens_prism.ViewModels.DialogsViewModels
{
    public class LanguageChangerDialogViewModel : BindableBase, IDialogAware
    {
        private ILocalize _localize;

        public event Action<IDialogParameters> RequestClose;


        public List<Language> Languages => Language.Languages;

        public Language SelectedLanguage
        {
            get => null;
            set
            {
                if(value != null)
                {
                    DialogParameters param = new()
                    {
                        { "Language", value }
                    };
                    RequestClose?.Invoke(param);
                }
            }
        }

        public LanguageChangerDialogViewModel(ILocalize localize)
        {
            _localize = localize;

        }

        public bool CanCloseDialog() => true;

        public void OnDialogClosed()
        {

        }

        public void OnDialogOpened(IDialogParameters parameters)
        {

        }
    }
}
