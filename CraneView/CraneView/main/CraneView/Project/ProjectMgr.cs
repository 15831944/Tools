using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CraneView.Project
{
	internal class ProjectMgr
	{
		internal List<Project> _projList = new List<Project>();

		internal MainEditor Editor { get; set; }

		public ProjectMgr()
		{ }

		internal Project FirstProject()
		{
			if (_projList.Count == 0) _projList.Add(new Project());
			return _projList[0];
		}
	}
}
