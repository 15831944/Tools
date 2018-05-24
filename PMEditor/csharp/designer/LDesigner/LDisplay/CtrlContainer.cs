using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LDisplay
{
	internal class CtrlContainer
	{
		private Control _control = null;
		internal CtrlContainer(Control ctrl)
		{
			_control = ctrl;
		}
		internal string ControlInfoName
		{
			get { return _control.GetType().ToString() + " " + _control.Name; }
		}
		internal Control Ctrl { get { return _control; } }
	}
}
