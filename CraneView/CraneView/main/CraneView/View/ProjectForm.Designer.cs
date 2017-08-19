namespace CraneView.View
{
	partial class ProjectForm
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
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this._tabOpen = new System.Windows.Forms.TabPage();
			this._tabCreate = new System.Windows.Forms.TabPage();
			this.dataGridView1 = new System.Windows.Forms.DataGridView();
			this.tabControl1.SuspendLayout();
			this._tabOpen.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
			this.SuspendLayout();
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.Add(this._tabOpen);
			this.tabControl1.Controls.Add(this._tabCreate);
			this.tabControl1.Location = new System.Drawing.Point(12, 12);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(622, 261);
			this.tabControl1.TabIndex = 1;
			// 
			// _tabOpen
			// 
			this._tabOpen.Controls.Add(this.dataGridView1);
			this._tabOpen.Location = new System.Drawing.Point(4, 23);
			this._tabOpen.Name = "_tabOpen";
			this._tabOpen.Padding = new System.Windows.Forms.Padding(3);
			this._tabOpen.Size = new System.Drawing.Size(614, 234);
			this._tabOpen.TabIndex = 0;
			this._tabOpen.Text = "Open";
			this._tabOpen.UseVisualStyleBackColor = true;
			// 
			// _tabCreate
			// 
			this._tabCreate.Location = new System.Drawing.Point(4, 23);
			this._tabCreate.Name = "_tabCreate";
			this._tabCreate.Padding = new System.Windows.Forms.Padding(3);
			this._tabCreate.Size = new System.Drawing.Size(614, 234);
			this._tabCreate.TabIndex = 1;
			this._tabCreate.Text = "Create";
			this._tabCreate.UseVisualStyleBackColor = true;
			// 
			// dataGridView1
			// 
			this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridView1.Location = new System.Drawing.Point(3, 3);
			this.dataGridView1.Name = "dataGridView1";
			this.dataGridView1.RowTemplate.Height = 23;
			this.dataGridView1.Size = new System.Drawing.Size(608, 228);
			this.dataGridView1.TabIndex = 1;
			// 
			// ProjectForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(646, 306);
			this.Controls.Add(this.tabControl1);
			this.Name = "ProjectForm";
			this.Text = "ProjectForm";
			this.Load += new System.EventHandler(this.ProjectCreater_Load);
			this.tabControl1.ResumeLayout(false);
			this._tabOpen.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage _tabOpen;
		private System.Windows.Forms.DataGridView dataGridView1;
		private System.Windows.Forms.TabPage _tabCreate;
	}
}