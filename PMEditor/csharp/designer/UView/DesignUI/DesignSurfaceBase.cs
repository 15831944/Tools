using System;
using System.IO;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel;
using System.ComponentModel.Design;
using System.ComponentModel.Design.Serialization;
using System.Collections.Generic;
using System.Diagnostics;

using DesignUI.Service.OptionService;

namespace DesignUI
{
    //- this class adds to a .NET 
    //-     DesignSurface instance
    //- the following facilities:
    //-     * TabOrder
    //-     * UndoEngine
    //-     * Cut/Copy/Paste/Delete commands
    //-
    //- DesignSurfaceBase
    //-     |
    //-     +--|DesignSurface|
    //-     +--|TabOrder|
    //-     +--|UndoEngine|
    //-     +--|Cut/Copy/Paste/Delete commands|
    //-
    public partial class DesignSurfaceBase : DesignSurface, Interface.IDesignSurfaceBase
    {
        #region IDesignSurfaceBase Members
        public void SwitchTabOrder()
        {
            if (IsTabOrderMode) DisposeTabOrder();
            else InvokeTabOrder();
        }

        //public void UseSnapLines()
        public void UseOptionService(Interface.AlignmentModeEnum option)
        {
            //UseNewOptionService(new DesignerOptionServiceSnapLines());
            switch (option)
            {
                case Interface.AlignmentModeEnum.SnapLines:
                    UseNewOptionService(new DesignerOptionServiceSnapLines());
                    break;
                case Interface.AlignmentModeEnum.Grid:
                    UseNewOptionService(new DesignerOptionServiceGrid(new Size(8, 8)));
                    break;
                //case AlignmentModeEnum.GridWithoutSnapping:
                //    UseNewOptionService(new DesignerOptionServiceGridWithoutSnapping(new Size(4, 4)));
                //    break;
                //case AlignmentModeEnum.NoGuides:
                //    UseNewOptionService(new DesignerOptionServiceNoGuides());
                //    break;
                default:
                    UseNewOptionService(new DesignerOptionServiceSnapLines());
                    break;
            }
        }

        public IComponent CreateRootComponent(Type type, string name, Size size)
        {
            return CreateRootComponentCore(type, name, size, null);
        }

        public IComponent CreateRootComponent(DesignerLoader loader, Size size)
        {
            return CreateRootComponentCore(null, "", size, loader);
        }

        public IComponent CreateControl(Type controlType, Size controlSize, Point controlLocation)
        {
            try
            {
                //- step.1
                //- Get the IDesignerHost, if we are not able to get it, then rollback (return without do nothing)
                //- check if the root component has already been set if not so then rollback (return without do nothing)
                IDesignerHost host = GetService(typeof(IDesignerHost)) as IDesignerHost;
                if (null == host || null == host.RootComponent) return null;

                //- step.2
                //- Create a new component and initialize it via its designer, if the component has not a designer.
                //- Then rollback (return without do nothing), else do the initialization
                IComponent newComp = host.CreateComponent(controlType);
                if (null == newComp) return null;
                IDesigner designer = host.GetDesigner(newComp);
                if (null == designer) return null;
                if (designer is IComponentInitializer)
                    ((IComponentInitializer)designer).InitializeNewComponent(null);

                //- step.3
                //- try to modify the Size/Location of the object just created
                PropertyDescriptorCollection pdc = TypeDescriptor.GetProperties(newComp);
                //- Sets a PropertyDescriptor to the specific property.
                PropertyDescriptor pdS = pdc.Find("Size", false);
                pdS?.SetValue(newComp, controlSize);
                PropertyDescriptor pdL = pdc.Find("Location", false);
                pdL?.SetValue(newComp, controlLocation);

                //- step.4
                //- Commit the Creation Operation, adding the control to the DesignSurface's root component
                //- and return the control just created to let further initializations
                ((Control)newComp).Parent = host.RootComponent as Control;
                return newComp;
            }
            catch (Exception exx)
            {
                Debug.WriteLine(exx.Message);
                if (null != exx.InnerException)
                    Debug.WriteLine(exx.InnerException.Message);
                throw;
            }
        }

        public Control GetView()
        {
            Control ctrl = this.View as Control;
            if (null != ctrl && ctrl.Dock != DockStyle.Fill)    // must be fill style
                ctrl.Dock = DockStyle.Fill;
            return ctrl;
        }
        #endregion

        #region TabOrder        

        public bool IsTabOrderMode
        {
            get { return _tabOrderMode; }
        }

        public TabOrderHooker TabOrder
        {
            get {
                if (_tabOrder == null)
                    _tabOrder = new TabOrderHooker();
                return _tabOrder;
            }
            set { _tabOrder = value; }
        }

        public void InvokeTabOrder()
        {
            TabOrder.HookTabOrder(GetService(typeof(IDesignerHost)) as IDesignerHost);
            _tabOrderMode = true;
        }

        public void DisposeTabOrder()
        {
            TabOrder.HookTabOrder(GetService(typeof(IDesignerHost)) as IDesignerHost);
            _tabOrderMode = false;
        }
        #endregion

        #region ctors
        //- ctors
        //- Summary:
        //-     Initializes a new instance of the System.ComponentModel.Design.DesignSurface class.
        //-
        //- Exceptions:
        //-   System.ObjectDisposedException:
        //-     The System.ComponentModel.Design.IDesignerHost attached to the System.ComponentModel.Design.DesignSurface has been disposed.
        public DesignSurfaceBase() : base() { InitServices(); }

