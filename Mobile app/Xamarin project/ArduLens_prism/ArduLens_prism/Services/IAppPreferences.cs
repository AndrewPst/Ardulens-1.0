using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.Services
{
    public interface IAppPreferences
    {
        void Save<T>(T value, string key, string directory);
        T Get<T>(string key, string directory, T defaultValue);
        void Remove<T>(string key, string directory);
        bool ConstrainKey(string key, string directory);
    }
}
