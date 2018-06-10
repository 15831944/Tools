namespace UView
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("Child");
            System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("Solution", new System.Windows.Forms.TreeNode[] {
            treeNode5});
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addFormToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.displayToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.formToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteFormToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.exitXToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemUnDo = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemReDo = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.ToolStripMenuItemCut = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemCopy = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemPaste = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemDelete = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemSelectAll = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gridToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gridNoneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.x1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.x4ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.x8ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.x16ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.x32ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemTools = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemTabOrder = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemAbout = new System.Windows.Forms.ToolStripMenuItem();
            this._ctrlToolBox = new System.Windows.Forms.ListBox();
            this.designerPanel = new System.Windows.Forms.Panel();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this._projTree = new System.Windows.Forms.TreeView();
            this.solutionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.toolStripMenuItemTools,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(911, 25);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addFormToolStripMenuItem,
            this.openStripMenuItem,
            this.deleteFormToolStripMenuItem,
            this.toolStripSeparator1,
            this.saveToolStripMenuItem,
            this.saveAllToolStripMenuItem,
            this.toolStripSeparator2,
            this.exitXToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(39, 21);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // addFormToolStripMenuItem
            // 
            this.addFormToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.solutionToolStripMenuItem,
            this.toolStripSeparator4,
            this.displayToolStripMenuItem,
            this.formToolStripMenuItem});
            this.addFormToolStripMenuItem.Name = "addFormToolStripMenuItem";
            this.addFormToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.addFormToolStripMenuItem.Text = "&Add";
            // 
            // displayToolStripMenuItem
            // 
            this.displayToolStripMenuItem.Name = "displayToolStripMenuItem";
            this.displayToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.displayToolStripMenuItem.Text = "&Display";
            this.displayToolStripMenuItem.Click += new System.EventHandler(this.displayToolStripMenuItem_Click);
            // 
            // formToolStripMenuItem
            // 
            this.formToolStripMenuItem.Name = "formToolStripMenuItem";
            this.formToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.formToolStripMenuItem.Text = "&Dialog";
            this.formToolStripMenuItem.Click += new System.EventHandler(this.formToolStripMenuItem_Click);
            // 
            // openStripMenuItem
            // 
            this.openStripMenuItem.Name = "openStripMenuItem";
            this.openStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.openStripMenuItem.Text = "&Open";
            this.openStripMenuItem.Click += new System.EventHandler(this.openStripMenuItem_Click);
            // 
            // deleteFormToolStripMenuItem
            // 
            this.deleteFormToolStripMenuItem.Name = "deleteFormToolStripMenuItem";
            this.deleteFormToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.deleteFormToolStripMenuItem.Text = "&Delete Display";
            this.deleteFormToolStripMenuItem.Click += new System.EventHandler(this.deleteFormToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(166, 6);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.saveToolStripMenuItem.Text = "&Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAllToolStripMenuItem
            // 
            this.saveAllToolStripMenuItem.Name = "saveAllToolStripMenuItem";
            this.saveAllToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.saveAllToolStripMenuItem.Text = "Save All (&L)";
            this.saveAllToolStripMenuItem.Click += new System.EventHandler(this.saveAllToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(166, 6);
            // 
            // exitXToolStripMenuItem
            // 
            this.exitXToolStripMenuItem.Name = "exitXToolStripMenuItem";
            this.exitXToolStripMenuItem.ShortcutKeyDisplayString = "Alt + F4";
            this.exitXToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.exitXToolStripMenuItem.Text = "Exit (&X)";
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItemUnDo,
            this.ToolStripMenuItemReDo,
            this.toolStripSeparator3,
            this.ToolStripMenuItemCut,
            this.ToolStripMenuItemCopy,
            this.ToolStripMenuItemPaste,
            this.ToolStripMenuItemDelete,
            this.ToolStripMenuItemSelectAll});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(42, 21);
            this.editToolStripMenuItem.Text = "&Edit";
            this.editToolStripMenuItem.DropDownOpening += new System.EventHandler(this.editToolStripMenuItem_DropDownOpening);
            // 
            // ToolStripMenuItemUnDo
            // 
            this.ToolStripMenuItemUnDo.Name = "ToolStripMenuItemUnDo";
            this.ToolStripMenuItemUnDo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.ToolStripMenuItemUnDo.Size = new System.Drawing.Size(173, 22);
            this.ToolStripMenuItemUnDo.Text = "Undo";
            this.ToolStripMenuItemUnDo.Click += new System.EventHandler(this.OnMenuClick);
            // 
            // ToolStripMenuItemReDo
            // 
            this.ToolStripMenuItemReDo.Name = "ToolStripMenuItemReDo";
            this.ToolStripMenuItemReDo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.ToolStripMenuItemReDo.Size = new System.Drawing.Size(173, 22);
            this.ToolStripMenuItemReDo.Text = "Redo";
            this.ToolStripMenuItemReDo.Click += new System.EventHandler(this.OnMenuClick);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(170, 6);
            // 
            // ToolStripMenuItemCut
            // 
            this.ToolStripMenuItemCut.Image = ((System.Drawing.Image)(resources.GetObject("ToolStripMenuItemCut.Image")));
            this.ToolStripMenuItemCut.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ToolStripMenuItemCut.Name = "ToolStripMenuItemCut";
            this.ToolStripMenuItemCut.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.ToolStripMenuItemCut.Size = new System.Drawing.Size(173, 22);
            this.ToolStripMenuItemCut.Text = "Cut";
            this.ToolStripMenuItemCut.Click += new System.EventHandler(this.OnMenuClick);
            // 
            // ToolStripMenuItemCopy
            // 
            this.ToolStripMenuItemCopy.Image = ((System.Drawing.Image)(resources.GetObject("ToolStripMenuItemCopy.Image")));
            this.ToolStripMenuItemCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ToolStripMenuItemCopy.Name = "ToolStripMenuItemCopy";
            this.ToolStripMenuItemCopy.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.ToolStripMenuItemCopy.Size = new System.Drawing.Size(173, 22);
            this.ToolStripMenuItemCopy.Text = "Copy";
            this.ToolStripMenuItemCopy.Click += new System.EventHandler(this.OnMenuClick);
            // 
            // ToolStripMenuItemPaste
            // 
            this.ToolStripMenuItemPaste.Image = ((System.Drawing.Image)(resources.GetObject("ToolStripMenuItemPaste.Image")));
            this.ToolStripMenuItemPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ToolStripMenuItemPaste.Name = "ToolStripMenuItemPaste";
            this.ToolStripMenuItemPaste.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.ToolStripMenuItemPaste.Size = new System.Drawing.Size(173, 22);
            this.ToolStripMenuItemPaste.Text = "Paste";
            this.ToolStripMenuItemPaste.Click += new System.EventHandler(this.OnMenuClick);
            // 
            // ToolStripMenuItemDelete
            // 
            this.ToolStripMenuItemDelete.Name = "ToolStripMenuItemDelete";
            this.ToolStripMenuItemDelete.ShortcutKeys = System.Windows.Forms.Keys.Delete;
            this.ToolStripMenuItemDelete.Size = new System.Drawing.Size(173, 22);
            this.ToolStripMenuItemDelete.Text = "Delete";
            this.ToolStripMenuItemDelete.Click += new System.EventHandler(this.OnMenuClick);
            // 
            // ToolStripMenuItemSelectAll
            // 
            this.ToolStripMenuItemSelectAll.Name = "ToolStripMenuItemSelectAll";
            this.ToolStripMenuItemSelectAll.ShortcutKeyDisplayString = "Ctrl+A";
            this.ToolStripMenuItemSelectAll.Size = new System.Drawing.Size(173, 22);
            this.ToolStripMenuItemSelectAll.Text = "Select All";
            this.ToolStripMenuItemSelectAll.Click += new System.EventHandler(this.OnMenuClick);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.gridToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(67, 21);
            this.viewToolStripMenuItem.Text = "View (&V)";
            // 
            // gridToolStripMenuItem
            // 
            this.gridToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.gridNoneToolStripMenuItem,
            this.x1ToolStripMenuItem,
            this.x4ToolStripMenuItem,
            this.x8ToolStripMenuItem,
            this.x16ToolStripMenuItem,
            this.x32ToolStripMenuItem});
            this.gridToolStripMenuItem.Name = "gridToolStripMenuItem";
            this.gridToolStripMenuItem.Size = new System.Drawing.Size(101, 22);
            this.gridToolStripMenuItem.Text = "&Grid";
            // 
            // gridNoneToolStripMenuItem
            // 
            this.gridNoneToolStripMenuItem.Name = "gridNoneToolStripMenuItem";
            this.gridNoneToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.gridNoneToolStripMenuItem.Text = "&None";
            this.gridNoneToolStripMenuItem.Click += new System.EventHandler(this.gridNoneToolStripMenuItem_Click);
            // 
            // x1ToolStripMenuItem
            // 
            this.x1ToolStripMenuItem.Name = "x1ToolStripMenuItem";
            this.x1ToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.x1ToolStripMenuItem.Text = "1 x 1";
            this.x1ToolStripMenuItem.Click += new System.EventHandler(this.x1ToolStripMenuItem_Click);
            // 
            // x4ToolStripMenuItem
            // 
            this.x4ToolStripMenuItem.Name = "x4ToolStripMenuItem";
            this.x4ToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.x4ToolStripMenuItem.Text = "4 x 4";
            this.x4ToolStripMenuItem.Click += new System.EventHandler(this.x4ToolStripMenuItem_Click);
            // 
            // x8ToolStripMenuItem
            // 
            this.x8ToolStripMenuItem.Name = "x8ToolStripMenuItem";
            this.x8ToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.x8ToolStripMenuItem.Text = "8 x 8";
            this.x8ToolStripMenuItem.Click += new System.EventHandler(this.x8ToolStripMenuItem_Click);
            // 
            // x16ToolStripMenuItem
            // 
            this.x16ToolStripMenuItem.Name = "x16ToolStripMenuItem";
            this.x16ToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.x16ToolStripMenuItem.Text = "16 x 16";
            this.x16ToolStripMenuItem.Click += new System.EventHandler(this.x16ToolStripMenuItem_Click);
            // 
            // x32ToolStripMenuItem
            // 
            this.x32ToolStripMenuItem.Name = "x32ToolStripMenuItem";
            this.x32ToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.x32ToolStripMenuItem.Text = "32 x 32";
            this.x32ToolStripMenuItem.Click += new System.EventHandler(this.x32ToolStripMenuItem_Click);
            // 
            // toolStripMenuItemTools
            // 
            this.toolStripMenuItemTools.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemTabOrder});
            this.toolStripMenuItemTools.Name = "toolStripMenuItemTools";
            this.toolStripMenuItemTools.Size = new System.Drawing.Size(52, 21);
            this.toolStripMenuItemTools.Text = "&Tools";
            // 
            // toolStripMenuItemTabOrder
            // 
            this.toolStripMenuItemTabOrder.Name = "toolStripMenuItemTabOrder";
            this.toolStripMenuItemTabOrder.Size = new System.Drawing.Size(137, 22);
            this.toolStripMenuItemTabOrder.Text = "Tab &Order";
            this.toolStripMenuItemTabOrder.Click += new System.EventHandler(this.toolStripMenuItemTabOrder_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItemAbout});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(47, 21);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // ToolStripMenuItemAbout
            // 
            this.ToolStripMenuItemAbout.Name = "ToolStripMenuItemAbout";
            this.ToolStripMenuItemAbout.Size = new System.Drawing.Size(120, 22);
            this.ToolStripMenuItemAbout.Text = "About...";
            this.ToolStripMenuItemAbout.Click += new System.EventHandler(this.OnAbout);
            // 
            // _ctrlToolBox
            // 
            this._ctrlToolBox.Dock = System.Windows.Forms.DockStyle.Bottom;
            this._ctrlToolBox.FormattingEnabled = true;
            this._ctrlToolBox.ItemHeight = 12;
            this._ctrlToolBox.Location = new System.Drawing.Point(0, 216);
            this._ctrlToolBox.Margin = new System.Windows.Forms.Padding(2);
            this._ctrlToolBox.Name = "_ctrlToolBox";
            this._ctrlToolBox.Size = new System.Drawing.Size(159, 280);
            this._ctrlToolBox.TabIndex = 0;
            // 
            // designerPanel
            // 
            this.designerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.designerPanel.Location = new System.Drawing.Point(0, 0);
            this.designerPanel.Margin = new System.Windows.Forms.Padding(2);
            this.designerPanel.Name = "designerPanel";
            this.designerPanel.Size = new System.Drawing.Size(748, 496);
            this.designerPanel.TabIndex = 3;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 25);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this._projTree);
            this.splitContainer1.Panel1.Controls.Add(this._ctrlToolBox);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.designerPanel);
            this.splitContainer1.Size = new System.Drawing.Size(911, 496);
            this.splitContainer1.SplitterDistance = 159;
            this.splitContainer1.TabIndex = 4;
            // 
            // _projTree
            // 
            this._projTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this._projTree.FullRowSelect = true;
            this._projTree.HideSelection = false;
            this._projTree.Location = new System.Drawing.Point(0, 0);
            this._projTree.Name = "_projTree";
            treeNode5.Name = "节点1";
            treeNode5.Text = "Child";
            treeNode6.Name = "节点0";
            treeNode6.Text = "Solution";
            this._projTree.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode6});
            this._projTree.ShowNodeToolTips = true;
            this._projTree.ShowRootLines = false;
            this._projTree.Size = new System.Drawing.Size(159, 216);
            this._projTree.TabIndex = 1;
            // 
            // solutionToolStripMenuItem
            // 
            this.solutionToolStripMenuItem.Name = "solutionToolStripMenuItem";
            this.solutionToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.solutionToolStripMenuItem.Text = "&Solution";
            this.solutionToolStripMenuItem.Click += new System.EventHandler(this.solutionToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(177, 6);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(911, 521);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "UView";
            this.Load += new System.EventHandler(this.pDesignerMainForm_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemUnDo;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemReDo;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemCut;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemCopy;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemPaste;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemDelete;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemAbout;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemTools;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemTabOrder;
        private System.Windows.Forms.ListBox _ctrlToolBox;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addFormToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteFormToolStripMenuItem;
        private System.Windows.Forms.Panel designerPanel;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem gridToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem gridNoneToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem x1ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem x4ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem x8ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem x16ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem x32ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem displayToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem formToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem exitXToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemSelectAll;
        private System.Windows.Forms.TreeView _projTree;
        private System.Windows.Forms.ToolStripMenuItem solutionToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
    }
}

