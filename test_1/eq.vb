Public Class eq
	Public equalizer As New _eq(10000, 22050)
	Public sp As Integer = 0
	Private Sub eq_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
		If e.CloseReason = CloseReason.UserClosing Then e.Cancel = True Else e.Cancel = False
	End Sub

	Private Sub eq_KeyDown(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles Me.KeyDown
		If e.KeyData = (Keys.Control Or Keys.A) Then
			Dim i As Integer
			For i = 0 To Me.equalizer.b.Length - 1
				Me.equalizer.b(i) = True
			Next
			Me.Invalidate()
		End If
	End Sub

	Private Sub eq_KeyUp(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles Me.KeyUp
		Me._i = -1
	End Sub

	Private Sub eq_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
		Me.ResizeRedraw = True
	End Sub
	Sub a(ByVal y As Integer)
		Dim pt As Double = (1 - ((y - 5) / (Me.ClientSize.Height - 10 - 16 - 20))) * 2
		If pt < 0 Then pt = 0
		If pt > 2 Then pt = 2
		Dim i As Integer
		For i = 0 To Me.equalizer.b.Length - 1
			If Me.equalizer.b(i) Then
				Me.equalizer._points(i) = pt
			End If
		Next
	End Sub
	Dim _i As Integer = -1
	Private Sub eq_MouseDown(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Me.MouseDown ', Me.MouseMove
		Dim e_x As Integer = e.X
		If e_x < 0 Then e_x = 0
		If e_x >= Me.ClientSize.Width Then e_x = Me.ClientSize.Width - 1
		e_x += Me.sp
		If e.Button = Windows.Forms.MouseButtons.Right Then
			If Control.ModifierKeys = Keys.Control Then
				a(e.Y)
				Me.Invalidate()
				Return
			Else
				Dim i As Integer
				For i = 0 To Me.equalizer.b.Length - 1
					Me.equalizer.b(i) = False
				Next
				Me.Invalidate()
				Return
			End If
		End If
		If Not e.Button = Windows.Forms.MouseButtons.Left Then Return
		Dim l As Double = Math.Truncate(Me.ClientSize.Width / Me.equalizer._points.Length)
		If l < 5 Then l = 5
		Dim __w As Integer = CInt(l * (Me.equalizer._points.Length - 1)) 'Math.Truncate(Me.ClientSize.Width / Me.equalizer._points.Length) * (Me.equalizer._points.Length - 1)
		Dim ind As Integer = Math.Truncate(e_x / __w * (Me.equalizer._points.Length - 1))
		If ind < 0 Then ind = 0
		If ind >= Me.equalizer._points.Length Then ind = Me.equalizer._points.Length - 1
		If Control.ModifierKeys = Keys.Control Then
			Me.equalizer.b(ind) = True
			Me.Invalidate()
			Return
		ElseIf Control.ModifierKeys = Keys.Shift Then
			If Me._i < 0 Then
				Me._i = ind
			Else
				Dim i As Integer
				For i = Me._i To ind Step IIf(Me._i < ind, 1, -1)
					Me.equalizer.b(i) = True
				Next
				Me.Invalidate()
			End If
			Return
		End If
		Dim pt As Double = (1 - ((e.Y - 5) / (Me.ClientSize.Height - 10 - 16 - 20))) * 2
		If pt < 0 Then pt = 0
		If pt > 2 Then pt = 2
		Me.equalizer._points(ind) = pt
		'Me.equalizer.b(ind) = True
		'a(e.Y)
		'Me.equalizer._points(ind) = pt
		Me.Invalidate()
	End Sub
	Dim _ind As Integer = -1
	Private Sub eq_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Me.MouseMove
		If Control.ModifierKeys = Keys.Shift Then Return
		Dim e_x As Integer = e.X
		If e_x < 0 Then e_x = 0
		If e_x >= Me.ClientSize.Width Then e_x = Me.ClientSize.Width - 1
		e_x += Me.sp
		If e.Button = Windows.Forms.MouseButtons.Right Then
			If Control.ModifierKeys = Keys.Control Then
				a(e.Y)
				Me.Invalidate()
				Return
			End If
		End If
		If e.Button = Windows.Forms.MouseButtons.Left Then
			Dim l As Double = Math.Truncate(Me.ClientSize.Width / Me.equalizer._points.Length)
			If l < 5 Then l = 5
			Dim __w As Integer = CInt(l * (Me.equalizer._points.Length - 1)) 'Math.Truncate(Me.ClientSize.Width / Me.equalizer._points.Length) * (Me.equalizer._points.Length - 1)
			Dim ind As Integer = Math.Truncate(e_x / __w * (Me.equalizer._points.Length - 1))
			If ind < 0 Then ind = 0
			If ind >= Me.equalizer._points.Length Then ind = Me.equalizer._points.Length - 1
			If Control.ModifierKeys = Keys.Control Then
				If Not _ind = -1 Then
					Dim i As Integer
					For i = _ind To ind Step IIf(_ind < ind, 1, -1)
						Me.equalizer.b(i) = True
					Next
				Else
					Me.equalizer.b(ind) = True
				End If
				_ind = ind
				Me.Invalidate()
				Return
			Else
				Dim pt As Double = (1 - ((e.Y - 5) / (Me.ClientSize.Height - 10 - 16 - 20))) * 2
				If pt < 0 Then pt = 0
				If pt > 2 Then pt = 2
				If Not _ind = -1 Then
					Dim i As Integer
					For i = _ind To ind Step IIf(_ind < ind, 1, -1)
						Me.equalizer._points(i) = pt
					Next
				Else
					Me.equalizer._points(ind) = pt
				End If
				_ind = ind
				'Me.equalizer.b(ind) = True
				'a(e.Y)
				'Me.equalizer._points(ind) = pt
				Me.Invalidate()
				Return
				End If
			End If
	End Sub

	Private Sub eq_MouseUp(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Me.MouseUp
		_ind = -1
		Return
		Dim i As Integer
		For i = 0 To Me.equalizer.b.Length - 1
			Me.equalizer.b(i) = False
		Next
	End Sub

	Private Sub eq_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Me.Paint
		Dim l As Double = Math.Truncate(Me.ClientSize.Width / Me.equalizer._points.Length)
		If l < 5 Then l = 5
		Dim g As System.Drawing.Graphics = e.Graphics
		g.TranslateTransform(-Me.sp, 0)
		Dim i As Integer, ii As Double = 0
		Dim h As Integer = Me.ClientSize.Height - 10 - 16 - 20
		Dim br As Drawing.Brush = Brushes.Black
		Dim br2 As Drawing.Brush = Brushes.Red
		Dim br_2 As Drawing.Brush = New SolidBrush(Color.FromArgb(255, 40, 40, 40))	'Brushes.DarkGray
		Dim br2_2 As Drawing.Brush = New SolidBrush(Color.FromArgb(255, 255, 40, 40))
		Dim _bg1 As Drawing.Brush = New SolidBrush(Color.FromArgb(255, 210, 210, 230))
		Dim txt_pos As Integer = -10
		Dim tmp_pos As Integer = Math.Truncate(Me.sp / l)
		ii = tmp_pos * l
		For i = tmp_pos To Me.equalizer._points.Length - 1
			If i Mod 2 = 0 Then
				g.FillRectangle(IIf(Me.equalizer.b(i), br2_2, br_2), CInt(ii), CInt(h - (Me.equalizer._points(i) / 2) * h) + 5, CInt(l), h - (CInt(h - (Me.equalizer._points(i) / 2) * h)))
				g.FillRectangle(Brushes.White, CInt(ii), 5, CInt(l), CInt(h - (Me.equalizer._points(i) / 2) * h))
			Else
				g.FillRectangle(IIf(Me.equalizer.b(i), br2, br), CInt(ii), CInt(h - (Me.equalizer._points(i) / 2) * h) + 5, CInt(l), h - (CInt(h - (Me.equalizer._points(i) / 2) * h)))
				g.FillRectangle(_bg1, CInt(ii), 5, CInt(l), CInt(h - (Me.equalizer._points(i) / 2) * h))
			End If
			If ii > txt_pos Then
				'draw text
				Dim s As String = CInt(i / (Me.equalizer._points.Length - 1) * Me.equalizer.max_freq + (Me.equalizer.max_freq / (Me.equalizer._points.Length - 1) / 2)).ToString()
				Dim szf As Drawing.SizeF = g.MeasureString(s, Me.Font)
				txt_pos = ii + szf.Width
				g.DrawString(s, Me.Font, Brushes.Black, csng(ii), csng(h + 5 + 5))
				g.DrawLine(Pens.Black, CInt(ii + l / 2), h + 5, CInt(ii + l / 2), h + 5 + 4)
			End If
			'If l > 2 Then g.DrawLine(Pens.White, ii, 5, ii, h)
			'g.DrawLine(Pens.Black, ii, CInt(h - Me.equalizer._points(i) * h) + 5, ii + l, CInt(h - Me.equalizer._points(i) * h) + 5)
			ii += l
			If ii - Me.sp > Me.ClientSize.Width Then Return
		Next
	End Sub
	Public Sub process(ByVal d As Double(), ByVal sampleRate As Integer)
		wave_lib2.FFTFilter.Process(d, Me.equalizer, sampleRate)
	End Sub

	Public Sub New()
		Me.SetStyle(ControlStyles.AllPaintingInWmPaint Or ControlStyles.ResizeRedraw Or ControlStyles.OptimizedDoubleBuffer Or ControlStyles.UserPaint, True)
		'Me.SetStyle(ControlStyles.SupportsTransparentBackColor, True)
		' This call is required by the Windows Form Designer.
		InitializeComponent()

		' Add any initialization after the InitializeComponent() call.

	End Sub

	Private Sub eq_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Resize
		Dim l As Double = Math.Truncate(Me.ClientSize.Width / Me.equalizer._points.Length)
		If l < 5 Then l = 5
		Dim __w As Integer = CInt(l * (Me.equalizer._points.Length - 1)) 'Math.Truncate(Me.ClientSize.Width / Me.equalizer._points.Length) * (Me.equalizer._points.Length - 1)
		Me.HScrollBar1.Maximum = __w
		Me.HScrollBar1.LargeChange = Me.HScrollBar1.ClientSize.Width
	End Sub

	Private Sub HScrollBar1_Scroll(ByVal sender As Object, ByVal e As System.Windows.Forms.ScrollEventArgs) Handles HScrollBar1.Scroll
		Me.sp = e.NewValue
		Me.Invalidate()
	End Sub
End Class
Public Class _eq
	Implements wave_lib2.IEqualizer
	Public _points() As Double
	Public max_freq As Integer
	Public b As Boolean()
	Public Function GetPoint(ByVal freq As UInteger) As Double Implements wave_lib2.IEqualizer.GetPoint
		Dim ind As Integer = CInt(CDbl(freq) / max_freq * (_points.Length - 1))
		If ind >= _points.Length Then ind = _points.Length - 1
		Return _points(ind)
	End Function

	Public Sub New(ByVal size As Integer, ByVal max_freq As Integer)
		Me.max_freq = max_freq
		ReDim _points(size - 1)
		ReDim b(size - 1)
		Dim i As Integer
		For i = 0 To size - 1
			_points(i) = 1.0 '0.5
		Next
	End Sub
End Class