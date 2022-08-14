using ArduLens_prism.Models;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace ArduLens_prism.Services
{
    public interface ILocalize
    {
        CultureInfo GetCurrentCultureInfo();
        public Language CurrentLanguage { get; set; }
    }
}
