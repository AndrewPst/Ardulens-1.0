using ArduLens_prism.Services;
using System;
using System.Collections.Generic;
using System.Text;
using Prism.Ioc;
using Newtonsoft.Json;
using System.Threading.Tasks;

namespace ArduLens_prism.Models.GlassesPageLogic
{

    public class Note
    {
        public string title;
        public string text;
    }

    public class NotesPageLogic : GlassesPageLogicBase
    {
        public override string Name => "Notes page";

        public override string Key => "notes_p";

        public override string ViewModelKey => "NotesPage_glasses";

        public List<Note> Notes { get; private set; }

        private IAppPreferences _preferences;
        public NotesPageLogic()
        {
            _preferences = App.Current.Container.Resolve<IAppPreferences>();
            string str = _preferences.Get<string>("notes_json", "notesPage", null);
            IList<Note> list;
            try
            {
                list = JsonConvert.DeserializeObject<IList<Note>>(str);
                Notes = new(list);
            }
            catch
            {
                Notes = new();
            }
        }

        public override IList<string> OnShow()
        {
            return null;
        }

        public override IList<string> OnAction(IList<string> list)
        {
            if (list[0] == "1")
            {
                sendAllNotes();
            }
            return null;
        }

        private async void sendAllNotes()
        {
            for (int i = 0; i < Notes.Count; i++)
            {
                Note note = Notes[i];
                GlassesController.Current.MyPagesController.SendCmdToPage(Key, "1", i.ToString(), note.title, note.text);
                await Task.Delay(100);
            }
            GlassesController.Current.MyPagesController.SendCmdToPage(Key, "3", Notes.Count.ToString());
        }

        public void Add(Note note)
        {
            Notes.Add(note);
            GlassesController.Current.MyPagesController.SendCmdToPage(Key, "0", note.title, note.text);
        }

        public void Delete(Note note)
        {
            GlassesController.Current.MyPagesController.SendCmdToPage(Key, "2", Notes.IndexOf(note).ToString());
            Notes.Remove(note);
        }

        public void Edit(Note note, string title, string text)
        {
            note.text = text;
            note.title = title;
            GlassesController.Current.MyPagesController.SendCmdToPage(Key, "1", Notes.IndexOf(note).ToString(), note.title, note.text);
        }
    }
}
