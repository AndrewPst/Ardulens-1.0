using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using ArduLens_prism.Models;

namespace ArduLens_prism.Services
{
    public class E2AParser : IReceivedDataParser
    {
        private const char Separator = '|';

        private const string CommandStr = "#";

        public ReceivedData Deserializate(byte[] value)
        {
            string str = Encoding.UTF8.GetString(value);

            if (str.StartsWith(CommandStr + Separator) && str.EndsWith(Separator + CommandStr))
            {
                str = str[(CommandStr.Length + 1)..^(CommandStr.Length + 1)];

                var strList = str.Split(Separator, StringSplitOptions.RemoveEmptyEntries).ToList();
                string key = strList[0];
                strList.RemoveRange(0, 1);

                ReceivedData result = new ReceivedData(key, strList);
                return result;
            }
            return new ReceivedData();
        }

        public byte[] Serializate(ReceivedData value)
        {
            StringBuilder str = new();
            str.Append(CommandStr);
            str.Append(Separator);
            str.Append(value.Key);
            for (int i = 0; i < value.Parameters.Count(); i++)
            {
                str.Append('|');
                str.Append(value.Parameters[i]);
            }
            str.Append(Separator);
            str.Append(CommandStr);
            return Encoding.UTF8.GetBytes(str.ToString());
        }
    }
}
