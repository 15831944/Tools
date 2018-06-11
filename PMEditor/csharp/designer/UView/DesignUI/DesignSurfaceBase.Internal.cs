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
    public partial class DesignSurfaceBase
    {
        private const string _name = "DesignSurfaceBase";
        private TabOrderHooker _tabOrder = null;
        private bool _tabOrderMode = false;

        // UndoEngine
        private Service.UndoServiceImpl _undoService = null;
        private Service.NameCreationServiceImp _nameCreationService = null;
        private Service.DesignerSerializationServiceImpl _designerSerializationService = null;
        private CodeDomComponentSerializationService _codeDomComponentSerializationService = null;

        private void UseNewOptionService(DesignerOptionService opsServiceNew)
        {
            IServiceContainer serviceProvider = this.GetService(typeof(IServiceContainer)) as IServiceContainer;
            DesignerOptionService opsService = serviceProvider.GetService(typeof(DesignerOptionService)) as DesignerOptionService;
            if (null != opsService) serviceProvider.RemoveService(typeof(DesignerOptionService));
            serviceProvider.AddService(typeof(DesignerOptionService), opsServiceNew);
        }

        private IComponent CreateRootComponentCore(Type controlType, string name, Size controlSize, DesignerLoader loader)
        {
            const string _signature_ = _name + @"::CreateRootComponentCore()";
            try
            {   //- step.1
                //- Get the IDesignerHost, if we are not not able to get it, then rollback (return without do nothing)
                //- Check if the root component has already been set, if so then rollback (return without do nothing)
                IDesignerHost host = GetService(typeof(IDesignerHost)) as IDesignerHost;
                if (null == host || null != host.RootComponent) return null;


                //- step.2
                //- Create a new root component and initialize it via its designer, if the component has not a designer
                //- then rollback (return without do nothing), else do the initialization
                if (null != loader) this.BeginLoad(loader);
                else this.BeginLoad(controlType);
                if (this.LoadErrors.Count > 0)
                    throw new Exception(_signature_ + $" - Exception: the BeginLoad failed!");

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
                if (!string.IsNullOrEmpty(name))
                {
                    if (host.RootComponent.Site != null)
                        host.RootComponent.Site.Name = name;
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
            if (s != null)
            {
                ServiceContainer.RemoveService(t, false);
                ServiceContainer.AddService(t, s);
            }
        }

    }
}
