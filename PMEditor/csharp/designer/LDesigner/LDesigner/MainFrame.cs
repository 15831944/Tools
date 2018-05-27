using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LDesigner
{
	public partial class MainFrame : Form
	{
		public MainFrame()
		{
			InitializeComponent();
		}

		private void _btTestRun_Click(object sender, EventArgs e)
		{

		}

		private void AddControl(Type t, int count)
		{
			if (count < 1) return;
            List<IComponent> ctrls = new List<IComponent>();
			for (int i = 0; i < count; i++)
			{
				var obj = t.Assembly.CreateInstance(t.ToString()) as IComponent;
				if (obj == null) return;
				string tn = t.ToString();
				int nidx = tn.LastIndexOf('.');
				if (nidx > 0)
					tn = tn.Substring(nidx + 1);

                if (obj is Control)
                {
                    var ctrl = obj as Control;
                    ctrl.Name = _designer.GetNewControlName(tn);
                    ctrl.Text = ctrl.Name;
                    ctrl.Size = new Size(100, 40);
                }
                ctrls.Add(obj);
			}
			_designer.AddControl(ctrls);
		}

		private void _btButton_Click(object sender, EventArgs e)
		{
			AddControl(typeof(Button), 1);
		}

		private void _btTextBox_Click(object sender, EventArgs e)
		{
			AddControl(typeof(TextBox), 1);
		}

		private void _btCheckBox_Click(object sender, EventArgs e)
		{
			AddControl(typeof(CheckBox), 1);
		}

		private void _btRadioButton_Click(object sender, EventArgs e)
		{
			AddControl(typeof(RadioButton), 3);
		}

		private void _btComboBox_Click(object sender, EventArgs e)
		{
			AddControl(typeof(ComboBox), 1);
		}

		private void _btPicture_Click(object sender, EventArgs e)
		{
			AddControl(typeof(PictureBox), 1);
		}

        private void _btGrid_Click(object sender, EventArgs e)
        {
            AddControl(typeof(MyControls.Grid.DGridView), 1);
        }

		private void _designer_Load(object sender, EventArgs e)
		{

		}
	}
}
