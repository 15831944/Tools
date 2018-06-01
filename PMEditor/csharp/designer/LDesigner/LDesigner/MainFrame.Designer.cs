using LDisplay;
namespace LDesigner
{
	partial class MainFrame
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

		#region Windows 窗体设计器生成的代码

		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		private void InitializeComponent()
		{
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this._btTestRun = new System.Windows.Forms.Button();
            this._btGrid = new System.Windows.Forms.Button();
            this._btPicture = new System.Windows.Forms.Button();
            this._btComboBox = new System.Windows.Forms.Button();
            this._btRadioButton = new System.Windows.Forms.Button();
            this._btCheckBox = new System.Windows.Forms.Button();
            this._btTextBox = new System.Windows.Forms.Button();
            this._btButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.splitContainer1.Panel1.Controls.Add(this._btTestRun);
            this.splitContainer1.Panel1.Controls.Add(this._btGrid);
            this.splitContainer1.Panel1.Controls.Add(this._btPicture);
            this.splitContainer1.Panel1.Controls.Add(this._btComboBox);
            this.splitContainer1.Panel1.Controls.Add(this._btRadioButton);
            this.splitContainer1.Panel1.Controls.Add(this._btCheckBox);
            this.splitContainer1.Panel1.Controls.Add(this._btTextBox);
            this.splitContainer1.Panel1.Controls.Add(this._btButton);
            this.splitContainer1.Size = new System.Drawing.Size(1204, 623);
            this.splitContainer1.SplitterDistance = 149;
            this.splitContainer1.SplitterWidth = 5;
            this.splitContainer1.TabIndex = 0;
            // 
            // _btTestRun
            // 
            this._btTestRun.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._btTestRun.Location = new System.Drawing.Point(16, 576);
            this._btTestRun.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this._btTestRun.Name = "_btTestRun";
            this._btTestRun.Size = new System.Drawing.Size(117, 31);
            this._btTestRun.TabIndex = 0;
            this._btTestRun.Text = "TestRun";
            this._btTestRun.UseVisualStyleBackColor = true;
            this._btTestRun.Click += new System.EventHandler(this._btTestRun_Click);
            // 
            // _btGrid
            // 
            this._btGrid.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._btGrid.Location = new System.Drawing.Point(16, 248);
            this._btGrid.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this._btGrid.Name = "_btGrid";
            this._btGrid.Size = new System.Drawing.Size(117, 31);
            this._btGrid.TabIndex = 0;
            this._btGrid.Text = "DataGrid";
            this._btGrid.UseVisualStyleBackColor = true;
            this._btGrid.Click += new System.EventHandler(this._btGrid_Click);
            // 
            // _btPicture
            // 
            this._btPicture.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._btPicture.Location = new System.Drawing.Point(16, 209);
            this._btPicture.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this._btPicture.Name = "_btPicture";
            this._btPicture.Size = new System.Drawing.Size(117, 31);
            this._btPicture.TabIndex = 0;
            this._btPicture.Text = "Picture";
            this._btPicture.UseVisualStyleBackColor = true;
            this._btPicture.Click += new System.EventHandler(this._btPicture_Click);
            // 
            // _btComboBox
            // 
            this._btComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._btComboBox.Location = new System.Drawing.Point(16, 171);
            this._btComboBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this._btComboBox.Name = "_btComboBox";
            this._btComboBox.Size = new System.Drawing.Size(117, 31);
            this._btComboBox.TabIndex = 0;
            this._btComboBox.Text = "ComboBox";
            this._btComboBox.UseVisualStyleBackColor = true;
            this._btComboBox.Click += new System.EventHandler(this._btComboBox_Click);
            // 
            // _btRadioButton
            // 
            this._btRadioButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._btRadioButton.Location = new System.Drawing.Point(16, 132);
            this._btRadioButton.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this._btRadioButton.Name = "_btRadioButton";
            this._btRadioButton.Size = new System.Drawing.Size(117, 31);
            this._btRadioButton.TabIndex = 0;
            this._btRadioButton.Text = "RadioButton";
            this._btRadioButton.UseVisualStyleBackColor = true;
            this._btRadioButton.Click += new System.EventHandler(this._btRadioButton_Click);
            // 
            // _btCheckBox
            // 
            this._btCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._btCheckBox.Location = new System.Drawing.Point(16, 93);
            this._btCheckBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this._btCheckBox.Name = "_btCheckBox";
            this._btCheckBox.Size = new System.Drawing.Size(117, 31);
            this._btCheckBox.TabIndex = 0;
            this._btCheckBox.Text = "CheckBox";
            this._btCheckBox.UseVisualStyleBackColor = true;
            this._btCheckBox.Click += new System.EventHandler(this._btCheckBox_Click);
            // 
            // _btTextBox
            // 
            this._btTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._btTextBox.Location = new System.Drawing.Point(16, 55);
            this._btTextBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this._btTextBox.Name = "_btTextBox";
            this._btTextBox.Size = new System.Drawing.Size(117, 31);
            this._btTextBox.TabIndex = 0;
            this._btTextBox.Text = "TextBox";
            this._btTextBox.UseVisualStyleBackColor = true;
            this._btTextBox.Click += new System.EventHandler(this._btTextBox_Click);
            // 
            // _btButton
            // 
            this._btButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._btButton.Location = new System.Drawing.Point(16, 16);
            this._btButton.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this._btButton.Name = "_btButton";
            this._btButton.Size = new System.Drawing.Size(117, 31);
            this._btButton.TabIndex = 0;
            this._btButton.Text = "Button";
            this._btButton.UseVisualStyleBackColor = true;
            this._btButton.Click += new System.EventHandler(this._btButton_Click);
            // 
            // MainFrame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1204, 623);
            this.Controls.Add(this.splitContainer1);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "MainFrame";
            this.Text = "Form1";
            this.splitContainer1.Panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.Button _btTestRun;
		private System.Windows.Forms.Button _btPicture;
		private System.Windows.Forms.Button _btComboBox;
		private System.Windows.Forms.Button _btRadioButton;
		private System.Windows.Forms.Button _btCheckBox;
		private System.Windows.Forms.Button _btTextBox;
		private System.Windows.Forms.Button _btButton;
        private System.Windows.Forms.Button _btGrid;
	}
}

