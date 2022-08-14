using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Text;
using ArduLens_prism.Models.GlassesPageLogic;
using ArduLens_prism.Services;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Prism.Commands;
using Prism.Mvvm;
using Prism.Services.Dialogs;
using Xamarin.Forms;
using System.Linq;

namespace ArduLens_prism.ViewModels.DialogsViewModels
{
    public class NotesPageDialogViewModel : DialogViewModelBase
    {
        public class NoteViewModel : BindableBase
        {
            public Note Note { get; }
            public string Title
            {
                get => Note.title;
                set
                {
                    Note.title = value;
                    RaisePropertyChanged();
                }
            }

            public string Text
            {
                get => Note.text;
                set
                {
                    Note.text = value;
                    RaisePropertyChanged();                
                }
            }

            bool _isSelected;
            public bool IsSelected
            {
                get => _isSelected;
                set => SetProperty(ref _isSelected, value);
            }
            public NoteViewModel(Note note)
            {
                Note = note;
                IsSelected = false;
            }
        }

        private NotesPageLogic _page;

        private ObservableCollection<NoteViewModel> _notes;
        public ObservableCollection<NoteViewModel> Notes { get => _notes; private set => SetProperty(ref _notes, value); }

        private NoteViewModel _selectedNote = null;
        public NoteViewModel SelectedNote
        {
            get => _selectedNote;
            set
            {
                if (_selectedNote != value || _selectedNote == null)
                {
                    if (_selectedNote != null)
                        _selectedNote.IsSelected = false;
                    _selectedNote = value;
                    _selectedNote.IsSelected = true;
                }
            }
        }

        public DelegateCommand DeleteNoteCommand { get; }
        public DelegateCommand EditNoteCommand { get; }
        public DelegateCommand AddNoteCommand { get; }

        public NotesPageDialogViewModel(IDialogService dialogService)
        {
            DeleteNoteCommand = new(() =>
            {
                Notes.Remove(SelectedNote);
                _page.Delete(SelectedNote.Note);
            });

            EditNoteCommand = new(() =>
            {
                Device.BeginInvokeOnMainThread(async () =>
                {
                    var result = await dialogService.ShowDialogAsync("EditNotesDialog", new DialogParameters()
                    {
                       {"title", SelectedNote.Title },
                       {"text", SelectedNote.Text }
                    });
                    _page.Edit(SelectedNote.Note, result.Parameters["title"] as string, result.Parameters["text"] as string);
                    SelectedNote.Title = SelectedNote.Title;
                    SelectedNote.Text = SelectedNote.Text;
                });
            });

            AddNoteCommand = new(() =>
            {
                Device.BeginInvokeOnMainThread(async () =>
                {
                    var result = await dialogService.ShowDialogAsync("EditNotesDialog", new DialogParameters()
                    {
                       {"title", string.Empty },
                       {"text", string.Empty }
                    });
                    NoteViewModel note = new NoteViewModel(new Note());
                    note.Title = result.Parameters["title"] as string;
                    note.Text = result.Parameters["text"] as string;
                    Notes.Add(note);
                    _page.Add(note.Note);
                });
            });
        }

        public override void Close()
        {
            base.Close();
        }

        public override void OnDialogClosed()
        {
            base.OnDialogClosed();
        }

        public override void OnDialogOpened(IDialogParameters parameters)
        {
            //base.OnDialogOpened(parameters);
            _page = parameters["glassesPageLogic"] as NotesPageLogic;
            Notes = new(((IList<Note>)_page.Notes).Select(n => new NoteViewModel(n)));
        }
    }
}
