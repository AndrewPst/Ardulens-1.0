using Prism.Commands;
using Prism.Mvvm;
using Prism.Services.Dialogs;
using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.ViewModels.DialogsViewModels
{
    public class EditNoteForNotesPageDialogViewModel : BindableBase, IDialogAware
    {
        public event Action<IDialogParameters> RequestClose;

        private string _title;
        public string Title { 
            get => _title;
            set 
            {
                if (value.Length > 9)
                    _title = value.Substring(0, 7) + "...";
                else
                    _title = value;
                IsTitleMax = value.Length >= 16;
                RaisePropertyChanged();
            } 
        }
        
        
        private string _text;
        public string Text
        {
            get => _text;
            set
            {
                if (value.Length > 27)
                    _text = value.Substring(0, 25) + "...";
                else
                    _text = value;
                IsTextMax = value.Length >= 32;
                RaisePropertyChanged();
            }
        }


        private bool _isTitleMax;
        private bool _isTextMax;

        public bool IsTitleMax
        {
            get => _isTitleMax;
            set => SetProperty(ref _isTitleMax, value);
        }

        public bool IsTextMax
        {
            get => _isTextMax;
            set => SetProperty(ref _isTextMax, value);
        }


        public DelegateCommand CloseCommand { get; }
        public bool CanCloseDialog() => true;

        public EditNoteForNotesPageDialogViewModel()
        {
            CloseCommand = new(() =>
            {
                DialogParameters param = new()
                {
                    { "title", Title },
                    { "text", Text }
                };
                RequestClose?.Invoke(param);
            });
        }

        public void OnDialogClosed()
        {
        }

        public void OnDialogOpened(IDialogParameters parameters)
        {
            Title = parameters["title"] as string;
            Text = parameters["text"] as string;
        }
    }
}
