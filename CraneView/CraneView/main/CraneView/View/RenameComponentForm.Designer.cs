namespace CraneView
{
    partial class RenameComponentForm
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
            this._txtCurName = new System.Windows.Forms.TextBox();
            this._txtNewName = new System.Windows.Forms.TextBox();
            this._btCancel = new System.Windows.Forms.Button();
            this._btOK = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // _txtCurName
            // 
            this._txtCurName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._txtCurName.Location = new System.Drawing.Point(118, 29);
            this._txtCurName.Name = "_txtCurName";
            this._txtCurName.ReadOnly = true;
            this._txtCurName.Size = new System.Drawing.Size(282, 23);
            this._txtCurName.TabIndex = 3;
            // 
            // _txtNewName
            // 
            this._txtNewName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._txtNewName.Location = new System.Drawing.Point(118, 69);
            this._txtNewName.Name = "_txtNewName";
            this._txtNewName.Size = new System.Drawing.Size(282, 23);
            this._txtNewName.TabIndex = 0;
            // 
            // _btCancel
            // 
            this._btCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this._btCancel.Location = new System.Drawing.Point(337, 113);
            this._btCancel.Margin = new System.Windows.Forms.Padding(4);
            this._btCancel.Name = "_btCancel";
            this._btCancel.Size = new System.Drawing.Size(63, 31);
            this._btCancel.TabIndex = 2;
            this._btCancel.Text = "Cancel";
            this._btCancel.UseVisualStyleBackColor = true;
            // 
            // _btOK
            // 
            this._btOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this._btOK.Location = new System.Drawing.Point(267, 113);
            this._btOK.Margin = new System.Windows.Forms.Padding(4);
            this._btOK.Name = "_btOK";
            this._btOK.Size = new System.Drawing.Size(63, 31);
            this._btOK.TabIndex = 1;
            this._btOK.Text = "OK";
            this._btOK.UseVisualStyleBackColor = true;
            this._btOK.Click += new System.EventHandler(this._btOK_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(22, 32);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 17);
            this.label1.TabIndex = 5;
            this.label1.Text = "Curent Name";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 72);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(73, 17);
            this.label2.TabIndex = 6;
            this.label2.Text = "New Name";
            // 
            // RenameComponentForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(424, 157);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this._btCancel);
            this.Controls.Add(this._btOK);
            this.Controls.Add(this._txtNewName);
            this.Controls.Add(this._txtCurName);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "RenameComponentForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "RenameComponentForm";
            this.Load += new System.EventHandler(this.RenameComponentForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox _txtCurName;
        private System.Windows.Forms.TextBox _txtNewName;
        private System.Windows.Forms.Button _btCancel;
        private System.Windows.Forms.Button _btOK;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
    }
}