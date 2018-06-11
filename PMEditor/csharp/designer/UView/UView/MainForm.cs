using System;
using System.Windows.Forms;
using System.Drawing.Design;
using System.ComponentModel.Design;

using USolution;
using DesignUI.Service;

namespace UView
{
    public partial class MainForm : Form
    {
        private string _version = string.Empty;
        private Solution _solution;
        private DesignUI.Interface.IUDesigner _idesigner = new DesignUI.View.UDesigner();

        public string Version
        {
            get
            {
                if (string.IsNullOrEmpty(_version))
                {
                    //- Get the actual version of the file hosted in running assembly
                    System.Diagnostics.FileVersionInfo FVI = System.Diagnostics.FileVersionInfo.GetVersionInfo(System.Reflection.Assembly.GetExecutingAssembly().Location);
                    _version = FVI.ProductVersion;
                }
                return _version;
            }
        }

        public DesignUI.Interface.IUDesigner Designer { get { return _idesigner; } }

        #region Init
        //- ctor
        public MainForm()
        {
            InitializeComponent();
            InitMenuText();

            //- the control: (UDesigner)_designer 
            (_idesigner as DesignUI.View.UDesigner).Parent = designerPanel;

            InitToolBox();
        }

        private void InitMenuText()
        {
            ToolStripMenuItemCut.Text = UResource.Properties.Resource.EDIT_CUT;
            ToolStripMenuItemCopy.Text = UResource.Properties.Resource.EDIT_COPY;
            ToolStripMenuItemPaste.Text = UResource.Properties.Resource.EDIT_PASTE;
            ToolStripMenuItemDelete.Text = UResource.Properties.Resource.EDIT_DELETE;
            ToolStripMenuItemSelectAll.Text = UResource.Properties.Resource.EDIT_SELECTALL;
            ToolStripMenuItemUnDo.Text = UResource.Properties.Resource.EDIT_UNDO;
            ToolStripMenuItemReDo.Text = UResource.Properties.Resource.EDIT_REDO;
        }

        private void InitToolBox()
        {
            //- Add the toolboxItems to the future toolbox 
            //- the pointer
            ToolboxItem toolPointer = new System.Drawing.Design.ToolboxItem();
            toolPointer.DisplayName = $"<Pointer>";
            toolPointer.Bitmap = new System.Drawing.Bitmap(16, 16);
            _ctrlToolBox.Items.Add(toolPointer);
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(Button)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(CheckBox)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(ComboBox)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(ContextMenuStrip)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(DataGridView)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(GroupBox)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(ImageList)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(Label)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(LinkLabel)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(ListView)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(OpenFileDialog)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(Panel)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(ProgressBar)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(MenuStrip)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(RadioButton)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(StatusBar)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(TabControl)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(TextBox)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(Timer)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(ToolBar)));
            _ctrlToolBox.Items.Add(new ToolboxItem(typeof(TreeView)));

