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
		internal List<DisplayGroup> ChildrenGroup { get; set; }

		internal DisplayGroup()
		{
			ChildrenGroup = new List<DisplayGroup>();
		}
		internal DisplayGroup(string name)
		{
			_name = name;
			ChildrenGroup = new List<DisplayGroup>();
		}

		//internal TreeNode InitGroupTree(View.ProjectTree treeCtrl, TreeNode root)
		//{
		//	TreeNode tn = treeCtrl.FindNode(this);
		//	if (tn != null)	return tn;
		//
		//	if (ParentGroup == null)
		//	{
		//		tn = treeCtrl.AddNode(this, _name, root);
		//	}
		//	else
		//	{
		//		TreeNode parent = ParentGroup.InitGroupTree(treeCtrl, root);
		//		tn = treeCtrl.AddNode(this, _name, parent);
		//	}
		//	tn.ContextMenuStrip = treeCtrl.MenuGroup;
		//	return tn;
		//}
		internal string Name { get { return _name; } }
	}
}
