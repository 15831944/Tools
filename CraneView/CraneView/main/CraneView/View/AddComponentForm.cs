using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CraneView.View
{
	public partial class AddComponentForm : Form
	{
		public AddComponentForm()
		{
			InitializeComponent();
		}

		private void _btOK_Click(object sender, EventArgs e)
		{
			string strName = _txtName.Text.Trim();
			string strError = "";
			if (!CraneTool.FileHelper.FileNameRight(strName, ref strError))
			{
				MessageBox.Show(this, strError, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				_txtName.Focus();
				_txtName.SelectAll();
				return;
			}
			NewName = strName;
			this.DialogResult = System.Windows.Forms.DialogResult.OK;
		}

		private void _btCancel_Click(object sender, EventArgs e)
		{
			this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
		}

		public string NewName { get; set; }
	}
}
