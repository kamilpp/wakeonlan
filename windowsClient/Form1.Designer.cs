namespace WinClient
{
    partial class Form1
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
            this.textMAC = new System.Windows.Forms.TextBox();
            this.textPort = new System.Windows.Forms.TextBox();
            this.textIP = new System.Windows.Forms.TextBox();
            this.buttonWOL = new System.Windows.Forms.Button();
            this.buttonShutdown = new System.Windows.Forms.Button();
            this.textIPBroadcast = new System.Windows.Forms.TextBox();
            this.InfoBox = new System.Windows.Forms.RichTextBox();
            this.textPortShut = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // textMAC
            // 
            this.textMAC.Location = new System.Drawing.Point(17, 32);
            this.textMAC.Name = "textMAC";
            this.textMAC.Size = new System.Drawing.Size(133, 20);
            this.textMAC.TabIndex = 5;
            this.textMAC.Text = "00:e0:4c:02:c5:bb";
            // 
            // textPort
            // 
            this.textPort.Location = new System.Drawing.Point(105, 6);
            this.textPort.Name = "textPort";
            this.textPort.Size = new System.Drawing.Size(45, 20);
            this.textPort.TabIndex = 2;
            this.textPort.Text = "12345";
            // 
            // textIP
            // 
            this.textIP.Location = new System.Drawing.Point(17, 87);
            this.textIP.Name = "textIP";
            this.textIP.Size = new System.Drawing.Size(82, 20);
            this.textIP.TabIndex = 1;
            this.textIP.Text = "192.168.0.12";
            // 
            // buttonWOL
            // 
            this.buttonWOL.Location = new System.Drawing.Point(17, 58);
            this.buttonWOL.Name = "buttonWOL";
            this.buttonWOL.Size = new System.Drawing.Size(133, 23);
            this.buttonWOL.TabIndex = 3;
            this.buttonWOL.Text = "Send WOL";
            this.buttonWOL.UseVisualStyleBackColor = true;
            this.buttonWOL.Click += new System.EventHandler(this.buttonWOL_Click);
            // 
            // buttonShutdown
            // 
            this.buttonShutdown.Location = new System.Drawing.Point(17, 113);
            this.buttonShutdown.Name = "buttonShutdown";
            this.buttonShutdown.Size = new System.Drawing.Size(133, 23);
            this.buttonShutdown.TabIndex = 6;
            this.buttonShutdown.Text = "Send shutdown";
            this.buttonShutdown.UseVisualStyleBackColor = true;
            this.buttonShutdown.Click += new System.EventHandler(this.buttonShutdown_Click);
            // 
            // textIPBroadcast
            // 
            this.textIPBroadcast.Location = new System.Drawing.Point(17, 6);
            this.textIPBroadcast.Name = "textIPBroadcast";
            this.textIPBroadcast.Size = new System.Drawing.Size(82, 20);
            this.textIPBroadcast.TabIndex = 7;
            this.textIPBroadcast.Text = "192.168.0.255";
            // 
            // InfoBox
            // 
            this.InfoBox.Location = new System.Drawing.Point(156, 6);
            this.InfoBox.Name = "InfoBox";
            this.InfoBox.Size = new System.Drawing.Size(160, 130);
            this.InfoBox.TabIndex = 8;
            this.InfoBox.Text = "";
            // 
            // textPortShut
            // 
            this.textPortShut.Location = new System.Drawing.Point(105, 87);
            this.textPortShut.Name = "textPortShut";
            this.textPortShut.Size = new System.Drawing.Size(45, 20);
            this.textPortShut.TabIndex = 9;
            this.textPortShut.Text = "1500";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(334, 146);
            this.Controls.Add(this.textPortShut);
            this.Controls.Add(this.InfoBox);
            this.Controls.Add(this.textIPBroadcast);
            this.Controls.Add(this.buttonShutdown);
            this.Controls.Add(this.textMAC);
            this.Controls.Add(this.textPort);
            this.Controls.Add(this.buttonWOL);
            this.Controls.Add(this.textIP);
            this.MaximumSize = new System.Drawing.Size(403, 266);
            this.Name = "Form1";
            this.Text = "wol&shutdown client";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonWOL;
        private System.Windows.Forms.TextBox textIP;
        private System.Windows.Forms.TextBox textPort;
        private System.Windows.Forms.TextBox textMAC;
        private System.Windows.Forms.Button buttonShutdown;
        private System.Windows.Forms.TextBox textIPBroadcast;
        private System.Windows.Forms.RichTextBox InfoBox;
        private System.Windows.Forms.TextBox textPortShut;
    }
}

