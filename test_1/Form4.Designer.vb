<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form4
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
		Me.components = New System.ComponentModel.Container
		Me.Button1 = New System.Windows.Forms.Button
		Me.Button2 = New System.Windows.Forms.Button
		Me.Button3 = New System.Windows.Forms.Button
		Me.Button4 = New System.Windows.Forms.Button
		Me.Button5 = New System.Windows.Forms.Button
		Me.Button6 = New System.Windows.Forms.Button
		Me.Button7 = New System.Windows.Forms.Button
		Me.OpenFileDialog1 = New System.Windows.Forms.OpenFileDialog
		Me.Button8 = New System.Windows.Forms.Button
		Me.Label4 = New System.Windows.Forms.Label
		Me.TextBox4 = New System.Windows.Forms.TextBox
		Me.Label2 = New System.Windows.Forms.Label
		Me.TextBox2 = New System.Windows.Forms.TextBox
		Me.Label1 = New System.Windows.Forms.Label
		Me.TextBox1 = New System.Windows.Forms.TextBox
		Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
		Me.CheckBox1 = New System.Windows.Forms.CheckBox
		Me.CheckBox2 = New System.Windows.Forms.CheckBox
		Me.Panel2 = New test_1.__panel
		Me.Panel1 = New test_1.__panel
		Me.__panel1 = New test_1.__panel
		Me.Label5 = New System.Windows.Forms.Label
		Me.Label3 = New System.Windows.Forms.Label
		Me.Button9 = New System.Windows.Forms.Button
		Me.SuspendLayout()
		'
		'Button1
		'
		Me.Button1.Location = New System.Drawing.Point(13, 13)
		Me.Button1.Name = "Button1"
		Me.Button1.Size = New System.Drawing.Size(75, 23)
		Me.Button1.TabIndex = 0
		Me.Button1.Text = "open"
		Me.Button1.UseVisualStyleBackColor = True
		'
		'Button2
		'
		Me.Button2.Location = New System.Drawing.Point(13, 51)
		Me.Button2.Name = "Button2"
		Me.Button2.Size = New System.Drawing.Size(75, 23)
		Me.Button2.TabIndex = 1
		Me.Button2.Text = "close"
		Me.Button2.UseVisualStyleBackColor = True
		'
		'Button3
		'
		Me.Button3.Location = New System.Drawing.Point(94, 13)
		Me.Button3.Name = "Button3"
		Me.Button3.Size = New System.Drawing.Size(75, 23)
		Me.Button3.TabIndex = 2
		Me.Button3.Text = "play"
		Me.Button3.UseVisualStyleBackColor = True
		'
		'Button4
		'
		Me.Button4.Location = New System.Drawing.Point(94, 51)
		Me.Button4.Name = "Button4"
		Me.Button4.Size = New System.Drawing.Size(75, 23)
		Me.Button4.TabIndex = 3
		Me.Button4.Text = "stop"
		Me.Button4.UseVisualStyleBackColor = True
		'
		'Button5
		'
		Me.Button5.Location = New System.Drawing.Point(175, 42)
		Me.Button5.Name = "Button5"
		Me.Button5.Size = New System.Drawing.Size(75, 23)
		Me.Button5.TabIndex = 4
		Me.Button5.Text = "pause"
		Me.Button5.UseVisualStyleBackColor = True
		Me.Button5.Visible = False
		'
		'Button6
		'
		Me.Button6.Location = New System.Drawing.Point(175, 60)
		Me.Button6.Name = "Button6"
		Me.Button6.Size = New System.Drawing.Size(75, 23)
		Me.Button6.TabIndex = 5
		Me.Button6.Text = "resume"
		Me.Button6.UseVisualStyleBackColor = True
		Me.Button6.Visible = False
		'
		'Button7
		'
		Me.Button7.Location = New System.Drawing.Point(175, 13)
		Me.Button7.Name = "Button7"
		Me.Button7.Size = New System.Drawing.Size(75, 23)
		Me.Button7.TabIndex = 6
		Me.Button7.Text = "open file..."
		Me.Button7.UseVisualStyleBackColor = True
		'
		'OpenFileDialog1
		'
		Me.OpenFileDialog1.FileName = "OpenFileDialog1"
		'
		'Button8
		'
		Me.Button8.Location = New System.Drawing.Point(138, 115)
		Me.Button8.Name = "Button8"
		Me.Button8.Size = New System.Drawing.Size(75, 23)
		Me.Button8.TabIndex = 18
		Me.Button8.Text = "init format"
		Me.Button8.UseVisualStyleBackColor = True
		'
		'Label4
		'
		Me.Label4.Location = New System.Drawing.Point(135, 92)
		Me.Label4.Name = "Label4"
		Me.Label4.Size = New System.Drawing.Size(85, 20)
		Me.Label4.TabIndex = 17
		Me.Label4.Text = "bitsPerSample"
		Me.Label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		'
		'TextBox4
		'
		Me.TextBox4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
		Me.TextBox4.Location = New System.Drawing.Point(225, 92)
		Me.TextBox4.Name = "TextBox4"
		Me.TextBox4.Size = New System.Drawing.Size(31, 20)
		Me.TextBox4.TabIndex = 16
		Me.TextBox4.Text = "16"
		'
		'Label2
		'
		Me.Label2.Location = New System.Drawing.Point(14, 118)
		Me.Label2.Name = "Label2"
		Me.Label2.Size = New System.Drawing.Size(66, 20)
		Me.Label2.TabIndex = 15
		Me.Label2.Text = "sample rate"
		Me.Label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		'
		'TextBox2
		'
		Me.TextBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
		Me.TextBox2.Location = New System.Drawing.Point(82, 118)
		Me.TextBox2.Name = "TextBox2"
		Me.TextBox2.Size = New System.Drawing.Size(47, 20)
		Me.TextBox2.TabIndex = 14
		Me.TextBox2.Text = "44100"
		'
		'Label1
		'
		Me.Label1.Location = New System.Drawing.Point(14, 92)
		Me.Label1.Name = "Label1"
		Me.Label1.Size = New System.Drawing.Size(62, 20)
		Me.Label1.TabIndex = 13
		Me.Label1.Text = "channels"
		Me.Label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		'
		'TextBox1
		'
		Me.TextBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
		Me.TextBox1.Location = New System.Drawing.Point(82, 92)
		Me.TextBox1.Name = "TextBox1"
		Me.TextBox1.Size = New System.Drawing.Size(47, 20)
		Me.TextBox1.TabIndex = 12
		Me.TextBox1.Text = "2"
		'
		'Timer1
		'
		Me.Timer1.Interval = 50
		'
		'CheckBox1
		'
		Me.CheckBox1.AutoSize = True
		Me.CheckBox1.Location = New System.Drawing.Point(225, 118)
		Me.CheckBox1.Name = "CheckBox1"
		Me.CheckBox1.Size = New System.Drawing.Size(61, 17)
		Me.CheckBox1.TabIndex = 20
		Me.CheckBox1.Text = "reverse"
		Me.CheckBox1.UseVisualStyleBackColor = True
		'
		'CheckBox2
		'
		Me.CheckBox2.AutoSize = True
		Me.CheckBox2.Location = New System.Drawing.Point(262, 92)
		Me.CheckBox2.Name = "CheckBox2"
		Me.CheckBox2.Size = New System.Drawing.Size(55, 17)
		Me.CheckBox2.TabIndex = 21
		Me.CheckBox2.Text = "pause"
		Me.CheckBox2.UseVisualStyleBackColor = True
		'
		'Panel2
		'
		Me.Panel2.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
					Or System.Windows.Forms.AnchorStyles.Left) _
					Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
		Me.Panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.Panel2.Location = New System.Drawing.Point(13, 144)
		Me.Panel2.Name = "Panel2"
		Me.Panel2.Size = New System.Drawing.Size(348, 99)
		Me.Panel2.TabIndex = 19
		'
		'Panel1
		'
		Me.Panel1.Dock = System.Windows.Forms.DockStyle.Bottom
		Me.Panel1.Location = New System.Drawing.Point(10, 304)
		Me.Panel1.Name = "Panel1"
		Me.Panel1.Size = New System.Drawing.Size(351, 12)
		Me.Panel1.TabIndex = 7
		'
		'__panel1
		'
		Me.__panel1.Dock = System.Windows.Forms.DockStyle.Bottom
		Me.__panel1.Location = New System.Drawing.Point(10, 249)
		Me.__panel1.Name = "__panel1"
		Me.__panel1.Size = New System.Drawing.Size(351, 55)
		Me.__panel1.TabIndex = 22
		'
		'Label5
		'
		Me.Label5.AutoSize = True
		Me.Label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.Label5.Font = New System.Drawing.Font("Arial", 12.0!, System.Drawing.FontStyle.Bold)
		Me.Label5.Location = New System.Drawing.Point(256, 34)
		Me.Label5.MinimumSize = New System.Drawing.Size(100, 0)
		Me.Label5.Name = "Label5"
		Me.Label5.Size = New System.Drawing.Size(100, 21)
		Me.Label5.TabIndex = 31
		'
		'Label3
		'
		Me.Label3.AutoSize = True
		Me.Label3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.Label3.Font = New System.Drawing.Font("Arial", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label3.Location = New System.Drawing.Point(256, 13)
		Me.Label3.MinimumSize = New System.Drawing.Size(100, 0)
		Me.Label3.Name = "Label3"
		Me.Label3.Size = New System.Drawing.Size(100, 21)
		Me.Label3.TabIndex = 30
		'
		'Button9
		'
		Me.Button9.Location = New System.Drawing.Point(256, 63)
		Me.Button9.Name = "Button9"
		Me.Button9.Size = New System.Drawing.Size(75, 23)
		Me.Button9.TabIndex = 32
		Me.Button9.Text = "open http..."
		Me.Button9.UseVisualStyleBackColor = True
		'
		'Form4
		'
		Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.ClientSize = New System.Drawing.Size(371, 326)
		Me.Controls.Add(Me.Button9)
		Me.Controls.Add(Me.Label5)
		Me.Controls.Add(Me.Label3)
		Me.Controls.Add(Me.__panel1)
		Me.Controls.Add(Me.CheckBox2)
		Me.Controls.Add(Me.CheckBox1)
		Me.Controls.Add(Me.Panel2)
		Me.Controls.Add(Me.Button8)
		Me.Controls.Add(Me.Label4)
		Me.Controls.Add(Me.TextBox4)
		Me.Controls.Add(Me.Label2)
		Me.Controls.Add(Me.TextBox2)
		Me.Controls.Add(Me.Label1)
		Me.Controls.Add(Me.TextBox1)
		Me.Controls.Add(Me.Panel1)
		Me.Controls.Add(Me.Button7)
		Me.Controls.Add(Me.Button6)
		Me.Controls.Add(Me.Button5)
		Me.Controls.Add(Me.Button4)
		Me.Controls.Add(Me.Button3)
		Me.Controls.Add(Me.Button2)
		Me.Controls.Add(Me.Button1)
		Me.MaximumSize = New System.Drawing.Size(10000, 10000)
		Me.Name = "Form4"
		Me.Padding = New System.Windows.Forms.Padding(10, 0, 10, 10)
		Me.Text = "Form4"
		Me.ResumeLayout(False)
		Me.PerformLayout()

	End Sub
	Friend WithEvents Button1 As System.Windows.Forms.Button
	Friend WithEvents Button2 As System.Windows.Forms.Button
	Friend WithEvents Button3 As System.Windows.Forms.Button
	Friend WithEvents Button4 As System.Windows.Forms.Button
	Friend WithEvents Button5 As System.Windows.Forms.Button
	Friend WithEvents Button6 As System.Windows.Forms.Button
	Friend WithEvents Button7 As System.Windows.Forms.Button
	Friend WithEvents OpenFileDialog1 As System.Windows.Forms.OpenFileDialog
	Friend WithEvents Button8 As System.Windows.Forms.Button
	Friend WithEvents Label4 As System.Windows.Forms.Label
	Friend WithEvents TextBox4 As System.Windows.Forms.TextBox
	Friend WithEvents Label2 As System.Windows.Forms.Label
	Friend WithEvents TextBox2 As System.Windows.Forms.TextBox
	Friend WithEvents Label1 As System.Windows.Forms.Label
	Friend WithEvents TextBox1 As System.Windows.Forms.TextBox
	Friend WithEvents Timer1 As System.Windows.Forms.Timer
	Friend WithEvents Panel1 As test_1.__panel
	Friend WithEvents Panel2 As test_1.__panel
	Friend WithEvents CheckBox1 As System.Windows.Forms.CheckBox
	Friend WithEvents CheckBox2 As System.Windows.Forms.CheckBox
	Friend WithEvents __panel1 As test_1.__panel
	Friend WithEvents Label5 As System.Windows.Forms.Label
	Friend WithEvents Label3 As System.Windows.Forms.Label
	Friend WithEvents Button9 As System.Windows.Forms.Button
End Class
