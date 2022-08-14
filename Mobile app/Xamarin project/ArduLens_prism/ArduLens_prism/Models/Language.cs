using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.Models
{
    public class Language
    {
        public string Name { get; }
        public string Coding { get; }
        public int Key { get; }

        public static List<Language> Languages { get; } = new()
        {
            new Language("Русский", "ru-RU", 0),
            new Language("English", "en-US", 1)
        };

        public Language(string name, string codeing, int key)
        {
            Name = name;
            Coding = codeing;
            Key = key;
        }
    }
}
