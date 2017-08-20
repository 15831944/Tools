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

		internal void InitDisplayTree(CraneTool.TreeCtrl treeCtrl, TreeNode root)
		{
			TreeNode node = treeCtrl.AddNode(this, "Displays", root);
			foreach (var g in _ltGroup)
				g.InitGroupTree(treeCtrl, node);
		}
	}
}
