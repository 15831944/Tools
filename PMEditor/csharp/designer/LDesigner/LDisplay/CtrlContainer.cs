using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LDisplay
{
	internal class CtrlContainer
	{
		private IComponent _control = null;
        private static int _sIndex = 0;
        private static int _index = 0;
		internal CtrlContainer(IComponent ctrl)
		{
			_control = ctrl;
            if (!(ctrl is Control))
            {
                _index = _sIndex;
                _sIndex++;
            }
		}
		internal string ControlInfoName
		{
			get { 
                if (_control is Control)
                    return _control.GetType().ToString() + " " + (_control as Control).Name;
                return _control.GetType().ToString() + " Component" + _index.ToString();
            }
		}
		internal IComponent Ctrl { get { return _control; } }
	}
}
