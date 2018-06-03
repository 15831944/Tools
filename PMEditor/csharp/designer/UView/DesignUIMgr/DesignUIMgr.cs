using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.ComponentModel.Design;
using System.Collections;

using DesignUI;

namespace DesignUIMgr
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
    public class DesignUIMgr : DesignSurfaceManager
    {
        private const string _name = "DesignUIMgr";

        //- this List<> is necessary to be able to delete the DesignSurfaces previously created
        //- Note: 
        //-     the DesignSurfaceManager.DesignSurfaces Property is a collection of design surfaces 
        //-     that are currently hosted by the DesignSurfaceManager but is readonly
        private List<DesignSurfaceUView> DesignSurfaceUViewCollection = new List<DesignSurfaceUView>();

        public PropertyGridHost PropertyGridHost { get; private set; }

        #region ctors
        //- ctors
        // Summary:
        //     Initializes a new instance of the System.ComponentModel.Design.DesignSurfaceManager
        //     class.
        public DesignUIMgr() : base() { Init(); }

        // Summary:
        //     Initializes a new instance of the System.ComponentModel.Design.DesignSurfaceManager
        //     class.
        //
        // Parameters:
        //   parentProvider:
        //     A parent service provider. Service requests are forwarded to this provider
        //     if they cannot be resolved by the design surface manager.
        public DesignUIMgr(IServiceProvider parentProvider) : base(parentProvider) { Init(); }

        private void Init()
        {
            this.PropertyGridHost = new PropertyGridHost(this);
            //- add the PropertyGridHost and ComboBox as services
            //- to let them available for every DesignSurface
            //- (the DesignSurface need a PropertyGridHost/ComboBox not a the UserControl hosting them so
            //- we provide the PropertyGridHost/ComboBox embedded inside our UserControl PropertyGridExt)
            this.ServiceContainer.AddService(typeof(PropertyGrid), PropertyGridHost.PropertyGrid);
            this.ServiceContainer.AddService(typeof(ComboBox), PropertyGridHost.ComboBox);

            this.ActiveDesignSurfaceChanged += (object sender, ActiveDesignSurfaceChangedEventArgs e) =>
            {
                DesignSurfaceUView surface = e.NewSurface as DesignSurfaceUView;
                if (null == surface)
                    return;

                UpdatePropertyGridHost(surface);
            };
        }

        public void UpdatePropertyGridHost(DesignSurfaceUView surface)
        {
            IDesignerHost host = (IDesignerHost)surface.GetService(typeof(IDesignerHost));
            if (null == host)
                return;
            if (null == host.RootComponent)
                return;

            //- sync the PropertyGridHost
            this.PropertyGridHost.SelectedObject = host.RootComponent;
            this.PropertyGridHost.ReloadComboBox();
        }
        #endregion

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
        public DesignSurfaceUView CreateDesignSurfaceUView()
        {
            //- with a DesignSurfaceManager class, is useless to add new services 
            //- to every design surface we are about to create,
            //- because of the "IServiceProvider" parameter of CreateDesignSurface(IServiceProvider) Method.
            //- This param let every design surface created 
            //- to use the services of the DesignSurfaceManager.
            //- A new merged service provider will be created that will first ask 
            //- this provider for a service, and then delegate any failures 
            //- to the design surface manager object. 
            //- Note:
            //-     the following line of code create a brand new DesignSurface which is added 
            //-     to the Designsurfeces collection, 
            //-     i.e. the property "this.DesignSurfaces" ( the .Count in incremented by one)
            DesignSurfaceUView surface = (DesignSurfaceUView)(this.CreateDesignSurface(this.ServiceContainer));

            //- each time a brand new DesignSurface is created,
            //- subscribe our handler to its SelectionService.SelectionChanged event
            //- to sync the PropertyGridHost
            ISelectionService selectionService = (ISelectionService)(surface.GetService(typeof(ISelectionService)));
            if (null != selectionService)
            {
                selectionService.SelectionChanged += (object sender, EventArgs e) =>
                {
                    ISelectionService selectService = sender as ISelectionService;
                    if (null == selectService)
                        return;

                    if (0 == selectService.SelectionCount)
                        return;

                    //- Sync the PropertyGridHost
                    PropertyGrid propertyGrid = (PropertyGrid)this.GetService(typeof(PropertyGrid));
                    if (null == propertyGrid)
                        return;

                    ArrayList comps = new ArrayList();
                    comps.AddRange(selectService.GetSelectedComponents());
                    propertyGrid.SelectedObjects = comps.ToArray();
                };
            }
            DesignSurfaceUViewCollection.Add(surface);
            this.ActiveDesignSurface = surface;

            //- and return the DesignSurface (to let the its BeginLoad() method to be called)
            return surface;
        }

        public void DeleteDesignSurfaceUView(DesignSurfaceUView item)
        {
            DesignSurfaceUViewCollection.Remove(item);
            try
            {
                item.Dispose();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }
            int currentIndex = DesignSurfaceUViewCollection.Count - 1;
            if (currentIndex >= 0)
                ActiveDesignSurface = DesignSurfaceUViewCollection[currentIndex];
            else
                ActiveDesignSurface = null;
        }

        public void DeleteDesignSurfaceUView(int index)
        {
            DesignSurfaceUView item = DesignSurfaceUViewCollection[index];
            DesignSurfaceUViewCollection.RemoveAt(index);
            try
            {
                item.Dispose();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }
            int currentIndex = DesignSurfaceUViewCollection.Count - 1;
            if (currentIndex >= 0)
                ActiveDesignSurface = DesignSurfaceUViewCollection[currentIndex];
            else
                ActiveDesignSurface = null;
        }

        //- loop through all the collection of DesignSurface 
        //- to find out a brand new Form name
        public string GetValidName(string type)
        {
            //- we choose to use "Form_" with an underscore char as trailer 
            //- because the .NET design services provide a name of type: "FormN"
            //- with N=1,2,3,4,...
            //- therefore using a "Form", without an underscore char as trailer,
            //- cause some troubles when we have to decide if a name is used or not
            //- using a different building pattern (with the underscore) avoid this issue
            string newFormNameHeader = $"{type}_";
            int newFormNametrailer = -1;
            string newFormName = string.Empty;
            bool isNew = true;
            do
            {
                isNew = true;
                newFormNametrailer++;
                newFormName = newFormNameHeader + newFormNametrailer;
                foreach (DesignSurfaceUView item in DesignSurfaceUViewCollection)
                {
                    string currentFormName = item.GetIDesignerHost().RootComponent.Site.Name;
                    isNew &= ((newFormName == currentFormName) ? false : true);
                }

            } while (!isNew);
            return newFormName;
        }

        public new DesignSurfaceUView ActiveDesignSurface
        {
            get { return base.ActiveDesignSurface as DesignSurfaceUView; }
            set { base.ActiveDesignSurface = value; }
        }
    }
}
