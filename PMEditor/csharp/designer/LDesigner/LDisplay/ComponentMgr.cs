using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LDisplay
{
    internal class CtrlMgr
    {
		private List<CtrlContainer> _ctrlList = new List<CtrlContainer>();
		private List<Component> _componentList = new List<Component>();

		private Dictionary<string, CtrlContainer> _components = new Dictionary<string, CtrlContainer>();

		internal CtrlMgr()
		{ }

		internal string GetNewControlName(string name)
		{
			int index = 0;
			while (true)
			{
				string n = name + index.ToString();
				if (_components.ContainsKey(n)) continue;
				return n;
			}
		}

		internal void AddControl(List<Control> ctrls)
		{
			foreach (var ctrl in ctrls)
			{
				var container = new CtrlContainer(ctrl);
				_ctrlList.Add(container);
				_components.Add(container.ControlInfoName, container);
			}
		}

		internal Control GetCtrl(string info)
		{
			if (_components.ContainsKey(info))
				return _components[info].Ctrl;
			return null;
		}

		internal List<CtrlContainer> CtrlList { get { return _ctrlList; } }
    }
}
