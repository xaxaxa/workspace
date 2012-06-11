Public Class Form5
	Dim buffersize As Integer = 4096
	Dim WithEvents a As New wave_lib.WaveIn(1024, 4096)
	Dim s As New _stream()
	Dim bmp1 As New System.Drawing.Bitmap(375, 186, Imaging.PixelFormat.Format24bppRgb)
	Dim bmp_2 As System.Drawing.Bitmap
	Dim bmp_lock_2 As New Object
	'Dim bmp2 As New System.Drawing.Bitmap(300, 100, Imaging.PixelFormat.Format32bppArgb)
	'Dim a_bmp As System.Drawing.Bitmap = bmp
	Dim tmp1(2048 - 1) As Short
	'Dim tmp2(2048 - 1) As Short
	Dim bmp_lock As New Object
	'Dim a_tmp As Short() = tmp1
	'Dim tmpf As IO.Stream
	Sub realloc_buffer(ByVal size As Integer)
		If buffersize = size Then Return
		buffersize = size
		ReDim tmp1(size / 2 - 1)
		a.ReallocBuffer(1024, size)
	End Sub
	Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
		Me.SaveFileDialog1.ShowDialog()
	End Sub

	Private Sub SaveFileDialog1_FileOk(ByVal sender As System.Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles SaveFileDialog1.FileOk
		Try
			s.s.Close()
		Catch ex As Exception

		End Try
		s.s = New IO.FileStream(Me.SaveFileDialog1.FileName, IO.FileMode.Create, IO.FileAccess.ReadWrite)
	End Sub

	Public Sub New()

		' This call is required by the Windows Form Designer.
		InitializeComponent()

		' Add any initialization after the InitializeComponent() call.
		a.src = s
		Dim th As New Threading.Thread(AddressOf th1)
		th.Priority = Threading.ThreadPriority.Lowest
		th.Start()

		'If Not IO.Directory.Exists("c:/temp") Then IO.Directory.CreateDirectory("c:/temp")
		'Dim i As Integer = 0
		'Do
		'	If Not IO.File.Exists("c:/temp/__tmp" + i.ToString()) Then
		'		tmpf = New IO.FileStream("c:/temp/__tmp" + i.ToString(), IO.FileMode.Create, IO.FileAccess.ReadWrite)
		'		Exit Do
		'	End If
		'Loop
	End Sub

	Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
		Dim tmp As Integer = CInt(4096 / 88200 * a.format.nAvgBytesPerSec)
		tmp = Math.Round(tmp / 4) * 4
		If tmp < 64 Then tmp = 64
		realloc_buffer(tmp)
		a.Open()
		a.Record()
	End Sub

	Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
		a.Stop()
		a.Close()
	End Sub

	Private Sub Panel2_Paint(ByVal sender As System.Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Panel2.Paint
		SyncLock bmp_lock
			'Dim _bmp1, _bmp2 As Bitmap
			'_bmp1 = IIf(__a, bmp2, bmp1)
			'_bmp2 = IIf(__a, bmp1, bmp2)
			'e.Graphics.DrawImageUnscaled(_bmp1, Me.Panel2.Width + ax - bmp1.Width, 0)
			'e.Graphics.DrawImageUnscaled(_bmp2, Me.Panel2.Width + ax - bmp1.Width - bmp2.Width, 0)
			e.Graphics.DrawImageUnscaled(bmp1, 0, 0)
		End SyncLock
	End Sub

	Private Sub Panel2_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles Panel2.Resize
		SyncLock bmp_lock
			Try
				If Not bmp1 Is Nothing Then bmp1.Dispose()
				bmp1 = New System.Drawing.Bitmap(Me.Panel2.ClientSize.Width, Me.Panel2.ClientSize.Height, Imaging.PixelFormat.Format24bppRgb)
			Catch ex As Exception
			End Try
		End SyncLock
	End Sub

	Sub d(ByVal bmp As Bitmap, ByVal data As Short())
		Try
			Dim g As System.Drawing.Graphics = System.Drawing.Graphics.FromImage(bmp)
			Dim i As Integer
			Dim p As Drawing.Pen = Pens.GreenYellow
			Dim sp As Integer = 0
			Dim b As Short() = data
			Dim _w As Integer = bmp.Width 'Me.ClientSize.Width	'200	'
			Dim _h As Integer = bmp.Height 'Me.ClientSize.Height - 85 '100	'
			Dim sc As Double = _w / data.Length	'0.025
			Dim e As Integer = CLng(sp + Math.Min(_w / sc, b.Length - 1))
			Dim ss As Integer = 0
			Dim se As Integer = 0
			Dim __h As Integer = _h / 2
			'Dim p As New Pen(Color.Green, 1)
			g.FillRectangle(Brushes.Black, 0, 0, _w, _h)
			Try
				Dim _step As Integer = CInt(IIf((1 / sc) / 2 < 1, 1, (1 / sc) / 2))
				For i = sp To e - _step Step _step
					Dim h As Integer
					Dim h2 As Integer

					h = CInt(_h / CInt(CInt(Short.MaxValue) - CInt(Short.MinValue)) * b(CInt(i))) + __h
					h2 = CInt(_h / CInt(CInt(Short.MaxValue) - CInt(Short.MinValue)) * b(CInt(i + _step))) + __h
					'  =  2550            / 255  * 5
					g.DrawLine(p, CInt((i - sp) * sc), _h - h, CInt(((i - sp) * sc) + IIf(CInt(sc) < 1, 1, CInt(sc))), _h - h2)
				Next i
			Catch ex As Exception

			End Try
			g.Dispose()
		Catch ex As Exception

		End Try
	End Sub
	Dim e As New Threading.EventWaitHandle(False, Threading.EventResetMode.AutoReset)
	'Const bs As Integer = 4096
	Sub th1()
		Dim _tmp1 As String = "hh:MM:ss"
		Dim _tmp2 As String = "###,###,###,###,##0"
		Do
			e.WaitOne(System.Threading.Timeout.Infinite, True)
			Me.Label3.Text = CLng(s.Length / a.format.nAvgBytesPerSec).ToString()
			Me.Label5.Text = s.Length.ToString(_tmp2)
			'Dim s As Short() = CType(IIf(__a, tmp2, tmp1), Short())
			If bmp1 Is Nothing Then GoTo a
			SyncLock bmp_lock
				'Dim _bmp1, _bmp2 As Bitmap
				'_bmp1 = IIf(__a, bmp2, bmp1)
				'_bmp2 = IIf(__a, bmp1, bmp2)
				'd(_bmp2, s)
				'ax = _bmp2.Width
				'__a = Not __a
				d(bmp1, tmp1)
			End SyncLock
			Me.Panel2.Refresh()
a:
			If bmp_2 Is Nothing Then GoTo b
			SyncLock bmp_lock_2
				Dim g As Graphics = Graphics.FromImage(bmp_2)
				Dim i As Integer
				Dim min, max As Short
				min = 0
				max = 0
				For i = 0 To buffersize / 2 - 1
					If tmp1(i) < min Then min = tmp1(i)
					If tmp1(i) > max Then max = tmp1(i)
				Next
				g.DrawLine(Pens.Black, ref_pos, 0, ref_pos, bmp_2.Height)
				g.DrawLine(Pens.GreenYellow, ref_pos, CInt(bmp_2.Height / 2) - CInt(min / Short.MaxValue * bmp_2.Height), ref_pos, CInt(bmp_2.Height / 2) - CInt(max / Short.MaxValue * bmp_2.Height))
				ref_pos += 1
				If ref_pos >= Me.bmp_2.Width Then ref_pos = 0
			End SyncLock
			Dim x As Integer = ref_pos / Me.bmp_2.Width * Me.__panel1.Width	'CInt(Math.Round((a.src.Position / 8) * Me.__panel1.Width / (a.src.Length / 8)))
			If x <> ref_pos2 Then Me.__panel1.Refresh()
b:
		Loop
	End Sub
	Private Sub a_AfterRecordBuffer(ByVal data As wave_lib.WaveBuffer) Handles a.AfterRecordBuffer
		data.to_array_16_1(tmp1)
		'tmpf.Write(data.data, 0, data.size)
		e.Set()
	End Sub
	Private Sub Form4_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
		System.Diagnostics.Process.GetCurrentProcess().Kill()
	End Sub

	Private Sub Button8_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button8.Click
		a.SetFormat(CInt(Me.TextBox2.Text), CInt(Me.TextBox4.Text), CInt(Me.TextBox1.Text))
	End Sub
	Dim ref_pos As Integer = 0
	Dim ref_pos2 As Integer = 0
	Private Sub __panel1_Paint(ByVal sender As System.Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles __panel1.Paint
		Try
			SyncLock bmp_lock_2
				e.Graphics.CompositingQuality = Drawing2D.CompositingQuality.HighQuality
				e.Graphics.InterpolationMode = Drawing2D.InterpolationMode.High
				If Not bmp_2 Is Nothing Then e.Graphics.DrawImage(bmp_2, 0, 0, Me.__panel1.Width, Me.__panel1.Height)
				Dim x As Integer = ref_pos / Me.bmp_2.Width * Me.__panel1.Width	'CInt(Math.Round((a.src.Position / 8) * Me.__panel1.Width / (a.src.Length / 8)))
				ref_pos2 = x
				'ref_pos = x
				e.Graphics.DrawLine(Pens.Red, x, 0, x, Me.__panel1.Height)
			End SyncLock
		Catch ex As Exception

		End Try
	End Sub
	Private Sub __panel1_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles __panel1.Resize
		Me.__panel1.Invalidate()
	End Sub
	Private Sub Form5_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
		SyncLock bmp_lock_2
			bmp_2 = New Bitmap(5000, Me.__panel1.ClientSize.Height, Imaging.PixelFormat.Format24bppRgb)
		End SyncLock
	End Sub
End Class