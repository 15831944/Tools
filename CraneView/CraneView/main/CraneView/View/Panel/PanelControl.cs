using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CraneView.View.Panel
{
	internal class PanelControl
	{
		private DevExpress.XtraBars.Docking.ControlContainer _container = new DevExpress.XtraBars.Docking.ControlContainer();
		private DevExpress.XtraBars.Docking.DockPanel _dockPanel = new DevExpress.XtraBars.Docking.DockPanel();
		internal PanelControl() { }
		internal static void CreatePanels(MainEditor editor)
		{
			List<PanelControl> lefts = new List<PanelControl>();
			List<PanelControl> rights = new List<PanelControl>();
			List<PanelControl> tops = new List<PanelControl>();
			List<PanelControl> buttoms = new List<PanelControl>();

			PanelSolution sln = new PanelSolution();
			sln.InitializeComponent(editor);
			editor.LtPanel.Add(sln);

			PanelProperty property = new PanelProperty();
			property.InitializeComponent(editor);
			editor.LtPanel.Add(property);

			PanelOutput output = new PanelOutput();
			output.InitializeComponent(editor);
			editor.LtPanel.Add(output);

			PanelFind find = new PanelFind();
			find.InitializeComponent(editor);
			editor.LtPanel.Add(find);

			foreach (var ctrl in editor.LtPanel)
			{
				switch (ctrl._dockPanel.Dock)
				{
					case DevExpress.XtraBars.Docking.DockingStyle.Left:
						lefts.Add(ctrl);
						break;

					case DevExpress.XtraBars.Docking.DockingStyle.Right:
						rights.Add(ctrl);
						break;

					case DevExpress.XtraBars.Docking.DockingStyle.Top:
						tops.Add(ctrl);
						break;

					case DevExpress.XtraBars.Docking.DockingStyle.Bottom:
						buttoms.Add(ctrl);
						break;
				}
			}

			InsertPanels(editor, lefts);
			InsertPanels(editor, rights);
			InsertPanels(editor, tops);
			InsertPanels(editor, buttoms);
		}

		private static void InsertPanels(MainEditor editor, List<PanelControl> panels)
		{
			if (panels.Count <= 0) return;
			if (panels.Count == 1)
			{
				((System.ComponentModel.ISupportInitialize)(editor.DockMgr)).BeginInit();
				editor.Controls.Add(panels[0]._dockPanel);
				editor.DockMgr.RootPanels.AddRange(new DevExpress.XtraBars.Docking.DockPanel[] {
            panels[0]._dockPanel});
				((System.ComponentModel.ISupportInitialize)(editor.DockMgr)).EndInit();
			}
			else
			{
				DevExpress.XtraBars.Docking.DockPanel dockPanel = new DevExpress.XtraBars.Docking.DockPanel();
				((System.ComponentModel.ISupportInitialize)(editor.DockMgr)).BeginInit();
				dockPanel.SuspendLayout();

				foreach(var panel in panels)
				{
					dockPanel.Controls.Add(panel._dockPanel);
				}
				dockPanel.ActiveChild = panels[0]._dockPanel;
				dockPanel.Dock = panels[0]._dockPanel.Dock;
				dockPanel.ID = Guid.NewGuid();
				dockPanel.Location = new System.Drawing.Point(3, 25);
				dockPanel.Name = "_dockPanel";
				dockPanel.OriginalSize = new System.Drawing.Size(199, 224);
				dockPanel.Size = new System.Drawing.Size(199, 224);
				dockPanel.Tabbed = true;
				dockPanel.Text = "";

				editor.Controls.Add(dockPanel);
				editor.DockMgr.RootPanels.AddRange(new DevExpress.XtraBars.Docking.DockPanel[] {
				dockPanel});

				dockPanel.ResumeLayout(false);
				((System.ComponentModel.ISupportInitialize)(editor.DockMgr)).EndInit();
			}
		}

		internal void InitializeCtrl(Control ctrl, string name, DevExpress.XtraBars.Docking.DockingStyle dock)
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
			this._dockPanel.Name = "_dockPanel";
			this._dockPanel.OriginalSize = new System.Drawing.Size(199, 554);
			this._dockPanel.Size = new System.Drawing.Size(199, 554);
			this._dockPanel.Text = name;
		}
	}
}
