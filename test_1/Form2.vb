Public Class Form2
	Dim _b2 As Byte()
	Dim _b As Short()
	Dim _c As wave_lib2.complex()
	Dim _s As Single()
	Dim bmp_sync As New Object()
	Dim bmp As System.Drawing.Bitmap
	Dim _g As System.Drawing.Graphics
	Public Sub cb(ByVal data As IntPtr, ByVal size As Integer)
		If Not s Is Nothing Then
			If _b2 Is Nothing OrElse _b2.Length <> size Then ReDim _b2(size - 1)
			System.Runtime.InteropServices.Marshal.Copy(data, _b2, 0, _b2.Length)
			s.Write(_b2, 0, _b2.Length)
			'b2 = Nothing
		End If
		If Me.WindowState = FormWindowState.Minimized Then Return
		If _b Is Nothing OrElse _b.Length <> size / 2 Then ReDim _b((size / 2) - 1)
		'Dim b((size / 2) - 1) As Short	'= System.Runtime.InteropServices.GCHandle.FromIntPtr(data).Target

		System.Runtime.InteropServices.Marshal.Copy(data, _b, 0, _b.Length)
		displayfft(_b)
		'b = Nothing
		GC.Collect()
	End Sub
	Sub display(ByVal data As Short())
		Try
			Dim g As System.Drawing.Graphics = Me.CreateGraphics()
			Dim i As Integer
			Dim p As Drawing.Pen = Pens.Black
			'Dim tmp As Integer = Math.Min(data.Length - 2, Me.ClientSize.Width) / 0.1
			'Dim h As Integer = Me.ClientSize.Height
			'For i = 0 To tmp
			'	g.DrawLine(p, CInt(i * 0.1), CInt(data(i) * h / Short.MaxValue) + h2, CInt((i + 1) * 0.1), CInt(data(i + 1) * h / Short.MaxValue) + h2)
			'Next
			Dim sp As Integer = 0
			Dim b As Short() = data
			Dim _w As Integer = Me.ClientSize.Width	'200	'
			Dim _h As Integer = Me.ClientSize.Height ' - 85 '100	'
			Dim sc As Double = _w / data.Length	'0.025
			Dim e As Integer = CLng(sp + Math.Min(_w / sc, b.Length - 1))
			Dim ss As Integer = 0
			Dim se As Integer = 0
			Dim __h As Integer = _h / 2
			'Dim p As New Pen(Color.Green, 1)
			g.FillRectangle(Brushes.White, 0, 0, _w, _h)
			Try
				Dim _step As Integer = CInt(IIf((1 / sc) / 2 < 1, 1, (1 / sc) / 2))
				For i = sp To e - _step Step _step
					Dim h As Integer
					Dim h2 As Integer

					h = CInt(_h / CInt(CInt(Short.MaxValue) - CInt(Short.MinValue)) * b(CInt(i))) + __h
					h2 = CInt(_h / CInt(CInt(Short.MaxValue) - CInt(Short.MinValue)) * b(CInt(i + _step))) + __h
					'  =  2550            / 255  * 5

					'If ((i > ss And i < se) Or (i < ss And i > se)) Or (i = ss And Not ss = se And ss < se) Or (i = se And Not ss = se And ss > se) Then
					'	Dim r As New Rectangle(CInt((i - sp) * sc), 0, CInt(sc + 0.7), Me.Size.Height)
					'	pe.Graphics.FillRectangle(New SolidBrush(Color.White), r)
					'	'r = New Rectangle(((i - sp) * sc) - sc, 0, sc, Me.Size.Height)
					'	'pe.Graphics.FillRectangle(New SolidBrush(Color.White), r)
					'	pe.Graphics.DrawLine(New Pen(Color.Black, 1), CInt((i - sp) * sc), Me.Size.Height - h, CInt(((i - sp) * sc) + sc), Me.Size.Height - h2)
					'Else
					''g.DrawLine(p, CInt((i - sp) * sc), _h - h, CInt(((i - sp) * sc) + sc), _h - h2)
					g.DrawLine(p, CInt((i - sp) * sc), _h - h, CInt(((i - sp) * sc) + IIf(CInt(sc) < 1, 1, CInt(sc))), _h - h2)
					'End If
					'If Not sc < 5 Then
					'	Dim r2 As Rectangle
					'	r2 = New Rectangle(CInt((i - sp) * sc), Me.Size.Height - h, 5, 5)
					'	pe.Graphics.DrawRectangle(New Pen(Color.Black, 1), r2)
					'	r2 = New Rectangle(CInt((i - sp) * sc - 1), Me.Size.Height - h - 1, 3, 3)
					'	pe.Graphics.FillRectangle(New SolidBrush(Color.White), r2)
					'End If

					'End If

				Next i
			Catch ex As Exception

			End Try
			g.Dispose()
		Catch ex As Exception

		End Try
	End Sub
	Sub displayfft(ByVal data As Short())
		If _c Is Nothing OrElse _c.Length <> data.Length Then ReDim _c(data.Length - 1)
		wave_lib2.VBFFT.to_complex(_c, data, cuint(data.Length))
		wave_lib2.VBFFT.forward(_c)
		wave_lib2.VBFFT.to_short(data, _c, cuint(_c.Length))
		'If _s Is Nothing OrElse _s.Length <> data.Length Then ReDim _s(data.Length - 1)
		'Array.Copy(data, _s, data.Length)
		'Exocortex.DSP.Fourier.FFT(_s, _s.Length / 2, Exocortex.DSP.FourierDirection.Forward)
		'Array.Copy(_s, data, data.Length)
		'Dim i As Integer
		'For i = 0 To data.Length - 1
		'	data(i) = CShort(CInt(_s(i)) / Integer.MaxValue * Short.MaxValue)
		'Next
		'display(data)
		SyncLock bmp_sync
			If _g Is Nothing Then Return
			Dim i As Integer
			Dim p As Drawing.Brush = Brushes.Blue
			Dim sp As Integer = 0
			Dim b As Short() = data
			Dim _w As Integer = Me.ClientSize.Width	'200	'
			Dim _h As Integer = Me.ClientSize.Height ' - 85 '100	'
			Dim sc As Double = _w / data.Length	'0.025
			Dim e As Integer = CLng(sp + Math.Min(_w / sc, b.Length - 1))
			Dim ss As Integer = 0
			Dim se As Integer = 0
			Dim __h As Integer = _h / 2
			_g.FillRectangle(Brushes.White, 0, 0, _w, _h)
			Try
				Dim _step As Integer = CInt(IIf((1 / sc) / 1 < 1, 1, (1 / sc) / 1))
				For i = sp To e - _step Step _step
					Dim h As Integer
					h = CInt(_h / CInt(CInt(Short.MaxValue) - CInt(Short.MinValue)) * b(CInt(i))) + __h
					_g.FillRectangle(p, CInt((i - sp) * sc), _h - h, IIf(CInt(sc) < 1, 1, CInt(sc)), h)
				Next
			Catch ex As Exception
			End Try
			Dim g As System.Drawing.Graphics = Me.CreateGraphics()
			g.DrawImage(bmp, 0, 0, bmp.Width, bmp.Height)
			g.Dispose()
		End SyncLock
	End Sub
	Dim s As System.IO.Stream
	Private Sub Form2_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.DoubleClick
		If Me.WindowState = FormWindowState.Maximized Then
			Me.WindowState = FormWindowState.Normal
		End If
	End Sub

	Private Sub Form2_FormClosed(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosedEventArgs) Handles Me.FormClosed
		System.Diagnostics.Process.GetCurrentProcess().Kill()
	End Sub

	Private Sub Form2_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
		If Me.SaveFileDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then
			s = New System.IO.FileStream(Me.SaveFileDialog1.FileName, IO.FileMode.Append)
		End If
		Dim a As New wave_lib2.WaveInRecorder(&HFFFFFFFF, New wave_lib2.WaveFormat(22050, 16, 1), 1024, 2, AddressOf cb)
	End Sub
	Const WM_NCHITTEST As Integer = &H84
	Const HTCLIENT As Integer = &H1
	Const HTCAPTION As Integer = &H2
	Const HT_RS_W As Integer = 11
	Const HT_RS_H As Integer = 15
	Const HT_RS_W1 As Integer = 10
	Const HT_RS_H1 As Integer = 12
	Const HT_RS1 As Integer = 13
	Const HT_RS As Integer = 17
	Protected Overrides Sub WndProc(ByRef m As System.Windows.Forms.Message)
		Select Case m.Msg
			Case WM_NCHITTEST
				MyBase.WndProc(m)
				If m.Result = HTCLIENT Then
					If Not Me.WindowState = FormWindowState.Maximized Then
						m.Result = HTCAPTION
					End If
				End If
			Case Else
				MyBase.WndProc(m)
		End Select
	End Sub

	Private Sub Form2_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Resize
		SyncLock bmp_sync
			If Not bmp Is Nothing Then bmp.Dispose()
			bmp = New System.Drawing.Bitmap(Me.Width, Me.Height, Imaging.PixelFormat.Format24bppRgb)
			_g = System.Drawing.Graphics.FromImage(bmp)
		End SyncLock
	End Sub
End Class