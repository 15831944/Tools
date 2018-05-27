using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LDisplay
{
	internal class LMethodGrid : DataGridView
	{
        private List<IComponent> _ctrls = new List<IComponent>();

		internal LMethodGrid()
		{ }

        internal void BindObj(IComponent ctrl)
		{
			_ctrls.Clear();
			_ctrls.Add(ctrl);
			//this.SelectedObject = ctrl;
		}
        internal void BindObj(List<IComponent> ctrls)
		{
			_ctrls.Clear();
			_ctrls.AddRange(ctrls);
			//this.SelectedObjects = ctrls.ToArray();
		}
	}
}
