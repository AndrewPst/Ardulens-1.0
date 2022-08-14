using System.Collections.Generic;

namespace ArduLens_prism.Models
{
    public struct ReceivedData 
    {
        public string Key { get; }
        public IList<string> Parameters { get; }

        public ReceivedData(string key, params string[] parameters)
        {
            Key = key;
            Parameters = parameters; 
        }

        public ReceivedData(string key, IList<string> parameters)
        {
            Key = key;
            Parameters = parameters;
        }

        public bool IsEmpty()
        {
            return string.IsNullOrEmpty(Key) || Parameters?.Count == 0;
        }
    }
}
