using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;

namespace ArduLens_prism.ViewModels
{
    public struct Message
    {
        public string Text { get; }
        public string Time { get; }
        public string IsUsers { get; }
        public Message(string text, bool isUsers, string time)
        {
            Text = text;
            IsUsers = isUsers ? "From app" : "From glass";
            Time = time;
        }
    }
    public class DebuggerPageViewModel : BindableBase
    { 

        public ObservableCollection<Message> Messages { get; }

        public DelegateCommand SendCommand { get; }
        public DelegateCommand<string> PastText { get; }
        public DelegateCommand SendLastCommand { get; }

        private string _userText;
        public string UserText
        {
            get => _userText;
            set
            {
                SetProperty(ref _userText, value);
            }
        }

        private string _lastCommand;

        public DebuggerPageViewModel()
        {
            Messages = new()
            {
                new Message("test 1", true, DateTime.Now.ToLongTimeString()),
                new Message("test 2 wqewefwe", false, DateTime.Now.ToLongTimeString())
            };
            SendCommand = new(() =>
            {
                _lastCommand = _userText;
                Messages.Add(new Message(_userText, true, DateTime.Now.ToLongTimeString()));
                UserText = string.Empty;
            });
            PastText = new(text =>
            {
                UserText += text;
            });
            SendLastCommand = new(() =>
            {
                UserText = _lastCommand;
            });
            
        }
    }
}
