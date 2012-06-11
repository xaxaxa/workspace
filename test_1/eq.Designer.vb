<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class eq
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
		Me.HScrollBar1 = New System.Windows.Forms.HScrollBar
		Me.SuspendLayout()
		'
		'HScrollBar1
		'
		Me.HScrollBar1.Dock = System.Windows.Forms.DockStyle.Bottom
		Me.HScrollBar1.Location = New System.Drawing.Point(0, 280)
		Me.HScrollBar1.Name = "HScrollBar1"
		Me.HScrollBar1.Size = New System.Drawing.Size(337, 16)
		Me.HScrollBar1.TabIndex = 0
		'
		'eq
		'
		Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.BackColor = System.Drawing.Color.FromArgb(CType(CType(210, Byte), Integer), CType(CType(210, Byte), Integer), CType(CType(230, Byte), Integer))
		Me.ClientSize = New System.Drawing.Size(337, 296)
		Me.Controls.Add(Me.HScrollBar1)
		Me.Name = "eq"
		Me.Text = "eq"
		Me.ResumeLayout(False)

	End Sub
	Friend WithEvents HScrollBar1 As System.Windows.Forms.HScrollBar
End Class