            //- init the (UDesigner)pDesignerCore 
            _idesigner.Toolbox = _ctrlToolBox;
        }

        private void pDesignerMainForm_Load(object sender, EventArgs e)
        {
        }
        #endregion

        #region Menu commands
        private void deleteFormToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.RemoveDesignSurface(_idesigner.ActiveDesignSurface);
        }

        private void OnMenuClick(object sender, EventArgs e)
        {
            string cmd = (sender as ToolStripMenuItem).Text;
            if (cmd == UResource.Properties.Resource.EDIT_CUT) _idesigner.ActionCommandOnDesignSurface(StandardCommands.Cut);
            else if (cmd == UResource.Properties.Resource.EDIT_COPY) _idesigner.ActionCommandOnDesignSurface(StandardCommands.Copy);
            else if (cmd == UResource.Properties.Resource.EDIT_PASTE) _idesigner.ActionCommandOnDesignSurface(StandardCommands.Paste);
            else if (cmd == UResource.Properties.Resource.EDIT_DELETE) _idesigner.ActionCommandOnDesignSurface(StandardCommands.Delete);
            else if (cmd == UResource.Properties.Resource.EDIT_SELECTALL) _idesigner.ActionCommandOnDesignSurface(StandardCommands.SelectAll);
            else if (cmd == UResource.Properties.Resource.EDIT_UNDO) _idesigner.ActionCommandOnDesignSurface(StandardCommands.Undo);
            else if (cmd == UResource.Properties.Resource.EDIT_REDO) _idesigner.ActionCommandOnDesignSurface(StandardCommands.Redo);
        }

        private void toolStripMenuItemTabOrder_Click(object sender, EventArgs e)
        {
            _idesigner.SwitchTabOrder();
        }

        private void OnAbout(object sender, EventArgs e)
        {
            MessageBox.Show($"UViewDesigner.\r\nVersion: " + Version, $"UView Designer", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        #endregion

        private void displayToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.AddDesignSurface<DesignUI.View.Display>(480, 320, DesignUI.Interface.AlignmentModeEnum.SnapLines);
        }

        private void formToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.AddDesignSurface<DesignUI.View.Dialog> (480, 320, DesignUI.Interface.AlignmentModeEnum.SnapLines);
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            _idesigner.AddDesignSurface<DesignUI.View.Display>(480, 320, DesignUI.Interface.AlignmentModeEnum.SnapLines);
        }

        private void useGridToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.AddDesignSurface<DesignUI.View.Display>(480, 320, DesignUI.Interface.AlignmentModeEnum.Grid);
        }

        private void editToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            CheckMenuItemEnable_Edit();
        }

        private void CheckMenuItemEnable_Edit()
        {
            ToolStripMenuItemUnDo.Enabled = false;
            ToolStripMenuItemReDo.Enabled = false;
            ToolStripMenuItemCut.Enabled = false;
            ToolStripMenuItemCopy.Enabled = false;
            ToolStripMenuItemPaste.Enabled = false;
            ToolStripMenuItemDelete.Enabled = false;

            var surface = _idesigner.ActiveDesignSurface;
            if (surface != null)
            {
                var undoMgr = surface.GetService(typeof(UndoEngine)) as UndoServiceImpl;
                if (undoMgr != null)
                {
                    ToolStripMenuItemUnDo.Enabled = !undoMgr.UndoEmpty;
                    ToolStripMenuItemReDo.Enabled = !undoMgr.RedoEmpty;
                }

                var selectionService = (ISelectionService)(surface.GetService(typeof(ISelectionService)));
                if (selectionService != null && selectionService.SelectionCount > 0)
                {
                    if (selectionService.SelectionCount > 1 ||
                        (!(selectionService.PrimarySelection is UserControl) &&
                         !(selectionService.PrimarySelection is Form)))
                    {
                        ToolStripMenuItemCopy.Enabled = true;
                        ToolStripMenuItemCut.Enabled = true;
                        ToolStripMenuItemDelete.Enabled = true;
                    }
                }
                ToolStripMenuItemPaste.Enabled = true;
            }
        }

        private void solutionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_solution == null)
            {
                CreateSolutionDialog dlg = new CreateSolutionDialog();
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    _solution = Solution.CreateSolution(dlg.FileName, dlg.FilePath);
                }
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.SaveCurrent();
        }

        private void saveAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.SaveAll();
        }

        private void openStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog fd = new OpenFileDialog();
            fd.InitialDirectory = "D:\\";
            fd.Filter = $"Display file(*.disp)|*.disp";
            if (fd.ShowDialog() == DialogResult.OK)
            {
                var fName = fd.FileName;
                _idesigner.Open(fName);
            }
        }
        //private void useGridWithoutSnappingToolStripMenuItem_Click(object sender, EventArgs e)
        //{
        //    _idesigner.AddDesignSurface<Display>(640, 480, AlignmentModeEnum.GridWithoutSnapping, new Size(8, 8));
        //}
        //
        //private void useNoGuidesToolStripMenuItem_Click(object sender, EventArgs e)
        //{
        //    _idesigner.AddDesignSurface<Form>(640, 480, AlignmentModeEnum.NoGuides, new Size(1, 1));
        //}
    }
}