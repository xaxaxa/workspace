Public Class Form3
	Dim freq(255) As Integer
	Dim equalizer As New _eq1(5000, 44100)
	Dim f As New IO.FileStream("c:/__tmp_test.pcm", IO.FileMode.Create)
	Dim ns As New wave_lib2.SineStream()
	Dim b As Boolean = False
	Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
		'Me.OpenFileDialog1.ShowDialog()
		If b Then Return
		b = True
		Dim o As New wave_lib.WaveOut(16, 512) '
		o.SetFormat(44100, 16, 1)
		'Dim fs As New IO.FileStream(Me.OpenFileDialog1.FileName, IO.FileMode.Open)
		AddHandler o.AfterPlayBuffer, AddressOf cb
		o.Open()
		'ns.equalizer = Me.equalizer
		ns.sampleRate = 44100
		o.src = ns
		o.Play()
	End Sub
	Sub cb(ByVal b As wave_lib.WaveBuffer)
		f.Write(b.data, 0, b.size)
	End Sub
	Dim _th As System.Threading.Thread
	Private Sub OpenFileDialog1_FileOk(ByVal sender As System.Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles OpenFileDialog1.FileOk
		Dim th As New System.Threading.Thread(AddressOf th1)
		th.Start()
		_th = th
	End Sub
	Dim th1_started As Boolean = False
	Dim fs As IO.FileStream
	Sub th1()
		th1_started = True
		Me.Button2.Text = "close file"
		Dim fs As New System.IO.FileStream(Me.OpenFileDialog1.FileName, IO.FileMode.Open)
		Me.fs = fs
		Dim is_13 As Boolean = False
		Dim prev_f As Integer = 0
		Dim _pos As Double = 0
		Do
a:
			Dim i As Integer = fs.ReadByte()
			If i = -1 Then GoTo b
			If i = 10 And is_13 Then
				is_13 = False
				GoTo a
			End If
			is_13 = (i = 13)
			i = Asc(Char.ToUpper(Chr(i)))
			Dim f As Integer = freq(i)
			If f = 0 Then GoTo a
			equalizer.SetPoint(f, 1)
			If Not prev_f = 0 Then
				_pos = ns.waves(ns.GetFreq(prev_f)).pos
				ns.RemoveFreq(prev_f)
			End If
			Dim sw As New wave_lib2.SineWave
			sw.pos = _pos
			sw.freq = f
			ns.waves.Add(sw)
			prev_f = f
			Me.TextBox1.AppendText(Chr(i))
			If is_13 Then Me.TextBox1.AppendText(Chr(10))
			System.Threading.Thread.Sleep(150)
			'f = freq(i)
			'If Not f = 0 Then
			'	equalizer.SetPoint(f, 0)
			'	ns.RemoveFreq(f)
			'End If
		Loop
b:
		If Not prev_f = 0 Then ns.RemoveFreq(prev_f)
		Me.TextBox1.AppendText(vbNewLine)
		fs.Close()
		Me.Button2.Text = "open file"
		th1_started = False
	End Sub
	Private Sub Button1_KeyDown(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles Button1.KeyDown
		Me.TextBox1.AppendText("down:" + e.KeyCode.ToString() + vbNewLine)
		Dim f As Integer = freq(e.KeyCode)
		If Not f = 0 Then
			equalizer.SetPoint(f, 1)
			ns.AddFreq(f)
		End If
	End Sub

	Private Sub Button1_KeyUp(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles Button1.KeyUp
		Me.TextBox1.AppendText("up:" + e.KeyCode.ToString() + vbNewLine)
		Dim f As Integer = freq(e.KeyCode)
		If Not f = 0 Then
			equalizer.SetPoint(f, 0)
			ns.RemoveFreq(f)
		End If
	End Sub

	Private Sub Form3_FormClosed(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosedEventArgs) Handles Me.FormClosed
		System.Diagnostics.Process.GetCurrentProcess().Kill()
	End Sub

	Private Sub Form3_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
		'freq(65) = 500
		'freq(66) = 550
		'freq(67) = 600
		'freq(68) = 650
		'freq(69) = 700
		'freq(70) = 750
		'freq(71) = 800
		'freq(72) = 850
		'freq(73) = 900
		'freq(74) = 950
		'freq(75) = 1000
		'freq(76) = 1050
		'freq(77) = 1100
		'freq(78) = 1150
		'freq(79) = 1200
		'freq(80) = 1250
		'freq(81) = 1300
		'freq(82) = 1350
		'freq(83) = 1400
		'freq(84) = 1450
		'freq(85) = 1500
		'freq(86) = 1550
		'freq(87) = 1600
		'freq(88) = 1650
		'freq(89) = 1700
		'freq(90) = 1750
		Dim f As Integer = 200
		Dim i As Integer
		For i = 10 To 128
			freq(i) = f
			If i > 83 Then
				f += 250
			ElseIf i > 60 Then
				f += 150
			ElseIf i > 30 Then
				f += 30
			Else
				f += 20
			End If
		Next
		freq(13) = freq(10)
		'Return
		freq(AscW("A")) = 523 / 4
		freq(AscW("B")) = 587 / 4 '*1.1223709369024856596558317399618
		freq(AscW("C")) = 659 / 4 '*1.1226575809199318568994889267462
		freq(AscW("D")) = 698 / 4 '*1.0591805766312594840667678300455
		freq(AscW("E")) = 784 / 4 '*1.1232091690544412607449856733524
		freq(AscW("F")) = 880 / 4 '*1.1224489795918367346938775510204
		freq(AscW("G")) = 988 / 4 '*1.1227272727272727272727272727273
		freq(AscW("H")) = 1046 / 4 '*1.0587044534412955465587044534413
		'freq(AscW("I")) = 1175 '*1.1233269598470363288718929254302
		'freq(AscW("J")) = 1318 '
		Dim i2 As Integer = AscW("H")
		'Dim tmp As Integer = 1046
		Dim i3 As Integer = 0
		Dim d0, d1, d2, d3, d4, d5, d6 As Double
		d0 = 1.1223709369024857
		d1 = 1.122657580919932
		d2 = 1.0591805766312594
		d3 = 1.1232091690544412
		d4 = 1.1224489795918366
		d5 = 1.1227272727272728
		d6 = 1.0587044534412955
		Do
			Select Case i3
				Case 0
					freq(i2 + 1) = freq(i2) * d0 '1.1223709369024857
				Case 1
					freq(i2 + 1) = freq(i2) * d1 '1.122657580919932
				Case 2
					freq(i2 + 1) = freq(i2) * d2 '1.0591805766312594
				Case 3
					freq(i2 + 1) = freq(i2) * d3 '1.1232091690544412
				Case 4
					freq(i2 + 1) = freq(i2) * d4 '1.1224489795918366
				Case 5
					freq(i2 + 1) = freq(i2) * d5 '1.1227272727272728
				Case 6
					freq(i2 + 1) = freq(i2) * d6 '1.0587044534412955
			End Select
			i3 += 1
			If i3 > 6 Then
				i3 -= 6
				d0 *= 1.0008517887563884
				d1 *= 1.0008517887563884
				d2 *= 1.0008517887563884
				d3 *= 1.0008517887563884
				d4 *= 1.0008517887563884
				d5 *= 1.0008517887563884
				d6 *= 1.0008517887563884
			End If
			i2 += 1
			If i2 > AscW("Y") Then Exit Do
		Loop
	End Sub

	Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
		If th1_started Then
			_th.Abort()
			SyncLock ns.waves
				ns.waves.Clear()
			End SyncLock
			Me.Button2.Text = "open file"
			th1_started = False
			Try
				Me.fs.Close()
			Catch ex As Exception
			End Try
			Me.TextBox1.AppendText(vbNewLine)
			Return
		End If
		Me.OpenFileDialog1.ShowDialog()
	End Sub
End Class
Public Class _eq1
	Implements wave_lib2.IEqualizer
	Public _points() As Double
	Public max_freq As Integer
	Public b As Boolean()
	Public Function GetPoint(ByVal freq As UInteger) As Double Implements wave_lib2.IEqualizer.GetPoint
		Dim ind As Integer = CInt(CDbl(freq) / max_freq * (_points.Length - 1))
		If ind >= _points.Length Then ind = _points.Length - 1
		Return _points(ind)
	End Function
	Public Sub SetPoint(ByVal freq As UInteger, ByVal value As Double)
		Dim ind As Integer = CInt(CDbl(freq) / max_freq * (_points.Length - 1))
		If ind >= _points.Length Then ind = _points.Length - 1
		_points(ind) = value
	End Sub
	Public Sub New(ByVal size As Integer, ByVal max_freq As Integer)
		Me.max_freq = max_freq
		ReDim _points(size - 1)
		ReDim b(size - 1)
		Dim i As Integer
		For i = 0 To size - 1
			_points(i) = 0.0 '0.5
		Next
	End Sub
End Class
