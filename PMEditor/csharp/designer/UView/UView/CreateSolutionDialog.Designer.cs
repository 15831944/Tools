namespace UView
{
    partial class CreateSolutionDialog
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
            this.lbName = new System.Windows.Forms.Label();
            this.btBrowsePath = new System.Windows.Forms.Button();
            this.txtName = new System.Windows.Forms.TextBox();
            this.lbPath = new System.Windows.Forms.Label();
            this.txtPath = new System.Windows.Forms.TextBox();
            this.btOK = new System.Windows.Forms.Button();
            this.btCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lbName
            // 
            this.lbName.AutoSize = true;
            this.lbName.Location = new System.Drawing.Point(16, 12);
            this.lbName.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbName.Name = "lbName";
            this.lbName.Size = new System.Drawing.Size(45, 17);
            this.lbName.TabIndex = 0;
            this.lbName.Text = "Name";
            // 
            // btBrowsePath
            // 
            this.btBrowsePath.Location = new System.Drawing.Point(637, 44);
            this.btBrowsePath.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btBrowsePath.Name = "btBrowsePath";
            this.btBrowsePath.Size = new System.Drawing.Size(55, 28);
            this.btBrowsePath.TabIndex = 2;
            this.btBrowsePath.Text = "...";
            this.btBrowsePath.UseVisualStyleBackColor = true;
            this.btBrowsePath.Click += new System.EventHandler(this.btBrowsePath_Click);
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(76, 8);
            this.txtName.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(552, 22);
            this.txtName.TabIndex = 0;
            // 
            // lbPath
            // 
            this.lbPath.AutoSize = true;
            this.lbPath.Location = new System.Drawing.Point(16, 48);
            this.lbPath.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lbPath.Name = "lbPath";
            this.lbPath.Size = new System.Drawing.Size(37, 17);
            this.lbPath.TabIndex = 0;
            this.lbPath.Text = "Path";
            // 
            // txtPath
            // 
            this.txtPath.Location = new System.Drawing.Point(76, 44);
            this.txtPath.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txtPath.Name = "txtPath";
            this.txtPath.Size = new System.Drawing.Size(552, 22);
            this.txtPath.TabIndex = 1;
            // 
            // btOK
            // 
            this.btOK.Location = new System.Drawing.Point(497, 85);
            this.btOK.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btOK.Name = "btOK";
            this.btOK.Size = new System.Drawing.Size(93, 28);
            this.btOK.TabIndex = 3;
            this.btOK.Text = "OK";
            this.btOK.UseVisualStyleBackColor = true;
            this.btOK.Click += new System.EventHandler(this.btOK_Click);
            // 
            // btCancel
            // 
            this.btCancel.Location = new System.Drawing.Point(599, 85);
            this.btCancel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btCancel.Name = "btCancel";
            this.btCancel.Size = new System.Drawing.Size(93, 28);
            this.btCancel.TabIndex = 4;
            this.btCancel.Text = "Cancel";
            this.btCancel.UseVisualStyleBackColor = true;
            this.btCancel.Click += new System.EventHandler(this.btCancel_Click);
            // 
            // CreateSolutionDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(701, 129);
            this.Controls.Add(this.txtPath);
            this.Controls.Add(this.txtName);
            this.Controls.Add(this.lbPath);
            this.Controls.Add(this.btCancel);
            this.Controls.Add(this.btOK);
            this.Controls.Add(this.btBrowsePath);
            this.Controls.Add(this.lbName);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "CreateSolutionDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CreateSolution";
            this.Load += new System.EventHandler(this.CreateSolutionDialog_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbName;
        private System.Windows.Forms.Button btBrowsePath;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.Label lbPath;
        private System.Windows.Forms.TextBox txtPath;
        private System.Windows.Forms.Button btOK;
        private System.Windows.Forms.Button btCancel;
    }
}