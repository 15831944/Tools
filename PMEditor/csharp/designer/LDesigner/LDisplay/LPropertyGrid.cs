using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace LDisplay
{
	internal class LPropertyGrid : PropertyGrid
	{
        private List<IComponent> _ctrls = new List<IComponent>();

		internal LPropertyGrid()
		{ }

        internal void BindObj(IComponent ctrl)
		{
			_ctrls.Clear();
			_ctrls.Add(ctrl);
            PropertyTabs.AddTabType(typeof(EventsTab), PropertyTabScope.Document);
            ShowEventsButton(true);
			this.SelectedObject = ctrl;
		}

        internal void BindObj(List<IComponent> ctrls)
		{
			_ctrls.Clear();
			_ctrls.AddRange(ctrls);
			this.SelectedObjects = ctrls.ToArray();
		}
	}
}
