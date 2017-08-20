using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CraneView.Project
{
	internal class Project
	{
		private ProjectHeadInfo _projInfo;
		private string _path = "";

		public ProjectHeadInfo ProjInfo { get { return _projInfo; } }
		public string Name { get { return _projInfo.Name; } }
		public string Path { get { return _path; } }
		public Project(ProjectHeadInfo projInfo)
		{
			_projInfo = projInfo;
		}
	}
}
