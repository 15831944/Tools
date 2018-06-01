namespace LDisplay
{
	partial class Designer
	{
		/// <summary> 
		/// 必需的设计器变量。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// 清理所有正在使用的资源。
		/// </summary>
		/// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this._display = new LDisplay.DisplayDT();
            this._cbControlList = new System.Windows.Forms.ComboBox();
            this._pageMethod = new System.Windows.Forms.TabPage();
            this._colMethod = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._pageEvent = new System.Windows.Forms.TabPage();
            this._colContext = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._colEvent = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._tabContainer = new System.Windows.Forms.TabControl();
            this._pageProperty = new System.Windows.Forms.TabPage();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this._tabContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Margin = new System.Windows.Forms.Padding(4);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this._display);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this._tabContainer);
            this.splitContainer1.Panel2.Controls.Add(this._cbControlList);
            this.splitContainer1.Size = new System.Drawing.Size(1011, 607);
            this.splitContainer1.SplitterDistance = 640;
            this.splitContainer1.SplitterWidth = 5;
            this.splitContainer1.TabIndex = 0;
            // 
            // _display
            // 
            this._display.BackColor = System.Drawing.Color.White;
            this._display.Location = new System.Drawing.Point(0, 0);
            this._display.Margin = new System.Windows.Forms.Padding(4);
            this._display.Name = "_display";
            this._display.Size = new System.Drawing.Size(533, 427);
            this._display.TabIndex = 0;
            // 
            // _cbControlList
            // 
            this._cbControlList.Dock = System.Windows.Forms.DockStyle.Top;
            this._cbControlList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._cbControlList.FormattingEnabled = true;
            this._cbControlList.Location = new System.Drawing.Point(0, 0);
            this._cbControlList.Margin = new System.Windows.Forms.Padding(4);
            this._cbControlList.Name = "_cbControlList";
            this._cbControlList.Size = new System.Drawing.Size(366, 24);
            this._cbControlList.TabIndex = 0;
            this._cbControlList.SelectedIndexChanged += new System.EventHandler(this._cbControlList_SelectedIndexChanged);
            // 
            // _pageMethod
            // 
            this._pageMethod.Location = new System.Drawing.Point(4, 4);
            this._pageMethod.Margin = new System.Windows.Forms.Padding(4);
            this._pageMethod.Name = "_pageMethod";
            this._pageMethod.Padding = new System.Windows.Forms.Padding(4);
            this._pageMethod.Size = new System.Drawing.Size(358, 554);
            this._pageMethod.TabIndex = 2;
            this._pageMethod.Text = "Method";
            this._pageMethod.UseVisualStyleBackColor = true;
            // 
            // _colMethod
            // 
            this._colMethod.HeaderText = "Method";
            this._colMethod.Name = "_colMethod";
            this._colMethod.ReadOnly = true;
            this._colMethod.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
            // 
            // _pageEvent
            // 
            this._pageEvent.Location = new System.Drawing.Point(4, 4);
            this._pageEvent.Margin = new System.Windows.Forms.Padding(4);
            this._pageEvent.Name = "_pageEvent";
            this._pageEvent.Padding = new System.Windows.Forms.Padding(4);
            this._pageEvent.Size = new System.Drawing.Size(358, 554);
            this._pageEvent.TabIndex = 1;
            this._pageEvent.Text = "Event";
            this._pageEvent.UseVisualStyleBackColor = true;
            // 
            // _colContext
            // 
            this._colContext.HeaderText = "Context";
            this._colContext.Name = "_colContext";
            this._colContext.ReadOnly = true;
            // 
            // _colEvent
            // 
            this._colEvent.HeaderText = "Event";
            this._colEvent.Name = "_colEvent";
            this._colEvent.ReadOnly = true;
            this._colEvent.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
            // 
            // _tabContainer
            // 
            this._tabContainer.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this._tabContainer.Controls.Add(this._pageProperty);
            this._tabContainer.Controls.Add(this._pageEvent);
            this._tabContainer.Controls.Add(this._pageMethod);
            this._tabContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this._tabContainer.Location = new System.Drawing.Point(0, 24);
            this._tabContainer.Margin = new System.Windows.Forms.Padding(4);
            this._tabContainer.Name = "_tabContainer";
            this._tabContainer.SelectedIndex = 0;
            this._tabContainer.Size = new System.Drawing.Size(366, 583);
            this._tabContainer.TabIndex = 1;
            // 
            // _pageProperty
            // 
            this._pageProperty.Location = new System.Drawing.Point(4, 4);
            this._pageProperty.Margin = new System.Windows.Forms.Padding(4);
            this._pageProperty.Name = "_pageProperty";
            this._pageProperty.Padding = new System.Windows.Forms.Padding(4);
            this._pageProperty.Size = new System.Drawing.Size(358, 554);
            this._pageProperty.TabIndex = 0;
            this._pageProperty.Text = "Property";
            this._pageProperty.UseVisualStyleBackColor = true;
            // 
            // Designer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "Designer";
            this.Size = new System.Drawing.Size(1011, 607);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this._tabContainer.ResumeLayout(false);
            this.ResumeLayout(false);

		}

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private DisplayDT _display;
        private System.Windows.Forms.TabControl _tabContainer;
        private System.Windows.Forms.TabPage _pageProperty;
        private System.Windows.Forms.TabPage _pageEvent;
        private System.Windows.Forms.TabPage _pageMethod;
        private System.Windows.Forms.ComboBox _cbControlList;
        private System.Windows.Forms.DataGridViewTextBoxColumn _colMethod;
        private System.Windows.Forms.DataGridViewTextBoxColumn _colContext;
        private System.Windows.Forms.DataGridViewTextBoxColumn _colEvent;
    }
}
