namespace CraneView.View
{
	partial class AddComponentForm
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
			this._btOK = new System.Windows.Forms.Button();
			this._btCancel = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this._txtName = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// _btOK
			// 
			this._btOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this._btOK.Location = new System.Drawing.Point(200, 48);
			this._btOK.Name = "_btOK";
			this._btOK.Size = new System.Drawing.Size(54, 23);
			this._btOK.TabIndex = 0;
			this._btOK.Text = "OK";
			this._btOK.UseVisualStyleBackColor = true;
			this._btOK.Click += new System.EventHandler(this._btOK_Click);
			// 
			// _btCancel
			// 
			this._btCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this._btCancel.Location = new System.Drawing.Point(260, 48);
			this._btCancel.Name = "_btCancel";
			this._btCancel.Size = new System.Drawing.Size(54, 23);
			this._btCancel.TabIndex = 0;
			this._btCancel.Text = "Cancel";
			this._btCancel.UseVisualStyleBackColor = true;
			this._btCancel.Click += new System.EventHandler(this._btCancel_Click);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 19);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(29, 12);
			this.label1.TabIndex = 1;
			this.label1.Text = "Name";
			// 
			// _txtName
			// 
			this._txtName.Location = new System.Drawing.Point(47, 15);
			this._txtName.Name = "_txtName";
			this._txtName.Size = new System.Drawing.Size(267, 21);
			this._txtName.TabIndex = 2;
			// 
			// AddComponentForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(326, 83);
			this.Controls.Add(this._txtName);
			this.Controls.Add(this.label1);
			this.Controls.Add(this._btCancel);
			this.Controls.Add(this._btOK);
			this.Name = "AddComponentForm";
			this.Text = "AddComponentForm";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button _btOK;
		private System.Windows.Forms.Button _btCancel;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox _txtName;
	}
}