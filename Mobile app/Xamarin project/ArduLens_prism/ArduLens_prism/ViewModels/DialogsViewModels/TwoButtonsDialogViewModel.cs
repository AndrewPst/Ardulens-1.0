using Prism.Commands;
using Prism.Mvvm;
using Prism.Services.Dialogs;
using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.ViewModels.DialogsViewModels
{
    public class TwoButtonsDialogViewModel : BindableBase, IDialogAware
    {
        private string _title;
        public string Title
        {
            get => _title;
            set => SetProperty(ref _title, value);
        }

        private string _message;
        public string Message
        {
            get => _message;
            set => SetProperty(ref _message, value);
        }

        public DelegateCommand<string> CloseCommand { get; }

        public event Action<IDialogParameters> RequestClose;

        public TwoButtonsDialogViewModel()
        {
            CloseCommand = new(result =>
            {
                bool res = result.Equals("yes");  
                DialogParameters param = new()
                {
                    { "result", res }
                };
                RequestClose?.Invoke(param);
            });
        }

        public bool CanCloseDialog() => true;

        public void OnDialogClosed()
        {

        }

        public void OnDialogOpened(IDialogParameters parameters)
        {
            Title = parameters["title"] as string;
            Message = parameters["message"] as string;
        }
    }
}
