using ArduLens_prism.Models;
using System;
using System.Text;

namespace ArduLens_prism.Services
{

    public interface IReceivedDataParser
    {
        public ReceivedData Deserializate(byte[] value);
        public byte[] Serializate(ReceivedData value); 
    }
}
