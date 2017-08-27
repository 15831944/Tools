using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CraneView.Project;
using CraneView.Project.Display;

namespace CraneView.Project.View
{
	internal partial class ProjectTree : CraneTool.TreeCtrl
	{
		private ProjectMgr _projMgr = null;

		internal ProjectTree()
		{
			InitializeComponent();

		}

		internal void InitProjTree(ProjectMgr projMgr)
		{
			_projMgr = projMgr;

			// Projects
			TreeNode root = AddNode(projMgr, "Projects", null);
			foreach(var proj in projMgr.ProjList)
			{
				TreeNode projNode = AddNode(proj, proj.Name, root);
				TreeNode displayMgrNode = AddNode(proj.DisplayMgr, "Displays", projNode);
				displayMgrNode.ContextMenuStrip = _menuProj;

				foreach(var group in proj.DisplayMgr.GroupList)
				{
					if (group.ParentGroup == null)
					{
						TreeNode groupNode = AddNode(group, group.Name, displayMgrNode);
						groupNode.ContextMenuStrip = _menuGroup;
						InitGroupTree(group, groupNode);
					}
				}
				foreach (var display in proj.DisplayMgr.DisplayList)
				{
					if (display.Group == null)
					{
						TreeNode dispNode = AddNode(display, display.Name, displayMgrNode);
						dispNode.ContextMenuStrip = _menuDisplay;
					}
					else
					{
						TreeNode dispNode = AddNode(display, display.Name, display.Group);
						dispNode.ContextMenuStrip = _menuDisplay;
					}
				}
			}
			root.ExpandAll();
		}

		private void InitGroupTree(DisplayGroup group, TreeNode groupNode)
		{
			if (group.ChildrenGroup.Count <= 0) return;
			foreach (var subGroup in group.ChildrenGroup)
			{
				TreeNode subGroupNode = AddNode(subGroup, subGroup.Name, groupNode);
				InitGroupTree(subGroup, subGroupNode);
			}
		}

		private void OnAddGroup(object sender, EventArgs e)
		{
			CraneView.View.AddComponentForm form = new CraneView.View.AddComponentForm();
			if (form.ShowDialog() == DialogResult.OK)
			{
				string name = form.NewName;
				string err = "";
				var group = _projMgr.FirstProject.DisplayMgr.AddGroup(name, null, ref err);
				if (group == null)
				{
					MessageBox.Show("Add group failed!\r\n"+err, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}

		private void OnAddDisplay(object sender, EventArgs e)
		{
			CraneView.View.AddComponentForm form = new CraneView.View.AddComponentForm();
			if (form.ShowDialog() == DialogResult.OK)
			{
				string name = form.NewName;
				string err = "";
				var display = _projMgr.FirstProject.DisplayMgr.AddDisplay(name, null, ref err);
				if (display == null)
				{
					MessageBox.Show("Add display failed!\r\n" + err, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}
		private void OnAddSubGroup(object sender, EventArgs e){}
		private void OnRenameGroup(object sender, EventArgs e){}
		private void OnDeleteGroup(object sender, EventArgs e){}
		private void OnAddSubDisplay(object sender, EventArgs e){}
		private void OnRenameDisplay(object sender, EventArgs e){}
		private void OnDeleteDisplay(object sender, EventArgs e) { }
	}
}
