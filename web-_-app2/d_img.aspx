<%@ Page Language="VB" Buffer="false" %><%
                                        	     'System.Threading.Thread.Sleep(200)
                                        	     Dim img As System.Drawing.Image = Session(Request.QueryString("session"))
                                        	     Dim tmp As New Drawing.Bitmap(CInt(Request.QueryString("w")), CInt(Request.QueryString("h")), Drawing.Imaging.PixelFormat.Format32bppPArgb)
                                        	     Dim g As Drawing.Graphics = Drawing.Graphics.FromImage(tmp)
                                        	     g.CompositingMode = Drawing.Drawing2D.CompositingMode.SourceCopy
                                        	     g.CompositingQuality = Drawing.Drawing2D.CompositingQuality.HighQuality Or Drawing.Drawing2D.CompositingQuality.GammaCorrected
                                        	     g.DrawImage(img, -CInt(Request.QueryString("x")), -CInt(Request.QueryString("y")), img.Width, img.Height)
                                        	     tmp.Save(Response.OutputStream, Drawing.Imaging.ImageFormat.Jpeg)
                                        	     Response.Flush()
                                        	     g.Dispose()
                                        	     tmp.Dispose()
%>