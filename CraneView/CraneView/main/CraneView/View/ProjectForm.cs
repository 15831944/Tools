using CraneTool;
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
	public partial class ProjectForm : DevExpress.XtraEditors.XtraForm
	{
		private MainEditor _editor;
		public ProjectForm(MainEditor editor)
		{
			_editor = editor;
			InitializeComponent();
		}

		private void ProjectCreater_Load(object sender, EventArgs e)
		{
			((System.ComponentModel.ISupportInitialize)(this._dgProj)).BeginInit();
			this._dgProj.Rows.Clear();
			this._dgProj.Rows.Add(_editor.ProjMgr.ProjHeadList.Count);
			int index = 0;
			foreach (var proj in _editor.ProjMgr.ProjHeadList)
			{
				this._dgProj.Rows[index++].Tag = proj;
			}
			((System.ComponentModel.ISupportInitialize)(this._dgProj)).EndInit();

			_pathBox.Text = Environment.GetFolderPath(Environment.SpecialFolder.Personal) + "\\Crane";
		}

		private void _btOK_Click(object sender, EventArgs e)
		{
			if (this._tabOpen.Visible)
			{
				if (this._dgProj.SelectedRows.Count > 0)
				{
					_editor.ProjMgr.LoadProj((Project.ProjectHeadInfo)this._dgProj.SelectedRows[0].Tag);
					this.DialogResult = System.Windows.Forms.DialogResult.OK;
				}
			}
			else if (this._tabCreate.Visible)
			{
				string strName = _nameBox.Text.Trim();
				string strPath = _pathBox.Text.Trim();
				if (!FileHelper.FileNameRight(strName))
				{
					MessageBox.Show(this, FileHelper.ErrorMessage, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					_nameBox.Focus();
					return;
				}
				if (string.IsNullOrEmpty(strPath))
				{
					MessageBox.Show(this, "Path is wrong.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					_pathBox.Focus();
					return;
				}
				if (_editor.ProjMgr.IfProjExist(strName))
				{
					MessageBox.Show(this, "Project already exist.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					_nameBox.Focus();
					return;
				}
				strPath = strPath + "\\" + strName;
				if (!CraneTool.FileHelper.CreatePath(strPath))
				{
					MessageBox.Show(this, "Create path failed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					_pathBox.Focus();
					return;
				}
				_editor.ProjMgr.CreateProj(strName, strPath,_commentBox.Text);
				this.DialogResult = System.Windows.Forms.DialogResult.OK;
			}
		}

		private void _dgProj_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
		{
			var row = this._dgProj.Rows[e.RowIndex];
			Project.ProjectHeadInfo phi = (Project.ProjectHeadInfo)row.Tag;

			string name = this._dgProj.Columns[e.ColumnIndex].Name;

			if (string.Compare(name, "_colNo", true) == 0)
			{
				e.Value = phi.ID.ToString();
			}
			else if (string.Compare(name, "_colName", true) == 0)
			{
				e.Value = phi.Name;
			}
			else if (string.Compare(name, "_colTime", true) == 0)
			{
				e.Value = phi.Time.ToLocalTime().ToString();
			}
			else if (string.Compare(name, "_colComment", true) == 0)
			{
				e.Value = phi.Comment;
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
