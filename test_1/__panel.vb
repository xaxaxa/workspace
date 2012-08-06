Public Class __panel
	Inherits Panel
	Protected Overrides Sub OnPaintBackground(ByVal e As System.Windows.Forms.PaintEventArgs)
		If Me.DesignMode Then MyBase.OnPaintBackground(e)
	End Sub
	Protected Overrides Sub OnPaint(ByVal e As System.Windows.Forms.PaintEventArgs)
		MyBase.OnPaint(e)
	End Sub
End Class
