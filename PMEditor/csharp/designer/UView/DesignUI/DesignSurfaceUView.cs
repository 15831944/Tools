using System;
using System.Drawing.Design;
using System.Windows.Forms;
using System.ComponentModel.Design;

namespace DesignUI
{
    //- this class adds to
    //-     DesignSurfaceBase instance
    //- the following facilities:
    //-     * Toolbox mechanisms (the ToolBox container must be provided by the user)
    //-     * ContextMenu on DesignSurface with Cut/Copy/Paste/Delete commands
    //-
    //- DesignSurfaceUView
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
    public class DesignSurfaceUView : DesignSurfaceBase
    {
        private const string _name = "DesignSurfaceUView";

        #region  IToolboxService
        private Service.ToolboxServiceImp _toolboxService = null;

        public Service.ToolboxServiceImp GetIToolboxService()
        {
            return (Service.ToolboxServiceImp)this.GetService(typeof(IToolboxService));
        }

        #region drag&Drop
        public void EnableDragandDrop()
        {
            // For the management of the drag and drop of the toolboxItems
            Control ctrl = this.GetView();
            if (null == ctrl)
                return;
            ctrl.AllowDrop = true;
            ctrl.DragDrop += new DragEventHandler(OnDragDrop);

            //- enable the Dragitem inside the our Toolbox
            Service.ToolboxServiceImp tbs = GetService(typeof(IToolboxService)) as Service.ToolboxServiceImp;
            if (null == tbs || null == tbs.Toolbox)
                return;
            tbs.Toolbox.MouseDown += new MouseEventHandler(OnListboxMouseDown);
        }

        //- Management of the Drag&Drop of the toolboxItems contained inside our Toolbox
        private void OnListboxMouseDown(object sender, MouseEventArgs e)
        {
            Service.ToolboxServiceImp tbs = GetService(typeof(IToolboxService)) as Service.ToolboxServiceImp;
            if (null == tbs || null == tbs.Toolbox || null == tbs.Toolbox.SelectedItem)
                return;

            tbs.Toolbox.DoDragDrop(tbs.Toolbox.SelectedItem, DragDropEffects.Copy | DragDropEffects.Move);
        }

        //- Management of the drag and drop of the toolboxItems
        public void OnDragDrop(object sender, DragEventArgs e)
        {
            //- if the user don't drag a ToolboxItem 
            //- then do nothing
            if (!e.Data.GetDataPresent(typeof(ToolboxItem)))
            {
                e.Effect = DragDropEffects.None;
                return;
            }
            //- now retrieve the data node
            ToolboxItem item = e.Data.GetData(typeof(ToolboxItem)) as ToolboxItem;
            e.Effect = DragDropEffects.Copy;
            item.CreateComponents(GetService(typeof(IDesignerHost)) as IDesignerHost);

        }
        #endregion
        #endregion

        #region  MenuCommandService
        private Service.MenuCommandServiceImpl _menuCommandService = null;
        #endregion

        #region ctors
        //- ctors
        //- Summary:
        //-     Initializes a new instance of the System.ComponentModel.Design.DesignSurface
        //-     class.
        //-
        //- Exceptions:
        //-   System.ObjectDisposedException:
        //-     The System.ComponentModel.Design.IDesignerHost attached to the System.ComponentModel.Design.DesignSurface
        //-     has been disposed
        public DesignSurfaceUView() : base() { InitServices(); }

        //- Summary:
        //-     Initializes a new instance of the System.ComponentModel.Design.DesignSurface
        //-     class.
        //-
        //- Parameters:
        //-   parentProvider:
        //-     The parent service provider, or null if there is no parent used to resolve
        //-     services.
        //-
        //- Exceptions:
        //-   System.ObjectDisposedException:
        //-     The System.ComponentModel.Design.IDesignerHost attached to the System.ComponentModel.Design.DesignSurface
        //-     has been disposed
        public DesignSurfaceUView(IServiceProvider parentProvider) : base(parentProvider) { InitServices(); }

        //- Summary:
        //-     Initializes a new instance of the System.ComponentModel.Design.DesignSurface
        //-     class.
        //-
        //- Parameters:
        //-   rootComponentType:
        //-     The type of root component to create
        //-
        //- Exceptions:
        //-   System.ArgumentNullException:
        //-     rootComponent is null.
        //-
        //-   System.ObjectDisposedException:
        //-     The System.ComponentModel.Design.IDesignerHost attached to the System.ComponentModel.Design.DesignSurface
        //-     has been disposed
        public DesignSurfaceUView(Type rootComponentType) : base(rootComponentType) { InitServices(); }

        //- Summary:
        //-     Initializes a new instance of the System.ComponentModel.Design.DesignSurface
        //-     class.
        //-
        //- Parameters:
        //-   parentProvider:
        //-     The parent service provider, or null if there is no parent used to resolve
        //-     services.
        //-
        //-   rootComponentType:
        //-     The type of root component to create.
        //-
        //- Exceptions:
        //-   System.ArgumentNullException:
        //-     rootComponent is null.
        //-
        //-   System.ObjectDisposedException:
        //-     The System.ComponentModel.Design.IDesignerHost attached to the System.ComponentModel.Design.DesignSurface
        //-     has been disposed.
        public DesignSurfaceUView(IServiceProvider parentProvider, Type rootComponentType) : base(parentProvider, rootComponentType) { InitServices(); }

        //- The DesignSurface class provides several design-time services automatically.
        //- The DesignSurface class adds all of its services in its constructor.
        //- Most of these services can be overridden by replacing them in the
        //- protected ServiceContainer property.To replace a service, override the constructor,
        //- call base, and make any changes through the protected ServiceContainer property.
        private void InitServices()
        {
            //- each DesignSurface has its own default services
            //- We can leave the default services in their present state,
            //- or we can remove them and replace them with our own.
            //- Now add our own services using IServiceContainer
            _menuCommandService = new Service.MenuCommandServiceImpl(this);
            if (_menuCommandService != null)
            {
                //- remove the old Service, i.e. the DesignsurfaceExt service
                this.ServiceContainer.RemoveService(typeof(IMenuCommandService), false);
                //- add the new IMenuCommandService
                this.ServiceContainer.AddService(typeof(IMenuCommandService), _menuCommandService);
            }

            //- IToolboxService
            _toolboxService = new Service.ToolboxServiceImp(GetService(typeof(IDesignerHost)) as IDesignerHost);
            if (_toolboxService != null)
            {
                this.ServiceContainer.RemoveService(typeof(IToolboxService), false);
                this.ServiceContainer.AddService(typeof(IToolboxService), _toolboxService);
            }
        }
        #endregion
    }
}
