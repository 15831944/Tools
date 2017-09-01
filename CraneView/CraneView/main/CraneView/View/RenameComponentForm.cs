using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CraneView
{
    public partial class RenameComponentForm : DevExpress.XtraEditors.XtraForm
    {
        private string _curName = "";
        private string _newName = "";

        public RenameComponentForm(string curName)
        {
            InitializeComponent();
            _curName = curName;
            _txtCurName.Text = curName;
        }

        private void RenameComponentForm_Load(object sender, EventArgs e)
        {
            _txtNewName.Text = _txtCurName.Text;
            _txtNewName.Focus();
            _txtNewName.SelectAll();
        }

        private void _btOK_Click(object sender, EventArgs e)
        {
            string strName = _txtNewName.Text.Trim();
            string strError = "";
            if (!CraneTool.FileHelper.FileNameRight(strName, ref strError))
            {
                MessageBox.Show(this, strError, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                _txtNewName.Focus();
                _txtNewName.SelectAll();
                return;
            }
            _newName = strName;
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
        }

        public string NewName { get { return _newName; } }
    }
}
