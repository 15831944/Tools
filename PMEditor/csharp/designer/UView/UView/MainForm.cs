using System;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Design;

using Designer;
using System.ComponentModel.Design;
using DesignUI.Service;
using System.ComponentModel;

namespace UView
{
    public partial class MainForm : Form
    {
        private string _version = string.Empty;
        private IUDesigner _idesigner = new UDesigner();

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

        public IUDesigner Designer { get { return _idesigner; } }

        #region Init
        //- ctor
        public MainForm()
        {
            InitializeComponent();

            //- the control: (UDesigner)_designer 
            (_idesigner as UDesigner).Parent = pnl4pDesigner;

            //- Add the toolboxItems to the future toolbox 
            //- the pointer
            ToolboxItem toolPointer = new System.Drawing.Design.ToolboxItem();
            toolPointer.DisplayName = "<Pointer>";
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

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.UndoOnDesignSurface();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.RedoOnDesignSurface();
        }

        private void OnMenuClick(object sender, EventArgs e)
        {
            string cmd = (sender as ToolStripMenuItem).Text;
            if (cmd == "Cut")               _idesigner.CutOnDesignSurface();
            else if (cmd == "Copy")         _idesigner.CopyOnDesignSurface();
            else if (cmd == "Paste")        _idesigner.PasteOnDesignSurface();
            else if (cmd == "Delete")       _idesigner.DeleteOnDesignSurface();
        }

        private void toolStripMenuItemTabOrder_Click(object sender, EventArgs e)
        {
            _idesigner.SwitchTabOrder();
        }

        private void OnAbout(object sender, EventArgs e)
        {
            MessageBox.Show("UViewDesigner.\r\nVersion: " + Version, "UView Designer", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        #endregion

        private void displayToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.AddDesignSurface<Display>(640, 480, AlignmentModeEnum.SnapLines, new Size(1, 1));
        }

        private void formToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.AddDesignSurface<Form>(640, 480, AlignmentModeEnum.SnapLines, new Size(1, 1));
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            _idesigner.AddDesignSurface<Display>(640, 480, AlignmentModeEnum.SnapLines, new Size(1, 1));
        }

        private void useGridToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.AddDesignSurface<Display>(640, 480, AlignmentModeEnum.Grid, new Size(8, 8));
        }

        private void gridNoneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.SetGrid(new Size(0, 0));
        }

        private void x1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.SetGrid(new Size(1, 1));
        }

        private void x4ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.SetGrid(new Size(4, 4));
        }

        private void x8ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.SetGrid(new Size(8, 8));
        }

        private void x16ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.SetGrid(new Size(16, 16));
        }

        private void x32ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _idesigner.SetGrid(new Size(32, 32));
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
                var undoMgr = surface.GetUndoService();
                ToolStripMenuItemUnDo.Enabled = undoMgr.EnableUndo;
                ToolStripMenuItemReDo.Enabled = undoMgr.EnableRedo;

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