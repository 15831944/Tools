using System;
using System.Linq;
using System.Windows.Forms;

namespace UView.View
{
    public partial class SolutionDialog : DevExpress.XtraEditors.XtraForm
    {
        private MainEditor _editor;

        public SolutionDialog(MainEditor editor)
        {
            _editor = editor;
            InitializeComponent();
        }

        private void SolutionCreater_Load(object sender, EventArgs e)
        {
            ((System.ComponentModel.ISupportInitialize)(this._dgProj)).BeginInit();
            this._dgProj.Rows.Clear();
            if (_editor.SolutionMgr.SolutionHeadList.Count > 0)
                this._dgProj.Rows.Add(_editor.SolutionMgr.SolutionHeadList.Count);
            int index = 0;
            foreach (var proj in _editor.SolutionMgr.SolutionHeadList)
            {
                this._dgProj.Rows[index++].Tag = proj;
            }
            ((System.ComponentModel.ISupportInitialize)(this._dgProj)).EndInit();

            _pathBox.Text = UTools.FileHelper.GetSpecialPath(Environment.SpecialFolder.Personal) + "\\UView\\Solution";
        }

        private void _btOK_Click(object sender, EventArgs e)
        {
            if (this._tabOpen.Visible)
            {
                if (this._dgProj.SelectedRows.Count > 0)
                {
                    _editor.SolutionMgr.LoadSolution((UCore.HeadInfo)this._dgProj.SelectedRows[0].Tag);
                    this.DialogResult = System.Windows.Forms.DialogResult.OK;
                }
            }
            else if (this._tabCreate.Visible)
            {
                string strName = _nameBox.Text.Trim();
                string strPath = _pathBox.Text.Trim();
                string strError = "";
                if (!UTools.FileHelper.FileNameRight(strName, ref strError))
                {
                    MessageBox.Show(this, strError, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    _nameBox.Focus();
                    _nameBox.SelectAll();
                    return;
                }
                if (string.IsNullOrEmpty(strPath))
                {
                    MessageBox.Show(this, "Path is wrong.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    _pathBox.Focus();
                    _pathBox.SelectAll();
                    return;
                }
                if (_editor.SolutionMgr.IfSolutionExist(strName))
                {
                    MessageBox.Show(this, "Project already exist.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    _nameBox.Focus();
                    _nameBox.SelectAll();
                    return;
                }
                strPath = strPath + "\\" + strName + "\\";
                if (!UTools.FileHelper.CreatePath(strPath))
                {
                    MessageBox.Show(this, "Create path failed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    _pathBox.Focus();
                    _pathBox.SelectAll();
                    return;
                }
                _editor.SolutionMgr.CreateSolution(strName, strPath, _commentBox.Text);
                this.DialogResult = System.Windows.Forms.DialogResult.OK;
            }
        }

        private void _dgProj_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            var row = this._dgProj.Rows[e.RowIndex];
            UCore.HeadInfo hi = (UCore.HeadInfo)row.Tag;

            string name = this._dgProj.Columns[e.ColumnIndex].Name;

            if (string.Compare(name, "_colNo", true) == 0)
            {
                e.Value = hi.ID.ToString();
            }
            else if (string.Compare(name, "_colName", true) == 0)
            {
                e.Value = hi.Name;
            }
            else if (string.Compare(name, "_colPath", true) == 0)
            {
                e.Value = hi.Path;
            }
            else if (string.Compare(name, "_colTime", true) == 0)
            {
                e.Value = hi.Time.ToLocalTime().ToString();
            }
            else if (string.Compare(name, "_colComment", true) == 0)
            {
                e.Value = hi.Comment;
            }
        }

        private void _dgProj_DoubleClick(object sender, EventArgs e)
        {
            _btOK_Click(sender, e);
        }

        private void _btPath_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderDlg = new FolderBrowserDialog();
            folderDlg.SelectedPath = _pathBox.Text;
            if (folderDlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                _pathBox.Text = folderDlg.SelectedPath;
            }
        }
    }
}
