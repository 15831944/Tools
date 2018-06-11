using System;
using System.ComponentModel.Design;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing;
using System.ComponentModel.Design.Serialization;

namespace DesignUI.Interface
{
    public enum AlignmentModeEnum : int { SnapLines = 0, Grid }//, GridWithoutSnapping, NoGuides };

    public interface IDesignSurfaceBase
    {
        //- de/activate the TabOrder facility
        void SwitchTabOrder();

        //- select the controls alignement mode
        void UseOptionService(AlignmentModeEnum option);

        //- perform Cut/Copy/Paste/Delete... commands
        void DoAction(string command);

        void DoAction(CommandID command);

        //- method usefull to create control without the ToolBox facility
        IComponent CreateRootComponent(Type type, string name, Size size);

        IComponent CreateRootComponent(DesignerLoader loader, Size size);

        IComponent CreateControl(Type controlType, Size controlSize, Point controlLocation);

        //- the HostControl of the .NET 2.0 DesignSurface is just a Control
        //- you can manipulate this Control just like any other WinForms Control
        //- (you can dock it and add it to another Control just to display it)
        //- Get the HostControl
        Control GetView();

        //void Save();
    }
}
