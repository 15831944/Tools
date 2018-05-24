using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LDisplay
{
	internal class LMethodGrid : DataGridView
	{
		private List<Control> _ctrls = new List<Control>();

		internal LMethodGrid()
		{ }

		internal void BindObj(Control ctrl)
		{
			_ctrls.Clear();
			_ctrls.Add(ctrl);
			//this.SelectedObject = ctrl;
		}
		internal void BindObj(List<Control> ctrls)
		{
			_ctrls.Clear();
			_ctrls.AddRange(ctrls);
			//this.SelectedObjects = ctrls.ToArray();
		}
	}
}
