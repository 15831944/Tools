using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Design;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MyControls.Grid
{
    public class DGridViewSite : ISite, IMenuCommandService, IEventBindingService
    {
        private IComponent _control;
        public DGridViewSite(IComponent obj) { this._control = obj; }

        // ISite实现
        public IComponent Component { get { return this._control; } }
        public IContainer Container { get { return null; } }
        public bool DesignMode { get { return true; } }
        public string Name { get; set; }
        public object GetService(Type serviceType)
        {
            if (serviceType == typeof(IMenuCommandService)) return this;
            if (serviceType == typeof(IEventBindingService)) return this;
            return null;
        }

        // IMenuCommandService实现
        public void AddCommand(MenuCommand command) { }
        public void AddVerb(DesignerVerb verb) { }
        public MenuCommand FindCommand(CommandID commandID) { return null; }
        public bool GlobalInvoke(CommandID commandID) { return false; }
        public void RemoveCommand(MenuCommand command) { }
        public void RemoveVerb(DesignerVerb verb) { }
        public void ShowContextMenu(CommandID menuID, int x, int y) { }
        public DesignerVerbCollection Verbs
        {
            get
            {
                return new DesignerVerbCollection(new DesignerVerb[]
                { 
                    new DesignerVerb("Column Setting", new EventHandler(this.OnColumnSetting)),
                });
            }
        }
        private void OnColumnSetting(object sender, EventArgs e)
        {
            //System.Windows.Forms.Design.DataGridViewColumnCollectionEditor editor = new System.Windows.Forms.Design.DataGridViewColumnCollectionEditor();
            //MessageBox.Show("Column Setting");
        }

        public string CreateUniqueMethodName(IComponent component, EventDescriptor e)
        {
            throw new NotImplementedException();
        }

        public System.Collections.ICollection GetCompatibleMethods(EventDescriptor e)
        {
            throw new NotImplementedException();
        }

        public EventDescriptor GetEvent(PropertyDescriptor property)
        {
            throw new NotImplementedException();
        }

        public PropertyDescriptorCollection GetEventProperties(EventDescriptorCollection events)
        {
            throw new NotImplementedException();
        }

        public PropertyDescriptor GetEventProperty(EventDescriptor e)
        {
            throw new NotImplementedException();
        }

        public bool ShowCode(IComponent component, EventDescriptor e)
        {
            throw new NotImplementedException();
        }

        public bool ShowCode(int lineNumber)
        {
            throw new NotImplementedException();
        }

        public bool ShowCode()
        {
            throw new NotImplementedException();
        }
    }
}
