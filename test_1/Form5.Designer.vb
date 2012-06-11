<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form5
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
		Me.SaveFileDialog1 = New System.Windows.Forms.SaveFileDialog
		Me.Button2 = New System.Windows.Forms.Button
		Me.Button3 = New System.Windows.Forms.Button
		Me.Button8 = New System.Windows.Forms.Button
		Me.Label4 = New System.Windows.Forms.Label
		Me.TextBox4 = New System.Windows.Forms.TextBox
		Me.Label2 = New System.Windows.Forms.Label
		Me.TextBox2 = New System.Windows.Forms.TextBox
		Me.Label1 = New System.Windows.Forms.Label
		Me.TextBox1 = New System.Windows.Forms.TextBox
		Me.Label3 = New System.Windows.Forms.Label
		Me.Label5 = New System.Windows.Forms.Label
		Me.__panel1 = New test_1.__panel
		Me.Panel2 = New test_1.__panel
		Me.SuspendLayout()
		'
		'Button1
		'
		Me.Button1.Location = New System.Drawing.Point(12, 12)
		Me.Button1.Name = "Button1"
		Me.Button1.Size = New System.Drawing.Size(75, 23)
		Me.Button1.TabIndex = 0
		Me.Button1.Text = "create file..."
		Me.Button1.UseVisualStyleBackColor = True
		'
		'SaveFileDialog1
		'
		'
		'Button2
		'
		Me.Button2.Location = New System.Drawing.Point(93, 12)
		Me.Button2.Name = "Button2"
		Me.Button2.Size = New System.Drawing.Size(75, 23)
		Me.Button2.TabIndex = 1
		Me.Button2.Text = "record"
		Me.Button2.UseVisualStyleBackColor = True
		'
		'Button3
		'
		Me.Button3.Location = New System.Drawing.Point(174, 12)
		Me.Button3.Name = "Button3"
		Me.Button3.Size = New System.Drawing.Size(75, 23)
		Me.Button3.TabIndex = 2
		Me.Button3.Text = "stop"
		Me.Button3.UseVisualStyleBackColor = True
		'
		'Button8
		'
		Me.Button8.Location = New System.Drawing.Point(138, 70)
		Me.Button8.Name = "Button8"
		Me.Button8.Size = New System.Drawing.Size(75, 23)
		Me.Button8.TabIndex = 27
		Me.Button8.Text = "init format"
		Me.Button8.UseVisualStyleBackColor = True
		'
		'Label4
		'
		Me.Label4.Location = New System.Drawing.Point(135, 47)
		Me.Label4.Name = "Label4"
		Me.Label4.Size = New System.Drawing.Size(85, 20)
		Me.Label4.TabIndex = 26
		Me.Label4.Text = "bitsPerSample"
		Me.Label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		'
		'TextBox4
		'
		Me.TextBox4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
		Me.TextBox4.Location = New System.Drawing.Point(225, 47)
		Me.TextBox4.Name = "TextBox4"
		Me.TextBox4.Size = New System.Drawing.Size(31, 20)
		Me.TextBox4.TabIndex = 25
		Me.TextBox4.Text = "16"
		'
		'Label2
		'
		Me.Label2.Location = New System.Drawing.Point(14, 73)
		Me.Label2.Name = "Label2"
		Me.Label2.Size = New System.Drawing.Size(66, 20)
		Me.Label2.TabIndex = 24
		Me.Label2.Text = "sample rate"
		Me.Label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		'
		'TextBox2
		'
		Me.TextBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
		Me.TextBox2.Location = New System.Drawing.Point(82, 73)
		Me.TextBox2.Name = "TextBox2"
		Me.TextBox2.Size = New System.Drawing.Size(47, 20)
		Me.TextBox2.TabIndex = 23
		Me.TextBox2.Text = "44100"
		'
		'Label1
		'
		Me.Label1.Location = New System.Drawing.Point(14, 47)
		Me.Label1.Name = "Label1"
		Me.Label1.Size = New System.Drawing.Size(62, 20)
		Me.Label1.TabIndex = 22
		Me.Label1.Text = "channels"
		Me.Label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		'
		'TextBox1
		'
		Me.TextBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
		Me.TextBox1.Location = New System.Drawing.Point(82, 47)
		Me.TextBox1.Name = "TextBox1"
		Me.TextBox1.Size = New System.Drawing.Size(47, 20)
		Me.TextBox1.TabIndex = 21
		Me.TextBox1.Text = "2"
		'
		'Label3
		'
		Me.Label3.AutoSize = True
		Me.Label3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.Label3.Font = New System.Drawing.Font("Arial", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label3.Location = New System.Drawing.Point(272, 12)
		Me.Label3.MinimumSize = New System.Drawing.Size(100, 0)
		Me.Label3.Name = "Label3"
		Me.Label3.Size = New System.Drawing.Size(100, 21)
		Me.Label3.TabIndex = 28
		'
		'Label5
		'
		Me.Label5.AutoSize = True
		Me.Label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.Label5.Font = New System.Drawing.Font("Arial", 12.0!, System.Drawing.FontStyle.Bold)
		Me.Label5.Location = New System.Drawing.Point(272, 33)
		Me.Label5.MinimumSize = New System.Drawing.Size(100, 0)
		Me.Label5.Name = "Label5"
		Me.Label5.Size = New System.Drawing.Size(100, 21)
		Me.Label5.TabIndex = 29
		'
		'__panel1
		'
		Me.__panel1.Dock = System.Windows.Forms.DockStyle.Bottom
		Me.__panel1.Location = New System.Drawing.Point(10, 240)
		Me.__panel1.Name = "__panel1"
		Me.__panel1.Size = New System.Drawing.Size(383, 55)
		Me.__panel1.TabIndex = 30
		'
		'Panel2
		'
		Me.Panel2.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
					Or System.Windows.Forms.AnchorStyles.Left) _
					Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
		Me.Panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.Panel2.Location = New System.Drawing.Point(12, 99)
		Me.Panel2.Name = "Panel2"
		Me.Panel2.Size = New System.Drawing.Size(378, 131)
		Me.Panel2.TabIndex = 20
		'
		'Form5
		'
		Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.ClientSize = New System.Drawing.Size(403, 305)
		Me.Controls.Add(Me.__panel1)
		Me.Controls.Add(Me.Label5)
		Me.Controls.Add(Me.Label3)
		Me.Controls.Add(Me.Button8)
		Me.Controls.Add(Me.Label4)
		Me.Controls.Add(Me.TextBox4)
		Me.Controls.Add(Me.Label2)
		Me.Controls.Add(Me.TextBox2)
		Me.Controls.Add(Me.Label1)
		Me.Controls.Add(Me.TextBox1)
		Me.Controls.Add(Me.Panel2)
		Me.Controls.Add(Me.Button3)
		Me.Controls.Add(Me.Button2)
		Me.Controls.Add(Me.Button1)
		Me.Name = "Form5"
		Me.Padding = New System.Windows.Forms.Padding(10, 0, 10, 10)
		Me.Text = "Form5"
		Me.ResumeLayout(False)
		Me.PerformLayout()

	End Sub
	Friend WithEvents Button1 As System.Windows.Forms.Button
	Friend WithEvents SaveFileDialog1 As System.Windows.Forms.SaveFileDialog
	Friend WithEvents Button2 As System.Windows.Forms.Button
	Friend WithEvents Button3 As System.Windows.Forms.Button
	Friend WithEvents Panel2 As test_1.__panel
	Friend WithEvents Button8 As System.Windows.Forms.Button
	Friend WithEvents Label4 As System.Windows.Forms.Label
	Friend WithEvents TextBox4 As System.Windows.Forms.TextBox
	Friend WithEvents Label2 As System.Windows.Forms.Label
	Friend WithEvents TextBox2 As System.Windows.Forms.TextBox
	Friend WithEvents Label1 As System.Windows.Forms.Label
	Friend WithEvents TextBox1 As System.Windows.Forms.TextBox
	Friend WithEvents Label3 As System.Windows.Forms.Label
	Friend WithEvents Label5 As System.Windows.Forms.Label
	Friend WithEvents __panel1 As test_1.__panel
End Class
