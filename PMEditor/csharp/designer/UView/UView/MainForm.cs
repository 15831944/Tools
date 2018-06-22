using System;
using System.Windows.Forms;
using System.Drawing.Design;
using System.ComponentModel.Design;

using UCore;
using DesignUI.Service;
using System.Drawing;
using System.ComponentModel;

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

        private Type[] _windowsFormsToolTypes = new Type[] {
                                                      typeof(System.Windows.Forms.Button),
                                                      typeof(System.Windows.Forms.CheckBox),
                                                      typeof(System.Windows.Forms.CheckedListBox),
                                                      typeof(System.Windows.Forms.ColorDialog),
                                                      typeof(System.Windows.Forms.ComboBox),
                                                      typeof(System.Windows.Forms.DataGrid),
                                                      typeof(System.Windows.Forms.DataGridView),
                                                      typeof(System.Windows.Forms.DateTimePicker),
                                                      typeof(System.Windows.Forms.DomainUpDown),
//                                                      typeof(System.Windows.Forms.ErrorProvider),
                                                      typeof(System.Windows.Forms.FontDialog),
                                                      typeof(System.Windows.Forms.GroupBox),
                                                      typeof(System.Windows.Forms.HelpProvider),
                                                      typeof(System.Windows.Forms.HScrollBar),
                                                      typeof(System.Windows.Forms.ImageList),
                                                      typeof(System.Windows.Forms.Label),
                                                      typeof(System.Windows.Forms.LinkLabel),
                                                      typeof(System.Windows.Forms.ListBox),
                                                      typeof(System.Windows.Forms.ListView),
                                                      typeof(System.Windows.Forms.MonthCalendar),
//                                                      typeof(System.Windows.Forms.NotifyIcon),
                                                      typeof(System.Windows.Forms.NumericUpDown),
                                                      typeof(System.Windows.Forms.OpenFileDialog),
                                                      typeof(System.Windows.Forms.PageSetupDialog),
                                                      typeof(System.Windows.Forms.Panel),
                                                      typeof(System.Windows.Forms.PictureBox),
                                                      typeof(System.Windows.Forms.PrintDialog),
                                                      typeof(System.Windows.Forms.PrintPreviewDialog),
                                                      typeof(System.Windows.Forms.PrintPreviewControl),
                                                      typeof(System.Windows.Forms.ProgressBar),
                                                      typeof(System.Windows.Forms.PropertyGrid),
                                                      typeof(System.Windows.Forms.RadioButton),
                                                      typeof(System.Windows.Forms.RichTextBox),
                                                      typeof(System.Windows.Forms.SaveFileDialog),
                                                      typeof(System.Windows.Forms.Splitter),
                                                      typeof(System.Windows.Forms.StatusBar),
                                                      typeof(System.Windows.Forms.TabControl),
                                                      typeof(System.Windows.Forms.TextBox),
                                                      typeof(System.Windows.Forms.Timer),
                                                      typeof(System.Windows.Forms.ToolBar),
                                                      typeof(System.Windows.Forms.ToolTip),
                                                      typeof(System.Windows.Forms.TrackBar),
                                                      typeof(System.Windows.Forms.TreeView),
 //                                                     typeof(System.Windows.Forms.UserControl),
                                                      typeof(System.Windows.Forms.VScrollBar),
                                                      typeof(System.Drawing.Printing.PrintDocument),
                                                  };

        private void InitToolBox()
        {
            //- Add the toolboxItems to the future toolbox 
            //- the pointer
            //ToolboxItem toolPointer = new System.Drawing.Design.ToolboxItem();
            //toolPointer.DisplayName = $"<Pointer>";
            //toolPointer.Bitmap = new System.Drawing.Bitmap(16, 16);
            //_ctrlToolBox.Items.Add(toolPointer);
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(Button)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(CheckBox)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(ComboBox)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(ContextMenuStrip)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(DataGridView)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(GroupBox)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(ImageList)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(Label)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(LinkLabel)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(ListView)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(OpenFileDialog)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(Panel)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(PictureBox)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(ProgressBar)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(MenuStrip)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(RadioButton)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(StatusBar)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(TabControl)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(TextBox)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(Timer)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(ToolBar)));
            //_ctrlToolBox.Items.Add(new ToolboxItem(typeof(TreeView)));
            var pointer = new ToolboxItem();
            pointer.DisplayName = "<Pointer>";
            pointer.Bitmap = new Bitmap(16, 16);
            _ctrlToolBox.Items.Add(pointer);

            foreach (Type type in _windowsFormsToolTypes)
            {
                ToolboxItem tbi = new ToolboxItem(type);
                ToolboxBitmapAttribute tba = TypeDescriptor.GetAttributes(type)[typeof(ToolboxBitmapAttribute)] as ToolboxBitmapAttribute;
                if (tba != null)
                {
                    tbi.Bitmap = (Bitmap)tba.GetImage(type);
                }
                _ctrlToolBox.Items.Add(tbi);
            }

            //- init the (UDesigner)pDesignerCore 
            _idesigner.CtrlToolbox = _ctrlToolBox;

            _ctrlToolBox.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.Toolbox_DrawItem);
            _ctrlToolBox.Invalidate();
        }

        private Color RowBackColorAlt = Color.FromArgb(185, 210, 234);//交替色
        private Color RowBackColorSel = Color.FromArgb(106, 197, 242);//选中项目颜色
        private void Toolbox_DrawItem(object sender, DrawItemEventArgs e)
        {
            Brush myBrush = Brushes.Black;
            if ((e.State & DrawItemState.Selected) == DrawItemState.Selected)
            {
                myBrush = new SolidBrush(RowBackColorSel);
            }
            else if (e.Index % 2 == 0)
            {
                myBrush = new SolidBrush(RowBackColorAlt);
            }
            else
            {
                myBrush = new SolidBrush(Color.White);
            }
            e.Graphics.FillRectangle(myBrush, e.Bounds);
            e.DrawFocusRectangle();//焦点框
            ToolboxItem tbi = _ctrlToolBox.Items[e.Index] as ToolboxItem;
            //绘制图表
            //Image image = Image.FromFile(@"..\..\ico\聊天界面头像2.png");
            Graphics g = e.Graphics;
            Rectangle bounds = e.Bounds;
            Rectangle imageRect = new Rectangle(bounds.X, bounds.Y, bounds.Height, bounds.Width);
            Rectangle textRect = new Rectangle(imageRect.Right, bounds.X,
                bounds.Width - imageRect.Right, bounds.Height);

            if (tbi.Bitmap != null)
            {
                Image image = (Image)tbi.Bitmap;
                g.DrawImage(image, imageRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel);
            }

            //文本
            StringFormat strFormat = new StringFormat();
            strFormat.LineAlignment = StringAlignment.Center;
            e.Graphics.DrawString(tbi.ToString(), e.Font, new SolidBrush(e.ForeColor), textRect, strFormat);
        }

        public void SelectPointer()
        {
            //ListBox list = this.tabControl.SelectedTab.Controls[0] as ListBox;
            _ctrlToolBox.Invalidate(_ctrlToolBox.GetItemRectangle(0));
            _ctrlToolBox.SelectedIndex = 0;
            _ctrlToolBox.Invalidate(_ctrlToolBox.GetItemRectangle(0));
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
            if (_solution != null)
            {
                DialogResult ret = MessageBox.Show("Do you want close and save current solution?", "Question", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (DialogResult.Cancel == ret) return;
                if (DialogResult.Yes == ret)
                    _solution.Save();
                _solution.Close();
                _solutionNode.Nodes.Clear();
                _solutionNode.Tag = null;
                _solutionNode.Text = "Solution";
            }

            if (_solution == null)
            {
                //SolutionDialog dlg = new SolutionDialog();
                //if (dlg.ShowDialog() == DialogResult.OK)
                //{
                //    _solution = Solution.CreateSolution(dlg.FileName, dlg.FilePath);
                //}
            }

            _solution.InitTreeNode(_solutionNode);
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _solution?.Save();
            //_idesigner.SaveCurrent();
        }

        private void saveAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //_idesigner.SaveAll();
        }

        private void openStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog fd = new OpenFileDialog();
            fd.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            fd.Filter = $"Solution file(*.usl)|*.usl";
            if (fd.ShowDialog() == DialogResult.OK)
            {
                var fName = fd.FileName;
                _idesigner.Open(fName);
            }
        }

        private void importStripMenuItem_Click(object sender, EventArgs e)
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
    }
}