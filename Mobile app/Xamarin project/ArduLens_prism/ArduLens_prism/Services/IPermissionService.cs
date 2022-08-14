using System.Threading.Tasks;
using Plugin.Permissions.Abstractions;

namespace ArduLens_prism.Services
{
    public interface IPermissionService
    {
        Task<bool> CheckAllPermissonsGranted(params Permission[] permissions);
        Task<bool> RequestPermissions(params Permission[] permissions);
    }
#pragma warning restore CS0618 // Тип или член устарел
}
