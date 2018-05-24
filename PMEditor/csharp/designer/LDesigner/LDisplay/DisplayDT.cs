using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LDisplay
{
	public partial class DisplayDT : UserControl
	{
		private CtrlMgr _ctrlMgr = new CtrlMgr();

		public DisplayDT()
		{
			InitializeComponent();
		}

		internal string GetNewControlName(string name)
		{
			return _ctrlMgr.GetNewControlName(name);
		}

		internal void AddControl(List<Control> ctrls)
		{
			_ctrlMgr.AddControl(ctrls);
			this.SuspendLayout();
			Controls.AddRange(ctrls.ToArray());
			this.ResumeLayout(false);
		}

		internal CtrlMgr CtrlMgr { get { return _ctrlMgr; } }
	}
}
