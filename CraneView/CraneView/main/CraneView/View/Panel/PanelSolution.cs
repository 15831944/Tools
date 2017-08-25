using System;
using System.Collections.Generic;
using System.Text;

namespace CraneView.View.Panel
{
	internal class PanelSolution : PanelControl
	{
		private MainEditor _editor;
		private CraneView.Project.View.ProjectTree _tree = new CraneView.Project.View.ProjectTree();

		internal PanelSolution()
		{ }

		internal override void InitializeComponent(MainEditor editor)
		{
			_editor = editor;

			// _tree
			this._tree.AllowDrop = true;
			this._tree.Dock = System.Windows.Forms.DockStyle.Fill;
			this._tree.HideSelection = false;
			this._tree.LineColor = System.Drawing.Color.Empty;
			this._tree.Location = new System.Drawing.Point(0, 0);
			this._tree.Name = "_tree";
			this._tree.Size = new System.Drawing.Size(228, 277);
			this._tree.TabIndex = 1;
			InitializeCtrl(this._tree, "Solution", DevExpress.XtraBars.Docking.DockingStyle.Left);
		}

		public CraneView.Project.View.ProjectTree SlnTree { get { return _tree; } }
	}
}
