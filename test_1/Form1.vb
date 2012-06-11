Public Class Form1
	Delegate Sub s(ByVal a As Integer)
	Dim WithEvents w As New wave_lib3.WaveOut()
	Dim sr As Integer = 44100
	Sub s1(ByVal a As Integer)
		MsgBox("xaxaxaaxax" + a.ToString())
	End Sub
	Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
		'Dim _s As s = AddressOf s1
		'Dim i As UInteger = tmp1(_s)
		'tmp2(i)
		If Me.Button1.Text = "open" Then
			w.Open()
			Me.Button1.Text = "close"
			Try
				cb.sampleRate = sr
			Catch ex As Exception

			End Try
		Else
			w.Close()
			Me.Button1.Text = "open"
		End If

		'wave_lib2.WaveOut.__test1()
	End Sub
	Sub cb1(ByVal b As Short(), ByVal length As UInteger)

	End Sub
	Declare Function tmp1 Lib "wave_lib.dll" Alias "?tmp1@@YAIPAX@Z" (ByVal d As s) As UInteger
	Declare Sub tmp2 Lib "wave_lib.dll" Alias "?tmp2@@YAXI@Z" (ByVal d As UInteger)

	Delegate Sub wave_callback()
	Declare Function create_output Lib "wave_lib.dll" Alias "?tmp1@@YAIPAX@Z" () As UInteger
	Declare Sub open_output Lib "wave_lib.dll" Alias "?tmp2@@YAXI@Z" (ByVal o As UInteger, ByVal callback As wave_callback)
	Dim f As System.IO.FileStream '("c:/tmpf.pcm", IO.FileMode.Open)
	Dim cb As wave_lib2.WaveOut.stream_cb = New wave_lib2.WaveOut.stream_cb(4096)
	Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
		w.Callback = cb.d
		w.Start()
	End Sub

	Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
		w.Stop()
	End Sub

	Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
		Me.OpenFileDialog1.ShowDialog()
	End Sub

	Private Sub OpenFileDialog1_FileOk(ByVal sender As System.Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles OpenFileDialog1.FileOk
		Try
			SyncLock cb.d
				Dim tmpf As IO.FileStream = f
				f = New System.IO.FileStream(Me.OpenFileDialog1.FileName, IO.FileMode.Open, IO.FileAccess.Read)
				Try
					tmpf.Close()
				Catch ex As Exception
				End Try
				cb.s = f
			End SyncLock
			'cb = wave_lib2.WaveOut.get_stream_callback(f)
		Catch ex As Exception
			MsgBox(ex.Message)
		End Try
		Me.Panel1.Refresh()
	End Sub

	Public Sub New()
		Try

			cb.s = f
			w.init_onPlayBuffer()
			' This call is required by the Windows Form Designer.
			InitializeComponent()
		Catch ex As Exception

		End Try

		' Add any initialization after the InitializeComponent() call.

	End Sub

	Private Sub w_OnPlayBuffer(ByVal data() As Short) Handles w.OnPlayBuffer
		Try
			Me.Panel1.Refresh()
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
			Dim _h As Integer = Me.ClientSize.Height - 85 '100	'
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

	Private Sub Panel1_MouseDown(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Panel1.MouseDown
		f.Position = Math.Round(e.X * f.Length / Me.Panel1.Width / 8) * 8
		Me.Panel1.Refresh()
	End Sub

	Private Sub Panel1_Paint(ByVal sender As System.Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Panel1.Paint
		If Not f Is Nothing Then
			Try
				Dim x As Integer = CInt(Math.Round((f.Position / 8) * Me.Panel1.Width / (f.Length / 8)))
				e.Graphics.FillRectangle(Brushes.Black, 0, 0, x, Me.Panel1.Height)
				e.Graphics.FillRectangle(Brushes.White, x, 0, Me.Panel1.Width - x, Me.Panel1.Height)
			Catch ex As Exception

			End Try
		End If
	End Sub

	Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
		w.InitFormat(CUShort(Me.TextBox1.Text), CUInt(Me.TextBox2.Text), CUShort(Me.TextBox4.Text))
		sr = CInt(Me.TextBox2.Text)
	End Sub

	Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
		Dim eq As New eq
		cb._eq = eq.equalizer
		eq.Show()
	End Sub
End Class
