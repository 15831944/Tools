using System;
using System.Windows.Forms;

namespace UView.View.Panel
{
    internal abstract class PanelControl
    {
        private DevExpress.XtraBars.Docking.ControlContainer _container = new DevExpress.XtraBars.Docking.ControlContainer();
        private DevExpress.XtraBars.Docking.DockPanel _dockPanel = new DevExpress.XtraBars.Docking.DockPanel();
        internal PanelControl() { }

        internal abstract void InitializeComponent(MainEditor editor);

        internal void InitializeCtrl(Control ctrl, string name, string text, DevExpress.XtraBars.Docking.DockingStyle dock)
        {
            // 
            // _container
            // 
            this._container.Controls.Add(ctrl);
            this._container.Location = new System.Drawing.Point(0, 0);
            this._container.Name = "_container";
            this._container.Size = new System.Drawing.Size(199, 554);
            this._container.TabIndex = 0;
            // 
            // _dockPanel
            // 
            this._dockPanel.Controls.Add(this._container);
            this._dockPanel.Dock = dock;
            this._dockPanel.ID = Guid.NewGuid();
            this._dockPanel.Location = new System.Drawing.Point(3, 25);
            this._dockPanel.Name = name;
            this._dockPanel.OriginalSize = new System.Drawing.Size(199, 554);
            this._dockPanel.Size = new System.Drawing.Size(199, 554);
            this._dockPanel.Text = text;
        }

        public DevExpress.XtraBars.Docking.ControlContainer Container { get { return _container; } }
        public DevExpress.XtraBars.Docking.DockPanel DockPanel { get { return _dockPanel; } }
    }
}
