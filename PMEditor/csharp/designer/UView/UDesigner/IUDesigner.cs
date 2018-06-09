using System.ComponentModel.Design;
using System.Drawing;
using System.Windows.Forms;

using DesignUI;
using DesignUIMgr;

namespace Designer
{
    public enum AlignmentModeEnum : int { SnapLines = 0, Grid }//, GridWithoutSnapping, NoGuides };

    //- Interface used to
    //-     * hosts: Toolbox;DesignSurfaces;PropertyGrid
    //-     * add/remove DesignSurfaces
    //-     * perform editing actions on active DesignSurface
    //-
    public interface IUDesigner {

        //- controls accessing section  -----------------------------------------------------------
        //-     +-------------+-----------------------------+-----------+
        //-     |toolboxItem1 | ____ ____ ____              |           |
        //-     |toolboxItem2 ||____|____|____|___________  +-----------+
        //-     |toolboxItem3 ||                          | |     |     |
        //-     |             ||                          | |     |     |
        //-     |  TOOLBOX    ||      DESIGNSURFACES      | | PROPERTY  |
        //-     |             ||                          | |   GRID    |
        //-     |             ||__________________________| |     |     |
        //-     +-------------+-----------------------------+-----------+
        ListBox Toolbox { get; set; }                       //- TOOLBOX
        TabControl TabControlHostingDesignSurfaces { get; } //- DESIGNSURFACES HOST
        PropertyGridHost PropertyGridHost { get; }          //- PROPERTYGRID

        //- DesignSurfaces management section -----------------------------------------------------
        DesignSurfaceUView ActiveDesignSurface { get; }
        //- Create the DesignSurface and the rootComponent (a .NET Control)
        //- using IDesignSurfaceBase.CreateRootComponent() 
        //- if the alignmentMode doesn't use the GRID, then the gridSize param is ignored
        //- Note:
        //-     the generics param is used to know which type of control to use as RootComponent
        //-     TT is requested to be derived from .NET Control class 
        DesignSurfaceUView AddDesignSurface<TT>(
                                               int startingFormWidth, int startingFormHeight,
                                               AlignmentModeEnum alignmentMode, Size gridSize
                                              ) where TT : Control;
        void RemoveDesignSurface ( DesignSurfaceUView activeSurface );

        //- Editing section  ----------------------------------------------------------------------
        void ActionCommandOnDesignSurface(CommandID command);
        void SwitchTabOrder();
        void SetGrid(Size size);
        void SaveCurrent();
        void SaveAll();
        void Open(string strFile);
    }
}
