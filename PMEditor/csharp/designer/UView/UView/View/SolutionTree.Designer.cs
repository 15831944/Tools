namespace UView.View
{
    partial class SolutionTree
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

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this._menuProj = new System.Windows.Forms.ContextMenuStrip(this.components);
            this._menuAddGroup = new System.Windows.Forms.ToolStripMenuItem();
            this._menuAddDisplay = new System.Windows.Forms.ToolStripMenuItem();
            this._menuGroup = new System.Windows.Forms.ContextMenuStrip(this.components);
            this._menuAddSubGroup = new System.Windows.Forms.ToolStripMenuItem();
            this._menuRenameGroup = new System.Windows.Forms.ToolStripMenuItem();
            this._menuDeleteGroup = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this._menuAddSubDisplay = new System.Windows.Forms.ToolStripMenuItem();
            this._menuDisplay = new System.Windows.Forms.ContextMenuStrip(this.components);
            this._menuRenameDisplay = new System.Windows.Forms.ToolStripMenuItem();
            this._menuDeleteDisplay = new System.Windows.Forms.ToolStripMenuItem();
            this._menuProj.SuspendLayout();
            this._menuGroup.SuspendLayout();
            this._menuDisplay.SuspendLayout();
            this.SuspendLayout();
            // 
            // _menuProj
            // 
            this._menuProj.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._menuAddGroup,
            this._menuAddDisplay});
            this._menuProj.Name = "_menuProj";
            this._menuProj.Size = new System.Drawing.Size(143, 48);
            // 
            // _menuAddGroup
            // 
            this._menuAddGroup.Name = "_menuAddGroup";
            this._menuAddGroup.Size = new System.Drawing.Size(142, 22);
            this._menuAddGroup.Text = "Add&Group";
            this._menuAddGroup.Click += new System.EventHandler(this.OnAddGroup);
            // 
            // _menuAddDisplay
            // 
            this._menuAddDisplay.Name = "_menuAddDisplay";
            this._menuAddDisplay.Size = new System.Drawing.Size(142, 22);
            this._menuAddDisplay.Text = "Add&Display";
            this._menuAddDisplay.Click += new System.EventHandler(this.OnAddDisplay);
            // 
            // _menuGroup
            // 
            this._menuGroup.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._menuAddSubGroup,
            this._menuRenameGroup,
            this._menuDeleteGroup,
            this.toolStripSeparator1,
            this._menuAddSubDisplay});
            this._menuGroup.Name = "_menuProj";
            this._menuGroup.Size = new System.Drawing.Size(166, 98);
            // 
            // _menuAddSubGroup
            // 
            this._menuAddSubGroup.Name = "_menuAddSubGroup";
            this._menuAddSubGroup.Size = new System.Drawing.Size(165, 22);
            this._menuAddSubGroup.Text = "Add&Group";
            this._menuAddSubGroup.Click += new System.EventHandler(this.OnAddSubGroup);
            // 
            // _menuRenameGroup
            // 
            this._menuRenameGroup.Name = "_menuRenameGroup";
            this._menuRenameGroup.Size = new System.Drawing.Size(165, 22);
            this._menuRenameGroup.Text = "&RenameDisplay";
            this._menuRenameGroup.Click += new System.EventHandler(this.OnRenameGroup);
            // 
            // _menuDeleteGroup
            // 
            this._menuDeleteGroup.Name = "_menuDeleteGroup";
            this._menuDeleteGroup.Size = new System.Drawing.Size(165, 22);
            this._menuDeleteGroup.Text = "&DeleteGroup";
            this._menuDeleteGroup.Click += new System.EventHandler(this.OnDeleteGroup);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(162, 6);
            // 
            // _menuAddSubDisplay
            // 
            this._menuAddSubDisplay.Name = "_menuAddSubDisplay";
            this._menuAddSubDisplay.Size = new System.Drawing.Size(165, 22);
            this._menuAddSubDisplay.Text = "&AddDisplay";
            this._menuAddSubDisplay.Click += new System.EventHandler(this.OnAddSubDisplay);
            // 
            // _menuDisplay
            // 
            this._menuDisplay.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._menuRenameDisplay,
            this._menuDeleteDisplay});
            this._menuDisplay.Name = "_menuProj";
            this._menuDisplay.Size = new System.Drawing.Size(124, 48);
            // 
            // _menuRenameDisplay
            // 
            this._menuRenameDisplay.Name = "_menuRenameDisplay";
            this._menuRenameDisplay.Size = new System.Drawing.Size(123, 22);
            this._menuRenameDisplay.Text = "&Rename";
            this._menuRenameDisplay.Click += new System.EventHandler(this.OnRenameDisplay);
            // 
            // _menuDeleteDisplay
            // 
            this._menuDeleteDisplay.Name = "_menuDeleteDisplay";
            this._menuDeleteDisplay.Size = new System.Drawing.Size(123, 22);
            this._menuDeleteDisplay.Text = "&Delete";
            this._menuDeleteDisplay.Click += new System.EventHandler(this.OnDeleteDisplay);
            // 
            // ProjectTree
            // 
            this.LineColor = System.Drawing.Color.Black;
            this.Size = new System.Drawing.Size(213, 272);
            this._menuProj.ResumeLayout(false);
            this._menuGroup.ResumeLayout(false);
            this._menuDisplay.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip _menuProj;
        private System.Windows.Forms.ToolStripMenuItem _menuAddGroup;
        private System.Windows.Forms.ToolStripMenuItem _menuAddDisplay;
        private System.Windows.Forms.ContextMenuStrip _menuGroup;
        private System.Windows.Forms.ToolStripMenuItem _menuAddSubGroup;
        private System.Windows.Forms.ToolStripMenuItem _menuRenameGroup;
        private System.Windows.Forms.ContextMenuStrip _menuDisplay;
        private System.Windows.Forms.ToolStripMenuItem _menuRenameDisplay;
        private System.Windows.Forms.ToolStripMenuItem _menuDeleteDisplay;
        private System.Windows.Forms.ToolStripMenuItem _menuDeleteGroup;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem _menuAddSubDisplay;
    }
}
