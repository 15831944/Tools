using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UView
{
    public partial class CreateSolutionDialog : Form
    {
        public string FileName { get; set; }
        public string FilePath { get; set; }

        public CreateSolutionDialog()
        {
            InitializeComponent();
            FilePath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "\\UView\\Solution";
        }

        private void CreateSolutionDialog_Load(object sender, EventArgs e)
        {
            if (!string.IsNullOrWhiteSpace(FileName))
                txtName.Text = FileName;
            if (!string.IsNullOrWhiteSpace(FilePath))
                txtPath.Text = FilePath;
        }

        private void btBrowsePath_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if (!string.IsNullOrWhiteSpace(txtPath.Text))
                dlg.SelectedPath = txtPath.Text;
            else
                dlg.RootFolder = Environment.SpecialFolder.MyDocuments;
            dlg.ShowNewFolderButton = true;
            dlg.Description = $"Create Solution";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                txtPath.Text = dlg.SelectedPath;
            }
        }

        private void btOK_Click(object sender, EventArgs e)
        {
            FileName = txtName.Text.Trim();
            FilePath = txtPath.Text.Trim();
            if (!string.IsNullOrEmpty(FilePath) && FilePath.Substring(FilePath.Length - 1) != "\\")
                FilePath += "\\";
            DialogResult = DialogResult.OK;
        }

        private void btCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }
    }
}
