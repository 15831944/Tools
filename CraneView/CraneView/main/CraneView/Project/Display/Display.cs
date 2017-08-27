using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CraneView.Project.Display
{
	internal class Display
	{
		private Guid _id;
		private string _name;
		private DisplayMgr _displayMgr = null;

		internal Display(DisplayMgr mgr, string name, DisplayGroup group)
		{
			_displayMgr = mgr;
			_id = Guid.NewGuid();
			_name = name;
			Group = group;
		}
		internal Display(DisplayMgr mgr, Guid id, string name, DisplayGroup group)
		{
			_displayMgr = mgr;
			_id = id;
			_name = name;
			Group = group;
		}
		//internal TreeNode InitDisplayTree(View.ProjectTree treeCtrl, TreeNode root)
		//{
		//	TreeNode tn = null;
		//	if (Group == null)
		//	{
		//		tn = treeCtrl.AddNode(this, Name, root);
		//	}
		//	else
		//	{
		//		TreeNode tr = treeCtrl.FindNode(Group);
		//		if (tr != null)
		//			tn = treeCtrl.AddNode(this, Name, tr);
		//	}
		//	return tn;
		//}

		internal Guid ID { get { return _id; } }
		internal string Name { get { return _name; } }
		internal DisplayGroup Group { get; set; }
	}
}
