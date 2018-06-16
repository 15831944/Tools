using System;
using System.ComponentModel.Design;

namespace DesignUI
{
    //- this class manages a collection of DesignSurfaceUView instances
    //- this class adds to
    //-     DesignSurfaceUView instances
    //- the following facilities:
    //-     * PropertyGridHost 
    //-
    //- DesignSurfaceUView
    //-     |
    //-     +--|PropertyGridHost|
    //-     |
    //-     +--|Toolbox|
    //-     |
    //-     +--|ContextMenu|
    //-     |
    //-     +--|DesignSurfaceBase|
    //-             |
    //-             +--|DesignSurface|
    //-             |
    //-             +--|TabOrder|
    //-             |
    //-             +--|UndoEngine|
    //-             |
    //-             +--|Cut/Copy/Paste/Delete commands|
    //-
    //- [Note FROM MSDN]:
    //- The ActiveDesignSurface property should be set 
    //- by the designer's Type user interface 
    //- whenever a designer becomes the active window!
    //- That is to say:
    //-   the DesignUIMgr is an OBSERVER of UI event: SelectedTab/SelectedIndex Changed
    //- usage:
    //       //- SelectedIndexChanged event fires when the TabControls SelectedIndex or SelectedTab value changes. 
    //       //- give the focus to the DesigneSurface accordingly to te selected TabPage and sync the propertyGrid
    //       this.tabControl1.SelectedIndexChanged += ( object sender, EventArgs e ) => {
    //                TabControl tabCtrl = sender as TabControl;
    //                mgr.ActiveDesignSurface = (DesignSurfaceUView) mgr.DesignSurfaces[tabCtrl.SelectedIndex];
    //       };
    //-
    public partial class DesignUIMgr : DesignSurfaceManager
    {
        public View.PropertyGridHost PropertyGridHost { get; private set; }

        #region ctors
        // Initializes a new instance of the System.ComponentModel.Design.DesignSurfaceManager class.
        public DesignUIMgr() : base() { Init(); }

        // Initializes a new instance of the System.ComponentModel.Design.DesignSurfaceManager class.
        // Parameters - parentProvider: A parent service provider. Service requests are forwarded to this provider 
        //                              if they cannot be resolved by the design surface manager.
        public DesignUIMgr(IServiceProvider parentProvider) : base(parentProvider) { Init(); }
        #endregion

        public void UpdatePropertyGridHost(DesignSurfaceUView surface)
        {
            IDesignerHost host = (IDesignerHost)surface.GetService(typeof(IDesignerHost));
            if (null == host || null == host.RootComponent)
                return;

            //- sync the PropertyGridHost
            this.PropertyGridHost.SelectedObject = host.RootComponent;
            this.PropertyGridHost.ReloadComboBox();
        }

        //- The CreateDesignSurfaceCore method is called by both CreateDesignSurface methods
        //- It is the implementation that actually creates the design surface
        //- The default implementation just returns a new DesignSurface, we override 
        //- this method to provide a custom object that derives from the DesignSurface class
        //- i.e. a new DesignSurfaceUView instance
        protected override DesignSurface CreateDesignSurfaceCore(IServiceProvider parentProvider)
        {
            return new DesignSurfaceUView(parentProvider);
        }

        //- Gets a new DesignSurfaceUView
        //- and loads it with the appropriate type of root component. 
        public DesignSurface CreateDesignSurfaceEx()
        {
            return CreateDesignSurfaceUView();
        }

        public void DeleteDesignSurfaceUView(DesignSurfaceUView item)
        {
            RemoveDesignSurface(item);
        }

        public void DeleteDesignSurfaceUView(int index)
        {
            DesignSurfaceUView item = DesignSurfaceUViewCollection[index];
            if (item != null)
                RemoveDesignSurface(item);
        }

        //- loop through all the collection of DesignSurface 
        //- to find out a brand new Form name
        public string GetValidName(string type)
        {
            return GetValidNameByType(type);
        }

        public new DesignSurfaceUView ActiveDesignSurface
        {
            get { return base.ActiveDesignSurface as DesignSurfaceUView; }
            set { base.ActiveDesignSurface = value; }
        }
    }
}
