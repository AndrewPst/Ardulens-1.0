using Plugin.BLE.Abstractions.Contracts;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Reflection;
using System.Text;
using Xamarin.Forms;

namespace ArduLens.Converters
{
    class DeviceMACaddressConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            //IDevice device = (IDevice)value;
            //var obj = device.NativeDevice;
            PropertyInfo propInfo = value.GetType().GetProperty("Address");
            return (string)propInfo.GetValue(value, null);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
