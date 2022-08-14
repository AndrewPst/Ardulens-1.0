using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;
using Xamarin.Forms;

namespace ArduLens_prism.Converters
{
    public class StateToButtonTextConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ((bool)value) ? Resources.Resource.Disconnect : Resources.Resource.Connect;
            //if ((bool)value)
            //{
            //    return Resources.Resource.Disconnect;
            //}
            //else
            //{
            //    return Resources.Resource.Connect;
            //}
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
