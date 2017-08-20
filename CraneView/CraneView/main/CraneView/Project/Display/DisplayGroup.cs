using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CraneView.Project.Display
{
	internal class DisplayGroup
	{
		private string _name = null;
		internal DisplayGroup ParentGroup { get; set; }

		internal DisplayGroup()
		{ }

		internal TreeNode InitGroupTree(CraneTool.TreeCtrl treeCtrl, TreeNode root)
		{
			TreeNode tn = treeCtrl.FindNode(this);
			if (tn != null)	return tn;

			if (ParentGroup == null)
				return treeCtrl.AddNode(this, _name, root);

			TreeNode parent = ParentGroup.InitGroupTree(treeCtrl, root);
			return treeCtrl.AddNode(this, _name, parent);
		}
	}
}
