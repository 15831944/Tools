using System;
using System.Collections.Generic;

namespace UView.View.Panel
{
    internal class PanelMgr
    {
        private List<View.Panel.PanelControl> _ltPanel = new List<View.Panel.PanelControl>();

        internal List<View.Panel.PanelControl> LtPanel { get { return _ltPanel; } }

        private delegate void AddPanel(PanelControl p);

        internal void CreatePanels(MainEditor editor)
        {
            List<PanelControl> lefts = new List<PanelControl>();
            List<PanelControl> rights = new List<PanelControl>();
            List<PanelControl> tops = new List<PanelControl>();
            List<PanelControl> buttoms = new List<PanelControl>();

            AddPanel add_panel = p => { p.InitializeComponent(editor); _ltPanel.Add(p); };
            add_panel(new PanelSolution());
            add_panel(new PanelProperty());
            add_panel(new PanelOutput());
            add_panel(new PanelFind());

            foreach (var ctrl in _ltPanel)
            {
                switch (ctrl.DockPanel.Dock)
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
                editor.Controls.Add(panels[0].DockPanel);
                editor.DockMgr.RootPanels.AddRange(new DevExpress.XtraBars.Docking.DockPanel[] {
            panels[0].DockPanel});
                ((System.ComponentModel.ISupportInitialize)(editor.DockMgr)).EndInit();
            }
            else
            {
                DevExpress.XtraBars.Docking.DockPanel dockPanel = new DevExpress.XtraBars.Docking.DockPanel();
                ((System.ComponentModel.ISupportInitialize)(editor.DockMgr)).BeginInit();
                dockPanel.SuspendLayout();

                foreach (var panel in panels)
                {
                    dockPanel.Controls.Add(panel.DockPanel);
                }
                dockPanel.ActiveChild = panels[0].DockPanel;
                dockPanel.Dock = panels[0].DockPanel.Dock;
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

        internal View.Panel.PanelControl GetPanel(string name)
        {
            return LtPanel.Find(x => (string.Compare(x.DockPanel.Text, name, true) == 0));
        }
    }
}
