Public Class Form4
	Dim buffersize As Integer = 4096 '* 4 ' * 4
	Dim WithEvents a As wave_lib3.WaveOut
	Dim WithEvents _s As _stream
	Dim s As New wave_lib.ReversedStream16
	'Dim s2 As New wave_lib.IIRFilterStream
	Dim bmp1 As New System.Drawing.Bitmap(321, 129, Imaging.PixelFormat.Format24bppRgb)
	'Dim bmp2 As New System.Drawing.Bitmap(300, 100, Imaging.PixelFormat.Format32bppArgb)
	'Dim a_bmp As System.Drawing.Bitmap = bmp
	Dim tmp1(buffersize / 4 - 1) As Short
	'Dim tmp2(2048 - 1) As Short
	Dim bmp_lock As New Object
	Dim bmp_2 As System.Drawing.Bitmap
	Dim bmp_lock_2 As New Object
	'Dim a_tmp As Short() = tmp1
	'Dim x1, x2 As Integer
	'Dim ax As Integer = 5
	'Dim __a As Boolean = False
	Sub realloc_buffer(ByVal size As Integer)
		If buffersize = size Then Return
		buffersize = size
		ReDim tmp1(size / 4 - 1)
		a.ReallocBuffer(4, size)
	End Sub
	Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
		'a.SetFormat(44100, 16, 1)
		'Dim t As Double = buffersize / a.format.nSamplesPerSec
		'Dim sc As Double = buffersize / bmp1.Width
		'Me.Timer1.Interval = buffersize / bmp1.Width / a.format.nSamplesPerSec * 1000
		Dim tmp As Integer = CInt(4096 / 88200 * a.format.nAvgBytesPerSec)
		tmp = Math.Round(tmp / 4) * 4
		If tmp < 64 Then tmp = 64
		realloc_buffer(tmp)
		a.Open()
		'Dim x As Net.HttpWebRequest = Net.HttpWebRequest.Create("")
		'x.Headers.Add(Net.HttpRequestHeader.Host, x.RequestUri.Host)
		'x.Headers.Add(Net.HttpRequestHeader.UserAgent, "Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.9.0.8) Gecko/2009032609 Firefox/3.0.8 FirePHP/0.2.4")
		'x.Headers.Add(Net.HttpRequestHeader.Accept, "image/png,image/*;q=0.8,*/*;q=0.5")
		'x.Headers.Add(Net.HttpRequestHeader.AcceptLanguage, "en-us,en;q=0.5")
		'x.Headers.Add(Net.HttpRequestHeader.AcceptEncoding, "gzip,deflate")
		'x.Headers.Add(Net.HttpRequestHeader.AcceptCharset, "ISO-8859-1,utf-8;q=0.7,*;q=0.7")
		'x.Headers.Add(Net.HttpRequestHeader.KeepAlive, "300")
		'x.Headers.Add(Net.HttpRequestHeader.Connection, "keep-alive")
		'x.UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.9.0.8) Gecko/2009032609 Firefox/3.0.8 FirePHP/0.2.4"
		'x.Accept = "image/png,image/*;q=0.8,*/*;q=0.5"
		'x.KeepAlive = True
		'x.Connection = "keep-alive"
		'x.Headers.Add(Net.HttpRequestHeader.AcceptLanguage, "en-us,en;q=0.5")
		'x.Headers.Add(Net.HttpRequestHeader.AcceptEncoding, "gzip,deflate")
		'x.Headers.Add(Net.HttpRequestHeader.AcceptCharset, "ISO-8859-1,utf-8;q=0.7,*;q=0.7")
		'Dim s As IO.Stream = x.GetResponse().GetResponseStream()
	End Sub

	Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
		a.Close()
	End Sub

	Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
		a.Play()
		'Me.Timer1.Enabled = True
	End Sub

	Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
		a.Stop()
	End Sub

	Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
		a.Pause()
	End Sub

	Private Sub Button6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button6.Click
		a.Resume()
	End Sub
	Dim th_load As Threading.Thread
	Private Sub Button7_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button7.Click
		If Me.Button7.Text = "abort" Then
			Try
				th_load.Abort()
			Catch ex As Exception
			End Try
			s.s.Position = 0
			Me.__panel1.Refresh()
			Me.Panel1.Refresh()
			Try
				Dim i1 As Integer
				For i1 = 0 To Me.Controls.Count - 1
					Me.Controls(i1).Enabled = True
				Next
				Me.Button7.Text = "open file..."
			Catch ex As Exception
			End Try
			th_load = Nothing
			Return
		End If
		If Me.OpenFileDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then
			a.Stop()
			s.s = New IO.FileStream(Me.OpenFileDialog1.FileName, IO.FileMode.Open, IO.FileAccess.Read)
			'Try
			If bmp_2 Is Nothing Then Return
			Dim th As New Threading.Thread(AddressOf loadfile)
			th.Priority = Threading.ThreadPriority.Lowest
			th.Start()
			th_load = th
		End If
	End Sub
	Sub loadfile()
		Try
			'Me.Enabled = False
			Me.Button7.Text = "abort"
			Dim i1 As Integer
			For i1 = 0 To Me.Controls.Count - 1
				If Not Me.Controls(i1) Is Me.Button7 Then Me.Controls(i1).Enabled = False
			Next
		Catch ex As Exception
		End Try
		Try
			Dim g As Graphics = Graphics.FromImage(bmp_2)
			g.Clear(Color.Black)
			Dim bs As Integer = CInt(s.Length / bmp_2.Width)
			If bs Mod 2 <> 0 Then bs -= 1
			If bs < 4 Then bs = 4
			Dim bs2 As Integer = bs
			If bs > 128 Then bs = 128
			Dim buf(bs - 1) As Byte
			Dim buf2(bs / 4 - 1) As Short
			Dim p As Pen = Pens.GreenYellow
			Dim _i As Integer = 0
			Dim _min, _max As Double
			Do
				_min = 0
				_max = 0
				Dim pos As Long = s.s.Position
				Dim __i As Integer
				Dim bufs As Integer = Math.DivRem(bs2, bs, Nothing)
				For __i = 0 To bufs - 1
					Dim tmp As Integer = s.s.Read(buf, 0, bs)
					If tmp <= 0 Then Exit Do
					wave_lib.WaveBuffer.byte_to_short2(buf, buf2, tmp)
					Dim i As Integer
					Dim min, max As Short
					min = Short.MaxValue
					max = Short.MinValue
					For i = 0 To buf2.Length - 1
						If buf2(i) < min Then min = buf2(i)
						If buf2(i) > max Then max = buf2(i)
					Next
					_min += min / bufs
					_max += max / bufs
				Next
				SyncLock bmp_lock_2
					Dim x As Integer = CInt(Math.Round((s.s.Position / 8) * bmp_2.Width / (s.s.Length / 8)))
					g.DrawLine(p, x, CInt(bmp_2.Height / 2) - CInt(_min / Short.MaxValue * bmp_2.Height), x, CInt(bmp_2.Height / 2) - CInt(_max / Short.MaxValue * bmp_2.Height))
				End SyncLock
				_i += 1
				If _i > 100 Then _i = 0
				If _i = 0 Then Me.__panel1.Refresh()
				s.s.Position = pos + bs2
				's.s.Seek(bs2 - bs, IO.SeekOrigin.Current)
			Loop
			g.DrawLine(Pens.White, 0, CInt(bmp_2.Height / 2), bmp_2.Width, CInt(bmp_2.Height / 2))
			g.Dispose()
			s.s.Position = 0
			Me.__panel1.Refresh()
			Me.Panel1.Refresh()
			'Catch ex As Exception

			'End Try
		Catch ex As Threading.ThreadAbortException
			Throw ex
		Catch ex As Exception
			MsgBox(ex.Message + vbNewLine + ex.StackTrace)
		End Try
		Try
			Dim i1 As Integer
			For i1 = 0 To Me.Controls.Count - 1
				Me.Controls(i1).Enabled = True
			Next
			Me.Button7.Text = "open file..."
		Catch ex As Exception
		End Try
		th_load = Nothing
	End Sub
	Private Sub OpenFileDialog1_FileOk(ByVal sender As System.Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles OpenFileDialog1.FileOk

	End Sub
	Private Sub Panel1_MouseDown(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Panel1.MouseDown, Panel1.MouseMove, __panel1.MouseDown, __panel1.MouseMove
		If Not e.Button = Windows.Forms.MouseButtons.Left Then Return
		try
			Dim pos As Long = Math.Round(e.X * a.src.Length / Me.Panel1.Width / 8) * 8
			If pos < 0 Then pos = 0
			If pos >= a.src.Length Then pos = a.src.Length - 1
			a.src.Position = pos
		catch e as exception
		end try
		'Me.Panel1.Refresh()
	End Sub

	Private Sub Panel1_Paint(ByVal sender As System.Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Panel1.Paint
		If Not a.src Is Nothing Then
			Try
				Dim x As Integer = CInt(Math.Round((a.src.Position / 8) * Me.Panel1.Width / (a.src.Length / 8)))
				e.Graphics.FillRectangle(Brushes.Black, 0, 0, x, Me.Panel1.Height)
				e.Graphics.FillRectangle(Brushes.White, x, 0, Me.Panel1.Width - x, Me.Panel1.Height)
			Catch ex As Exception
				e.Graphics.Clear(Color.White)
			End Try
		Else
			e.Graphics.Clear(Color.White)
		End If
	End Sub
	dim updating as boolean=false
	sub update_screen()
		d(bmp1, tmp1)
		Me.Panel2.Refresh()
		updating=false
	end sub
	Private Sub a_AfterPlayBuffer(ByVal data As wave_lib.WaveBuffer) Handles a.AfterPlayBuffer
		'Me.Panel1.Refresh()
		'Try
		'	Dim g As Graphics = Me.Panel1.CreateGraphics()
		'	Dim x As Integer = CInt(Math.Round((a.src.Position / 8) * Me.Panel1.Width / (a.src.Length / 8)))
		'	g.FillRectangle(Brushes.Black, 0, 0, x, Me.Panel1.Height)
		'	g.FillRectangle(Brushes.White, x, 0, Me.Panel1.Width - x, Me.Panel1.Height)
		'Catch ex As Exception

		'End Try
		'Me.Panel1.Invalidate()
		'data.to_array_16(CType(IIf(__a, tmp2, tmp1), Short()))
		if updating then return
		updating=true
		data.to_array_16_2(tmp1)
		dim d as system.delegate=system.Delegate.CreateDelegate(gettype(tmp_del), me, "update_screen")
		Me.BeginInvoke(d)
	End Sub
	Private Sub a_BeforePlayBuffer(ByVal data As wave_lib.WaveBuffer) Handles a.BeforePlayBuffer
	End Sub
	Private Sub Button8_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button8.Click
		a.SetFormat(CInt(Me.TextBox2.Text), CInt(Me.TextBox4.Text), CInt(Me.TextBox1.Text))
	End Sub

	Public Sub New()

		' This call is required by the Windows Form Designer.
		InitializeComponent()

		' Add any initialization after the InitializeComponent() call.
		a=New wave_lib3.WaveOut(4, buffersize)
		_s=new _stream
		a.src = _s
		''s2.s = _s
		_s.s = s
		s.reverseRead = False
		
	End Sub

	Dim _tmp2 As String = "###,###,###,###,##0"
	
	public sub do_update_scrollbar()
		''console.writeline("fdsfsdfs")
		Me.Label3.Text = CLng(s.Position / a.format.nAvgBytesPerSec).ToString()
		Me.Label5.Text = s.Position.ToString(_tmp2)
		Me.Panel1.Refresh()
		Dim x As Integer = CInt(Math.Round((a.src.Position / 8) * Me.__panel1.Width / (a.src.Length / 8)))
		If x <> ref_pos Then Me.__panel1.Refresh()

	end sub
	Private Sub s_position_chg() Handles _s.position_chg
		dim d as system.delegate=system.Delegate.CreateDelegate(gettype(tmp_del), me, "do_update_scrollbar")
		Me.BeginInvoke(d)
	End Sub
	delegate sub tmp_del()
	Sub d(ByVal bmp As Bitmap, ByVal data As Short())
		Try
			Dim g As System.Drawing.Graphics = System.Drawing.Graphics.FromImage(bmp)
			Dim i As Integer
			Dim p As Drawing.Pen = Pens.GreenYellow
			'Dim tmp As Integer = Math.Min(data.Length - 2, Me.ClientSize.Width) / 0.1
			'Dim h As Integer = Me.ClientSize.Height
			'For i = 0 To tmp
			'	g.DrawLine(p, CInt(i * 0.1), CInt(data(i) * h / Short.MaxValue) + h2, CInt((i + 1) * 0.1), CInt(data(i + 1) * h / Short.MaxValue) + h2)
			'Next
			Dim sp As Integer = 0
			Dim b As Short() = data
			Dim _w As Integer = bmp.Width 'Me.ClientSize.Width	'200	'
			Dim _h As Integer = bmp.Height 'Me.ClientSize.Height - 85 '100	'
			Dim sc As Double = _w / data.Length	'0.025
			Dim e As Integer = CLng(sp + Math.Min(_w / sc, b.Length - 1))
			Dim ss As Integer = 0
			Dim se As Integer = 0
			Dim __h As Integer = _h / 2
			Dim min, max As Short
			'Dim p As New Pen(Color.Green, 1)
			g.FillRectangle(Brushes.Black, 0, 0, _w, _h)
			Try
				Dim _step As Integer = CInt(IIf((1 / sc) / 2 < 1, 1, (1 / sc) / 2))
				For i = 0 To b.Length - 1 'sp To e - _step Step _step
					Dim h As Integer
					Dim h2 As Integer
					If b(i) > max Then max = b(i)
					If b(i) < min Then min = b(i)
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
			GoTo a
			If bmp_2 Is Nothing Then GoTo a
			Dim x As Integer = CInt(Math.Round((a.src.Position / 8) * bmp_2.Width / (a.src.Length / 8)))
			Dim _g As Graphics = Graphics.FromImage(bmp_2)
			_g.DrawLine(p, x, CInt(bmp_2.Height / 2) - CInt(min / Short.MaxValue * bmp_2.Height), x, CInt(bmp_2.Height / 2) - CInt(max / Short.MaxValue * bmp_2.Height))
			_g.Dispose()
			Me.__panel1.Refresh()
a:
			g.Dispose()
		Catch ex As Exception

		End Try
	End Sub
	Private Sub Panel2_Paint(ByVal sender As System.Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Panel2.Paint
		Try
			SyncLock bmp_lock
				'Dim _bmp1, _bmp2 As Bitmap
				'_bmp1 = IIf(__a, bmp2, bmp1)
				'_bmp2 = IIf(__a, bmp1, bmp2)
				'e.Graphics.DrawImageUnscaled(_bmp1, Me.Panel2.Width + ax - bmp1.Width, 0)
				'e.Graphics.DrawImageUnscaled(_bmp2, Me.Panel2.Width + ax - bmp1.Width - bmp2.Width, 0)
				e.Graphics.DrawImageUnscaled(bmp1, 0, 0)
			End SyncLock
		Catch ex As Exception

		End Try
	End Sub

	Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
		'ax -= Me.Timer1.Interval / (buffersize / bmp1.Width / a.format.nAvgBytesPerSec * 1000)
		Me.Panel2.Refresh()
	End Sub

	Private Sub Form4_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
		System.Diagnostics.Process.GetCurrentProcess().Kill()
	End Sub

	Private Sub Panel2_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles Panel2.Resize
		SyncLock bmp_lock
			Try
				If Not bmp1 Is Nothing Then
					bmp1.Dispose()
					bmp1 = Nothing
				End If
				bmp1 = New System.Drawing.Bitmap(Me.Panel2.ClientSize.Width, Me.Panel2.ClientSize.Height, Imaging.PixelFormat.Format24bppRgb)
			Catch ex As Exception
			End Try
		End SyncLock
	End Sub

	Private Sub CheckBox1_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox1.CheckedChanged
		s.reverseRead = Me.CheckBox1.Checked
	End Sub

	Private Sub CheckBox2_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBox2.CheckedChanged
		s.restorePosition = Me.CheckBox2.Checked
	End Sub
	Dim ref_pos As Integer = 0
	Private Sub __panel1_Paint(ByVal sender As System.Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles __panel1.Paint
		Try
			SyncLock bmp_lock_2
				e.Graphics.CompositingQuality = Drawing2D.CompositingQuality.HighQuality
				e.Graphics.InterpolationMode = Drawing2D.InterpolationMode.High
				If Not bmp_2 Is Nothing Then e.Graphics.DrawImage(bmp_2, 0, 0, Me.__panel1.Width, Me.__panel1.Height)
				Dim x As Integer = CInt(Math.Round((a.src.Position / 8) * Me.Panel1.Width / (a.src.Length / 8)))
				ref_pos = x
				e.Graphics.DrawLine(Pens.Red, x, 0, x, Me.__panel1.Height)
			End SyncLock
		Catch ex As Exception

		End Try
	End Sub

	Private Sub Form4_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
		SyncLock bmp_lock_2
			bmp_2 = New Bitmap(5000, Me.__panel1.ClientSize.Height, Imaging.PixelFormat.Format24bppRgb)
		End SyncLock
	End Sub

	Private Sub __panel1_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles __panel1.Resize
		'SyncLock bmp_lock_2
		'	Try
		'		bmp_2.Dispose()
		'		bmp_2 = Nothing
		'	Catch ex As Exception
		'	End Try
		'	Try
		'		bmp_2 = New Bitmap(Me.__panel1.ClientSize.Width, Me.__panel1.ClientSize.Height, Imaging.PixelFormat.Format24bppRgb)
		'	Catch ex As Exception
		'	End Try
		'End SyncLock
		Me.__panel1.Invalidate()
	End Sub

	Private Sub Panel1_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles Panel1.Resize
		Me.Panel1.Invalidate()
	End Sub

	Private Sub Button9_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button9.Click
		Dim a As String = InputBox("url", "", "http://")
		If a = Nothing Then Return
		Dim r As Net.HttpWebRequest = ctype(Net.HttpWebRequest.Create(a), Net.HttpWebRequest)
		s.s = r.GetResponse().GetResponseStream()
		's.s.ReadByte()
	End Sub
End Class
Public Class _stream
	Inherits IO.Stream
	Public s As IO.Stream
	Public Event position_chg()
	Public Overrides ReadOnly Property CanRead() As Boolean
		Get
			Return s.CanRead
		End Get
	End Property

	Public Overrides ReadOnly Property CanSeek() As Boolean
		Get
			Return s.CanSeek
		End Get
	End Property

	Public Overrides ReadOnly Property CanWrite() As Boolean
		Get
			Return s.CanWrite
		End Get
	End Property

	Public Overrides Sub Flush()
		s.Flush()
	End Sub

	Public Overrides ReadOnly Property Length() As Long
		Get
			Return s.Length
		End Get
	End Property

	Public Overrides Property Position() As Long
		Get
			Return s.Position
		End Get
		Set(ByVal value As Long)
			s.Position = value
			'RaiseEvent position_chg()
			RaiseEvent position_chg()
		End Set
	End Property

	Public Overrides Function Read(ByVal buffer() As Byte, ByVal offset As Integer, ByVal count As Integer) As Integer
		Dim i As Integer = s.Read(buffer, offset, count)
		'RaiseEvent position_chg()
		If Me.CanSeek Then RaiseEvent position_chg()
		Return i
	End Function

	Public Overrides Function Seek(ByVal offset As Long, ByVal origin As System.IO.SeekOrigin) As Long
		Dim i As Integer = s.Seek(offset, origin)
		'RaiseEvent position_chg()
		RaiseEvent position_chg()
		Return i
	End Function

	Public Overrides Sub SetLength(ByVal value As Long)
		s.SetLength(value)
	End Sub

	Public Overrides Sub Write(ByVal buffer() As Byte, ByVal offset As Integer, ByVal count As Integer)
		s.Write(buffer, offset, count)
		'RaiseEvent position_chg()
		If Me.CanSeek Then RaiseEvent position_chg()
	End Sub
End Class