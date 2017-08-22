using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CraneView.Project.Display
{
	internal class DisplayMgr
	{
		private List<DisplayGroup> _ltGroup = new List<DisplayGroup>();
		private List<Display> _ltDisplay = new List<Display>();
		private Project _project;

		internal DisplayMgr(Project proj)
		{
			_project = proj;
		}

		//internal void InitDisplayTree(View.ProjectTree treeCtrl, TreeNode root)
		//{
		//	TreeNode node = treeCtrl.AddNode(this, "Displays", root);
		//	node.ContextMenuStrip = treeCtrl.MenuProj;
		//	node.ContextMenuStrip.Click += new System.EventHandler(this._menuProj_Click);
		//
		//	foreach (var g in _ltGroup)
		//		g.InitGroupTree(treeCtrl, node);
		//	foreach (var d in _ltDisplay)
		//		d.InitDisplayTree(treeCtrl, node);
		//}

		private void _menuProj_Click(object sender, EventArgs e)
		{
			string str1 = sender.ToString();
			string str2 = e.ToString();
		}

		public DisplayGroup AddGroup(string name)
		{
			DisplayGroup group = new DisplayGroup(name);
			_ltGroup.Add(group);
			return group;
		}

		public List<DisplayGroup> GroupList { get { return _ltGroup; } }
		public List<Display> DisplayList { get { return _ltDisplay; } }
	}
}
