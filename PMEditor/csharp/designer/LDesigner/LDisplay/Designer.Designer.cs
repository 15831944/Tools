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
			this._tabContainer = new System.Windows.Forms.TabControl();
			this._pageProperty = new System.Windows.Forms.TabPage();
			this._pageEvent = new System.Windows.Forms.TabPage();
			this._pageMethod = new System.Windows.Forms.TabPage();
			this._cbControlList = new System.Windows.Forms.ComboBox();
			this._display = new LDisplay.DisplayDT();
			this._propGrid = new LDisplay.LPropertyGrid();
			this._eventGrid = new LDisplay.LEventGrid();
			this._colEvent = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this._colContext = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this._methodGrid = new LDisplay.LMethodGrid();
			this._colMethod = new System.Windows.Forms.DataGridViewTextBoxColumn();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this._tabContainer.SuspendLayout();
			this._pageProperty.SuspendLayout();
			this._pageEvent.SuspendLayout();
			this._pageMethod.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this._eventGrid)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this._methodGrid)).BeginInit();
			this.SuspendLayout();
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(0, 0);
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
			this.splitContainer1.Size = new System.Drawing.Size(758, 455);
			this.splitContainer1.SplitterDistance = 480;
			this.splitContainer1.TabIndex = 0;
			// 
			// _tabContainer
			// 
			this._tabContainer.Alignment = System.Windows.Forms.TabAlignment.Bottom;
			this._tabContainer.Controls.Add(this._pageProperty);
			this._tabContainer.Controls.Add(this._pageEvent);
			this._tabContainer.Controls.Add(this._pageMethod);
			this._tabContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this._tabContainer.Location = new System.Drawing.Point(0, 20);
			this._tabContainer.Name = "_tabContainer";
			this._tabContainer.SelectedIndex = 0;
			this._tabContainer.Size = new System.Drawing.Size(274, 435);
			this._tabContainer.TabIndex = 1;
			// 
			// _pageProperty
			// 
			this._pageProperty.Controls.Add(this._propGrid);
			this._pageProperty.Location = new System.Drawing.Point(4, 4);
			this._pageProperty.Name = "_pageProperty";
			this._pageProperty.Padding = new System.Windows.Forms.Padding(3);
			this._pageProperty.Size = new System.Drawing.Size(266, 409);
			this._pageProperty.TabIndex = 0;
			this._pageProperty.Text = "Property";
			this._pageProperty.UseVisualStyleBackColor = true;
			// 
			// _pageEvent
			// 
			this._pageEvent.Controls.Add(this._eventGrid);
			this._pageEvent.Location = new System.Drawing.Point(4, 4);
			this._pageEvent.Name = "_pageEvent";
			this._pageEvent.Padding = new System.Windows.Forms.Padding(3);
			this._pageEvent.Size = new System.Drawing.Size(266, 409);
			this._pageEvent.TabIndex = 1;
			this._pageEvent.Text = "Event";
			this._pageEvent.UseVisualStyleBackColor = true;
			// 
			// _pageMethod
			// 
			this._pageMethod.Controls.Add(this._methodGrid);
			this._pageMethod.Location = new System.Drawing.Point(4, 4);
			this._pageMethod.Name = "_pageMethod";
			this._pageMethod.Padding = new System.Windows.Forms.Padding(3);
			this._pageMethod.Size = new System.Drawing.Size(266, 409);
			this._pageMethod.TabIndex = 2;
			this._pageMethod.Text = "Method";
			this._pageMethod.UseVisualStyleBackColor = true;
			// 
			// _cbControlList
			// 
			this._cbControlList.Dock = System.Windows.Forms.DockStyle.Top;
			this._cbControlList.FormattingEnabled = true;
			this._cbControlList.Location = new System.Drawing.Point(0, 0);
			this._cbControlList.Name = "_cbControlList";
			this._cbControlList.Size = new System.Drawing.Size(274, 20);
			this._cbControlList.TabIndex = 0;
			this._cbControlList.SelectedIndexChanged += new System.EventHandler(this._cbControlList_SelectedIndexChanged);
			// 
			// _display
			// 
			this._display.BackColor = System.Drawing.Color.White;
			this._display.Location = new System.Drawing.Point(0, 0);
			this._display.Name = "_display";
			this._display.Size = new System.Drawing.Size(400, 320);
			this._display.TabIndex = 0;
			// 
			// _propGrid
			// 
			this._propGrid.Dock = System.Windows.Forms.DockStyle.Fill;
			this._propGrid.Location = new System.Drawing.Point(3, 3);
			this._propGrid.Name = "_propGrid";
			this._propGrid.Size = new System.Drawing.Size(260, 403);
			this._propGrid.TabIndex = 0;
			// 
			// _eventGrid
			// 
			this._eventGrid.AllowUserToAddRows = false;
			this._eventGrid.AllowUserToDeleteRows = false;
			this._eventGrid.AllowUserToResizeRows = false;
			this._eventGrid.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
			this._eventGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this._eventGrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this._colEvent,
            this._colContext});
			this._eventGrid.Dock = System.Windows.Forms.DockStyle.Fill;
			this._eventGrid.GridColor = System.Drawing.SystemColors.ButtonFace;
			this._eventGrid.Location = new System.Drawing.Point(3, 3);
			this._eventGrid.MultiSelect = false;
			this._eventGrid.Name = "_eventGrid";
			this._eventGrid.ReadOnly = true;
			this._eventGrid.RowHeadersVisible = false;
			this._eventGrid.RowTemplate.Height = 23;
			this._eventGrid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this._eventGrid.Size = new System.Drawing.Size(260, 403);
			this._eventGrid.TabIndex = 0;
			// 
			// _colEvent
			// 
			this._colEvent.HeaderText = "Event";
			this._colEvent.Name = "_colEvent";
			this._colEvent.ReadOnly = true;
			this._colEvent.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
			// 
			// _colContext
			// 
			this._colContext.HeaderText = "Context";
			this._colContext.Name = "_colContext";
			this._colContext.ReadOnly = true;
			// 
			// _methodGrid
			// 
			this._methodGrid.AllowUserToAddRows = false;
			this._methodGrid.AllowUserToDeleteRows = false;
			this._methodGrid.AllowUserToResizeRows = false;
			this._methodGrid.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
			this._methodGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this._methodGrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this._colMethod});
			this._methodGrid.Dock = System.Windows.Forms.DockStyle.Fill;
			this._methodGrid.GridColor = System.Drawing.SystemColors.ButtonFace;
			this._methodGrid.Location = new System.Drawing.Point(3, 3);
			this._methodGrid.MultiSelect = false;
			this._methodGrid.Name = "_methodGrid";
			this._methodGrid.ReadOnly = true;
			this._methodGrid.RowHeadersVisible = false;
			this._methodGrid.RowTemplate.Height = 23;
			this._methodGrid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this._methodGrid.Size = new System.Drawing.Size(260, 403);
			this._methodGrid.TabIndex = 1;
			// 
			// _colMethod
			// 
			this._colMethod.HeaderText = "Method";
			this._colMethod.Name = "_colMethod";
			this._colMethod.ReadOnly = true;
			this._colMethod.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
			// 
			// Designer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.splitContainer1);
			this.Name = "Designer";
			this.Size = new System.Drawing.Size(758, 455);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			this._tabContainer.ResumeLayout(false);
			this._pageProperty.ResumeLayout(false);
			this._pageEvent.ResumeLayout(false);
			this._pageMethod.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this._eventGrid)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this._methodGrid)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.ComboBox _cbControlList;
		private System.Windows.Forms.TabControl _tabContainer;
		private System.Windows.Forms.TabPage _pageProperty;
		private LPropertyGrid _propGrid;
		private System.Windows.Forms.TabPage _pageEvent;
		private LEventGrid _eventGrid;
		private System.Windows.Forms.DataGridViewTextBoxColumn _colEvent;
		private System.Windows.Forms.DataGridViewTextBoxColumn _colContext;
		private System.Windows.Forms.TabPage _pageMethod;
		private LMethodGrid _methodGrid;
		private System.Windows.Forms.DataGridViewTextBoxColumn _colMethod;
		private DisplayDT _display;
	}
}
