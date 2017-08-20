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
		public static readonly string ErrorMessage = "Name is error, can not contains below character:\r\n" + ErrorStr;
		public static bool FileNameRight(string strFile)
		{
			strFile = strFile.Trim();
			string[] errorStr = new string[] {  };

			if (string.IsNullOrEmpty(strFile))	return false;
			for (int i = 0; i < ErrorStr.Length; i++)
			{
				if (strFile.Contains(ErrorStr[i]))
					return false;
			}
			return true;
		}
	}
}
