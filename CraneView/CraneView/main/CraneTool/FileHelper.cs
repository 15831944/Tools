using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CraneTool
{
	public class FileHelper
	{
		private static readonly string ErrorStr = "\\/:*?\"<>|,;@#$%^&";
		public static bool FileNameRight(string strFile, ref string strError)
		{
			strFile = strFile.Trim();
			if (string.IsNullOrEmpty(strFile))
			{
				strError = "Name can not be empty";
				return false;
			}

			string[] errorStr = new string[] {  };
			for (int i = 0; i < ErrorStr.Length; i++)
			{
				if (strFile.Contains(ErrorStr[i]))
				{
					strError = "Name is error, can not contains below character:\r\n" + ErrorStr;
					return false;
				}
			}
			return true;
		}

		#region 1.创建文件夹
		//using System.IO;
		public static bool CreatePath(string strPath)
		{
			DirectoryInfo di = Directory.CreateDirectory(strPath);
			return di.Exists;
		}
		#endregion

		#region 2.创建文件
		//using System.IO;
		//File.Create(%%1);
		#endregion

		#region 3.删除文件
		//using System.IO;
		//File.Delete(%%1);
		#endregion

		#region 4.删除文件夹
		//using System.IO;
		//Directory.Delete(%%1);
		#endregion

		#region 5.删除一个目录下所有的文件夹
		//using System.IO;
		//foreach (string dirStr in Directory.GetDirectories(%%1))
		//{
		//DirectoryInfo dir = new DirectoryInfo(dirStr);
		//ArrayList folders=new ArrayList();
		//FileSystemInfo[] fileArr = dir.GetFileSystemInfos();
		//for (int i = 0; i < folders.Count; i++)
		//{
		//FileInfo f = folders[i] as FileInfo;
		//if (f == null)
		//{
		//DirectoryInfo d = folders[i] as DirectoryInfo;
		//d.Delete();
		//}
		//}
		//} 
		#endregion

		#region 6.清空文件夹
		//using System.IO;
		//Directory.Delete(%%1,true);
		//Directory.CreateDirectory(%%1); 
		#endregion

		#region 7.读取文件
		public static string ReadFileStr(string strFilePathName)
		{
			return File.ReadAllText(strFilePathName, Encoding.UTF8);
		}
		public static byte[] ReadFileByte(string strFilePathName)
		{
			return File.ReadAllBytes(strFilePathName);
		}
		#endregion

		#region 8.写入文件
		public static void WriteFileStr(string strFilePathName, string str)
		{
			File.WriteAllText(strFilePathName, str, Encoding.UTF8);
		}
		public static void WriteFileByte(string strFilePathName, byte[] data)
		{
			File.WriteAllBytes(strFilePathName, data);
		}
		#endregion

		#region 9.写入随机文件
		//using System.IO;
		//byte[] dataArray = new byte[100000];//new Random().NextBytes(dataArray);
		//using(FileStream FileStream = new FileStream(%%1, FileMode.Create)){
		//// Write the data to the file, byte by byte.
		//for(int i = 0; i < dataArray.Length; i++){
		//FileStream.WriteByte(dataArray[i]);
		//}
		//// Set the stream position to the beginning of the file.
		//FileStream.Seek(0, SeekOrigin.Begin);
		//// Read and verify the data.
		//for(int i = 0; i < FileStream.Length; i++){
		//if(dataArray[i] != FileStream.ReadByte()){
		////写入数据错误
		//return;
		//}
		//}
		////"数据流"+FileStream.Name+"已验证"
		//} 
		#endregion

		#region 10.读取文件属性
		//using System.IO;
		//FileInfo f = new FileInfo(%%1);//f.CreationTime,f.FullName
		//if((f.Attributes & FileAttributes.ReadOnly) != 0){
		//%%2
		//}
		//else{
		//%%3
		//} 
		#endregion

		#region 11.写入属性
		//using System.IO;
		//FileInfo f = new FileInfo(%%1);
		////设置只读
		//f.Attributes = myFile.Attributes | FileAttributes.ReadOnly;
		////设置可写
		//f.Attributes = myFile.Attributes & ~FileAttributes.ReadOnly;
		#endregion

		#region 12.枚举一个文件夹中的所有文件夹
		//using System.IO;
		//foreach (string %%2 in Directory.GetDirectories(%%1)){
		//%%3
		//} 
		#endregion

		#region 13.复制文件夹
		//string path = (%%2.LastIndexOf("\") == %%2.Length - 1) ? %%2 : %%2+"\";
		//string parent = Path.GetDirectoryName(%%1);
		//Directory.CreateDirectory(path + Path.GetFileName(%%1));
		//DirectoryInfo dir = new DirectoryInfo((%%1.LastIndexOf("\") == %%1.Length - 1) ? %%1 : %%1 + "\");
		//FileSystemInfo[] fileArr = dir.GetFileSystemInfos();
		//Queue Folders = new Queue(dir.GetFileSystemInfos());
		//while (Folders.Count>0)
		//{
		//					FileSystemInfo tmp = Folders.Dequeue();
		//					FileInfo f = tmp as FileInfo;
		//					if (f == null)
		//					{
		//						DirectoryInfo d = tmp as DirectoryInfo;
		//						Directory.CreateDirectory(d.FullName.Replace((parent.LastIndexOf("\") == parent.Length - 1) ? parent : parent + "\", path));
		//						foreach (FileSystemInfo fi in d.GetFileSystemInfos())
		//						{
		//							Folders.Enqueue(fi);
		//						}
		//					}
		//					else
		//					{
		//						f.CopyTo(f.FullName.Replace(parent, path));
		//					}
		//}
		#endregion

		#region 14.复制目录下所有的文件夹到另一个文件夹下
		//DirectoryInfo d = new DirectoryInfo(%%1);
		//foreach (DirectoryInfo dirs in d.GetDirectories())
		//{
		//	Queue al = new Queue(dirs.GetFileSystemInfos());
		//	while (al.Count > 0)
		//	{
		//		FileSystemInfo temp = al.Dequeue();
		//		FileInfo file = temp as FileInfo;
		//		if (file == null)
		//		{
		//			DirectoryInfo directory = temp as DirectoryInfo;
		//			Directory.CreateDirectory(path + directory.Name);
		//			foreach (FileSystemInfo fsi in directory.GetFileSystemInfos())
		//				al.Enqueue(fsi);
		//		}
		//		else
		//			File.Copy(file.FullName, path + file.Name);
		//	}
		//}
		#endregion

		#region 15.移动文件夹
		//string filename = Path.GetFileName(%%1);
		//string path=(%%2.LastIndexOf("\") == %%2.Length - 1) ? %%2 : %%2 + "\";
		//if (Path.GetPathRoot(%%1) == Path.GetPathRoot(%%2))
		//	Directory.Move(%%1, path + filename);
		//else
		//{
		//	string parent = Path.GetDirectoryName(%%1);
		//	Directory.CreateDirectory(path + Path.GetFileName(%%1));
		//	DirectoryInfo dir = new DirectoryInfo((%%1.LastIndexOf("\") == %%1.Length - 1) ? %%1 : %%1 + "\");
		//	FileSystemInfo[] fileArr = dir.GetFileSystemInfos();
		//	Queue Folders = new Queue(dir.GetFileSystemInfos());
		//	while (Folders.Count > 0)
		//	{
		//		FileSystemInfo tmp = Folders.Dequeue();
		//		FileInfo f = tmp as FileInfo;
		//		if (f == null)
		//		{
		//			DirectoryInfo d = tmp as DirectoryInfo;
		//			DirectoryInfo dpath = new DirectoryInfo(d.FullName.Replace((parent.LastIndexOf("\") == parent.Length - 1) ? parent : parent + "\", path));
		//			dpath.Create();
		//			foreach (FileSystemInfo fi in d.GetFileSystemInfos())
		//			{
		//				Folders.Enqueue(fi);
		//			}
		//		}
		//		else
		//		{
		//			f.MoveTo(f.FullName.Replace(parent, path));
		//		}
		//	}
		//	Directory.Delete(%%1, true);
		//}
		#endregion

		#region 16.移动目录下所有的文件夹到另一个目录下
		//string filename = Path.GetFileName(%%1);
		//if (Path.GetPathRoot(%%1) == Path.GetPathRoot(%%2))
		//	foreach (string dir in Directory.GetDirectories(%%1))
		//		Directory.Move(dir, Path.Combine(%%2,filename));
		//else
		//{
		//	foreach (string dir2 in Directory.GetDirectories(%%1))
		//	{
		//		string parent = Path.GetDirectoryName(dir2);
		//		Directory.CreateDirectory(Path.Combine(%%2, Path.GetFileName(dir2)));
		//		string dir = (dir2.LastIndexOf("\") == dir2.Length - 1) ? dir2 : dir2 + "\";
		//		DirectoryInfo dirdir = new DirectoryInfo(dir);
		//		FileSystemInfo[] fileArr = dirdir.GetFileSystemInfos();
		//		Queue Folders = new Queue(dirdir.GetFileSystemInfos());
		//		while (Folders.Count > 0)
		//		{
		//			FileSystemInfo tmp = Folders.Dequeue();
		//			FileInfo f = tmp as FileInfo;
		//			if (f == null)
		//			{
		//				DirectoryInfo d = tmp as DirectoryInfo;
		//				DirectoryInfo dpath = new DirectoryInfo(d.FullName.Replace((parent.LastIndexOf("\") == parent.Length - 1) ? parent : parent + "\", %%2));
		//				dpath.Create();
		//				foreach (FileSystemInfo fi in d.GetFileSystemInfos())
		//				{
		//					Folders.Enqueue(fi);
		//				}
		//			}
		//			else
		//			{
		//				f.MoveTo(f.FullName.Replace(parent, %%2));
		//			}
		//		}
		//		dirdir.Delete(true);
		//	}
		//}
		#endregion

		#region 17.以一个文件夹的框架在另一个目录创建文件夹和空文件
		//bool b=false;
		//string path = (%%2.LastIndexOf("\") == %%2.Length - 1) ? %%2 : %%2 + "\";
		//string parent = Path.GetDirectoryName(%%1);
		//Directory.CreateDirectory(path + Path.GetFileName(%%1));
		//DirectoryInfo dir = new DirectoryInfo((%%1.LastIndexOf("\") == %%1.Length - 1) ? %%1 : %%1 + "\");
		//FileSystemInfo[] fileArr = dir.GetFileSystemInfos();
		//Queue Folders = new Queue(dir.GetFileSystemInfos());
		//while (Folders.Count > 0)
		//{
		//	FileSystemInfo tmp = Folders.Dequeue();
		//	FileInfo f = tmp as FileInfo;
		//	if (f == null)
		//	{
		//		DirectoryInfo d = tmp as DirectoryInfo;
		//		Directory.CreateDirectory(d.FullName.Replace((parent.LastIndexOf("\") == parent.Length - 1) ? parent : parent + "\", path));
		//		foreach (FileSystemInfo fi in d.GetFileSystemInfos())
		//		{
		//			Folders.Enqueue(fi);
		//		}
		//	}
		//	else
		//	{
		//		if(b) File.Create(f.FullName.Replace(parent, path));
		//	}
		//}
		#endregion

		#region 18.复制文件
		//using System.IO;
		//File.Copy(%%1,%%2); 
		#endregion

		#region 19.复制一个文件夹下所有的文件到另一个目录
		//using System.IO;
		//foreach (string fileStr in Directory.GetFiles(%%1))
		//File.Copy((%%1.LastIndexOf("\") == %%1.Length - 1) ? %%1 +Path.GetFileName(fileStr): %%1 + "\"+Path.GetFileName(fileStr),(%%2.LastIndexOf("\") == %%2.Length - 1) ? %%2 +Path.GetFileName(fileStr): %%2 + "\"+Path.GetFileName(fileStr)); 
		#endregion

		#region 20.提取扩展名
		public static string GetFileExt(string strFilePathName)
		{
			return Path.GetExtension(strFilePathName);
		}
		#endregion

		#region 21.提取文件名
		public static string GetFileName(string strFilePathName)
		{
			return Path.GetFileName(strFilePathName); 
		}
		#endregion

		#region 22.提取文件路径
		//using System.IO;
		public static string GetFilePath(string strFilePathName)
		{
			return Path.GetDirectoryName(strFilePathName);
		}
		#endregion

		#region 23.替换扩展名
		//using System.IO;
		//File.ChangeExtension(%%1,%%2); 
		#endregion

		#region 24.追加路径
		//using System.IO;
		//string %%3=Path.Combine(%%1,%%2); 
		#endregion

		#region 25.移动文件
		//using System.IO;
		//File.Move(%%1,%%2+"\"+file.getname(%%1)); 
		#endregion

		#region 26.移动一个文件夹下所有文件到另一个目录
		//foreach (string fileStr in Directory.GetFiles(%%1))
		//File.Move((%%1.LastIndexOf("\") == %%1.Length - 1) ? %%1 +Path.GetFileName(fileStr): %%1 + "\"+Path.GetFileName(fileStr),(%%2.LastIndexOf("\") == %%2.Length - 1) ? %%2 +Path.GetFileName(fileStr): %%2 + "\"+Path.GetFileName(fileStr)); 
		#endregion

		#region 27.指定目录下搜索文件
		//string fileName=%%1;
		//string dirName=%%2;
		//DirectoryInfo   dirc=new   DirectoryInfo(dirName);
		//foreach(FileInfo   file   in   dirc.GetFiles()) {
		//if(file.Name.IndexOf(fileName)>-1)
		//return file.FullName;
		//}
		//foreach(DirectoryInfo   dir   in   dirc.GetDirectories())   {   
		//return   GetFile(fileName,dir.FullName);   
		//}
		//return   "找不到指定的文件";   
		//}
		#endregion

		#region 28.打开对话框
		//OpenFileDialog openFileDialog=new OpenFileDialog(); 
		//openFileDialog.InitialDirectory="c:\\\\";//注意这里写路径时要用c:\\\\而不是c:\\ 
		//openFileDialog.Filter="文本文件|*.*|C#文件|*.cs|所有文件|*.*"; 
		//openFileDialog.RestoreDirectory=true; 
		//openFileDialog.FilterIndex=1; 
		//if (openFileDialog.ShowDialog()==DialogResult.OK) { 
		//fName=openFileDialog.FileName; 
		//File fileOpen=new File(fName); 
		//isFileHaveName=true; 
		//%%1=fileOpen.ReadFile(); 
		//%%1.AppendText(""); 
		//}
		#endregion

		#region 29.文件分割
		//using System.IO;
		//FileStream fsr = new FileStream(%%1, FileMode.Open, FileAccess.Read);
		//byte[] btArr = new byte[fsr.Length];
		//fsr.Read(btArr, 0, btArr.Length);
		//fsr.Close();
		//string strFileName=%%1.Substring(%%1.LastIndexOf("\")+1);
		//FileStream fsw = new FileStream(%%2 + strFileName + "1", FileMode.Create, FileAccess.Write);
		//fsw.Write(btArr, 0, btArr.Length/2);
		//fsw.Close();
		//fsw = new FileStream(%%2 + strFileName + "2", FileMode.Create, FileAccess.Write);
		//fsw.Write(btArr, btArr.Length/2, btArr.Length-btArr.Length/2);
		//fsw.Close(); 
		#endregion

		#region 30.文件合并
		//using System.IO;
		//string strFileName = %%1.Substring(%%1.LastIndexOf("\") + 1);
		//FileStream fsr1 = new FileStream(%%2 + strFileName + "1", FileMode.Open, FileAccess.Read);
		//FileStream fsr2 = new FileStream(%%2 + strFileName + "2", FileMode.Open, FileAccess.Read);
		//byte[] btArr = new byte[fsr1.Length+fsr2.Length];
		//fsr1.Read(btArr, 0, Convert.ToInt32(fsr1.Length));
		//fsr2.Read(btArr, Convert.ToInt32(fsr1.Length), Convert.ToInt32(fsr2.Length));
		//fsr1.Close();fsr2.Close();
		//FileStream fsw = new FileStream(%%2 + strFileName, FileMode.Create, FileAccess.Write);
		//fsw.Write(btArr, 0, btArr.Length);
		//fsw.Close(); 
		#endregion

		#region 31.文件简单加密
		//using System.IO;
		//读文件
		//FileStream fsr = new FileStream(%%1, FileMode.Open, FileAccess.Read);
		//byte[] btArr = new byte[fsr.Length];
		//fsr.Read(btArr, 0, btArr.Length);
		//fsr.Close(); 
		//for (int i = 0; i < btArr.Length; i++){ //加密
		//int ibt = btArr[i];
		//ibt += 100;
		//ibt %= 256;
		//btArr[i] = Convert.ToByte(ibt);
		//}
		////写文件
		//string strFileName = Path.GetExtension(%%1);
		//FileStream fsw = new FileStream(%%2+"/" + "enc_" + strFileName, FileMode.Create, FileAccess.Write);
		//fsw.Write(btArr, 0, btArr.Length);
		//fsw.Close(); 
		#endregion

		#region 32.文件简单解密
		//using System.IO;
		//FileStream fsr = new FileStream(%%1, FileMode.Open, FileAccess.Read);
		//byte[] btArr = new byte[fsr.Length];
		//fsr.Read(btArr, 0, btArr.Length);
		//fsr.Close();
		//for (int i = 0; i < btArr.Length; i++){　//解密
		//int ibt = btArr[i];
		//ibt -= 100;
		//ibt += 256;
		//ibt %= 256;
		//btArr[i] = Convert.ToByte(ibt);
		//}
		////写文件
		//string strFileName = Path.GetExtension(%%1);
		//FileStream fsw = new FileStream(%%2 +"/" + strFileName, FileMode.Create, FileAccess.Write);
		//fsw.Write(btArr, 0, btArr.Length);
		//fsw.Close(); 
		#endregion

		#region 33.读取ini文件属性
		//using System.Runtime.InteropServices;
		//[DllImport("kernel32")]//返回取得字符串缓冲区的长度
		//private static extern long GetPrivateProfileString(string section,string key, string def,StringBuilder retVal,int size,string filePath);
		//string Section=%%1;
		//string Key=%%2;
		//string NoText=%%3;
		//string iniFilePath="Setup.ini";
		//string %%4=String.Empty;
		//if(File.Exists(iniFilePath)){
		//StringBuilder temp = new StringBuilder(1024);
		//GetPrivateProfileString(Section,Key,NoText,temp,1024,iniFilePath);
		//%%4=temp.ToString();
		//}
		#endregion

		#region 34.合并一个目录下所有的文件
		//using System.IO;
		//FileStream fsw = new FileStream(%%2, FileMode.Create, FileAccess.Write);
		//foreach (string fileStr in Directory.GetFiles(%%1))
		//{
		//FileStream fsr1 = new FileStream(fileStr, FileMode.Open, FileAccess.Read);
		//byte[] btArr = new byte[fsr1.Length];
		//fsr1.Read(btArr, 0, Convert.ToInt32(fsr1.Length));
		//fsr1.Close();
		//fsw.Write(btArr, 0, btArr.Length);
		//}
		//fsw.Close();
		#endregion

		#region 35.写入ini文件属性
		//using System.Runtime.InteropServices;
		//[DllImport("kernel32")]//返回0表示失败，非0为成功
		//private static extern long WritePrivateProfileString(string section,string key, string val,string filePath);
		//string Section=%%1;
		//string Key=%%2;
		//string Value=%%3;
		//string iniFilePath="Setup.ini";
		//bool %%4=false;
		//	if(File.Exists(iniFilePath))
		//	{
		//long OpStation = WritePrivateProfileString(Section,Key,Value,iniFilePath);    
		//if(OpStation == 0)
		//{
		//	%%4=false;
		//}
		//else
		//{
		//	%%4=true;
		//}
		//}
		#endregion

		#region 36.获得当前路径
		//string %%1=Environment.CurrentDirectory; 
		#endregion

		#region 37.读取XML数据库
		//using System.Xml;
		//XmlDocument doc=new XmlDocument();
		//doc.Load(%%1);
		//string %%9;
		//XmlElement xe=doc.GetElementById(%%7);
		//XmlNodeList elemList=xe.ChildNodes;
		//foreach(XmlNode elem in elemList)
		//{
		//if(elem.NodeType==%%8)
		//{
		//%%9=elem.Value;
		//break;
		//}
		//} 
		#endregion

		#region 38.写入XML数据库
		//using System.Xml;
		//XmlDocument doc=new XmlDocument();
		//doc.Load(%%1);
		//XmlNode root=doc.DocumentElement;
		//XmlElement book=doc.createElement_x_x(%%3);
		//XmlElement book=doc.createElement_x_x(%%5);
		//XmlElement port=doc.createElement_x_x(%%6);
		//book.SetAttribute(%%4,root.ChildNodes.Count.ToString());
		//author.InnerText=%%8;
		//book.a(author);
		//book.a(port);
		//root.a(book);
		//doc.Save(%%1); 
		#endregion

		#region 39.ZIP压缩文件
		//FileStream infile;
		//try
		//{
		//// Open the file as a FileStream object.
		//infile = new FileStream(%%1, FileMode.Open, FileAccess.Read, FileShare.Read);
		//byte[] buffer = new byte[infile.Length];
		//// Read the file to ensure it is readable.
		//int count = infile.Read(buffer, 0, buffer.Length);
		//if (count != buffer.Length)
		//{
		//infile.Close();
		////Test Failed: Unable to read data from file
		//return;
		//}
		//infile.Close();
		//MemoryStream ms = new MemoryStream();
		//// Use the newly created memory stream for the compressed data.
		//DeflateStream compressedzipStream = new DeflateStream(ms, CompressionMode.Compress, true);
		////Compression
		//compressedzipStream.Write(buffer, 0, buffer.Length);
		//// Close the stream.
		//compressedzipStream.Close();
		////Original size: {0}, Compressed size: {1}", buffer.Length, ms.Length);
		//FileInfo f = new FileInfo(%%2);
		//StreamWriter w = f.CreateText();
		//w.Write(buffer,0,ms.Length);
		//w.Close();
		//} // end try
		//catch (InvalidDataException)
		//{
		////Error: The file being read contains invalid data.
		//} catch (FileNotFoundException)
		//{
		////Error:The file specified was not found.
		//} catch (ArgumentException)
		//{
		////Error: path is a zero-length string, contains only white space, or contains one or more invalid characters
		//} catch (PathTooLongException)
		//{
		////Error: The specified path, file name, or both exceed the system-defined maximum length. For example, on Windows-based 

		//platforms, paths must be less than 248 characters, and file names must be less than 260 characters.
		//} catch (DirectoryNotFoundException)
		//{
		////Error: The specified path is invalid, such as being on an unmapped drive.
		//} catch (IOException)
		//{
		////Error: An I/O error occurred while opening the file.
		//} catch (UnauthorizedAccessException)
		//{
		////Error: path specified a file that is read-only, the path is a directory, or caller does not have the required 
		//permissions.
		//} catch (IndexOutOfRangeException)
		//{
		////Error: You must provide parameters for MyGZIP.
		//}
		#endregion

		#region 40.ZIP解压缩
		//FileStream infile;
		//try
		//{
		//	// Open the file as a FileStream object.
		//	infile = new FileStream(%%1, FileMode.Open, FileAccess.Read, FileShare.Read);
		//	byte[] buffer = new byte[infile.Length];
		//	// Read the file to ensure it is readable.
		//	int count = infile.Read(buffer, 0, buffer.Length);
		//	if (count != buffer.Length)
		//	{
		//infile.Close();
		////Test Failed: Unable to read data from file
		//return;
		//	}
		//	infile.Close();
		//	MemoryStream ms = new MemoryStream();
		//	// ms.Position = 0;
		//	DeflateStream zipStream = new DeflateStream(ms, CompressionMode.Decompress);
		//	//Decompression
		//	byte[] decompressedBuffer = new byte[buffer.Length *2];
		//	zipStream.Close();
		//FileInfo f = new FileInfo(%%2);
		//StreamWriter w = f.CreateText();
		//w.Write(decompressedBuffer);
		//w.Close();
		//} // end try
		//catch (InvalidDataException)
		//{
		//	//Error: The file being read contains invalid data.
		//}
		//catch (FileNotFoundException)
		//{
		//	//Error:The file specified was not found.
		//}
		//catch (ArgumentException)
		//{
		//	//Error: path is a zero-length string, contains only white space, or contains one or more invalid characters
		//}
		//catch (PathTooLongException)
		//{
		//	//Error: The specified path, file name, or both exceed the system-defined maximum length. For example, on Windows-based 

		//platforms, paths must be less than 248 characters, and file names must be less than 260 characters.
		//}
		//catch (DirectoryNotFoundException)
		//{
		//	//Error: The specified path is invalid, such as being on an unmapped drive.
		//}
		//catch (IOException)
		//{
		//	//Error: An I/O error occurred while opening the file.
		//}
		//catch (UnauthorizedAccessException)
		//{
		//	//Error: path specified a file that is read-only, the path is a directory, or caller does not have the required 

		//permissions.
		//}
		//catch (IndexOutOfRangeException)
		//{
		//	//Error: You must provide parameters for MyGZIP.
		//}
		#endregion

		#region 41.获得应用程序完整路径
		//string %%1=Application.ExecutablePath;
		#endregion

		#region 42.ZIP压缩文件夹
		//private void CreateCompressFile(Stream source, string destinationName)
		//{
		//	using (Stream destination = new FileStream(destinationName, FileMode.Create, FileAccess.Write))
		//	{
		//		using (GZipStream output = new GZipStream(destination, CompressionMode.Compress))
		//		{
		//			byte[] bytes = new byte[4096];
		//			int n;
		//			while ((n = source.Read(bytes, 0, bytes.Length)) != 0)
		//			{
		//				output.Write(bytes, 0, n);
		//			}
		//		}
		//	}
		//}
		//ArrayList list = new ArrayList();
		//foreach (string f in Directory.GetFiles(%%1))
		//{
		//	byte[] destBuffer = File.ReadAllBytes(f);
		//	SerializeFileInfo sfi = new SerializeFileInfo(f, destBuffer);
		//	list.Add(sfi);
		//}
		//IFormatter formatter = new BinaryFormatter();
		//using (Stream s = new MemoryStream())
		//{
		//	formatter.Serialize(s, list);
		//	s.Position = 0;
		//	CreateCompressFile(s, %%2);
		//} 
		//[Serializable]
		//class SerializeFileInfo
		//{
		//	public SerializeFileInfo(string name, byte[] buffer)
		//	{
		//		fileName = name;
		//		fileBuffer = buffer;
		//	}
		//	string fileName;
		//	public string FileName
		//	{
		//		get
		//		{
		//			return fileName;
		//		}
		//	}
		//	byte[] fileBuffer;
		//	public byte[] FileBuffer
		//	{
		//		get
		//		{
		//			return fileBuffer;
		//		}
		//	}
		//}
		#endregion

		#region 43.递归删除目录下的文件
		//using System.IO;
		//DirectoryInfo DInfo=new DirectoryInfo(%%1);
		//FileSystemInfo[] FSInfo=DInfo.GetFileSystemInfos();
		//for(int i=0;i
		//{
		//FileInfo FInfo=new FileInfo(%%1+FSInfo[i].ToString());
		//FInfo.Delete();
		//}
		#endregion

		#region 44.验证DTD
		//XmlReaderSettings settings = new XmlReaderSettings(); 
		//settings.ProhibitDtd = false; 
		//settings.ValidationType = ValidationType.DTD; 
		//settings.ValidationEventHandler += new ValidationEventHandler(ValidationCallBack); 
		//// Create the XmlReader object. 
		//XmlReader reader = XmlReader.Create("my book.xml", settings); 
		//// Parse the file. 
		//while (reader.Read());
		//// Display any validation errors. 
		//private static void ValidationCallBack(object sender, ValidationEventArgs e) 
		//{ 
		//Console.WriteLine("Validation Error: {0}", e.Message); 
		//} 
		#endregion

		#region 45.Schema 验证
		//Boolean m_success;
		//XmlValidatingReader reader = null;
		//   XmlSchemaCollection myschema = new XmlSchemaCollection();
		//ValidationEventHandler eventHandler = new ValidationEventHandler(ShowCompileErrors);
		//try
		//{
		////Create the XML fragment to be parsed.
		//String xmlFrag = "" +
		//"Herman" +
		//"Melville" +
		//"";
		////Create the XmlParserContext.
		//XmlParserContext context = new XmlParserContext(null, null, "", XmlSpace.None);
		////Implement the reader.
		//reader = new XmlValidatingReader(xmlFrag, XmlNodeType.Element, context);
		////Add the schema.
		//myschema.Add("urn:bookstore-schema", "c:\\Books.xsd");
		////Set the schema type and add the schema to the reader.
		//reader.ValidationType = ValidationType.Schema;
		//reader.Schemas.Add(myschema);
		//while (reader.Read())
		//{
		//}
		//Console.WriteLine("Completed validating xmlfragment");
		//}
		//catch (XmlException XmlExp)
		//{
		//Console.WriteLine(XmlExp.Message);
		//}
		//catch(XmlSchemaException XmlSchExp)
		//{
		//Console.WriteLine(XmlSchExp.Message);
		//}
		//catch(Exception GenExp)
		//{
		//Console.WriteLine(GenExp.Message);
		//}
		//finally
		//{
		//Console.Read();
		//}
		//public static void ShowCompileErrors(object sender, ValidationEventArgs args)
		//{
		//Console.WriteLine("Validation Error: {0}", args.Message);
		//} 
		#endregion

		#region 46.Grep
		//Traditionally grep stands for "Global Regular Expression Print".
		//Global means that an entire file is searched. 
		//Regular Expression means that a regular expression string is used to establish a search pattern. 
		//Print means that the command will display its findings. 
		//Simply put, grep searches an entire file for the pattern you want and displays its findings.
		//
		//The use syntax is different from the traditional Unix syntax, I prefer a syntax similar to
		//csc, the C# compiler.
		//
		// grep [/h|/H] - Usage Help
		//
		// grep [/c] [/i] [/l] [/n] [/r] /E:reg_exp /F:files
		//
		// /c - print a count of matching lines for each input file;
		// /i - ignore case in pattern;
		// /l - print just files (scanning will stop on first match);
		// /n - prefix each line of output with line number;
		// /r - recursive search in subdirectories;
		//
		// /E:reg_exp - the Regular Expression used as search pattern. The Regular Expression can be delimited by
		// quotes like "..." and '...' if you want to include in it leading or trailing blanks;
		//
		// /F:files - the list of input files. The files can be separated by commas as in /F:file1,file2,file3
		//and wildcards can be used for their specification as in /F:*file?.txt;
		//
		//Example:
		//
		// grep /c /n /r /E:" C Sharp " /F:*.cs 
		//Option Flags
		//private bool m_bRecursive;
		//private bool m_bIgnoreCase;
		//private bool m_bJustFiles;
		//private bool m_bLineNumbers;
		//private bool m_bCountLines;
		//private string m_strRegEx;
		//private string m_strFiles;
		////ArrayList keeping the Files
		//private ArrayList m_arrFiles = new ArrayList();
		////Properties
		//public bool Recursive
		//{
		//get { return m_bRecursive; }
		//set { m_bRecursive = value; }
		//} 

		//public bool IgnoreCase
		//{
		//get { return m_bIgnoreCase; }
		//set { m_bIgnoreCase = value; }
		//} 

		//public bool JustFiles
		//{
		//get { return m_bJustFiles; }
		//set { m_bJustFiles = value; }
		//} 

		//public bool LineNumbers
		//{
		//get { return m_bLineNumbers; }
		//set { m_bLineNumbers = value; }
		//} 

		//public bool CountLines
		//{
		//get { return m_bCountLines; }
		//set { m_bCountLines = value; }
		//} 

		//public string RegEx
		//{
		//get { return m_strRegEx; }
		//set { m_strRegEx = value; }
		//} 

		//public string Files
		//{
		//get { return m_strFiles; }
		//set { m_strFiles = value; }
		//} 

		////Build the list of Files
		//private void GetFiles(String strDir, String strExt, bool bRecursive)
		//{
		////search pattern can include the wild characters '*' and '?'
		//string[] fileList = Directory.GetFiles(strDir, strExt);
		//for(int i=0; i
		//{
		//if(File.Exists(fileList[i]))
		//m_arrFiles.Add(fileList[i]);
		//}
		//if(bRecursive==true)
		//{
		////Get recursively from subdirectories
		//string[] dirList = Directory.GetDirectories(strDir);
		//for(int i=0; i
		//{
		//GetFiles(dirList[i], strExt, true);
		//}
		//}
		//} 

		////Search Function
		//public void Search()
		//{
		//String strDir = Environment.CurrentDirectory;
		////First empty the list
		//m_arrFiles.Clear();
		////Create recursively a list with all the files complying with the criteria
		//String[] astrFiles = m_strFiles.Split(new Char[] {','});
		//for(int i=0; i
		//{
		////Eliminate white spaces
		//astrFiles[i] = astrFiles[i].Trim();
		//GetFiles(strDir, astrFiles[i], m_bRecursive);
		//}
		////Now all the Files are in the ArrayList, open each one
		////iteratively and look for the search string
		//String strResults = "Grep Results:\r\n\r\n";
		//String strLine;
		//int iLine, iCount;
		//bool bEmpty = true;
		//IEnumerator enm = m_arrFiles.GetEnumerator();
		//while(enm.MoveNext())
		//{
		//try
		//{
		//StreamReader sr = File.OpenText((string)enm.Current);
		//iLine = 0;
		//iCount = 0;
		//bool bFirst = true;
		//while((strLine = sr.ReadLine()) != null)
		//{
		//iLine++;
		////Using Regular Expressions as a real Grep
		//Match mtch;
		//if(m_bIgnoreCase == true)
		//mtch = Regex.Match(strLine, m_strRegEx, RegexOptions.IgnoreCase);
		//else
		//mtch = Regex.Match(strLine, m_strRegEx);
		//if(mtch.Success == true)
		//{
		//bEmpty = false;
		//iCount++;
		//if(bFirst == true)
		//{
		//if(m_bJustFiles == true)
		//{
		//strResults += (string)enm.Current + "\r\n";
		//break;
		//}
		//else
		//strResults += (string)enm.Current + ":\r\n";
		//bFirst = false;
		//}
		////Add the Line to Results string
		//if(m_bLineNumbers == true)
		//strResults += " " + iLine + ": " + strLine + "\r\n";
		//else
		//strResults += " " + strLine + "\r\n";
		//}
		//}
		//sr.Close();
		//if(bFirst == false)
		//{
		//if(m_bCountLines == true)
		//strResults += " " + iCount + " Lines Matched\r\n";
		//strResults += "\r\n";
		//}
		//}
		//catch(SecurityException)
		//{
		//strResults += "\r\n" + (string)enm.Current + ": Security Exception\r\n\r\n"; 
		//}
		//catch(FileNotFoundException)
		//{
		//strResults += "\r\n" + (string)enm.Current + ": File Not Found Exception\r\n";
		//}
		//}
		//if(bEmpty == true)
		//Console.WriteLine("No matches found!");
		//else
		//Console.WriteLine(strResults);
		//} 

		////Print Help
		//private static void PrintHelp()
		//{
		//Console.WriteLine("Usage: grep [/h|/H]");
		//Console.WriteLine("       grep [/c] [/i] [/l] [/n] [/r] /E:reg_exp /F:files");
		//} 

		//Arguments CommandLine = new Arguments(args);
		//if(CommandLine["h"] != null || CommandLine["H"] != null)
		//{
		//PrintHelp();
		//return;
		//}
		//// The working object
		//ConsoleGrep grep = new ConsoleGrep();
		//// The arguments /e and /f are mandatory
		//if(CommandLine["E"] != null)
		//grep.RegEx = (string)CommandLine["E"];
		//else
		//{
		//Console.WriteLine("Error: No Regular Expression specified!");
		//Console.WriteLine();
		//PrintHelp();
		//return;
		//}
		//if(CommandLine["F"] != null)
		//grep.Files = (string)CommandLine["F"];
		//else
		//{
		//Console.WriteLine("Error: No Search Files specified!");
		//Console.WriteLine();
		//PrintHelp();
		//return;
		//}
		//grep.Recursive = (CommandLine["r"] != null);
		//grep.IgnoreCase = (CommandLine["i"] != null);
		//grep.JustFiles = (CommandLine["l"] != null);
		//if(grep.JustFiles == true)
		//grep.LineNumbers = false;
		//else
		//grep.LineNumbers = (CommandLine["n"] != null);
		//if(grep.JustFiles == true)
		//grep.CountLines = false;
		//else
		//grep.CountLines = (CommandLine["c"] != null);
		//// Do the search
		//grep.Search();
		#endregion

		#region 47.直接创建多级目录
		////using System.IO;
		//DirectoryInfo di=new DirectoryInfo(%%1);
		//di.CreateSubdirectory(%%2);
		#endregion

		#region 48.批量重命名
		//using System.IO;
		//string strOldFileName; string strNewFileName; string strOldPart =this.textBox1.Text.Trim();//重命名文件前的文件名等待替换字符串
		//string strNewPart = this.textBox2.Text.Trim();//重命名文件后的文件名替换字符串
		//string strNewFilePath;
		//string strFileFolder;    //原始图片目录
		//int TotalFiles = 0; DateTime StartTime = DateTime.Now; //获取开始时间   
		//FolderBrowserDialog f1 = new FolderBrowserDialog(); //打开选择目录对话框
		//if (f1.ShowDialog() == DialogResult.OK) {
		//strFileFolder = f1.SelectedPath;
		//DirectoryInfo di = new DirectoryInfo(strFileFolder);
		//FileInfo[] filelist = di.GetFiles("*.*"); 
		//int i = 0;
		//foreach (FileInfo fi in filelist) { 
		//strOldFileName = fi.Name;
		//strNewFileName = fi.Name.Replace(strOldPart, strNewPart);
		//strNewFilePath = @strFileFolder + "\" + strNewFileName;
		//filelist[i].MoveTo(@strNewFilePath); TotalFiles += 1; 
		//this.listBox1.Items.Add("文件名：" + strOldFileName + "已重命名为" + strNewFileName);
		//i += 1; 
		//} 
		//}
		//DateTime EndTime = DateTime.Now;//获取结束时间
		//TimeSpan ts = EndTime - StartTime; this.listBox1.Items.Add("总耗时：" + ts.Hours.ToString() + "时" +ts.Minutes.ToString() + "分" + ts.Seconds.ToString() + "秒");
		#endregion

		#region 49.文本查找替换
		//if (args.Length == 3)
		//{
		//ReplaceFiles(args[0],args[1],args[2],null);
		//}
		//if (args.Length == 4)
		//{
		//if (args[3].Contains("v"))
		//{
		//	ReplaceVariable(args[0], args[1], args[2], args[3]);
		//}
		//else
		//{
		//	ReplaceFiles(args[0], args[1], args[2], args[3]);
		//}
		//}
		/////
		///// 替换环境变量中某个变量的文本值。可以是系统变量，用户变量，临时变量。替换时会覆盖原始值。小心使用
		/////
		/////
		/////
		/////
		/////
		//public static void ReplaceVariable(string variable, string search, string replace, string options)
		//{
		//string variable=%%1;
		//string search=%%2;
		//string replace=%%3;
		//string text=Environment.GetEnvironmentVariable(variable);
		//System.Windows.Forms.MessageBox.Show(text);
		//text=ReplaceText(text, search, replace, options);
		//Environment.SetEnvironmentVariable(variable, text);
		//text = Environment.GetEnvironmentVariable(variable);
		//System.Windows.Forms.MessageBox.Show(text);
		//}
		/////
		///// 批量替换文件文本
		/////
		/////
		//public static void ReplaceFiles(string path,string search, string replace, string options)
		//{
		//string path=%%1;
		//string search=%%2;
		//string replace=%%3;
		//string[] fs;
		//if(File.Exists(path)){
		//ReplaceFile(path, search, replace, options);
		//return;
		//}
		//if (Directory.Exists(path))
		//{
		//fs = Directory.GetFiles(path);
		//foreach (string f in fs)
		//{
		//	ReplaceFile(f, search, replace, options);
		//}
		//return;
		//}
		//int i=path.LastIndexOf("");
		//if(i<0)i=path.LastIndexOf("/");
		//string d, searchfile;
		//if (i > -1)
		//{
		//d = path.Substring(0, i + 1);
		//searchfile = path.Substring(d.Length);
		//}
		//else
		//{
		//d = System.Environment.CurrentDirectory;
		//searchfile = path;
		//}
		//searchfile = searchfile.Replace(".", @".");
		//searchfile = searchfile.Replace("?", @"[^.]?");
		//searchfile = searchfile.Replace("*", @"[^.]*");
		////System.Windows.Forms.MessageBox.Show(d);  System.Windows.Forms.MessageBox.Show(searchfile);
		//if (!Directory.Exists(d)) return;
		//fs = Directory.GetFiles(d);
		//foreach (string f in fs)
		//{
		//if(System.Text.RegularExpressions.Regex.IsMatch(f,searchfile))
		//	ReplaceFile(f, search, replace, options);
		//}
		//}
		/////
		///// 替换单个文本文件中的文本
		/////
		/////
		/////
		/////
		/////
		/////
		//public static bool ReplaceFile(string filename, string search, string replace,string options)
		//{
		//string path=%%1;
		//string search=%%2;
		//string replace=%%3;
		//FileStream fs = File.OpenRead(filename);
		////判断文件是文本文件还二进制文件。该方法似乎不科学
		//byte b;
		//for (long i = 0; i < fs.Length; i++)
		//{
		//b = (byte)fs.ReadByte();
		//if (b == 0)
		//{
		//	System.Windows.Forms.MessageBox.Show("非文本文件");
		//	return false;//有此字节则表示改文件不是文本文件。就不用替换了
		//}
		//}
		////判断文本文件编码规则。
		//byte[] bytes = new byte[2];
		//Encoding coding=Encoding.Default;
		//if (fs.Read(bytes, 0, 2) > 2)
		//{
		//if (bytes == new byte[2] { 0xFF, 0xFE }) coding = Encoding.Unicode;
		//if (bytes == new byte[2] { 0xFE, 0xFF }) coding = Encoding.BigEndianUnicode;
		//if (bytes == new byte[2] { 0xEF, 0xBB }) coding = Encoding.UTF8;
		//}
		//fs.Close();
		////替换数据
		//string text=File.ReadAllText(filename, coding);
		//text=ReplaceText(text,search, replace, options);
		//File.WriteAllText(filename, text, coding);
		//return true;
		//}
		/////
		///// 替换文本
		/////
		/////
		/////
		/////
		/////
		/////
		//public static string ReplaceText(string text, string search, string replace, string options)
		//{
		//	RegexOptions ops = RegexOptions.None;
		//	if (options == null)  //纯文本替换
		//	{
		//	search = search.Replace(".", @".");
		//	search = search.Replace("?", @"?");
		//	search = search.Replace("*", @"*");
		//	search = search.Replace("(", @"(");
		//	search = search.Replace(")", @")");
		//	search = search.Replace("[", @"[");
		//	search = search.Replace("[", @"[");
		//	search = search.Replace("[", @"[");
		//	search = search.Replace("{", @"{");
		//	search = search.Replace("}", @"}");
		//	ops |= RegexOptions.IgnoreCase;
		//	}
		//	else
		//	{
		//	if(options.Contains("I"))ops |= RegexOptions.IgnoreCase;
		//	}
		//	text = Regex.Replace(text, search, replace, ops);
		//	return text;
		//}
		#endregion

		#region 50.文件关联
		//using Microsoft.Win32;
		//string keyName; 
		//string keyValue; 
		//keyName = %%1; //"WPCFile"
		//keyValue = %%2; //"资源包文件"
		//RegistryKey isExCommand = null; 
		//bool isCreateRegistry = true; 
		//try 
		//{ 
		//	/// 检查 文件关联是否创建 
		//	isExCommand = Registry.ClassesRoot.OpenSubKey(keyName); 
		//	if (isExCommand == null) 
		//	{ 
		//		isCreateRegistry = true; 
		//	} 
		//	else 
		//	{ 
		//		if (isExCommand.GetValue("Create").ToString() == Application.ExecutablePath.ToString()) 
		//		{ 
		//			isCreateRegistry = false; 
		//		} 
		//		else 
		//		{ 
		//			Registry.ClassesRoot.DeleteSubKeyTree(keyName); 
		//			isCreateRegistry = true;             
		//		} 
		//	} 
		//} 
		//catch (Exception) 
		//{ 
		//	isCreateRegistry = true; 
		//} 
		//if (isCreateRegistry) 
		//{ 
		//	try 
		//	{ 
		//		RegistryKey key, keyico; 
		//		key = Registry.ClassesRoot.CreateSubKey(keyName); 
		//		key.SetValue("Create", Application.ExecutablePath.ToString()); 
		//		keyico = key.CreateSubKey("DefaultIcon"); 
		//		keyico.SetValue("", Application.ExecutablePath + ",0"); 
		//		key.SetValue("", keyValue); 
		//		key = key.CreateSubKey("Shell"); 
		//		key = key.CreateSubKey("Open"); 
		//		key = key.CreateSubKey("Command"); 
		//		key.SetValue("", """ + Application.ExecutablePath.ToString() + "" "%1""); 
		//		keyName = %%3; //".wpc"
		//		keyValue = %%1; 
		//		key = Registry.ClassesRoot.CreateSubKey(keyName); 
		//		key.SetValue("", keyValue); 
		//	} 
		//	catch (Exception) 
		//	{ 
		//	} 
		//}
		#endregion

		#region 51.操作Excel文件
		//using Excel;
		//private static string Connstring ;//连接字符串
		//Workbook myBook = null;
		//Worksheet mySheet=null;
		//Excel.ApplicationClass ExlApp = new ApplicationClass();
		//ExlApp.Visible =true;
		//object oMissiong = System.Reflection.Missing.Value;
		//string reqpath = this.Request.PhysicalPath;
		//int pos = reqpath.LastIndexOf("\");
		//reqpath = reqpath.Substring(0,pos);
		//ExlApp.Workbooks.Open(%%1,oMissiong, oMissiong, oMissiong,oMissiong, oMissiong, oMissiong,
		//oMissiong,oMissiong,oMissiong, oMissiong, oMissiong, oMissiong);//, oMissiong);//, oMissiong); // reqpath + "\\scxx.xls"
		//myBook = ExlApp.Workbooks[1];
		//mySheet = (Worksheet)myBook.Worksheets[1];
		//Excel.Range rg;  
		//string mySelectQuery = %%2; //"SELECT dh, qy,zb FROM SCXXB"
		//using(SqlConnection myConnection = new SqlConnection(Connstring)){
		//SqlCommand myCommand = new SqlCommand(mySelectQuery,myConnection);
		//myConnection.Open();
		//SqlDataReader myReader;
		//myReader = myCommand.ExecuteReader();
		//// Always call Read before accessing data.
		//int recount=0;
		//while (myReader.Read()) 
		//{
		//recount=recount+1;
		//}
		//myReader.Close();
		//myConnection.Close();
		//int gho=3;
		//for(int i = 1; i < recount ; i ++)
		//{
		//rg = mySheet.get_Range("A" +  gho.ToString(), "C" + ( gho ).ToString());
		//rg.Copy(oMissiong);
		//rg.Insert(XlInsertShiftDirection.xlShiftDown);
		//}
		////从数据表中取数据
		//mySelectQuery = %%2; //"SELECT dh, qy,zb FROM SCXXB ORDER BY qy,zb";
		//myConnection = new SqlConnection(Connstring);
		//myCommand = new SqlCommand(mySelectQuery,myConnection);
		//myConnection.Open();
		//myReader = myCommand.ExecuteReader();
		//int Curhs =  gho ;
		//while (myReader.Read()) 
		//{
		//mySheet.Cells[Curhs,1] =myReader["qy"].ToString() ;
		//mySheet.Cells[Curhs,2] =myReader["zb"].ToString() ;
		//mySheet.Cells[Curhs,3] =myReader["dh"].ToString() ;
		//Curhs ++;
		//}
		//myReader.Close();
		////合并最后一页
		//MergeCell(ref mySheet,3 , recount ,"A"); //调用函数实现A列合并
		//MergeCell(ref mySheet,3 , recount ,"B"); //调用函数实现A列合并
		//myBook.SaveAs(%%1, oMissiong,oMissiong, oMissiong,oMissiong,oMissiong,Excel.XlSaveAsAccessMode.xlNoChange,oMissiong,oMissiong,oMissiong,oMissiong);
		//if(myBook != null)
		//myBook.Close(true, %%1, true);
		//if(mySheet != null)
		//System.Runtime.InteropServices.Marshal.ReleaseComObject (mySheet);
		//mySheet = null;
		//if(myBook != null)
		//System.Runtime.InteropServices.Marshal.ReleaseComObject (myBook);
		//myBook = null;
		//if(ExlApp != null)
		//{
		//ExlApp.Quit();
		//System.Runtime.InteropServices.Marshal.ReleaseComObject ((object)ExlApp);
		//ExlApp = null;
		//}
		//GC.Collect();
		///// 合并单元格
		//private void MergeCell(ref Worksheet mySheet, int startLine,int RecCount, string Col)
		//{
		//string qy1 = mySheet.get_Range(Col + startLine.ToString(), Col + startLine.ToString()).Text.ToString();
		//Excel.Range rg1,rg ;
		//int ms1, me1;
		//string strtemp = "";
		//int ntemp = 0;
		//me1 = startLine;
		//for (int i=1; i<=RecCount; i++) 
		//{
		//ntemp = startLine + i;
		//rg = mySheet.get_Range(Col+ ntemp.ToString(), Col+ ntemp.ToString());
		//strtemp = rg.Text.ToString().Trim();
		//if (qy1.Trim() != strtemp.Trim())
		//{
		//ms1 = me1;
		//me1 = i + startLine - 1;
		////合并
		//if (me1-ms1>0)
		//{
		//rg1 = mySheet.get_Range(Col + ms1.ToString(), Col + me1.ToString());
		//rg1.ClearContents();
		//rg1.MergeCells = true;
		//if(Col == "A")
		//mySheet.Cells[ms1,1] = qy1;
		//else if (Col == "B")
		//mySheet.Cells[ms1,2] = qy1;
		//}
		//me1 += 1;
		//strtemp = mySheet.get_Range(Col + me1.ToString(), Col + me1.ToString()).Text.ToString();
		//if(strtemp.Trim() != "")
		//qy1 = strtemp;
		//}
		//} 
		//}
		#endregion

		#region 52.设置JDK环境变量
		//using Microsoft.Win32;
		//int isFileNum=0;
		//int i=0;
		//Environment.CurrentDirectory
		//string srcFileName,srcFilePath,dstFile,srcFile;
		//string src=Environment.CurrentDirectory+"\\*.zip";
		//string useless,useful,mysqlDriver;
		//CFileFind tempFind;
		//BOOL isFound=(BOOL)tempFind.FindFile(src);
		//RegistryKey rkLocalM = Registry.CurrentUser; //Registry.ClassesRoot, Registry.LocalMachine, Registry.Users, Registry.CurrentConfig
		//const string strSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU";
		//RegistryKey rkSub = rkLocalM.CreateSubKey( strSubKey );
		//rkSub.SetValue("a","winword -q\\1");
		//rkSub.SetValue("MRUList","azyxwvutsrqponmlkjihgfedcb");
		//rkSub.SetValue("b","cmd /k\\1");
		//rkSub.SetValue("c","iexplore -k\\1");
		//rkSub.SetValue("d","iexpress\\1");
		//rkSub.SetValue("e","mmc\\1");
		//rkSub.SetValue("f","msconfig\\1");
		//rkSub.SetValue("g","regedit\\1");
		//rkSub.SetValue("h","regedt32\\1");
		//rkSub.SetValue("i","Regsvr32 /u wmpshell.dll\\1");
		//rkSub.SetValue("j","sfc /scannow\\1");
		//rkSub.SetValue("k","shutdown -s -f -t 600\\1");
		//rkSub.SetValue("l","shutdown -a\\1");
		//rkSub.SetValue("m","C:\\TurboC\\BIN\\TC.EXE\\1");
		//rkSub.SetValue("n","services.msc\\1");
		//rkSub.SetValue("o","gpedit.msc\\1");
		//rkSub.SetValue("p","fsmgmt.msc\\1");
		//rkSub.SetValue("q","diskmgmt.msc\\1");
		//rkSub.SetValue("r","dfrg.msc\\1");
		//rkSub.SetValue("s","devmgmt.msc\\1");
		//rkSub.SetValue("t","compmgmt.msc\\1");
		//rkSub.SetValue("u","ciadv.msc\\1");
		//rkSub.SetValue("v","C:\\MATLAB701\\bin\\win32\\MATLAB.exe -nosplash -nojvm\\1");
		//rkSub.SetValue("w","C:\\MATLAB701\\bin\\win32\\MATLAB.exe -nosplash\\1");
		//rkSub.SetValue("x","C:\\Program Files\\Kingsoft\\PowerWord 2005\\XDICT.EXE" -nosplash\\1");
		//rkSub.SetValue("y","powerpnt -splash\\1");
		//rkSub.SetValue("z","excel -e\\1");
		//RegistryKey rkSub = rkLocalM.OpenSubKey("Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Regedit\\Favorites");
		//rkSub.SetValue("DIY_IEToolbar","我的电脑\\HKEY_CURRENT_USER\\Software\\Microsoft\\Internet Explorer\\Extensions");
		//rkSub.SetValue("文件夹右键菜单","我的电脑\\HKEY_CLASSES_ROOT\\Folder");
		//rkSub.SetValue("指向“收藏夹”","我的电脑\\HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Regedit\\Favorites");
		//rkSub.SetValue("默认安装目录（SourcePath）","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
		//rkSub.SetValue("设定字体替换","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\FontSubstitutes");
		//rkSub.SetValue("设置光驱自动运行功能（AutoRun）","我的电脑\\HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Cdrom");
		//rkSub.SetValue("改变鼠标设置","我的电脑\\HKEY_CURRENT_USER\\Control Panel\\Mouse");
		//rkSub.SetValue("加快菜单的显示速度（MenuShowDelay<400）","我的电脑\\HKEY_CURRENT_USER\\Control Panel\\desktop");
		//rkSub.SetValue("修改系统的注册单位（RegisteredOrganization）","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
		//rkSub.SetValue("查看启动","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
		//rkSub.SetValue("查看单次启动1","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce");
		//rkSub.SetValue("查看单次启动2","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx");
		//rkSub.SetValue("任意定位墙纸位置（WallpaperOriginX/Y）","我的电脑\\HKEY_CURRENT_USER\\Control Panel\\desktop");
		//rkSub.SetValue("设置启动信息提示（LegalNoticeCaption/Text）","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon");
		//rkSub.SetValue("更改登陆时的背景图案（Wallpaper）","我的电脑\\HKEY_USERS\\.DEFAULT\\Control Panel\\Desktop");
		//rkSub.SetValue("限制远程修改本机注册表（\\winreg\\AllowedPaths\\Machine）","我的电脑\\HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SecurePipeServers");
		//rkSub.SetValue("修改环境变量","我的电脑\\HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
		//rkSub.SetValue("设置网络服务器（severname","\\\\ROBERT）");
		//rkSub.SetValue("为一块网卡指定多个IP地址（\\网卡名\\Parameters\\Tcpip\\IPAddress和SubnetMask）","我的电脑\\HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services");
		//rkSub.SetValue("去除可移动设备出错信息（\\设备名\\ErrorControl）","我的电脑\\HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services");
		//rkSub.SetValue("限制使用显示属性","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\policies\\system");
		//rkSub.SetValue("不允许拥护在控制面板中改变显示模式（NoDispAppearancePage）","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\policies\\system");
		//rkSub.SetValue("隐藏控制面板中的“显示器”设置（NoDispCPL）","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\policies\\system");
		//rkSub.SetValue("不允许用户改变主面背景和墙纸（NoDispBackgroundPage）","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\policies\\system");
		//rkSub.SetValue("“显示器”属性中将不会出现“屏幕保护程序”标签页（NoDispScrSavPage）","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\policies\\system");
		//rkSub.SetValue("“显示器”属性中将不会出现“设置”标签页（NoDispSettingPage）","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\policies\\system");
		//rkSub.SetValue("阻止用户运行任务管理器（DisableTaskManager）","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\policies\\system");
		//rkSub.SetValue("“启动”菜单记录信息","我的电脑\\HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU");
		//rkSub.SetValue("Office2003用户指定文件夹","我的电脑\\HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\11.0\\Common\\Open Find\\Places\\UserDefinedPlaces");
		//rkSub.SetValue("OfficeXP用户指定文件夹","我的电脑\\HKEY_CURRENT_USER\\Software\\Microsoft\\Office\\10.0\\Common\\Open Find\\Places\\UserDefinedPlaces");
		//rkSub.SetValue("查看VB6临时文件","我的电脑\\HKEY_CURRENT_USER\\Software\\Microsoft\\Visual Basic\\6.0\\RecentFiles");
		//rkSub.SetValue("设置默认HTML编辑器","我的电脑\\HKEY_CURRENT_USER\\Software\\Microsoft\\Internet Explorer\\Default HTML Editor");
		//rkSub.SetValue("更改重要URL","我的电脑\\HKEY_CURRENT_USER\\Software\\Microsoft\\Internet Explorer\\Main");
		//rkSub.SetValue("控制面板注册位置","我的电脑\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Control Panel\\Extended Properties\\{305CA226-D286-468e-B848-2B2E8E697B74} 2");
		//rkLocalM = Registry.ClassesRoot; //Registry.ClassesRoot, Registry.LocalMachine, Registry.Users, Registry.CurrentConfig
		//rkSub = rkLocalM.OpenSubKey("Directory\\shell\\cmd");
		//rkSub.SetValue("","在这里打开命令行窗口");
		//rkSub = rkLocalM.OpenSubKey("Directory\\shell\\cmd\\command");
		//rkSub.SetValue("","cmd.exe /k "cd %L"");
		//rkLocalM = Registry.LocalMachine; //Registry.ClassesRoot, Registry.LocalMachine, Registry.Users, Registry.CurrentConfig
		//rkSub = rkLocalM.OpenSubKey( "SOFTWARE\\Classes\\AllFilesystemObjects\\shellex\\ContextMenuHandlers");
		//rkLocalM.CreateSubKey("Copy To");
		//rkLocalM.CreateSubKey("Move To");
		//rkLocalM.CreateSubKey("Send To");
		//rkSub = rkLocalM.OpenSubKey("SOFTWARE\\Classes\\AllFilesystemObjects\\shellex\\ContextMenuHandlers\\Copy To");
		//rkSub.SetValue("","{C2FBB630-2971-11D1-A18C-00C04FD75D13}");
		//rkSub = rkLocalM.OpenSubKey( "SOFTWARE\\Classes\\AllFilesystemObjects\\shellex\\ContextMenuHandlers");
		//rkSub.SetValue("","{C2FBB631-2971-11D1-A18C-00C04FD75D13}");
		//rkSub = rkLocalM.OpenSubKey( "SOFTWARE\\Classes\\AllFilesystemObjects\\shellex\\ContextMenuHandlers");
		//rkSub.SetValue("","{7BA4C740-9E81-11CF-99D3-00AA004AE837}");
		//rkSub = rkLocalM.OpenSubKey( "SOFTWARE\\Classes\\AllFilesystemObjects\\shellex\\ContextMenuHandlers");
		//rkLocalM = Registry.LocalMachine; 
		//rkSub = rkLocalM.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\Folder\\Hidden\\SHOWALL");
		//rkSub.SetValue( "RegPath","Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced");
		//rkSub.SetValue( "ValueName","Hidden");
		//rkSub = rkLocalM.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ControlPanel\\NameSpace\\{6DFD7C5C-2451-11d3-A299-00C04F8EF6AF}");
		//rkSub.SetValue("","Folder Options");
		//rkLocalM = Registry.ClassesRoot;
		//rkSub = rkLocalM.OpenSubKey("CLSID\\{6DFD7C5C-2451-11d3-A299-00C04F8EF6AF}"))
		//rkSub.SetValue(CLSID.WriteString("","文件夹选项");
		//rkSub = rkLocalM.OpenSubKey("CLSID\\{6DFD7C5C-2451-11d3-A299-00C04F8EF6AF}\\Shell\\RunAs\\Command"))
		//rkSub.SetValue("Extended","");
		//}
		//else if(srcFileName.Find("resin")==0)
		//{
		//useless.Format("%s\\%s",szDir,"resin.exe");
		//srcFile=useless.GetBuffer(0);
		//dstFile=srcFilePath+"\\resin2.exe";
		//CopyFile(srcFile,dstFile,false);
		//useless.Format("%s\\%s",szDir,"DLL1.dll");
		//srcFile=useless.GetBuffer(0);
		//dstFile=srcFilePath+"\\DLL1.dll";
		//CopyFile(srcFile,dstFile,false);
		//useless.Format("%s\\%s",szDir,"DeployDoc.exe");
		//srcFile=useless.GetBuffer(0);
		//dstFile=srcFilePath+"\\DeployDoc.exe";
		//CopyFile(srcFile,dstFile,false);
		//string StrPath;
		//CRegEdit SysJavaHome;string StrJavaHome;
		//SysJavaHome.m_RootKey=HKEY_LOCAL_MACHINE;
		//SysJavaHome.OpenKey("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
		//CRegEdit RegHomePath;
		//RegHomePath.m_RootKey=HKEY_CURRENT_USER;
		//RegHomePath.OpenKey("Environment");
		//RegHomePath.WriteString("RESIN_HOME",(LPCTSTR)srcFilePath); //D:\resin-3.2.0
		//useless.Format("%s\\bin;%s",srcFilePath.GetBuffer(0),useful.GetBuffer(0));
		//useful=useless;
		//SysJavaHome.WriteString("Path",(LPCTSTR)useful);
		//Sleep(5000);
		//}
		//else if(srcFileName.Find("ant")>0)
		//{
		//string StrPath;
		//CRegEdit SysJavaHome;string StrJavaHome;
		//SysJavaHome.m_RootKey=HKEY_LOCAL_MACHINE;
		//SysJavaHome.OpenKey("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
		//CRegEdit RegHomePath;
		//RegHomePath.m_RootKey=HKEY_CURRENT_USER;
		//RegHomePath.OpenKey("Environment");
		//RegHomePath.WriteString("ANT_HOME",(LPCTSTR)srcFilePath); //D:\apache-ant-1.7.1\ PATH=%ANT_HOME%/bin
		//useless.Format("%s\\bin;%s",srcFilePath.GetBuffer(0),useful.GetBuffer(0));
		//useful=useless;
		//SysJavaHome.WriteString("Path",(LPCTSTR)useful);
		//Sleep(5000);
		//}
		//else if(srcFileName.Find("eclipse")==0 || srcFileName.Find("NetBeans")==0)
		//{
		////char * xmFile="";
		////SaveFileToStr("deploy.xml",xmFile);
		//}
		//}
		//else
		//continue;
		//}
		//*/
		#endregion

		#region 53.选择文件夹对话框
		//public class FolderDialog : FolderNameEditor
		//{
		//	FolderNameEditor.FolderBrowser fDialog = new
		//	System.Windows.Forms.Design.FolderNameEditor.FolderBrowser();
		//	public FolderDialog()
		//	{
		//	}
		//	public DialogResult DisplayDialog()
		//	{
		//	return DisplayDialog("请选择一个文件夹");
		//	}
		//	public DialogResult DisplayDialog(string description)
		//	{
		//	fDialog.Description = description;
		//	return fDialog.ShowDialog();
		//	}
		//	public string Path
		//	{
		//	get
		//	{
		//		return fDialog.DirectoryPath;
		//	}
		//	}
		//	~FolderDialog()
		//	{
		//	fDialog.Dispose();
		//	}
		//} 
		//FolderDialog aa = new FolderDialog();
		//aa.DisplayDialog();
		//if(aa.ShowDialog()==DialogResult.OK)
		//{
		//	%%1 = aa.SelectedPath;
		//}
		#endregion

		#region 54.删除空文件夹
		//bool   IsValidFileChars(string   strIn)   
		//{   
		//	Regex   regEx   =   new   Regex("[\\*\\\\/:?<>|"]");
		//	return   !regEx.IsMatch("aj\\pg");   
		//}   
		//try
		//{
		//string path = %%1;
		//if(!IsValidFileChars(path))
		//throw new Exception("非法目录名！");
		//if(!Directory.Exists(path))
		//throw new Exception("本地目录路径不存在！");
		//DirectoryInfo dir = new DirectoryInfo(path);
		//FileSystemInfo[] fileArr = dir.GetFileSystemInfos();
		//Queue Folders = new Queue(Directory.GetDirectories(aa.Path));
		//while (Folders.Count > 0)
		//{
		//	path = Folders.Dequeue();
		//	string[] dirs = Directory.GetDirectories(path);
		//		Directory.Delete(path);
		//	}
		//		foreach (string direct in dirs)
		//		{
		//			Folders.Enqueue(direct);
		//		}
		//	catch (Exception ep)
		//	{
		//	MessageBox.show(ep.ToString());
		//	}
		//}
		#endregion

		#region 55.发送数据到剪贴板
		//using System.Windows.Forms;
		//Clipboard.SetText(%%1);
		#endregion

		#region 56.从剪贴板中取数据
		//using System.Windows.Forms;
		//IDataObject iData = Clipboard.GetDataObject();
		//string %%1;
		//// 将数据与指定的格式进行匹配，返回bool
		//if (iData.GetDataPresent(DataFormats.Text))
		//{
		//// GetData检索数据并指定一个格式
		//%%1 = (string)iData.GetData(DataFormats.Text);
		//}
		//else
		//{
		//MessageBox.Show("目前剪贴板中数据不可转换为文本","错误");
		//}
		#endregion

		#region 57.获取文件路径的父路径
		//using System.IO;
		//string %%2=Directory.GetParent(%%1);
		#endregion

		#region 58.创建快捷方式
		//首先添加以下引用：COM下Windows Script Host Object Model，然后可以通过以下方法创建快捷方式。 
		//string app = %%1;"http://localhost/TrainManage/Default.aspx"
		//string location1 = System.Environment.GetFolderPath(System.Environment.SpecialFolder.Favorites) + "\\培训教学教务管理系统.url"; 
		//string location2 = System.Environment.GetFolderPath(System.Environment.SpecialFolder.DesktopDirectory) + "\\培训教学教务管理系统.url";
		//string location3 = System.Environment.GetFolderPath(System.Environment.SpecialFolder.Programs) + "\\培训教学教务管理系统.url";
		//try { 
		//  // Create a Windows Script Host Shell class 
		//  IWshShell_Class shell = new IWshShell_ClassClass(); 
		//  // Define the shortcut file 
		//  IWshURLShortcut shortcut = shell.CreateShortcut(location1) as IWshURLShortcut; 
		//  shortcut.TargetPath = app; 
		//  // Save it 
		//  shortcut.Save(); 
		//  shortcut = shell.CreateShortcut(location2) as IWshURLShortcut;shortcut.TargetPath = app; 
		//	// Save it 
		//	shortcut.Save(); 
		//	shortcut = shell.CreateShortcut(location3) as IWshURLShortcut; 
		//	shortcut.TargetPath = app; 
		//	// Save it 
		//	shortcut.Save();
		//}
		//catch(COMException ex)
		//{ 
		// Console.WriteLine(ex.Message); 
		//}
		#endregion

		#region 59.弹出快捷菜单
		//在工具箱中找到ContextMenuStrip控件，并拖放至Form1窗体
		//设计菜单内容
		//将contextMenuStrip1与窗体关联。方法是先选定Form1，为其ContextMenuStrip属性设置属性值为contextMenuStrip1
		#endregion

		#region 60.文件夹复制到整合操作
		//FolderDialog aa = new FolderDialog();
		//aa.DisplayDialog();
		//if (aa.Path != "")
		//{
		//	string path = (aa.Path.LastIndexOf("\") == aa.Path.Length - 1) ? aa.Path : aa.Path+"\";
		//	string parent = Path.GetDirectoryName(%%1);
		//	Directory.CreateDirectory(path + Path.GetFileName(%%1));
		//	%%1 = (%%1.LastIndexOf("\") == %%1.Length - 1) ? %%1 : %%1 + "\";
		//	DirectoryInfo dir = new DirectoryInfo(%%1);
		//	FileSystemInfo[] fileArr = dir.GetFileSystemInfos();
		//	Queue Folders = new Queue(dir.GetFileSystemInfos());
		//	while (Folders.Count>0)
		//	{
		//		FileSystemInfo tmp = Folders.Dequeue();
		//		FileInfo f = tmp as FileInfo;
		//		if (f == null)
		//		{
		//			DirectoryInfo d = tmp as DirectoryInfo;
		//			Directory.CreateDirectory(d.FullName.Replace((parent.LastIndexOf("\") == parent.Length - 1) ? parent : parent + "\", path));
		//			foreach (FileSystemInfo fi in d.GetFileSystemInfos())
		//			{
		//				Folders.Enqueue(fi);
		//			}
		//		}
		//		else
		//		{
		//			f.CopyTo(f.FullName.Replace(parent, path));
		//		}
		//	}
		//}
		#endregion

		#region 61.文件夹移动到整合操作
		//FolderDialog aa = new FolderDialog();
		//aa.DisplayDialog();
		//if (aa.Path != "")
		//{
		//	string filename = Path.GetFileName(%%1);
		//	string path=(aa.Path.LastIndexOf("\") == aa.Path.Length - 1) ? aa.Path : aa.Path + "\";
		//	if (Path.GetPathRoot(%%1) == Path.GetPathRoot(aa.Path))
		//		Directory.Move(%%1, path + filename);
		//	else
		//	{
		//		string parent = Path.GetDirectoryName(%%1);
		//		Directory.CreateDirectory(path + Path.GetFileName(%%1));
		//		%%1 = (%%1.LastIndexOf("\") == %%1.Length - 1) ? %%1 : %%1 + "\";
		//		DirectoryInfo dir = new DirectoryInfo(%%1);
		//		FileSystemInfo[] fileArr = dir.GetFileSystemInfos();
		//		Queue Folders = new Queue(dir.GetFileSystemInfos());
		//		while (Folders.Count > 0)
		//		{
		//			FileSystemInfo tmp = Folders.Dequeue();
		//			FileInfo f = tmp as FileInfo;
		//			if (f == null)
		//			{
		//				DirectoryInfo d = tmp as DirectoryInfo;
		//				DirectoryInfo dpath = new DirectoryInfo(d.FullName.Replace((parent.LastIndexOf("\") == parent.Length - 1) ? parent : parent + "\", path));
		//				dpath.Create();
		//				foreach (FileSystemInfo fi in d.GetFileSystemInfos())
		//				{
		//					Folders.Enqueue(fi);
		//				}
		//			}
		//			else
		//			{
		//				f.MoveTo(f.FullName.Replace(parent, path));
		//			}
		//		}
		//		Directory.Delete(%%1, true);
		//	}
		//}
		#endregion

		#region 62.目录下所有文件夹复制到整合操作
		//FolderDialog aa = new FolderDialog();
		//aa.DisplayDialog();
		//if (aa.Path != "")
		//{
		//	string direc = %%1;//获取选中的节点的完整路径
		//	foreach (string dirStr in Directory.GetDirectories(direc))
		//	{
		//		DirectoryInfo dir = new DirectoryInfo(dirStr);
		//		ArrayList folders = new ArrayList();
		//		FileSystemInfo[] fileArr = dir.GetFileSystemInfos();
		//		folders.AddRange(fileArr);
		//		for (int i = 0; i < folders.Count; i++)
		//		{
		//			FileInfo f = folders[i] as FileInfo;
		//			if (f == null)
		//			{
		//				DirectoryInfo d = folders[i] as DirectoryInfo;
		//				Directory.CreateDirectory(aa.Path + d.Name);
		//				folders.AddRange(d.GetFileSystemInfos());
		//			}
		//			else
		//				File.Copy(f.FullName, aa.Path + f.Name);
		//		}
		//	}
		//}
		#endregion

		#region 63.目录下所有文件夹移动到整合操作
		//FolderDialog aa = new FolderDialog();
		//aa.DisplayDialog();
		//if (aa.Path != "")
		//{
		//	TreeNode CurSelNode = this.DirectorytreeView.SelectedNode;//获取选中的节点
		//	string direc = this.GetNodeFullPath(CurSelNode);//获取选中的节点的完整路径
		//	if (Path.GetPathRoot(direc) == Path.GetPathRoot(aa.Path))
		//		foreach (string dir in Directory.GetDirectories(direc))
		//			Directory.Move(dir, aa.Path);
		//	else
		//	{
		//		foreach (string dir2 in Directory.GetDirectories(direc))
		//		{
		//			string parent = Path.GetDirectoryName(dir2);
		//			Directory.CreateDirectory(Path.Combine(aa.Path, Path.GetFileName(dir2)));
		//			string dir = (dir2.LastIndexOf("\") == dir2.Length - 1) ? dir2 : dir2 + "\";
		//			DirectoryInfo dirdir = new DirectoryInfo(dir);
		//			FileSystemInfo[] fileArr = dirdir.GetFileSystemInfos();
		//			Queue Folders = new Queue(dirdir.GetFileSystemInfos());
		//			while (Folders.Count > 0)
		//			{
		//				FileSystemInfo tmp = Folders.Dequeue();
		//				FileInfo f = tmp as FileInfo;
		//				if (f == null)
		//				{
		//					DirectoryInfo d = tmp as DirectoryInfo;
		//					DirectoryInfo dpath = new DirectoryInfo(d.FullName.Replace((parent.LastIndexOf("\") == parent.Length - 1) ? parent : parent + "\", aa.Path));
		//					dpath.Create();
		//					foreach (FileSystemInfo fi in d.GetFileSystemInfos())
		//					{
		//						Folders.Enqueue(fi);
		//					}
		//				}
		//				else
		//				{
		//					f.MoveTo(f.FullName.Replace(parent, aa.Path));
		//				}
		//			}
		//			dirdir.Delete(true);
		//		}
		//	}
		//}
		#endregion

		#region 64.目录下所有文件复制到整合操作
		//FolderDialog aa = new FolderDialog();
		//aa.DisplayDialog();
		//if (aa.Path != "")
		//{
		//	string direc = %%1;//获取选中的节点的完整路径
		//	foreach (string fileStr in Directory.GetFiles(direc))
		//		File.Copy((direc.LastIndexOf("\") == direc.Length - 1) ? direc + Path.GetFileName(fileStr) : direc + "\" + Path.GetFileName(fileStr), (aa.Path.LastIndexOf("\") == aa.Path.Length - 1) ? aa.Path + Path.GetFileName(fileStr) : aa.Path + "\" + Path.GetFileName(fileStr));
		//}
		#endregion

		#region 65.目录下所有文件移动到整合操作
		//FolderDialog aa = new FolderDialog();
		//aa.DisplayDialog();
		//if (aa.Path != "")
		//{
		//	string direc = %%1;//获取选中的节点的完整路径
		//	foreach (string fileStr in Directory.GetFiles(direc))
		//		File.Move((direc.LastIndexOf("\") == direc.Length - 1) ? direc + Path.GetFileName(fileStr) : direc + "\" + Path.GetFileName(fileStr), (aa.Path.LastIndexOf("\") == aa.Path.Length - 1) ? aa.Path + Path.GetFileName(fileStr) : aa.Path + "\" + Path.GetFileName(fileStr));
		//	DirectoryInfolistView.Clear();
		//}
		#endregion

		#region 66.对目标压缩文件解压缩到指定文件夹
		//private void DeSerializeFiles(Stream s, string dirPath)
		//{
		//	BinaryFormatter b = new BinaryFormatter();
		//	ArrayList list = (ArrayList)b.Deserialize(s);
		//	foreach (SerializeFileInfo f in list)
		//	{
		//		string newName = dirPath + Path.GetFileName(f.FileName);
		//		using (FileStream fs = new FileStream(newName, FileMode.Create, FileAccess.Write))
		//		{
		//			fs.Write(f.FileBuffer, 0, f.FileBuffer.Length);
		//			fs.Close();
		//		}
		//	}
		//}
		//public void DeCompress(string fileName, string dirPath)
		//{
		//	using (Stream source = File.OpenRead(fileName))
		//	{
		//		using (Stream destination = new MemoryStream())
		//		{
		//			using (GZipStream input = new GZipStream(source, CompressionMode.Decompress, true))
		//			{
		//				byte[] bytes = new byte[4096];
		//				int n;
		//				while ((n = input.Read(bytes, 0, bytes.Length)) != 0)
		//				{
		//					destination.Write(bytes, 0, n);
		//				}
		//			}
		//			destination.Flush();
		//			destination.Position = 0;
		//			DeSerializeFiles(destination, dirPath);
		//		}
		//	}
		//}
		#endregion

		#region 67.创建目录副本整合操作
		//FolderDialog aa = new FolderDialog();
		//aa.DisplayDialog();
		//bool b = MessageBox.Show("是否也创建空文件？", "构建文件夹框架", MessageBoxButtons.OKCancel, MessageBoxIcon.Information) == DialogResult.OK ? true : false;
		//if (aa.Path != "")
		//{
		//	string path = (aa.Path.LastIndexOf("\") == aa.Path.Length - 1) ? aa.Path : aa.Path + "\";
		//	string parent = Path.GetDirectoryName(%%1);
		//	Directory.CreateDirectory(path + Path.GetFileName(%%1));
		//	%%1 = (%%1.LastIndexOf("\") == %%1.Length - 1) ? %%1 : %%1 + "\";
		//	DirectoryInfo dir = new DirectoryInfo(%%1);
		//	FileSystemInfo[] fileArr = dir.GetFileSystemInfos();
		//	Queue Folders = new Queue(dir.GetFileSystemInfos());
		//	while (Folders.Count > 0)
		//	{
		//		FileSystemInfo tmp = Folders.Dequeue();
		//		FileInfo f = tmp as FileInfo;
		//		if (f == null)
		//		{
		//			DirectoryInfo d = tmp as DirectoryInfo;
		//			Directory.CreateDirectory(d.FullName.Replace((parent.LastIndexOf("\") == parent.Length - 1) ? parent : parent + "\", path));
		//			foreach (FileSystemInfo fi in d.GetFileSystemInfos())
		//			{
		//				Folders.Enqueue(fi);
		//			}
		//		}
		//		else
		//		{
		//			if(b) File.Create(f.FullName.Replace(parent, path));
		//		}
		//	}
		//}
		#endregion

		#region 68.打开网页
		//System.Diagnostics.Process.Start("IEXPLORE.EXE", "http://ant.sourceforge.net/");
		#endregion

		#region 69.删除空文件夹整合操作
		//using System.IO;
		//FolderDialog aa = new FolderDialog();
		//aa.DisplayDialog();
		//if (aa.Path != "")
		//{
		//	string path = aa.Path;
		//	DirectoryInfo dir = new DirectoryInfo(aa.Path);
		//	FileSystemInfo[] fileArr = dir.GetFileSystemInfos();
		//	Queue Folders = new Queue(Directory.GetDirectories(aa.Path));
		//	while (Folders.Count > 0)
		//	{
		//		path = Folders.Dequeue();
		//		string[] dirs = Directory.GetDirectories(path);
		//		try
		//		{
		//			Directory.Delete(path);
		//		}
		//		catch (Exception)
		//		{
		//			foreach (string direct in dirs)
		//			{
		//				Folders.Enqueue(direct);
		//			}
		//		}
		//	}
		//}
		#endregion

		#region 70.获取磁盘所有分区后再把光驱盘符去除(用"\0"代替)，把结果放在数组allfenqu[] 中，数组中每个元素代表一个分区盘符，不包括 :\\ 这样的路径，allfenqu[]数组开始时存放的是所有盘符。 
		//当我用这样的代码测试结果是正确的，光驱盘符会被去掉： 
		//using System.IO;
		//stringroot; //root代表盘符路径 
		//for(i=0;i<20;i++) //0-20代表最大的盘符数 
		//{ 
		//root.Format("%c:\",allfenqu[i]); 
		//if(GetDriveType(root)==5) 
		//allfenqu[i]='\0'; 
		//} 
		//但我用这样的代码时结果却无法去掉光驱盘符，allfenqu[]中还是会包含光驱盘符： 
		//stringroot; 
		//for(i=0;i<20;i++) 
		//{ 
		//root=allfenqu[i]+":\"; 
		//if(GetDriveType(root)==5) 
		//allfenqu[i]='\0'; 
		//}
		#endregion

		#region 71.激活一个程序或程序关联的文件
		//using System.Diagnostics;
		//Process LandFileDivisison;
		//LandFileDivisison = new System.Diagnostics.Process();
		//LandFileDivisison.StartInfo.FileName = %%1;
		//LandFileDivisison.Start();
		#endregion

		#region 72.HTTP下载
		//private WebClient client = new WebClient(); 
		//	Thread th = new Thread(new ThreadStart(StartDownload));
		//	th.Start();
		//private void StartDownload()
		//{
		//	//Start.Enabled = false;
		//	string URL = %%1;
		//	int n = URL.LastIndexOf("/");
		//	string URLAddress = URL.Substring(0, n);
		//	string fileName = URL.Substring(n + 1, URL.Length - n - 1);
		//	string Dir = %%2;
		//	string Path = Dir.ToString() + "\" + fileName; 
		//	try
		//	{
		//		WebRequest myre = WebRequest.Create(URLAddress);
		//	}
		//	catch (WebException exp)
		//	{
		//		MessageBox.Show(exp.Message, "Error");
		//	}
		//	try
		//	{
		//		//statusBar.Text = "开始下载文件...";
		//		client.DownloadFile(URLAddress, fileName);
		//		Stream str = client.OpenRead(URLAddress);
		//		StreamReader reader = new StreamReader(str);
		//		byte[] mbyte = new byte[100000];
		//		int allmybyte = (int)mbyte.Length;
		//		int startmbyte = 0;
		//		//statusBar.Text = "正在接收数据...";
		//		while (allmybyte > 0)
		//		{
		//			int m = str.Read(mbyte, startmbyte, allmybyte);
		//			if (m == 0)
		//				break;
		//			startmbyte += m;
		//			allmybyte -= m;
		//		}
		//		FileStream fstr = new FileStream(Path, FileMode.OpenOrCreate, FileAccess.Write);
		//		fstr.Write(mbyte, 0, startmbyte);
		//		str.Close();
		//		fstr.Close();
		//		//statusBar.Text = "下载完毕！";
		//		MessageBox.Show("下载完毕");
		//	}
		//	catch (WebException exp)
		//	{
		//		MessageBox.Show(exp.Message, "Error");
		//		//statusBar.Text = "";
		//	}
		//	Start.Enabled = true;
		//}
		#endregion

		#region 73.FTP下载
		//bool   IsValidFileChars(string   strIn)   
		//{   
		//	Regex   regEx   =   new   Regex("[\\*\\\\/:?<>|"]");
		//	return   !regEx.IsMatch("aj\\pg");   
		//}   
		//public bool DownloadFile(string RemoteFileName, string LocalPath) 
		//{ 
		//return DownloadFile(RemoteFileName, LocalPath, RemoteFileName); 
		//} 
		/////  
		///// 从FTP服务器下载文件，指定本地路径和本地文件名 
		/////  
		///// 远程文件名 
		///// 本地路径 
		///// 保存文件的本地路径,后面带有"" 
		///// 保存本地的文件名 
		//public bool DownloadFile(string RemoteFileName, string LocalPath, string LocalFileName) 
		//{ 
		//byte[] bt = null; 
		//try 
		//{ 
		//if (!IsValidFileChars(RemoteFileName) || !IsValidFileChars(LocalFileName) || !IsValidPathChars(LocalPath)) 
		//{ 
		//throw new Exception("非法文件名或目录名!"); 
		//} 
		//if (!Directory.Exists(LocalPath)) 
		//{ 
		//throw new Exception("本地文件路径不存在!"); 
		//} 
		//string LocalFullPath = Path.Combine(LocalPath, LocalFileName); 
		//if (File.Exists(LocalFullPath)) 
		//{ 
		//throw new Exception("当前路径下已经存在同名文件！"); 
		//} 
		//bt = DownloadFile(RemoteFileName); 
		//if (bt != null) 
		//{ 
		//FileStream stream = new FileStream(LocalFullPath, FileMode.Create); 
		//stream.Write(bt, 0, bt.Length); 
		//stream.Flush(); 
		//stream.Close(); 
		//return true; 
		//} 
		//else 
		//{ 
		//return false; 
		//} 
		//} 
		//catch (Exception ep) 
		//{ 
		//ErrorMsg = ep.ToString(); 
		//throw ep; 
		//} 
		//} 
		/////  
		///// 从FTP服务器下载文件，返回文件二进制数据 
		/////  
		///// 远程文件名 
		//public byte[] DownloadFile(string RemoteFileName) 
		//{ 
		//try 
		//{ 
		//if (!IsValidFileChars(RemoteFileName)) 
		//{ 
		//throw new Exception("非法文件名或目录名!"); 
		//} 
		//Response = Open(new Uri(this.Uri.ToString() + RemoteFileName), WebRequestMethods.Ftp.DownloadFile); 
		//Stream Reader = Response.GetResponseStream(); 
		//MemoryStream mem = new MemoryStream(1024 * 500); 
		//byte[] buffer = new byte[1024]; 
		//int bytesRead = 0; 
		//int TotalByteRead = 0; 
		//while (true) 
		//{ 
		//bytesRead = Reader.Read(buffer, 0, buffer.Length); 
		//TotalByteRead += bytesRead; 
		//if (bytesRead == 0) 
		//break; 
		//mem.Write(buffer, 0, bytesRead); 
		//} 
		//if (mem.Length > 0) 
		//{ 
		//return mem.ToArray(); 
		//} 
		//else 
		//{ 
		//return null; 
		//} 
		//} 
		//catch (Exception ep) 
		//{ 
		//ErrorMsg = ep.ToString(); 
		//throw ep; 
		//} 
		//}
		#endregion

		#region 74.写图像到剪切板 setClipboardImage
		//using System.IO;
		//Bitmap   bm   =new   Bitmap(filename);   
		//Clipboard.SetDataObject(bm,true);
		#endregion

		#region 75.从剪贴板复制图像到窗体
		//if (Clipboard.ContainsImage())
		//{
		//	this.pictureBox1.Image =  Clipboard.GetImage();
		//}
		//剪贴板中的数据类型   
		////using System.IO;
		//d.GetDataPresent(DataFormats.Bitmap)//(.Text       .Html)   
		//  Bitmap   b   =   (Bitmap)d.GetData(DataFormat   Bitmap)   
		//  粘贴   
		//  IDataObject   data   =   Clipboard.GetDataObjects;   
		//  if(Data.GetDataPresent(DataFormats.Bipmap))   
		//  {   
		//  b.Save(@"C:\mymap.bmp");   
		//  }
		#endregion

		#region 76.删除文件夹下的所有文件且不删除文件夹下的文件夹
		//using System.IO;
		#endregion

		#region 77.XML遍历结点属性值
		//using System.IO;
		#endregion

		#region 78.拷贝文件名复制文件
		//添加引用System.Windows.Forms
		//using System;
		//using System.Collections.Generic;
		//using System.Text;
		//using System.IO;
		//using System.Windows.Forms;
		//namespace ConsoleApplication1
		//{
		//	class Program
		//	{
		//		static void Main(string[] args)
		//		{
		//			IDataObject iData = Clipboard.GetDataObject();
		//			string str;
		//			// 将数据与指定的格式进行匹配，返回bool
		//			if (iData.GetDataPresent(DataFormats.Text))
		//			{
		//				// GetData检索数据并指定一个格式
		//				str = (string)iData.GetData(DataFormats.Text);
		//				File.Copy(str, @"C:" + Path.GetFileName(str));
		//			}
		//			else
		//			{
		//				MessageBox.Show("目前剪贴板中数据不可转换为文本", "错误");
		//			}
		//		}
		//	}
		//}
		#endregion

		#region 79.开源程序库Xercesc-C++代码工程中内联
		//using System;
		//using System.IO;
		//using System.Collections;
		//using System.Collections.Generic;
		//using System.Text.RegularExpressions;
		//using System.Text;
		//public class InlineXercesc
		//{
		//	private const String filter = ".cpp";
		//	private ArrayList all = new ArrayList();
		//	private Queue fal2 = new Queue();
		//	private static String CurDir = Environment.CurrentDirectory;
		//	public InlineXercesc(String lib)
		//	{
		//		string SourceLib = "D:\\Desktop\\大项目\\xerces-c-3.0.1\\src";
		//		string pattern = "include.*?" + lib + ".*?>"; // 第一个参数为需要匹配的字符串
		//		Match matcher = null;
		//		Queue fal = new Queue();
		//		DirectoryInfo delfile = new DirectoryInfo(CurDir);
		//		foreach (DirectoryInfo files2 in delfile.GetDirectories())
		//		{
		//			String enumDir = CurDir + "\" + files2.Name + "\";
		//			FileSystemInfo[] fileArr = files2.GetFileSystemInfos();
		//			Queue folderList = new Queue(fileArr);
		//			while (folderList.Count > 0)
		//			{
		//				FileSystemInfo tmp = folderList.Dequeue();
		//				FileInfo f = tmp as FileInfo;
		//				if (f == null)
		//				{
		//					DirectoryInfo d = tmp as DirectoryInfo;
		//					foreach (FileSystemInfo fi in d.GetFileSystemInfos())
		//					{
		//						folderList.Enqueue(fi);
		//					}
		//				}
		//				else
		//				{
		//					StreamReader br = null;
		//					try
		//					{
		//						br = new StreamReader(file);
		//						// 打开文件
		//					}
		//					catch (IOException e)
		//					{
		//						// 没有打开文件，则产生异常
		//						System.Console.Error.WriteLine("Cannot read '" + f.FullName + "': " + e.Message);
		//						continue;
		//					}
		//					String line;
		//					StringBuilder sb = new StringBuilder(2048);
		//					while ((line = br.ReadLine()) != null)
		//					{
		//						// 读入一行，直到文件结束
		//						matcher = Regex.Match(line, pattern); // 匹配字符串
		//						if (matcher.Success == true)
		//						{
		//							// 如果有匹配的字符串，则输出
		//							sb.Append(line.Replace(line.Substring(line.IndexOf("<"), (line.LastIndexOf("/") + 1) - (line.IndexOf("<"))), """).Replace('>', '"'));
		//							line = line.Substring(line.IndexOf("<") + 1, (line.LastIndexOf(">")) - (line.IndexOf("<") + 1)).Replace('/', '\\');
		//							fal.Enqueue(SourceLib + "\" + line);
		//						}
		//						else
		//						{
		//							sb.Append(line);
		//						}
		//						sb.Append("\r\n");
		//					}
		//					br.Close(); // 关闭文件
		//					StreamWriter w = new StreamWriter(f.FullName);
		//					w.WriteLine(sb.ToString());
		//					w.Close();
		//				}
		//			}
		//			while (fal.Count > 0)
		//			{
		//				String file = fal.Dequeue(); // 第2个参数开始，均为文件名。
		//				String targetPath = enumDir + file.Substring(file.LastIndexOf("\") + 1);
		//				if (targetPath.IndexOf('<') == -1 && !!File.Exists(targetPath))
		//				{
		//					File.CreateText(targetPath);
		//					StreamReader br = null;
		//					String line;
		//					try
		//					{
		//						br = new StreamReader(new StreamReader(file).BaseStream, System.Text.Encoding.UTF7);
		//						// 打开文件
		//					}
		//					catch (IOException e)
		//					{
		//						// 没有打开文件，则产生异常
		//						//UPGRADE_TODO: 在 .NET 中，method 'java.lang.Throwable.getMessage' 的等效项可能返回不同的值。. 'ms-help://MS.VSCC.2003/commoner/redir/redirect.htm?keyword="jlca1043"'
		//						System.Console.Error.WriteLine("Cannot read '" + file + "': " + e.Message);
		//						continue;
		//					}
		//					StreamWriter fw = new StreamWriter(targetPath);
		//					while ((line = br.ReadLine()) != null)
		//					{
		//						// 读入一行，直到文件结束
		//						matcher = Regex.Match(line, pattern); // 匹配字符串
		//						if (matcher.Success == true)
		//						{
		//							// 如果有匹配的字符串，则输出
		//							fal.Enqueue(SourceLib + "\" + line.Substring(line.IndexOf("<") + 1, (line.LastIndexOf(">")) - (line.IndexOf("<") + 1)).Replace('/', '\\'));
		//							line = line.Replace(line.Substring(line.IndexOf("<"), (line.LastIndexOf("/") + 1) - (line.IndexOf("<"))), """);
		//							line = line.Replace(">", """);
		//						}
		//						fw.Write(line + "\r\n");
		//					}
		//					fw.Flush();
		//					fw.Close();
		//					br.Close(); // 关闭文件
		//				}
		//			}
		//			Queue folderListArr = new Queue();
		//			folderListArr.Enqueue(CurDir);
		//			while (folderListArr.Count > 0)
		//			{
		//				DirectoryInfo file = new DirectoryInfo(folderListArr.Dequeue());
		//				FileSystemInfo[] files = file.GetFileSystemInfos();
		//				for (int i = 0; i < files.Length; i++)
		//				{
		//					DirectoryInfo ddd = files[i] as DirectoryInfo;
		//					if (ddd != null)
		//					{
		//						folderListArr.Enqueue(files[i].FullName);
		//					}
		//					else
		//					{
		//						if (files[i].Extension == ".hpp")
		//						{
		//							all.Add(files[i].FullName.Replace(".hpp", ".cpp"));
		//						}
		//					}
		//				}
		//			}
		//			int count = 1;
		//			while (count > 0)
		//			{
		//				doSearch(SourceLib);
		//				all.Clear();
		//				while (fal2.Count > 0)
		//				{
		//					String file1 = fal2.Dequeue(); // 第2个参数开始，均为文件名。
		//					String targetPath = enumDir + file1.Substring(file1.LastIndexOf("\") + 1);
		//					if (targetPath.IndexOf('<') == -1 && !File.Exists(targetPath))
		//					{
		//						File.CreateText(targetPath);
		//						StreamReader br = null;
		//						String line;
		//						try
		//						{
		//							br = new StreamReader(file1);
		//							// 打开文件
		//						}
		//						catch (IOException e)
		//						{
		//							System.Console.Error.WriteLine("Cannot read '" + file1 + "': " + e.Message);
		//							continue;
		//						}
		//						StreamWriter fw;
		//						try
		//						{
		//							fw = new StreamWriter(targetPath);
		//							while ((line = br.ReadLine()) != null)
		//							{
		//								// 读入一行，直到文件结束
		//								matcher = Regex.Match(line, pattern); // 匹配字符串
		//								if (matcher.Success == true)
		//								{
		//									// 如果有匹配的字符串，则输出
		//									fal2.Enqueue(SourceLib + "\" + line.Substring(line.IndexOf('<') + 1, (line.LastIndexOf('>')) - (line.IndexOf('<') + 1)).Replace('/', '\\'));
		//									all.Add(fal2.Peek().Replace(".hpp", ".cpp"));
		//									line = line.Replace(line.Substring(line.IndexOf('<'), (line.LastIndexOf('/') + 1) - (line.IndexOf('<'))), """);
		//									line = line.Replace('>', '"');
		//								}
		//								fw.Write(line + "\r\n");
		//							}
		//							fw.Flush();
		//							fw.Close();
		//							br.Close(); // 关闭文件
		//						}
		//						catch (IOException e)
		//						{
		//							Console.Error.WriteLine(e.StackTrace);
		//						}
		//					}
		//				}
		//				count = all.Count;
		//			}
		//		}
		//	}
		//	private void doSearch(string path)
		//	{
		//		DirectoryInfo filepath = new DirectoryInfo(path);
		//		if (filepath.Exists)
		//		{
		//			FileSystemInfo[] fileArray = filepath.GetFileSystemInfos();
		//			foreach (FileSystemInfo f in fileArray)
		//			{
		//				DirectoryInfo dd = f as DirectoryInfo;
		//				if (dd != null)
		//				{
		//					doSearch(f.FullName);
		//				}
		//				else
		//				{
		//					FileInfo ff = f as FileInfo;
		//					if (f.Name.IndexOf(filter) > -1)
		//					{
		//						foreach (string file in all)
		//						{
		//							if (file.IndexOf('<') == -1 && Path.GetFileName(file) == f.Name)
		//							{
		//								fal2.Enqueue(f.FullName);
		//							}
		//						}
		//					}
		//				}
		//			}
		//		}
		//	}
		//	static void Main(String[] args)
		//	{
		//		new InlineXercesc("xercesc");
		//		FileInfo f = new FileInfo(CurDir + "\\DetailCpp.cmd");
		//		StreamWriter w = f.CreateText();
		//		w.WriteLine("copy StdAfx.cpp+*.c+*.cpp " + CurDir
		//										+ "\\StdAfx.cpp && del *.c && del *.cpp");
		//		w.Close();
		//	}
		//}
		#endregion

		#region 80.提取包含头文件列表
		//InlineExt.cs
		//using System;
		//using System.IO;
		//using System.Collections;
		//using System.Collections.Generic;
		//using System.Text.RegularExpressions;
		//using System.Text;
		//public class InlineExt
		//{
		//private System.String CurDir = Environment.CurrentDirectory;
		//public InlineExt()
		//{
		//		string pattern = "include.*?".*?.hpp""; // 第一个参数为需要匹配的字符串
		//		Match matcher = null;
		//FileInfo delfile = new System.IO.FileInfo(CurDir);
		//FileInfo[] files2 = SupportClass.FileSupport.GetFiles(delfile);
		//for (int l = 0; l < files2.Length; l++)
		//{
		//if (Directory.Exists(files2[l].FullName))
		//{
		//				Queue ts = new Queue();
		//FileInfo file = new FileInfo(Path.Combine(files2[l].FullName , "StdAfx.cpp"));
		//StreamReader br = null;
		//StreamWriter fw = null;
		//String line;
		//try
		//{
		//br = new StreamReader(new StreamReader(file.FullName, System.Text.Encoding.Default).BaseStream, new System.IO.StreamReader(file.FullName, System.Text.Encoding.Default).CurrentEncoding); // 打开文件
		//while ((line = br.ReadLine()) != null)
		//{
		//						matcher = Regex.Match(line, pattern); // 匹配字符串
		//						if (matcher.Success == true)
		//{
		//// 如果有匹配的字符串，则输出
		//ts.Enqueue(line.Substring(line.IndexOf('"') + 1, (line.LastIndexOf('"')) - (line.IndexOf('"') + 1)));
		//}
		//}
		//FileInfo file2 = new FileInfo(Path.Combine(files2[l].FullName , "ReadMe.txt"));
		//if (File.Exists(file2.FullName))
		//{
		//						fw = new StreamWriter(file2.FullName, false, System.Text.Encoding.GetEncoding("GB2312")); //System.Text.Encoding.Default
		//						foreach(string it in ts)
		//{
		//fw.Write("#include "" + it + ""\r\n");
		//}
		//}
		//}
		//catch (IOException e)
		//{
		//// 没有打开文件，则产生异常
		//Console.Error.WriteLine("Cannot read '" + file + "': " + e.Message);
		//continue;
		//}
		//finally
		//{
		//try
		//{
		//if (br != null)
		//br.Close();
		//if (fw != null)
		//fw.Close();
		//}
		//catch (IOException e)
		//{
		//Console.WriteLine(e.StackTrace);
		//}
		//}
		//}
		//}
		//}
		//public static void  Main(System.String[] args)
		//{
		//new InlineExt();
		//}
		//}
		////SupportClass.cs
		//using System;
		/////
		///// Contains conversion support elements such as classes, interfaces and static methods.
		/////
		//public class SupportClass
		//{
		/////
		///// Writes the exception stack trace to the received stream
		/////
		///// Exception to obtain information from
		///// Output sream used to write to
		//public static void WriteStackTrace(System.Exception throwable, System.IO.TextWriter stream)
		//{
		//stream.Write(throwable.StackTrace);
		//stream.Flush();
		//}
		/////
		///// Represents the methods to support some operations over files.
		/////
		//public class FileSupport
		//{
		/////
		///// Creates a new empty file with the specified pathname.
		/////
		///// The abstract pathname of the file
		///// True if the file does not exist and was succesfully created
		//public static bool CreateNewFile(System.IO.FileInfo path)
		//{
		//if (path.Exists)
		//{
		//return false;
		//}
		//else
		//{
		//				System.IO.FileStream createdFile = path.Create();
		//				createdFile.Close();
		//return true;
		//}
		//}
		/////
		///// Compares the specified object with the specified path
		/////
		///// An abstract pathname to compare with
		///// An object to compare with the given pathname
		///// A value indicating a lexicographically comparison of the parameters
		//public static int CompareTo(System.IO.FileInfo path, System.Object file)
		//{
		//if( file is System.IO.FileInfo )
		//{
		//System.IO.FileInfo fileInfo = (System.IO.FileInfo)file;
		//return path.FullName.CompareTo( fileInfo.FullName );
		//}
		//else
		//{
		//throw new System.InvalidCastException();
		//}
		//}
		/////
		///// Returns an array of abstract pathnames representing the files and directories of the specified path.
		/////
		///// The abstract pathname to list it childs.
		///// An array of abstract pathnames childs of the path specified or null if the path is not a directory
		//public static System.IO.FileInfo[] GetFiles(System.IO.FileInfo path)
		//{
		//if ( (path.Attributes & System.IO.FileAttributes.Directory) > 0 )
		//{  
		//String[] fullpathnames = System.IO.Directory.GetFileSystemEntries(path.FullName);
		//System.IO.FileInfo[] result = new System.IO.FileInfo[fullpathnames.Length];
		//for(int i = 0; i < result.Length ; i++)
		//result[i] = new System.IO.FileInfo(fullpathnames[i]);
		//return result;
		//}
		//else return null;
		//}
		/////
		///// Creates an instance of System.Uri class with the pech specified
		/////
		///// The abstract path name to create the Uri
		///// A System.Uri instance constructed with the specified path
		//public static System.Uri ToUri(System.IO.FileInfo path)
		//{
		//System.UriBuilder uri = new System.UriBuilder();
		//uri.Path = path.FullName;
		//uri.Host = String.Empty;
		//uri.Scheme = System.Uri.UriSchemeFile;
		//return uri.Uri;
		//}
		/////
		///// Returns true if the file specified by the pathname is a hidden file.
		/////
		///// The abstract pathname of the file to test
		///// True if the file is hidden, false otherwise
		//public static bool IsHidden(System.IO.FileInfo file)
		//{
		//return ((file.Attributes & System.IO.FileAttributes.Hidden) > 0); 
		//}
		/////
		///// Sets the read-only property of the file to true.
		/////
		///// The abstract path name of the file to modify
		//public static bool SetReadOnly(System.IO.FileInfo file)
		//{
		//try 
		//{
		//file.Attributes = file.Attributes | System.IO.FileAttributes.ReadOnly;
		//return true;
		//}
		//catch (System.Exception exception)
		//{
		//String exceptionMessage = exception.Message;
		//return false;
		//}
		//}
		/////
		///// Sets the last modified time of the specified file with the specified value.
		/////
		///// The file to change it last-modified time
		///// Total number of miliseconds since January 1, 1970 (new last-modified time)
		///// True if the operation succeeded, false otherwise
		//public static bool SetLastModified(System.IO.FileInfo file, long date)
		//{
		//try 
		//{
		//long valueConstant = (new System.DateTime(1969, 12, 31, 18, 0, 0)).Ticks;
		//file.LastWriteTime = new System.DateTime( (date * 10000L) + valueConstant );
		//return true;
		//}
		//catch (System.Exception exception)
		//{
		//String exceptionMessage = exception.Message;
		//return false;
		//}
		//}
		//}
		//}
		#endregion

		#region 81.剪贴扳转换成打印字符
		//using System.Windows.Forms;
		//IDataObject iData = Clipboard.GetDataObject();
		//string str;
		//// 将数据与指定的格式进行匹配，返回bool
		//if (iData.GetDataPresent(DataFormats.Text))
		//{
		//	// GetData检索数据并指定一个格式
		//	str = (string)iData.GetData(DataFormats.Text);
		//	string[] arr = str.Split("\r\n".ToCharArray());
		//	StringBuilder sb = new StringBuilder(1024);
		//	sb.Append("System.out.println("@echo off");\r\n");
		//	foreach (string s in arr)
		//	{
		//		if (s.Trim()!="")
		//		{
		//			sb.Append("System.out.println("ECHO " + s.Replace("^", "^^").Replace("&", "^&").Replace(":", "^:").Replace(">", "^>").Replace("<", "^<").Replace("|", "^|").Replace(""", "^"").Replace(@"", @"\").Replace(""", "\\"") + "");");
		//			sb.Append("\r\n");
		//		}
		//	}
		//	Clipboard.SetText(sb.ToString());
		//}
		//else
		//{
		//	MessageBox.Show("目前剪贴板中数据不可转换为文本", "错误");
		//}
		#endregion

		#region 82.把JButton或JTree组件写到一个流中
		#endregion

		#region 83.注册全局热键
		//注册全局热键要用到Windows的API方法RegisterHotKey和UnregisterHotKey。
		//一、声明注册热键方法 [DllImport("user32.dll")]
		//private static extern int RegisterHotKey(IntPtr hwnd, int id, int fsModifiers, int vk);
		//[DllImport("user32.dll")]
		//private static extern int UnregisterHotKey(IntPtr hwnd, int id);
		//int Space = 32; //热键ID
		//private const int WM_HOTKEY = 0x312; //窗口消息-热键
		//private const int WM_CREATE = 0x1; //窗口消息-创建
		//private const int WM_DESTROY = 0x2; //窗口消息-销毁
		//private const int MOD_ALT = 0x1; //ALT
		//private const int MOD_CONTROL = 0x2; //CTRL
		//private const int MOD_SHIFT = 0x4; //SHIFT
		//private const int VK_SPACE = 0x20; //SPACE
		//二、注册热键方法 ///
		///// 注册热键
		/////
		///// 窗口句柄
		///// 热键ID
		///// 组合键
		///// 热键
		//private void RegKey(IntPtr hwnd, int hotKey_id, int fsModifiers, int vk)
		//{
		//bool result;
		//if (RegisterHotKey(hwnd,hotKey_id,fsModifiers,vk) == 0)
		//{
		//result = false;
		//}
		//else
		//{
		//result = true;
		//}
		//if (!result)
		//{
		//MessageBox.Show("注册热键失败！");
		//}
		//}
		/////
		///// 注销热键
		/////
		///// 窗口句柄
		///// 热键ID
		//private void UnRegKey(IntPtr hwnd, int hotKey_id)
		//{
		//UnregisterHotKey(hwnd,hotKey_id);
		//}
		//三、重写WndProc方法，实现注册 protected override void WndProc(ref Message m)
		//{
		//base.WndProc(ref m);
		//switch(m.Msg)
		//{
		//case WM_HOTKEY: //窗口消息-热键
		//switch(m.WParam.ToInt32())
		//{
		//case 32: //热键ID
		//MessageBox.Show("Hot Key : Ctrl + Alt + Shift + Space");
		//break;
		//   default:
		//   break;
		//}
		//break;
		//case WM_CREATE: //窗口消息-创建
		//RegKey(Handle,Space,MOD_ALT | MOD_CONTROL | MOD_SHIFT,VK_SPACE); //注册热键
		//break;
		//case WM_DESTROY: //窗口消息-销毁
		//UnRegKey(Handle,Space); //销毁热键
		//break;
		//default:
		//break;
		//}
		//}
		//附：虚拟键值表
		//{ Virtual Keys, Standard Set }
		//{$EXTERNALSYM VK_LBUTTON}
		//VK_LBUTTON = 1;
		//{$EXTERNALSYM VK_RBUTTON}
		//VK_RBUTTON = 2;
		//{$EXTERNALSYM VK_CANCEL}
		//VK_CANCEL = 3;
		//{$EXTERNALSYM VK_MBUTTON}
		//VK_MBUTTON = 4; { NOT contiguous with L & RBUTTON }
		//{$EXTERNALSYM VK_BACK}
		//VK_BACK = 8;
		//{$EXTERNALSYM VK_TAB}
		//VK_TAB = 9;
		//{$EXTERNALSYM VK_CLEAR}
		//VK_CLEAR = 12;
		//{$EXTERNALSYM VK_RETURN}
		//VK_RETURN = 13;
		//{$EXTERNALSYM VK_SHIFT}
		//VK_SHIFT = $10;
		//{$EXTERNALSYM VK_CONTROL}
		//VK_CONTROL = 17;
		//{$EXTERNALSYM VK_MENU}
		//VK_MENU = 18;
		//{$EXTERNALSYM VK_PAUSE}
		//VK_PAUSE = 19;
		//{$EXTERNALSYM VK_CAPITAL}
		//VK_CAPITAL = 20;
		//{$EXTERNALSYM VK_KANA }
		//VK_KANA = 21;
		//{$EXTERNALSYM VK_HANGUL }
		//VK_HANGUL = 21;
		//{$EXTERNALSYM VK_JUNJA }
		//VK_JUNJA = 23;
		//{$EXTERNALSYM VK_FINAL }
		//VK_FINAL = 24;
		//{$EXTERNALSYM VK_HANJA }
		//VK_HANJA = 25;
		//{$EXTERNALSYM VK_KANJI }
		//VK_KANJI = 25;
		//{$EXTERNALSYM VK_CONVERT }
		//VK_CONVERT = 28;
		//{$EXTERNALSYM VK_NONCONVERT }
		//VK_NONCONVERT = 29;
		//{$EXTERNALSYM VK_ACCEPT }
		//VK_ACCEPT = 30;
		//{$EXTERNALSYM VK_MODECHANGE }
		//VK_MODECHANGE = 31;
		//{$EXTERNALSYM VK_ESCAPE}
		//VK_ESCAPE = 27;
		//{$EXTERNALSYM VK_SPACE}
		//VK_SPACE = $20;
		//{$EXTERNALSYM VK_PRIOR}
		//VK_PRIOR = 33;
		//{$EXTERNALSYM VK_NEXT}
		//VK_NEXT = 34;
		//{$EXTERNALSYM VK_END}
		//VK_END = 35;
		//{$EXTERNALSYM VK_HOME}
		//VK_HOME = 36;
		//{$EXTERNALSYM VK_LEFT}
		//VK_LEFT = 37;
		//{$EXTERNALSYM VK_UP}
		//VK_UP = 38;
		//{$EXTERNALSYM VK_RIGHT}
		//VK_RIGHT = 39;
		//{$EXTERNALSYM VK_DOWN}
		//VK_DOWN = 40;
		//{$EXTERNALSYM VK_SELECT}
		//VK_SELECT = 41;
		//{$EXTERNALSYM VK_PRINT}
		//VK_PRINT = 42;
		//{$EXTERNALSYM VK_EXECUTE}
		//VK_EXECUTE = 43;
		//{$EXTERNALSYM VK_SNAPSHOT}
		//VK_SNAPSHOT = 44;
		//{$EXTERNALSYM VK_INSERT}
		//VK_INSERT = 45;
		//{$EXTERNALSYM VK_DELETE}
		//VK_DELETE = 46;
		//{$EXTERNALSYM VK_HELP}
		//VK_HELP = 47;
		//{ VK_0 thru VK_9 are the same as ASCII '0' thru '9' ($30 - $39) }
		//{ VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' ($41 - $5A) }
		//{$EXTERNALSYM VK_LWIN}
		//VK_LWIN = 91;
		//{$EXTERNALSYM VK_RWIN}
		//VK_RWIN = 92;
		//{$EXTERNALSYM VK_APPS}
		//VK_APPS = 93;
		//{$EXTERNALSYM VK_NUMPAD0}
		//VK_NUMPAD0 = 96;
		//{$EXTERNALSYM VK_NUMPAD1}
		//VK_NUMPAD1 = 97;
		//{$EXTERNALSYM VK_NUMPAD2}
		//VK_NUMPAD2 = 98;
		//{$EXTERNALSYM VK_NUMPAD3}
		//VK_NUMPAD3 = 99;
		//{$EXTERNALSYM VK_NUMPAD4}
		//VK_NUMPAD4 = 100;
		//{$EXTERNALSYM VK_NUMPAD5}
		//VK_NUMPAD5 = 101;
		//{$EXTERNALSYM VK_NUMPAD6}
		//VK_NUMPAD6 = 102;
		//{$EXTERNALSYM VK_NUMPAD7}
		//VK_NUMPAD7 = 103;
		//{$EXTERNALSYM VK_NUMPAD8}
		//VK_NUMPAD8 = 104;
		//{$EXTERNALSYM VK_NUMPAD9}
		//VK_NUMPAD9 = 105;
		//{$EXTERNALSYM VK_MULTIPLY}
		//VK_MULTIPLY = 106;
		//{$EXTERNALSYM VK_ADD}
		//VK_ADD = 107;
		//{$EXTERNALSYM VK_SEPARATOR}
		//VK_SEPARATOR = 108;
		//{$EXTERNALSYM VK_SUBTRACT}
		//VK_SUBTRACT = 109;
		//{$EXTERNALSYM VK_DECIMAL}
		//VK_DECIMAL = 110;
		//{$EXTERNALSYM VK_DIVIDE}
		//VK_DIVIDE = 111;
		//{$EXTERNALSYM VK_F1}
		//VK_F1 = 112;
		//{$EXTERNALSYM VK_F2}
		//VK_F2 = 113;
		//{$EXTERNALSYM VK_F3}
		//VK_F3 = 114;
		//{$EXTERNALSYM VK_F4}
		//VK_F4 = 115;
		//{$EXTERNALSYM VK_F5}
		//VK_F5 = 116;
		//{$EXTERNALSYM VK_F6}
		//VK_F6 = 117;
		//{$EXTERNALSYM VK_F7}
		//VK_F7 = 118;
		//{$EXTERNALSYM VK_F8}
		//VK_F8 = 119;
		//{$EXTERNALSYM VK_F9}
		//VK_F9 = 120;
		//{$EXTERNALSYM VK_F10}
		//VK_F10 = 121;
		//{$EXTERNALSYM VK_F11}
		//VK_F11 = 122;
		//{$EXTERNALSYM VK_F12}
		//VK_F12 = 123;
		//{$EXTERNALSYM VK_F13}
		//VK_F13 = 124;
		//{$EXTERNALSYM VK_F14}
		//VK_F14 = 125;
		//{$EXTERNALSYM VK_F15}
		//VK_F15 = 126;
		//{$EXTERNALSYM VK_F16}
		//VK_F16 = 127;
		//{$EXTERNALSYM VK_F17}
		//VK_F17 = 128;
		//{$EXTERNALSYM VK_F18}
		//VK_F18 = 129;
		//{$EXTERNALSYM VK_F19}
		//VK_F19 = 130;
		//{$EXTERNALSYM VK_F20}
		//VK_F20 = 131;
		//{$EXTERNALSYM VK_F21}
		//VK_F21 = 132;
		//{$EXTERNALSYM VK_F22}
		//VK_F22 = 133;
		//{$EXTERNALSYM VK_F23}
		//VK_F23 = 134;
		//{$EXTERNALSYM VK_F24}
		//VK_F24 = 135;
		//{$EXTERNALSYM VK_NUMLOCK}
		//VK_NUMLOCK = 144;
		//{$EXTERNALSYM VK_SCROLL}
		//VK_SCROLL = 145;
		//{ VK_L & VK_R - left and right Alt, Ctrl and Shift virtual keys.
		//Used only as parameters to GetAsyncKeyState() and GetKeyState().
		//No other API or message will distinguish left and right keys in this way. }
		//{$EXTERNALSYM VK_LSHIFT}
		//VK_LSHIFT = 160; 
		#endregion

		#region 84.菜单勾选/取消完成后关闭计算机
		//int flg=EWX_FORCE | EWX_POWEROFF;
		//bool ok;
		//TokPriv1Luid tp;
		//IntPtr hproc = GetCurrentProcess();
		//IntPtr htok = IntPtr.Zero;
		//ok = OpenProcessToken(hproc, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, ref   htok);
		//tp.Count = 1;
		//tp.Luid = 0;
		//tp.Attr = SE_PRIVILEGE_ENABLED;
		//ok = LookupPrivilegeValue(null, SE_SHUTDOWN_NAME, ref   tp.Luid);
		//ok = AdjustTokenPrivileges(htok, false, ref   tp, 0, IntPtr.Zero, IntPtr.Zero);
		//ok = ExitWindowsEx(flg, 0);
		#endregion

		#region 85.菜单勾选/取消完成后重新启动计算机
		//int flg=EWX_FORCE | EWX_REBOOT;
		//bool ok;
		//TokPriv1Luid tp;
		//IntPtr hproc = GetCurrentProcess();
		//IntPtr htok = IntPtr.Zero;
		//ok = OpenProcessToken(hproc, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, ref   htok);
		//tp.Count = 1;
		//tp.Luid = 0;
		//tp.Attr = SE_PRIVILEGE_ENABLED;
		//ok = LookupPrivilegeValue(null, SE_SHUTDOWN_NAME, ref   tp.Luid);
		//ok = AdjustTokenPrivileges(htok, false, ref   tp, 0, IntPtr.Zero, IntPtr.Zero);
		//ok = ExitWindowsEx(flg, 0);
		#endregion

		#region 86.菜单勾选/取消完成后注销计算机
		//int flg=EWX_FORCE | EWX_LOGOFF;
		//bool ok;
		//TokPriv1Luid tp;
		//IntPtr hproc = GetCurrentProcess();
		//IntPtr htok = IntPtr.Zero;
		//ok = OpenProcessToken(hproc, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, ref   htok);
		//tp.Count = 1;
		//tp.Luid = 0;
		//tp.Attr = SE_PRIVILEGE_ENABLED;
		//ok = LookupPrivilegeValue(null, SE_SHUTDOWN_NAME, ref   tp.Luid);
		//ok = AdjustTokenPrivileges(htok, false, ref   tp, 0, IntPtr.Zero, IntPtr.Zero);
		//ok = ExitWindowsEx(flg, 0);
		#endregion

		#region 87.菜单勾选/取消开机自启动程序
		//public void RunWhenStart(bool Started)
		//{
		//string name=%%1;
		//string path=Application.ExecutablePath;
		//RegistryKey HKLM = Registry.LocalMachine;
		//RegistryKey Run = HKLM.CreateSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
		//if (Started == true)
		//{
		//	try
		//	{
		//		Run.SetValue(name, path);
		//		HKLM.Close();
		//	}
		//	catch (Exception)
		//	{
		//		MessageBox.Show("注册表修改错误(开机自启未实现)");
		//	}
		//}
		//else
		//{
		//	try
		//	{
		//		if (Run.GetValue(name) != null)
		//		{
		//			Run.DeleteValue(name);
		//			HKLM.Close();
		//		}
		//		else
		//			return;
		//	}
		//	catch (Exception e)
		//	{
		//	 //ExceptionTransact.WriteErrLog(base.GetType().Name, e.Message);
		//	MessageBox(e.Message);
		//		}
		//	}
		//}
		#endregion

		#region 88.菜单勾选/取消自动登录系统
		#endregion

		#region 89.模拟键盘输入字符串
		//using System;
		//using System.Collections.Generic;
		//using System.ComponentModel;
		//using System.Data;
		//using System.Drawing;
		//using System.Linq;
		//using System.Text;
		//using System.Windows.Forms;
		//using KenZhang.Free.VirtualInput;
		//using System.Runtime.InteropServices;
		//namespace VirtualInputDemo
		//{
		//	public partial class Form1 : Form
		//	{
		//		public const int INPUT_KEYBOARD = 1;
		//		public const int KEYEVENTF_KEYUP = 0x0002;
		//		[DllImport("user32.dll")]
		//		public static extern UInt32 SendInput(UInt32 nInputs, ref INPUT pInputs, int cbSize);
		//		[StructLayout(LayoutKind.Explicit)]
		//		public struct INPUT
		//		{
		//			[FieldOffset(0)]
		//			public Int32 type;
		//			[FieldOffset(4)]
		//			public KEYBDINPUT ki;
		//			[FieldOffset(4)]
		//			public MOUSEINPUT mi;
		//			[FieldOffset(4)]
		//			public HARDWAREINPUT hi;
		//		}
		//		[StructLayout(LayoutKind.Sequential)]
		//		public struct MOUSEINPUT
		//		{
		//			public Int32 dx;
		//			public Int32 dy;
		//			public Int32 mouseData;
		//			public Int32 dwFlags;
		//			public Int32 time;
		//			public IntPtr dwExtraInfo;
		//		}
		//		[StructLayout(LayoutKind.Sequential)]
		//		public struct KEYBDINPUT
		//		{
		//			public Int16 wVk;
		//			public Int16 wScan;
		//			public Int32 dwFlags;
		//			public Int32 time;
		//			public IntPtr dwExtraInfo;
		//		}
		//		[StructLayout(LayoutKind.Sequential)]
		//		public struct HARDWAREINPUT
		//		{
		//			public Int32 uMsg;
		//			public Int16 wParamL;
		//			public Int16 wParamH;
		//		}
		//		public Form1()
		//		{
		//			InitializeComponent();
		//		}
		//		private void button1_Click(object sender, EventArgs e)
		//		{
		//			textBox1.Focus();
		//			INPUT inDown = new INPUT();
		//			inDown.type = INPUT_KEYBOARD;
		//			inDown.ki.wVk = (int)Keys.A;
		//			//INPUT inUp = new INPUT();
		//			//inUp.type = INPUT_KEYBOARD;
		//			//inUp.ki.wVk = (int)Keys.A;
		//			//inUp.ki.dwFlags = KEYEVENTF_KEYUP;
		//			SendInput(1, ref  inDown, Marshal.SizeOf(inDown));
		//			//SendInput(1, ref  inUp, Marshal.SizeOf(inUp));
		//		}
		//	}
		//}
		#endregion

		#region 90.提取PDF文件中的文本
		//xpdf
		//   public partial class Form1 : Form
		//	{
		//		public OpenFileDialog ofdlg = new OpenFileDialog();//打开文件对话框
		//		public string filename;
		//		public Form1()
		//		{
		//			InitializeComponent();
		//		}
		//		private void button1_Click(object sender, EventArgs e)
		//		{
		//			ofdlg.Filter = "pdf文件(*.pdf)|*.pdf";//选择pdf文件
		//			if (ofdlg.ShowDialog() == DialogResult.OK)
		//			{
		//				filename = string.Format("{0}", ofdlg.FileName);
		//			}            
		//		}
		//	  //传送打开文件对话框中得到的filename来做为外部程序的参数来做转化
		//		private void button2_Click(object sender, EventArgs e)
		//		{
		//			Process p = new Process();
		//			string path = "pdftotext.exe"; //进程启用外部程序
		//							//这个exe我放在debug文件夹下面        
		//			p.StartInfo.FileName = path;
		//			p.StartInfo.Arguments = string.Format( filename + " -");//很怪异的一行
		//								  //参数“－”表示可以得到输出流
		//			p.StartInfo.UseShellExecute = false;
		//			p.StartInfo.RedirectStandardInput = true;
		//			p.StartInfo.RedirectStandardOutput = true;
		//			p.StartInfo.RedirectStandardError = true;
		//			p.StartInfo.CreateNoWindow = true;
		//			p.Start();
		//			string s = p.StandardOutput.ReadToEnd();//得到pdf文档中的文本内容
		//			textBox1.Text = s; 
		//			p.Close();
		//		}
		//	}
		//}
		//上面的程序运行后，如果是在Debug文件夹下的pdf文件就可以得到输出，可是如果在打开文件对话框中打开我桌面上的一个pdf如：@"d:\我的文档\test.pdf"，输出就会是空，但是如果把上面那怪异的一行改为： 
		//C# code
		//p.StartInfo.Arguments = string.Format( @"d:\我的文档\test.pdf" + " -");
		//程序就又会得到输出。 
		//呵呵，谢谢楼上的兄台，下载的xpdf中xpdftotext.exe用到的配置文件xpdfrc需要手动配置，我如果把那些字体啊，什么的映射成绝对路径下的文件，就不会出现上面的问题，但是我把配置文件中的路径改成了相对路径，于是就出现了上面的问题了，看兄台能够很轻易的就运行成功，一定是做过很多代码的，这里还得劳烦兄台再给看一下，帮下忙，能遇到一个大神不容易，大神可不能吝啬啊，先谢过了哈
		#endregion

		#region 91.操作内存映射文件
		//IntPtr vFileHandle = CreateFile(@"c:\temp\temp.txt",
		//	GENERIC_READ | GENERIC_WRITE, FileShare.Read | FileShare.Write,
		//	IntPtr.Zero,  FileMode.Open,
		//	FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, IntPtr.Zero);
		//if (INVALID_HANDLE_VALUE != (int)vFileHandle) 
		//{
		//	IntPtr vMappingHandle = CreateFileMapping(
		//		vFileHandle, IntPtr.Zero, PAGE_READWRITE, 0, 0, "~MappingTemp");
		//	if (vMappingHandle != IntPtr.Zero)
		//	{
		//		IntPtr vHead = MapViewOfFile(vMappingHandle,
		//			FILE_MAP_COPY | FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, IntPtr.Zero);
		//		if (vHead != IntPtr.Zero)
		//		{
		//			uint vSize = GetFileSize(vFileHandle, IntPtr.Zero);
		//			for (int i = 0; i <= vSize / 2; i++)
		//			{
		//				byte vTemp = Marshal.ReadByte((IntPtr)((int)vHead + i));
		//				Marshal.WriteByte((IntPtr)((int)vHead + i),
		//					Marshal.ReadByte((IntPtr)((int)vHead + vSize - i - 1)));
		//				Marshal.WriteByte((IntPtr)((int)vHead + vSize - i - 1), vTemp);
		//			}
		//			UnmapViewOfFile(vHead);
		//		}
		//		CloseHandle(vMappingHandle);
		//	}
		//	CloseHandle(vFileHandle);
		//}
		#endregion

		#region 92.重定向windows控制台程序的输出信息
		//delegate void dReadLine(string strLine);
		//private void excuteCommand(string strFile, string args, dReadLine onReadLine)
		//{
		//		System.Diagnostics.Process p = new System.Diagnostics.Process();
		//		p.StartInfo = new System.Diagnostics.ProcessStartInfo();
		//		p.StartInfo.FileName = strFile;
		//		p.StartInfo.Arguments = args;
		//		p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
		//		p.StartInfo.RedirectStandardOutput = true;
		//		p.StartInfo.UseShellExecute = false;
		//		p.StartInfo.CreateNoWindow = true;
		//		p.Start();
		//		System.IO.StreamReader reader = p.StandardOutput;//截取输出流
		//	string line = reader.ReadLine();//每次读取一行
		//	while (!reader.EndOfStream)
		//	{
		//			onReadLine(line);
		//			line = reader.ReadLine();
		//		}
		//		p.WaitForExit();
		//	}
		//private void PrintMessage(string strLine)
		//{
		//	this.textBox1.Text += strLine + " ";
		//}
		//excuteCommand("ipconfig", "", new dReadLine(PrintMessage));
		#endregion

		#region 93.接受邮件
		#endregion

		#region 94.发送邮件
		//using System;
		//using System.Net.Sockets;
		//using System.Net;
		//using System.Security.Cryptography;
		//using System.IO; 
		//// 类名：Pop3 
		//// 功能：接收电子邮件
		//namespace ZTSX.Email
		//{
		/////
		///// Pop3 的摘要说明。
		/////
		//public class Pop3
		//{
		//private string mstrHost     = null; //主机名称或IP地址
		//private int mintPort     = 110; //主机的端口号（默认为110）
		//private TcpClient mtcpClient   = null; //客户端
		//private NetworkStream mnetStream = null; //网络基础数据流
		//private StreamReader m_stmReader = null; //读取字节流
		//private string mstrStatMessage   = null; //执行STAT命令后得到的消息（从中得到邮件数）
		/////
		///// 构造函数
		/////
		///// 一个邮件接收对象
		//public Pop3()
		//{
		//}
		/////
		///// 构造函数
		/////
		///// 主机名称或IP地址
		//public Pop3(string host)
		//{
		//   mstrHost = host;
		//}
		/////
		///// 构造函数
		/////
		///// 主机名称或IP地址
		///// 主机的端口号
		///// 一个邮件接收对象
		//public Pop3(string host,int port)
		//{
		//   mstrHost = host;
		//   mintPort = port;
		//}
		#endregion

		#region 95.可执行文件所在目录
		public static string GetExePath()
		{
			return System.Environment.CurrentDirectory;
		}
		#endregion

		#region 96.获得特殊目录
		public static string GetSpecialPath(Environment.SpecialFolder sf)
		{
			return Environment.GetFolderPath(sf);
		}
		#endregion
	}
}
