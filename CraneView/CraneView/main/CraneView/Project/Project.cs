using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CraneView.Project
{
	internal class Project
	{
		private string _name = "Empty Project";
		private string _path = "";

		public string Name { get { return _name; } }
		public string Path { get { return _path; } }
		public Project()
		{ }
	}
}