        //- Summary:
        //-     Initializes a new instance of the System.ComponentModel.Design.DesignSurface class.
        //-
        //- Parameters:
        //-   parentProvider: The parent service provider, or null if there is no parent used to resolve services.
        //-
        //- Exceptions:
        //-   System.ObjectDisposedException:
        //-     The System.ComponentModel.Design.IDesignerHost attached to the System.ComponentModel.Design.DesignSurface has been disposed.
        public DesignSurfaceBase(IServiceProvider parentProvider) : base(parentProvider) { InitServices(); }

        //- Summary:
        //-     Initializes a new instance of the System.ComponentModel.Design.DesignSurface class.
        //-
        //- Parameters:
        //-   rootComponentType: The type of root component to create.
        //-
        //- Exceptions:
        //-   System.ArgumentNullException: rootComponent is null.
        //-   System.ObjectDisposedException: The System.ComponentModel.Design.IDesignerHost attached to the System.ComponentModel.Design.DesignSurface has been disposed.
        public DesignSurfaceBase(Type rootComponentType) : base(rootComponentType) { InitServices(); }

        //- Summary:
        //-     Initializes a new instance of the System.ComponentModel.Design.DesignSurface class.
        //-
        //- Parameters:
        //-   parentProvider: The parent service provider, or null if there is no parent used to resolve services.
        //-   rootComponentType: The type of root component to create.
        //-
        //- Exceptions:
        //-   System.ArgumentNullException: rootComponent is null.
        //-   System.ObjectDisposedException: The System.ComponentModel.Design.IDesignerHost attached to the System.ComponentModel.Design.DesignSurface has been disposed.
        public DesignSurfaceBase(IServiceProvider parentProvider, Type rootComponentType) : base(parentProvider, rootComponentType) { InitServices(); }
        #endregion

        //- do some Edit menu command using the MenuCommandServiceImp
        public void DoAction(string command)
        {
            if (string.IsNullOrEmpty(command)) return;
            switch (command.ToUpper()) {
                case "CUT":         DoAction(StandardCommands.Cut); break;
                case "COPY":        DoAction(StandardCommands.Copy); break;
                case "PASTE":       DoAction(StandardCommands.Paste); break;
                case "DELETE":      DoAction(StandardCommands.Delete); break;
                case "SELECT ALL":  DoAction(StandardCommands.SelectAll); break;
                default:            break;  // do nothing;
            }
        }

        public void DoAction(CommandID command)
        {
            try
            {
                IMenuCommandService ims = this.GetService(typeof(IMenuCommandService)) as IMenuCommandService;
                ims?.GlobalInvoke(command);
            }
            catch (Exception exx)
            {
                Debug.WriteLine(exx.Message);
                if (null != exx.InnerException)
                    Debug.WriteLine(exx.InnerException.Message);
                throw;
            }
        }

        //private void SaveToFile(string strFile)
        //{
        //    var service = this.GetService(typeof(IDesignerSerializationService)) as Service.DesignerSerializationServiceImpl;
        //    var host = GetIDesignerHost();
        //    List<IComponent> ltcomps = new List<IComponent>();
        //    foreach (IComponent p in host.Container.Components)
        //    {
        //        ltcomps.Add(p);
        //    }
        //    //ltcomps.RemoveAt(0);
        //    //var store = service.Serialize(host.Container.Components);
        //    var store = service.Serialize(ltcomps);
        //    var serializeStore = store as SerializationStore;
        //
        //    MemoryStream memoryStream = new MemoryStream();
        //
        //    using (FileStream fileStream = new FileStream(strFile, FileMode.OpenOrCreate, FileAccess.Write, FileShare.Write))
        //    {
        //        serializeStore.Save(fileStream);
        //    }
        //}
        //
        //public void Save()
        //{
        //    SaveToFile(@"D:\\disp.disp");
        //}
        //
        //public void Open(string strFile)
        //{
        //    using (FileStream fileStream = new FileStream(strFile, FileMode.Open, FileAccess.Read, FileShare.Read))
        //    {
        //        ComponentSerializationService componentStoreService = this.GetService(typeof(ComponentSerializationService)) as ComponentSerializationService;
        //        using (SerializationStore serializationStore = componentStoreService.LoadStore(fileStream))
        //        {
        //            var service = this.GetService(typeof(IDesignerSerializationService)) as Service.DesignerSerializationServiceImpl;
        //            var designer = GetIDesignerHost();
        //            var components = componentStoreService.Deserialize(serializationStore, designer.Container);
        //        
        //            // reset parent
        //            var rootComponent = designer.RootComponent as ContainerControl;
        //            foreach (var p in components)
        //            {
        //                if (p is Control)
        //                    rootComponent.Controls.Add(p as Control);
        //            }
        //        }
        //        //ComponentSerializationService componentStoreService = this.GetService(typeof(ComponentSerializationService)) as ComponentSerializationService;
        //        //using (SerializationStore serializationStore = componentStoreService.LoadStore(fileStream))
        //        //{
        //        //    var service = this.GetService(typeof(IDesignerSerializationService)) as Service.DesignerSerializationServiceImpl;
        //        //    var designer = GetIDesignerHost();
        //        //    var components = service.Deserialize(serializationStore);
        //        //
        //        //    var rootComponent = designer.RootComponent as ContainerControl;
        //        //    foreach (var p in components)
        //        //    {
        //        //        if (p is Control)
        //        //            rootComponent.Controls.Add(p as Control);
        //        //    }
        //        //}
        //    }
        //}
    }
}
