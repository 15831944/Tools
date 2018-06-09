using System;
using System.ComponentModel.Design;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing;
using System.ComponentModel.Design.Serialization;

namespace DesignUI
{
    public interface IDesignSurfaceBase
    {
        //- perform Cut/Copy/Paste/Delete... commands
        void DoAction(string command);
        void DoAction(CommandID command);

        //- de/activate the TabOrder facility
        void SwitchTabOrder();

        //- select the controls alignement mode
        void UseSnapLines();

        void UseGrid(System.Drawing.Size gridSize);

        void UseGridWithoutSnapping(System.Drawing.Size gridSize);

        void UseNoGuides();

        //- method usefull to create control without the ToolBox facility
        IComponent CreateRootComponent(Type controlType, Size controlSize);

        IComponent CreateRootComponent(DesignerLoader loader, Size controlSize);

        IComponent CreateControl(Type controlType, Size controlSize, Point controlLocation);

        //- Get the UndoEngineExtended object
        Service.UndoServiceImpl GetUndoService();

        //- Get the IDesignerHost of the .NET 2.0 DesignSurface
        IDesignerHost GetIDesignerHost();

        //- the HostControl of the .NET 2.0 DesignSurface is just a Control
        //- you can manipulate this Control just like any other WinForms Control
        //- (you can dock it and add it to another Control just to display it)
        //- Get the HostControl
        Control GetView();

        void Save();
    }
}
