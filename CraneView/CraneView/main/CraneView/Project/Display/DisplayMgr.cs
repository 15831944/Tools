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

		private void _menuProj_Click(object sender, EventArgs e)
		{
			string str1 = sender.ToString();
			string str2 = e.ToString();
		}

		public DisplayGroup AddGroup(string name, DisplayGroup parent, ref string err)
		{
			foreach (var g in GroupList)
			{
				if (g.ParentGroup == parent)
				{
					if (string.Compare(name, g.Name, true) == 0)
					{
						err = "Name already exist";
						return null;
					}
				}
			}
			DisplayGroup group = new DisplayGroup(this, name);
			_ltGroup.Add(group);
			return group;
		}

		public Display AddDisplay(string name, DisplayGroup parent, ref string err)
		{
			foreach (var d in DisplayList)
			{
				if (d.Group == parent)
				{
					if (string.Compare(name, d.Name, true) == 0)
					{
						err = "Name already exist";
						return null;
					}

				}
			}
			Display display = new Display(this, name, null);
			_ltDisplay.Add(display);
			return display;
		}

		public List<DisplayGroup> GroupList { get { return _ltGroup; } }
		public List<Display> DisplayList { get { return _ltDisplay; } }
	}
}
