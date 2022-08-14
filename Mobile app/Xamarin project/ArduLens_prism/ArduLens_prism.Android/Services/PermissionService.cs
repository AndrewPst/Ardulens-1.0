using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Threading.Tasks;
using Plugin.Permissions;
using Plugin.Permissions.Abstractions;

using Xamarin.Forms;
using ArduLens_prism.Services;

[assembly: Dependency(typeof(ArduLens_prism.Droid.PermissionService))]
namespace ArduLens_prism.Droid
{

#pragma warning disable CS0618 // Тип или член устарел
    public class PermissionService : IPermissionService
    {
        public async Task<bool> CheckAllPermissonsGranted(params Permission[] permissions)
        {

            bool isAllGranted = true;

            foreach (var perm in permissions)
            {
                try
                {
                    var status = await CrossPermissions.Current.CheckPermissionStatusAsync(perm);
                    if (status != PermissionStatus.Granted && status != PermissionStatus.Unknown)
                        isAllGranted = false;
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.Message);
                    isAllGranted = false;
                }

            }
            return isAllGranted;
        }

        public async Task<bool> RequestPermissions(params Permission[] permissions)
        {

            try
            {
                var list = await CrossPermissions.Current.RequestPermissionsAsync(permissions);
                bool isAllChecking = true;
                foreach (var a in permissions)
                {
                    if (list[a] != PermissionStatus.Granted && list[a] != PermissionStatus.Unknown)
                        isAllChecking = false;
                }
                return isAllChecking;
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                return false;
            }
        }
    }
#pragma warning restore CS0618 // Тип или член устарел
}
