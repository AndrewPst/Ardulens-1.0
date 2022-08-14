using ArduLens_prism.Models.GlassesPageLogic;
using Prism.Commands;
using Prism.Mvvm;
using Prism.Services.Dialogs;
using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.ViewModels.DialogsViewModels
{
    public class DialogViewModelBase : BindableBase, IDialogAware
    {
        public event Action<IDialogParameters> RequestClose;

        protected GlassesPageLogicBase _glassesPageLogic;

        public DelegateCommand CloseCommand { get; }
        public DialogViewModelBase()
        {
            CloseCommand = new(Close);
        }
        public virtual bool CanCloseDialog() => true;

        public virtual void Close()
        {
            RequestClose(null);
        }

        public virtual void OnDialogClosed()
        {

        }

        public virtual void OnDialogOpened(IDialogParameters parameters)
        {
            _glassesPageLogic = (GlassesPageLogicBase)parameters["glassesPageLogic"];
        }
    }
}
