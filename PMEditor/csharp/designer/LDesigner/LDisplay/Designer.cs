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
	public partial class Designer : UserControl
	{
		public Designer()
		{
			InitializeComponent();
		}

		public void AddControl(List<IComponent> ctrls)
		{
			_display.AddControl(ctrls);
			string selStr = "";
			_cbControlList.Items.Clear();
			foreach (var obj in _display.CtrlMgr.CtrlList)
			{
				_cbControlList.Items.Add(obj.ControlInfoName);
				selStr = obj.ControlInfoName;
			}
			_cbControlList.SelectedIndex = _cbControlList.Items.Count - 1;
		}

		public string GetNewControlName(string name)
		{
			return _display.GetNewControlName(name);
		}

		private void _cbControlList_SelectedIndexChanged(object sender, EventArgs e)
		{
			string str = _cbControlList.SelectedItem.ToString();
			var ctrl = _display.CtrlMgr.GetCtrl(str);
			if (ctrl != null)
			{
				_propGrid.BindObj(ctrl);
				_eventGrid.BindObj(ctrl);
				_methodGrid.BindObj(ctrl);
			}
		}
	}
}
