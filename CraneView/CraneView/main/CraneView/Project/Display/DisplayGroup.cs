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
		private Guid _id;
		private string _name = null;
		private DisplayMgr _displayMgr = null;

		internal DisplayGroup(DisplayMgr mgr, string name)
		{
			_displayMgr = mgr;
			_id = Guid.NewGuid();
			_name = name;
			ChildrenGroup = new List<DisplayGroup>();
		}
		internal DisplayGroup(DisplayMgr mgr, Guid id, string name)
		{
			_displayMgr = mgr;
			_id = id;
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
		internal Guid ID { get { return _id; } }
		internal string Name { get { return _name; } }
		internal DisplayGroup ParentGroup { get; set; }
		internal List<DisplayGroup> ChildrenGroup { get; set; }
	}
}
