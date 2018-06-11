using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel.Design;
using System.Diagnostics;

using DesignUI;
using System.IO;
using System.ComponentModel.Design.Serialization;
using DesignUI.Service;
using System.Drawing.Design;

namespace DesignUI.View
{
    //- [Note FROM MSDN]:
    //- The DesignSurfaceManager.ActiveDesignSurface property should be set
    //- by the designer's Type user interface
    //- whenever a designer becomes the active window!
    //- That is to say:
    //-   the DesignUIMgr is an OBSERVER of UI event: SelectedTab/SelectedIndex Changed
    //- usage:
    //       //- SelectedIndexChanged event fires when the TabControls SelectedIndex or SelectedTab value changes.
    //       //- give the focus to the DesigneSurface accordingly to te selected TabPage and sync the propertyGrid
    //       this.tabControl1.SelectedIndexChanged += ( object sender, EventArgs e ) => {
    //            TabControl tabCtrl = sender as TabControl;
    //            DesignSurfaceManagerExtObject.ActiveDesignSurface = (DesignSurfaceUView) DesignSurfaceManagerExtObject.DesignSurfaces[tabCtrl.SelectedIndex];
    //       };
    //-
    //- UDesigner class
    public partial class UDesigner : UserControl, Interface.IUDesigner
    {
        private const string _name = "UDesigner";

        //- the DesignUIMgr instance must be an OBSERVER
        //- of the UI event which change the active DesignSurface
        //- DesignSurfaceManager is exposed as public getter properties as test facility
        public DesignUIMgr DesignSurfaceManager { get; private set; }

        #region ctors
        //- usage:
        //-         if (a){
        //-             // do work
        //-         }
        //-         else{
        //-             // a is not valid
        //-         }
        public static implicit operator bool(UDesigner d)
        {
            //bool isValid = true;
            //- the object 'd' must be correctly initialized
            //isValid &= ((null == d.Toolbox) ? false : true);
            //return isValid;
            return d.Toolbox != null;
        }

        //- ctor
        public UDesigner()
        {
            InitializeComponent();

            DesignSurfaceManager = new DesignUIMgr();
            DesignSurfaceManager.PropertyGridHost.Parent = this.splitterpDesigner.Panel2;

            Toolbox = null;
            this.Dock = DockStyle.Fill;
        }
        #endregion

        private void tbCtrlpDesigner_SelectedIndexChanged(object sender, EventArgs e)
        {
            TabControl tabCtrl = sender as TabControl;
            int index = this.tbCtrlpDesigner.SelectedIndex;
            if (index >= 0)
                DesignSurfaceManager.ActiveDesignSurface = (DesignSurfaceUView)DesignSurfaceManager.DesignSurfaces[index];
            else {
                DesignSurfaceManager.ActiveDesignSurface = null;
                DesignSurfaceManager.PropertyGridHost.PropertyGrid.SelectedObject = null;
                DesignSurfaceManager.PropertyGridHost.ComboBox.Items.Clear();
            }
        }

        #region IUDesigner Members
        //- to get and set the real Toolbox which is provided by the user
        public ListBox Toolbox { get; set; }

        public TabControl TabControlHostingDesignSurfaces
        {
            get { return this.tbCtrlpDesigner; }
        }

        public View.PropertyGridHost PropertyGridHost
        {
            get { return DesignSurfaceManager.PropertyGridHost; }
        }

        public DesignSurfaceUView ActiveDesignSurface
        {
            get { return DesignSurfaceManager.ActiveDesignSurface as DesignSurfaceUView; }
        }

        //- Create the DesignSurface and the rootComponent (a .NET Control)
        //- using IDesignSurfaceBase.CreateRootComponent() 
        //- if the alignmentMode doesn't use the GRID, then the gridSize param is ignored
        //- Note:
        //-     the generics param is used to know which type of control to use as RootComponent
        //-     TT is requested to be derived from .NET Control class 
        public DesignSurfaceUView AddDesignSurface<TT>
            (int startingFormWidth, int startingFormHeight, Interface.AlignmentModeEnum alignmentMode)
            where TT : Control
        {
            const string _signature_ = _name + @"::AddDesignSurface<>()";

            if (!this)
                throw new Exception(_signature_ + " - Exception: " + _name + " is not initialized! Please set the Property: IUDesigner::Toolbox before calling any methods!");

            //- step.0
            //- create a DesignSurface
            DesignSurfaceUView surface = DesignSurfaceManager.CreateDesignSurfaceUView();
            this.DesignSurfaceManager.ActiveDesignSurface = surface;

            //- step.1
            //- choose an alignment mode...
            surface.UseOptionService(alignmentMode);

            //- step.2
            //- enable the UndoEngine
            (surface.GetService(typeof(UndoEngine)) as UndoEngine).Enabled = true;

            //- step.3
            //- Select the service IToolboxService
            //- and hook it to our ListBox
            var tbox = surface.GetService(typeof(IToolboxService)) as ToolboxServiceImp;
            //- we don't check if Toolbox is null because the very first check: if(!this)...
            if (null != tbox)
                tbox.Toolbox = this.Toolbox;

            //- step.4
            //- create the Root compoment, in these cases a Form
            Control rootComponent = null;
            //- cast to .NET Control because the TT object 
            //- has a constraint: to be a ".NET Control"
            rootComponent = surface.CreateRootComponent(typeof(TT), new Size(startingFormWidth, startingFormHeight)) as Control;
            //- rename the Sited component
            //- (because the user may add more then one Form
            //- and every new Form will be called "Form1"
            //- if we don't set its Name)
            rootComponent.Site.Name = this.DesignSurfaceManager.GetValidName(typeof(TT).Name);

            //- step.5
            //- enable the Drag&Drop on RootComponent
            surface.EnableDragandDrop();

            //- step.6
            //- IComponentChangeService is marked as Non replaceable service
            IComponentChangeService componentChangeService = (IComponentChangeService)(surface.GetService(typeof(IComponentChangeService)));
            if (null != componentChangeService)
            {
                //- the Type "ComponentEventHandler Delegate" Represents the method that will
                //- handle the ComponentAdding, ComponentAdded, ComponentRemoving, and ComponentRemoved
                //- events raised for component-level events
                componentChangeService.ComponentChanged += (Object sender, ComponentChangedEventArgs e) =>
                {
                    // do nothing
                };
                componentChangeService.ComponentAdded += (Object sender, ComponentEventArgs e) =>
                {
                    DesignSurfaceManager.UpdatePropertyGridHost(surface);
                };
                componentChangeService.ComponentRemoved += (Object sender, ComponentEventArgs e) =>
                {
                    DesignSurfaceManager.UpdatePropertyGridHost(surface);
                };
            }

            //- step.7
            //- now set the Form::Text Property
            //- (because it will be an empty string
            //- if we don't set it)
            if (!InitSurfaceText(surface, rootComponent, rootComponent.Site.Name + " (design mode)"))
                return null;

            //- step.8
            //- display the DesignSurface
            AddSurfaceToNewTabPage(surface, rootComponent.Site.Name);

            //- step.9
            //- finally return the DesignSurface created to let it be modified again by user
            return surface;
        }

