namespace UView.View
{
    partial class SolutionDialog
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
            this._tabCtrl = new System.Windows.Forms.TabControl();
            this._tabOpen = new System.Windows.Forms.TabPage();
            this._dgProj = new System.Windows.Forms.DataGridView();
            this._colName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._colNo = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._colTime = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._colComment = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._tabCreate = new System.Windows.Forms.TabPage();
            this._btPath = new System.Windows.Forms.Button();
            this._commentBox = new System.Windows.Forms.TextBox();
            this._pathBox = new System.Windows.Forms.TextBox();
            this._nameBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this._btOK = new System.Windows.Forms.Button();
            this._tabCtrl.SuspendLayout();
            this._tabOpen.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this._dgProj)).BeginInit();
            this._tabCreate.SuspendLayout();
            this.SuspendLayout();
            // 
            // _tabCtrl
            // 
            this._tabCtrl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this._tabCtrl.Controls.Add(this._tabOpen);
            this._tabCtrl.Controls.Add(this._tabCreate);
            this._tabCtrl.Location = new System.Drawing.Point(12, 12);
            this._tabCtrl.Name = "_tabCtrl";
            this._tabCtrl.SelectedIndex = 0;
            this._tabCtrl.Size = new System.Drawing.Size(622, 261);
            this._tabCtrl.TabIndex = 1;
            // 
            // _tabOpen
            // 
            this._tabOpen.Controls.Add(this._dgProj);
            this._tabOpen.Location = new System.Drawing.Point(4, 23);
            this._tabOpen.Name = "_tabOpen";
            this._tabOpen.Padding = new System.Windows.Forms.Padding(3);
            this._tabOpen.Size = new System.Drawing.Size(614, 234);
            this._tabOpen.TabIndex = 0;
            this._tabOpen.Text = "Open";
            this._tabOpen.UseVisualStyleBackColor = true;
            // 
            // _dgProj
            // 
            this._dgProj.AllowUserToAddRows = false;
            this._dgProj.AllowUserToResizeRows = false;
            this._dgProj.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this._colName,
            this._colNo,
            this._colTime,
            this._colComment});
            this._dgProj.Dock = System.Windows.Forms.DockStyle.Fill;
            this._dgProj.Location = new System.Drawing.Point(3, 3);
            this._dgProj.Name = "_dgProj";
            this._dgProj.ReadOnly = true;
            this._dgProj.RowTemplate.Height = 23;
            this._dgProj.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this._dgProj.Size = new System.Drawing.Size(608, 228);
            this._dgProj.TabIndex = 1;
            this._dgProj.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler(this._dgProj_CellFormatting);
            this._dgProj.DoubleClick += new System.EventHandler(this._dgProj_DoubleClick);
            // 
            // _colName
            // 
            this._colName.Frozen = true;
            this._colName.HeaderText = "Name";
            this._colName.Name = "_colName";
            this._colName.ReadOnly = true;
            // 
            // _colNo
            // 
            this._colNo.Frozen = true;
            this._colNo.HeaderText = "No";
            this._colNo.Name = "_colNo";
            this._colNo.ReadOnly = true;
            this._colNo.Width = 240;
            // 
            // _colTime
            // 
            this._colTime.HeaderText = "Time";
            this._colTime.Name = "_colTime";
            this._colTime.ReadOnly = true;
            this._colTime.Width = 120;
            // 
            // _colComment
            // 
            this._colComment.HeaderText = "Comment";
            this._colComment.Name = "_colComment";
            this._colComment.ReadOnly = true;
            this._colComment.Width = 200;
            // 
            // _tabCreate
            // 
            this._tabCreate.Controls.Add(this._btPath);
            this._tabCreate.Controls.Add(this._commentBox);
            this._tabCreate.Controls.Add(this._pathBox);
            this._tabCreate.Controls.Add(this._nameBox);
            this._tabCreate.Controls.Add(this.label2);
            this._tabCreate.Controls.Add(this.label3);
            this._tabCreate.Controls.Add(this.label1);
            this._tabCreate.Location = new System.Drawing.Point(4, 23);
            this._tabCreate.Name = "_tabCreate";
            this._tabCreate.Padding = new System.Windows.Forms.Padding(3);
            this._tabCreate.Size = new System.Drawing.Size(614, 234);
            this._tabCreate.TabIndex = 1;
            this._tabCreate.Text = "Create";
            this._tabCreate.UseVisualStyleBackColor = true;
            // 
            // _btPath
            // 
            this._btPath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this._btPath.Location = new System.Drawing.Point(558, 47);
            this._btPath.Name = "_btPath";
            this._btPath.Size = new System.Drawing.Size(41, 23);
            this._btPath.TabIndex = 3;
            this._btPath.Text = "...";
            this._btPath.UseVisualStyleBackColor = true;
            this._btPath.Click += new System.EventHandler(this._btPath_Click);
            // 
            // _commentBox
            // 
            this._commentBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this._commentBox.Location = new System.Drawing.Point(13, 98);
            this._commentBox.Multiline = true;
            this._commentBox.Name = "_commentBox";
            this._commentBox.Size = new System.Drawing.Size(586, 120);
            this._commentBox.TabIndex = 2;
            // 
            // _pathBox
            // 
            this._pathBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this._pathBox.Location = new System.Drawing.Point(78, 48);
            this._pathBox.Name = "_pathBox";
            this._pathBox.Size = new System.Drawing.Size(474, 22);
            this._pathBox.TabIndex = 0;
            // 
            // _nameBox
            // 
            this._nameBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this._nameBox.Location = new System.Drawing.Point(78, 17);
            this._nameBox.Name = "_nameBox";
            this._nameBox.Size = new System.Drawing.Size(521, 22);
            this._nameBox.TabIndex = 0;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 81);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(60, 14);
            this.label2.TabIndex = 0;
            this.label2.Text = "Comment";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 51);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(32, 14);
            this.label3.TabIndex = 0;
            this.label3.Text = "Path";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 14);
            this.label1.TabIndex = 0;
            this.label1.Text = "Name";
            // 
            // _btOK
            // 
            this._btOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this._btOK.Location = new System.Drawing.Point(559, 279);
            this._btOK.Name = "_btOK";
            this._btOK.Size = new System.Drawing.Size(75, 23);
            this._btOK.TabIndex = 2;
            this._btOK.Text = "OK";
            this._btOK.UseVisualStyleBackColor = true;
            this._btOK.Click += new System.EventHandler(this._btOK_Click);
            // 
            // ProjectForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(646, 312);
            this.Controls.Add(this._btOK);
            this.Controls.Add(this._tabCtrl);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ProjectForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ProjectForm";
            this.Load += new System.EventHandler(this.SolutionCreater_Load);
            this._tabCtrl.ResumeLayout(false);
            this._tabOpen.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this._dgProj)).EndInit();
            this._tabCreate.ResumeLayout(false);
            this._tabCreate.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl _tabCtrl;
        private System.Windows.Forms.TabPage _tabOpen;
        private System.Windows.Forms.DataGridView _dgProj;
        private System.Windows.Forms.TabPage _tabCreate;
        private System.Windows.Forms.Button _btOK;
        private System.Windows.Forms.DataGridViewTextBoxColumn _colName;
        private System.Windows.Forms.DataGridViewTextBoxColumn _colNo;
        private System.Windows.Forms.DataGridViewTextBoxColumn _colTime;
        private System.Windows.Forms.DataGridViewTextBoxColumn _colComment;
        private System.Windows.Forms.TextBox _commentBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button _btPath;
        private System.Windows.Forms.TextBox _pathBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox _nameBox;
        private System.Windows.Forms.Label label1;
    }
}