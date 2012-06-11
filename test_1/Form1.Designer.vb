<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing AndAlso components IsNot Nothing Then
            components.Dispose()
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
		Me.Button1 = New System.Windows.Forms.Button
		Me.Button2 = New System.Windows.Forms.Button
		Me.Button3 = New System.Windows.Forms.Button
		Me.Button4 = New System.Windows.Forms.Button
		Me.OpenFileDialog1 = New System.Windows.Forms.OpenFileDialog
		Me.Panel1 = New System.Windows.Forms.Panel
		Me.TextBox1 = New System.Windows.Forms.TextBox
		Me.Label1 = New System.Windows.Forms.Label
		Me.Label2 = New System.Windows.Forms.Label
		Me.TextBox2 = New System.Windows.Forms.TextBox
		Me.Label4 = New System.Windows.Forms.Label
		Me.TextBox4 = New System.Windows.Forms.TextBox
		Me.Button5 = New System.Windows.Forms.Button
		Me.Panel2 = New System.Windows.Forms.Panel
		Me.Panel2.SuspendLayout()
		Me.SuspendLayout()
		'
		'Button1
		'
		Me.Button1.Location = New System.Drawing.Point(0, 29)
		Me.Button1.Name = "Button1"
		Me.Button1.Size = New System.Drawing.Size(75, 23)
		Me.Button1.TabIndex = 0
		Me.Button1.Text = "open"
		Me.Button1.UseVisualStyleBackColor = True
		'
		'Button2
		'
		Me.Button2.Location = New System.Drawing.Point(0, 0)
		Me.Button2.Name = "Button2"
		Me.Button2.Size = New System.Drawing.Size(75, 23)
		Me.Button2.TabIndex = 1
		Me.Button2.Text = "play"
		Me.Button2.UseVisualStyleBackColor = True
		'
		'Button3
		'
		Me.Button3.Location = New System.Drawing.Point(81, 0)
		Me.Button3.Name = "Button3"
		Me.Button3.Size = New System.Drawing.Size(75, 23)
		Me.Button3.TabIndex = 2
		Me.Button3.Text = "stop"
		Me.Button3.UseVisualStyleBackColor = True
		'
		'Button4
		'
		Me.Button4.Location = New System.Drawing.Point(81, 29)
		Me.Button4.Name = "Button4"
		Me.Button4.Size = New System.Drawing.Size(75, 23)
		Me.Button4.TabIndex = 3
		Me.Button4.Text = "open file..."
		Me.Button4.UseVisualStyleBackColor = True
		'
		'OpenFileDialog1
		'
		Me.OpenFileDialog1.FileName = "OpenFileDialog1"
		'
		'Panel1
		'
		Me.Panel1.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
					Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
		Me.Panel1.Location = New System.Drawing.Point(12, 190)
		Me.Panel1.Name = "Panel1"
		Me.Panel1.Size = New System.Drawing.Size(406, 12)
		Me.Panel1.TabIndex = 4
		'
		'TextBox1
		'
		Me.TextBox1.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
		Me.TextBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
		Me.TextBox1.Location = New System.Drawing.Point(80, 208)
		Me.TextBox1.Name = "TextBox1"
		Me.TextBox1.Size = New System.Drawing.Size(47, 20)
		Me.TextBox1.TabIndex = 5
		Me.TextBox1.Text = "2"
		'
		'Label1
		'
		Me.Label1.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
		Me.Label1.Location = New System.Drawing.Point(12, 208)
		Me.Label1.Name = "Label1"
		Me.Label1.Size = New System.Drawing.Size(62, 20)
		Me.Label1.TabIndex = 6
		Me.Label1.Text = "channels"
		Me.Label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		'
		'Label2
		'
		Me.Label2.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
		Me.Label2.Location = New System.Drawing.Point(12, 234)
		Me.Label2.Name = "Label2"
		Me.Label2.Size = New System.Drawing.Size(66, 20)
		Me.Label2.TabIndex = 8
		Me.Label2.Text = "sample rate"
		Me.Label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		'
		'TextBox2
		'
		Me.TextBox2.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
		Me.TextBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
		Me.TextBox2.Location = New System.Drawing.Point(80, 234)
		Me.TextBox2.Name = "TextBox2"
		Me.TextBox2.Size = New System.Drawing.Size(47, 20)
		Me.TextBox2.TabIndex = 7
		Me.TextBox2.Text = "44100"
		'
		'Label4
		'
		Me.Label4.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
		Me.Label4.Location = New System.Drawing.Point(133, 208)
		Me.Label4.Name = "Label4"
		Me.Label4.Size = New System.Drawing.Size(85, 20)
		Me.Label4.TabIndex = 10
		Me.Label4.Text = "bitsPerSample"
		Me.Label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		'
		'TextBox4
		'
		Me.TextBox4.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
		Me.TextBox4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
		Me.TextBox4.Location = New System.Drawing.Point(223, 208)
		Me.TextBox4.Name = "TextBox4"
		Me.TextBox4.Size = New System.Drawing.Size(31, 20)
		Me.TextBox4.TabIndex = 9
		Me.TextBox4.Text = "16"
		'
		'Button5
		'
		Me.Button5.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
		Me.Button5.Location = New System.Drawing.Point(136, 231)
		Me.Button5.Name = "Button5"
		Me.Button5.Size = New System.Drawing.Size(75, 23)
		Me.Button5.TabIndex = 11
		Me.Button5.Text = "init format"
		Me.Button5.UseVisualStyleBackColor = True
		'
		'Panel2
		'
		Me.Panel2.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
		Me.Panel2.Controls.Add(Me.Button2)
		Me.Panel2.Controls.Add(Me.Button1)
		Me.Panel2.Controls.Add(Me.Button3)
		Me.Panel2.Controls.Add(Me.Button4)
		Me.Panel2.Location = New System.Drawing.Point(262, 208)
		Me.Panel2.Name = "Panel2"
		Me.Panel2.Size = New System.Drawing.Size(170, 66)
		Me.Panel2.TabIndex = 12
		'
		'Form1
		'
		Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.ClientSize = New System.Drawing.Size(430, 272)
		Me.Controls.Add(Me.Panel2)
		Me.Controls.Add(Me.Button5)
		Me.Controls.Add(Me.Label4)
		Me.Controls.Add(Me.TextBox4)
		Me.Controls.Add(Me.Label2)
		Me.Controls.Add(Me.TextBox2)
		Me.Controls.Add(Me.Label1)
		Me.Controls.Add(Me.TextBox1)
		Me.Controls.Add(Me.Panel1)
		Me.Name = "Form1"
		Me.Text = "Form1"
		Me.Panel2.ResumeLayout(False)
		Me.ResumeLayout(False)
		Me.PerformLayout()

	End Sub
	Friend WithEvents Button1 As System.Windows.Forms.Button
	Friend WithEvents Button2 As System.Windows.Forms.Button
	Friend WithEvents Button3 As System.Windows.Forms.Button
	Friend WithEvents Button4 As System.Windows.Forms.Button
	Friend WithEvents OpenFileDialog1 As System.Windows.Forms.OpenFileDialog
	Friend WithEvents Panel1 As System.Windows.Forms.Panel
	Friend WithEvents TextBox1 As System.Windows.Forms.TextBox
	Friend WithEvents Label1 As System.Windows.Forms.Label
	Friend WithEvents Label2 As System.Windows.Forms.Label
	Friend WithEvents TextBox2 As System.Windows.Forms.TextBox
	Friend WithEvents Label4 As System.Windows.Forms.Label
	Friend WithEvents TextBox4 As System.Windows.Forms.TextBox
	Friend WithEvents Button5 As System.Windows.Forms.Button
	Friend WithEvents Panel2 As System.Windows.Forms.Panel

End Class
