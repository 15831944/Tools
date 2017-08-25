using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CraneView.Project
{
	internal class Project
	{
		private ProjectHeadInfo _projInfo;
		private string _path = "";
		private Display.DisplayMgr _displayMgr;

		public Project(ProjectHeadInfo projInfo)
		{
			_projInfo = projInfo;
			_displayMgr = new Display.DisplayMgr(this);
		}

		//internal void InitProjTree(View.ProjectTree treeCtrl, TreeNode root)
		//{
		//	TreeNode node = treeCtrl.AddNode(this, Name, root);
		//	_displayMgr.InitDisplayTree(treeCtrl, node);
		//}

		public ProjectHeadInfo ProjInfo { get { return _projInfo; } }
		public string Name { get { return _projInfo.Name; } }
		public string Path { get { return _path; } }
		public Display.DisplayMgr DisplayMgr { get { return _displayMgr; } }

	}
}
