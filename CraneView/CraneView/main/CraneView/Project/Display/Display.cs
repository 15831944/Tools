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
		internal DisplayGroup Group { get; set; }
		internal string Name { get; set; }

		internal Display()
		{ }
		internal TreeNode InitDisplayTree(View.ProjectTree treeCtrl, TreeNode root)
		{
			TreeNode tn = null;
			if (Group == null)
			{
				tn = treeCtrl.AddNode(this, Name, root);
			}
			else
			{
				TreeNode tr = treeCtrl.FindNode(Group);
				if (tr != null)
					tn = treeCtrl.AddNode(this, Name, tr);
			}
			return tn;
		}
	}
}
