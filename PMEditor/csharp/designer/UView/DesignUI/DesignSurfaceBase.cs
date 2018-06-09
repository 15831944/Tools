using System;
using System.ComponentModel.Design;
using System.Drawing;
using System.ComponentModel;
using System.Windows.Forms;
using System.ComponentModel.Design.Serialization;
using System.Diagnostics;
using DesignUI.Service.OptionService;
using System.IO;
using System.Collections.Generic;

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
    public class DesignSurfaceBase : DesignSurface, IDesignSurfaceBase
    {
        private const string _name = "DesignSurfaceBase";

        #region IDesignSurfaceBase Members
        public void SwitchTabOrder()
        {
            if (IsTabOrderMode) DisposeTabOrder();
            else InvokeTabOrder();
        }

        private void UseNewOptionService(DesignerOptionService opsServiceNew)
        {
            IServiceContainer serviceProvider = this.GetService(typeof(IServiceContainer)) as IServiceContainer;
            DesignerOptionService opsService = serviceProvider.GetService(typeof(DesignerOptionService)) as DesignerOptionService;
            if (null != opsService) serviceProvider.RemoveService(typeof(DesignerOptionService));
            serviceProvider.AddService(typeof(DesignerOptionService), opsServiceNew);
        }

        public void UseSnapLines()
        {
            UseNewOptionService(new DesignerOptionServiceSnapLines());
        }

        public void UseGrid(Size gridSize)
        {
            UseNewOptionService(new DesignerOptionServiceGrid(gridSize));
        }

        public void UseGridWithoutSnapping(Size gridSize)
        {
            UseNewOptionService(new DesignerOptionServiceGridWithoutSnapping(gridSize));
        }

        public void UseNoGuides()
        {
            UseNewOptionService(new DesignerOptionServiceNoGuides());
        }

        public Service.UndoServiceImpl GetUndoService()
        {
            return this._undoService;
        }

        private IComponent CreateRootComponentCore(Type controlType, Size controlSize, DesignerLoader loader)
        {
            const string _signature_ = _name + @"::CreateRootComponentCore()";
            try
            {   //- step.1
                //- Get the IDesignerHost, if we are not not able to get it, then rollback (return without do nothing)
                //- Check if the root component has already been set, if so then rollback (return without do nothing)
                IDesignerHost host = GetIDesignerHost();
                if (null == host || null != host.RootComponent) return null;


                //- step.2
                //- Create a new root component and initialize it via its designer, if the component has not a designer
                //- then rollback (return without do nothing), else do the initialization
                if (null != loader) this.BeginLoad(loader);
                else                this.BeginLoad(controlType);
                if (this.LoadErrors.Count > 0)
                    throw new Exception(_signature_ + " - Exception: the BeginLoad failed!");

                //- step.3
                //- try to modify the Size of the object just created
                //IDesignerHost ihost = GetIDesignerHost();
                //- Set the backcolor and the Size
                Control ctrl = this.View as Control;
                if (host.RootComponent is Control) // Form, UserControl, Panel, Control
                {
                    ctrl.BackColor = Color.LightGray;
                    PropertyDescriptorCollection pdc = TypeDescriptor.GetProperties(ctrl);  //- set the Size
                    pdc.Find("Size", false)?.SetValue(host.RootComponent, controlSize);
                }
                else if (host.RootComponent is Component) // Timer, ImageList
                {
                    ctrl.BackColor = Color.White;
                }
                else     //- Undefined Host Type
                {
                    ctrl.BackColor = Color.Red;
                }
                return host.RootComponent;
            }
            catch (Exception exx)
            {
                Debug.WriteLine(exx.Message);
                if (null != exx.InnerException)
                    Debug.WriteLine(exx.InnerException.Message);
                throw;
            }
        }

        public IComponent CreateRootComponent(Type controlType, Size controlSize)
        {
            return CreateRootComponentCore(controlType, controlSize, null);
        }

        public IComponent CreateRootComponent(DesignerLoader loader, Size controlSize)
        {
            return CreateRootComponentCore(null, controlSize, loader);
        }

        public IComponent CreateControl(Type controlType, Size controlSize, Point controlLocation)
        {
            try
            {
                //- step.1
                //- Get the IDesignerHost, if we are not able to get it, then rollback (return without do nothing)
                //- check if the root component has already been set if not so then rollback (return without do nothing)
                IDesignerHost host = GetIDesignerHost();
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

        public IDesignerHost GetIDesignerHost()
        {
            return (IDesignerHost)(this.GetService(typeof(IDesignerHost)));
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
        private TabOrderHooker _tabOrder = null;
        private bool _tabOrderMode = false;

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
            TabOrder.HookTabOrder(this.GetIDesignerHost());
            _tabOrderMode = true;
        }

        public void DisposeTabOrder()
        {
            TabOrder.HookTabOrder(this.GetIDesignerHost());
            _tabOrderMode = false;
        }
        #endregion

        #region  UndoEngine
        private Service.UndoServiceImpl _undoService = null;
        private Service.NameCreationServiceImp _nameCreationService = null;
        private Service.DesignerSerializationServiceImpl _designerSerializationService = null;
        private CodeDomComponentSerializationService _codeDomComponentSerializationService = null;
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

        //- The DesignSurface class provides several design-time services automatically. The DesignSurface class adds all of its services in its constructor.
        //- Most of these services can be overridden by replacing them in the protected ServiceContainer property.To replace a service, override the constructor,
        //- call base, and make any changes through the protected ServiceContainer property.
        private void InitServices()
        {
            //- Each DesignSurface has its own default services, We can leave the default services in their present state,
            //- or we can remove them and replace them with our own. Now add our own services using IServiceContainer.

            //- Note, before loading the root control in the design surface, we must add an instance of naming service to the service container.
            //- otherwise the root component did not have a name and this caused troubles when we try to use the UndoEngine

            //- 1. NameCreationService
            _nameCreationService = new Service.NameCreationServiceImp();
            InitServier(typeof(INameCreationService), _nameCreationService);

            //- 2. CodeDomComponentSerializationService, the CodeDomComponentSerializationService is ready to be replaced
            _codeDomComponentSerializationService = new CodeDomComponentSerializationService(this.ServiceContainer);
            InitServier(typeof(ComponentSerializationService), _codeDomComponentSerializationService);

            //- 3. IDesignerSerializationService, the IDesignerSerializationService is ready to be replaced
            _designerSerializationService = new Service.DesignerSerializationServiceImpl(this.ServiceContainer);
            InitServier(typeof(IDesignerSerializationService), _designerSerializationService);

            //- 4. UndoEngine, the UndoEngine is ready to be replaced
            _undoService = new Service.UndoServiceImpl(this.ServiceContainer);
            _undoService.Enabled = false;   //- disable the UndoEngine
            InitServier(typeof(UndoEngine), _undoService);

            //- 5. IMenuCommandService
            InitServier(typeof(IMenuCommandService), new MenuCommandService(this));
        }

        private void InitServier(Type t, object s)
        {
            if (s != null) {
                ServiceContainer.RemoveService(t, false);
                ServiceContainer.AddService(t, s);
            }
        }
        #endregion

        //- do some Edit menu command using the MenuCommandServiceImp
        public void DoAction(string command)
        {
            if (string.IsNullOrEmpty(command)) return;

            IMenuCommandService ims = this.GetService(typeof(IMenuCommandService)) as IMenuCommandService;
            if (null == ims) return;
            try {
                switch (command.ToUpper()) {
                    case "CUT":     ims.GlobalInvoke(StandardCommands.Cut); break;
                    case "COPY":    ims.GlobalInvoke(StandardCommands.Copy); break;
                    case "PASTE":   ims.GlobalInvoke(StandardCommands.Paste); break;
                    case "DELETE":  ims.GlobalInvoke(StandardCommands.Delete); break;
                    default:        break;  // do nothing;
                }
            }
            catch (Exception exx)
            {
                Debug.WriteLine(exx.Message);
                if (null != exx.InnerException)
                    Debug.WriteLine(exx.InnerException.Message);
                throw;
            }
        }

        private void SaveToFile(string strFile)
        {
            var service = this.GetService(typeof(IDesignerSerializationService)) as Service.DesignerSerializationServiceImpl;
            var host = GetIDesignerHost();
            List<IComponent> ltcomps = new List<IComponent>();
            foreach (IComponent p in host.Container.Components)
            {
                ltcomps.Add(p);
            }
            //ltcomps.RemoveAt(0);
            //var store = service.Serialize(host.Container.Components);
            var store = service.Serialize(ltcomps);
            var serializeStore = store as SerializationStore;

            MemoryStream memoryStream = new MemoryStream();

            using (FileStream fileStream = new FileStream(strFile, FileMode.OpenOrCreate, FileAccess.Write, FileShare.Write))
            {
                serializeStore.Save(fileStream);
            }
        }

        public void Save()
        {
            SaveToFile(@"D:\\disp.disp");
        }

        public void Open(string strFile)
        {
            using (FileStream fileStream = new FileStream(strFile, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                ComponentSerializationService componentStoreService = this.GetService(typeof(ComponentSerializationService)) as ComponentSerializationService;
                using (SerializationStore serializationStore = componentStoreService.LoadStore(fileStream))
                {
                    var service = this.GetService(typeof(IDesignerSerializationService)) as Service.DesignerSerializationServiceImpl;
                    var designer = GetIDesignerHost();
                    var components = componentStoreService.Deserialize(serializationStore, designer.Container);
                
                    // reset parent
                    var rootComponent = designer.RootComponent as ContainerControl;
                    foreach (var p in components)
                    {
                        if (p is Control)
                            rootComponent.Controls.Add(p as Control);
                    }
                }
                //ComponentSerializationService componentStoreService = this.GetService(typeof(ComponentSerializationService)) as ComponentSerializationService;
                //using (SerializationStore serializationStore = componentStoreService.LoadStore(fileStream))
                //{
                //    var service = this.GetService(typeof(IDesignerSerializationService)) as Service.DesignerSerializationServiceImpl;
                //    var designer = GetIDesignerHost();
                //    var components = service.Deserialize(serializationStore);
                //
                //    var rootComponent = designer.RootComponent as ContainerControl;
                //    foreach (var p in components)
                //    {
                //        if (p is Control)
                //            rootComponent.Controls.Add(p as Control);
                //    }
                //}
            }
        }
    }
}
