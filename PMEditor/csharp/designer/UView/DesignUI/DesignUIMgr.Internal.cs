using System;
using System.Windows.Forms;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel.Design;

namespace DesignUI
{
    public partial class DesignUIMgr
    {
        private const string _name = "DesignUIMgr";

        //- this List<> is necessary to be able to delete the DesignSurfaces previously created
        //- Note: 
        //-     the DesignSurfaceManager.DesignSurfaces Property is a collection of design surfaces 
        //-     that are currently hosted by the DesignSurfaceManager but is readonly
        private List<DesignSurfaceUView> DesignSurfaceUViewCollection = new List<DesignSurfaceUView>();

        private void Init()
        {
            this.PropertyGridHost = new View.PropertyGridHost(this);
            //- add the PropertyGridHost and ComboBox as services
            //- to let them available for every DesignSurface
            //- (the DesignSurface need a PropertyGridHost/ComboBox not a the UserControl hosting them so
            //- we provide the PropertyGridHost/ComboBox embedded inside our UserControl PropertyGridExt)
            this.ServiceContainer.AddService(typeof(PropertyGrid), PropertyGridHost.PropertyGrid);
            this.ServiceContainer.AddService(typeof(ComboBox), PropertyGridHost.ComboBox);

            this.ActiveDesignSurfaceChanged += (object sender, ActiveDesignSurfaceChangedEventArgs e) =>
            {
                DesignSurfaceUView surface = e.NewSurface as DesignSurfaceUView;
                if (null == surface) return;

                UpdatePropertyGridHost(surface);
            };
        }

        private DesignSurface CreateDesignSurfaceUView()
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
                    if (comps.Count == 1)
                        propertyGrid.SelectedObject = comps[0];
                    else
                        propertyGrid.SelectedObjects = comps.ToArray();
                };
            }
            DesignSurfaceUViewCollection.Add(surface);
            this.ActiveDesignSurface = surface;

            //- and return the DesignSurface (to let the its BeginLoad() method to be called)
            return surface;
        }

        private void RemoveDesignSurface(DesignSurfaceUView item)
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

        private string GetValidNameByType(string type)
        {
            //- we choose to use "Form_" with an underscore char as trailer 
            //- because the .NET design services provide a name of type: "FormN"
            //- with N=1,2,3,4,...
            //- therefore using a "Form", without an underscore char as trailer,
            //- cause some troubles when we have to decide if a name is used or not
            //- using a different building pattern (with the underscore) avoid this issue
            string newFormNameHeader = $"{type}";
            int newFormNametrailer = 0;
            string newFormName = string.Empty;
            bool isNew = true;
            do
            {
                isNew = true;
                newFormNametrailer++;
                newFormName = newFormNameHeader + newFormNametrailer;
                foreach (DesignSurfaceUView item in DesignSurfaceUViewCollection)
                {
                    var designHost = item.GetService(typeof(IDesignerHost)) as IDesignerHost;
                    if (designHost == null || designHost.RootComponent == null) continue;
                    string currentFormName = designHost.RootComponent.Site.Name;
                    isNew &= ((newFormName == currentFormName) ? false : true);
                }
            } while (!isNew);
            return newFormName;
        }
    }
}
