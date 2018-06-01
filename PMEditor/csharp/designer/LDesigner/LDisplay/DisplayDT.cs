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

		internal void AddControl(List<IComponent> ctrls)
		{
			_ctrlMgr.AddControl(ctrls);
			this.SuspendLayout();
            foreach (var obj in ctrls)
            {
                if (obj is Control)
                    Controls.Add(obj as Control);
            }
			this.ResumeLayout(false);
            IsInDesignMode();
		}

		internal CtrlMgr CtrlMgr { get { return _ctrlMgr; } }

        public static bool IsInDesignMode()
        {
            if (Application.ExecutablePath.IndexOf("devenv.exe", StringComparison.OrdinalIgnoreCase) > -1)
            {
                return true;
            }
            return false;
        }
	}
}