        private bool InitSurfaceText(DesignSurfaceUView surface, Control rootComponent, string text)
        {
            Control view = surface.GetView();
            if (null == view)
                return false;
            PropertyDescriptorCollection pdc = TypeDescriptor.GetProperties(view);
            //- Sets a PropertyDescriptor to the specific property
            PropertyDescriptor pdS = pdc.Find("Text", false);
            if (null != pdS)
                pdS.SetValue(rootComponent, text);
            return true;
        }

        private void AddSurfaceToNewTabPage(DesignSurfaceUView surface, string pageName)
        {
            var view = surface.GetView();

            TabPage newPage = new TabPage(pageName);
            newPage.Name = pageName;
            newPage.SuspendLayout();
            view.Dock = DockStyle.Fill;
            view.Parent = newPage; //- Note this assignment
            this.tbCtrlpDesigner.TabPages.Add(newPage);
            newPage.ResumeLayout();
            //- select the TabPage created
            this.tbCtrlpDesigner.SelectedIndex = this.tbCtrlpDesigner.TabPages.Count - 1;
        }

        public void RemoveDesignSurface(DesignSurfaceUView surfaceToErase)
        {
            try
            {
                //- remove the TabPage which has the same name of
                //- the RootComponent host by DesignSurface "surfaceToErase"
                //- Note:
                //-     DesignSurfaceManager continues to reference the DesignSurface erased
                //-     that Designsurface continue to exist but it is no more reachable
                //-     this fact is usefull when generate new names for Designsurfaces just created
                //-     avoiding name clashing
                var designHost = surfaceToErase.GetService(typeof(IDesignerHost)) as IDesignerHost;
                string dsRootComponentName = designHost?.RootComponent.Site.Name;
                TabPage tpToRemove = null;
                foreach (TabPage tp in this.tbCtrlpDesigner.TabPages)
                {
                    if (tp.Name == dsRootComponentName)
                    {
                        tpToRemove = tp;
                        break;
                    }
                }
                if (null != tpToRemove)
                    this.tbCtrlpDesigner.TabPages.Remove(tpToRemove);

                //- now remove the DesignSurface
                this.DesignSurfaceManager.DeleteDesignSurfaceUView(surfaceToErase);

                //- finally the DesignSurfaceManager remove the DesignSurface
                //- AND set as active DesignSurface the last one
                //- therefore we set as active the last TabPage
                this.tbCtrlpDesigner.SelectedIndex = this.tbCtrlpDesigner.TabPages.Count - 1;
            }
            catch (Exception exx)
            {
                Debug.WriteLine(exx.Message);
                if (null != exx.InnerException)
                    Debug.WriteLine(exx.InnerException.Message);

                throw;
            }
        }

        public void ActionCommandOnDesignSurface(CommandID command)
        {
            var surf = DesignSurfaceManager.ActiveDesignSurface;
            if (surf == null) return;
            if (command == StandardCommands.Undo)
                (surf.GetService(typeof(UndoEngine)) as UndoServiceImpl)?.Undo();
            else if (command == StandardCommands.Redo)
                (surf.GetService(typeof(UndoEngine)) as UndoServiceImpl)?.Redo();
            else
                DesignSurfaceManager.ActiveDesignSurface?.DoAction(command);
        }

        public void SwitchTabOrder()
        {
            Interface.IDesignSurfaceBase isurf = DesignSurfaceManager.ActiveDesignSurface;
            isurf?.SwitchTabOrder();
        }

        public void SaveCurrent()
        {
            if (null == DesignSurfaceManager.ActiveDesignSurface) return;

            //isurf.Save();
        }

        public void SaveAll()
        {
            foreach (var isurf in DesignSurfaceManager.DesignSurfaces)
            {
                //isurf?.Save();
            }
        }

        public void Open(string strFile)
        {
            //DesignSurfaceManager.CreateDesignSurfaceUView();
            //FileStream fileStream = new FileStream("D:\\disp.txt", FileMode.Open, FileAccess.Read, FileShare.Read);
            //SerializationStore store = new SerializationStore();
            //fileStream.
            //fileStream.Close();
            var surface = AddDesignSurface<Display>(640, 480, Interface.AlignmentModeEnum.SnapLines);
            //surface.Open(strFile);
        }
        #endregion
    }
}
