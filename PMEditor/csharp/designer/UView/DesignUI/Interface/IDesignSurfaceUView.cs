
namespace DesignUI
{
    public interface IDesignSurfaceUView : IDesignSurfaceBase
    {
        //- Get the IDesignerHost of the .NET 2.0 DesignSurface
        Service.ToolboxServiceImp GetIToolboxService();
        void EnableDragandDrop();
    }
}
