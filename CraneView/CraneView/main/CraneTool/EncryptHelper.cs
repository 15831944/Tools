using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CraneTool
{
	public static class EncryptHelper
	{
		public enum EncryptResult{
			Good,
			DataBad,
			CrcBad,
			VersionBad,
			TypeBad,

		}
		//private static byte[] _keyC01 = { 0x00, 0x00, 0x00, 0x00}; //, 0x41, 0x74, 0x6c, 0x61, 0x73, 0x4c, 0x69, 0x75 };
		private static byte[] _version = {0x01, 0x00, 0x00, 0x00};

		public static EncryptResult Encrypt(string xml, string type, out byte[] data)
		{
			if (xml.Length == 0) { data = null; return EncryptResult.DataBad; }

			// add head info
			xml = string.Format("type={0};", type) + xml;
			byte[] byteArray = System.Text.Encoding.Default.GetBytes(xml);

			// get key
			byte[] byLen = BitConverter.GetBytes(byteArray.Length);
            uint crc32FromLen = CRC32.GetCRC32(byLen);
            byte[] key = BitConverter.GetBytes(crc32FromLen);
			//Array.Copy(byCrc32FromLen, _keyC01, byLen.Length);

			// encrypt
			data = new byte[byteArray.Length + 8];
			byte[] dataOut;
			EncryptData(key, byteArray, 0, out dataOut);

			// crc32
			uint crc32 = CRC32.GetCRC32(dataOut, 0);
			byte[] byCrc32 = BitConverter.GetBytes(crc32);

			// copy data
			Array.Copy(byCrc32, data, byCrc32.Length);
			Array.Copy(_version, 0, data, byCrc32.Length, _version.Length);
			Array.Copy(dataOut, 0, data, byCrc32.Length + _version.Length, dataOut.Length);

			return EncryptResult.Good;
		}
		public static EncryptResult Decrypt(byte[] data, string type, ref string xml)
		{
			if (data.Length <= 8)	return EncryptResult.DataBad;

			byte[] dataXml = new byte[data.Length - 8];
			Array.Copy(data, 8, dataXml, 0, dataXml.Length);

			// crc32
			uint crc32 = CRC32.GetCRC32(data, 8);
			uint crcSource = BitConverter.ToUInt32(data, 0);
			if (crc32 != crcSource) return EncryptResult.CrcBad;

			// version
			uint version = BitConverter.ToUInt32(data, 4);
			if (version != BitConverter.ToUInt32(_version, 0))
				return EncryptResult.VersionBad;

			// key
			byte[] byLen = BitConverter.GetBytes(dataXml.Length);
            uint crc32FromLen = CRC32.GetCRC32(byLen);
            byte[] key = BitConverter.GetBytes(crc32FromLen);
			//Array.Copy(byLen, _keyC01, byLen.Length);

			// decrypt
			byte[] dataOut;
			EncryptData(key, dataXml, 0, out dataOut);

			// type
			xml = System.Text.Encoding.Default.GetString(dataOut);
			int idx = xml.IndexOf('<');
			if (idx < 0)	return EncryptResult.DataBad;
			string cfg = xml.Substring(0, idx);
			string[] kvlist = cfg.Split(';');
			bool bType = false;
			foreach (string kvstr in kvlist)
			{
				string[] kv = kvstr.Split('=');
				if (kv.Length != 2) continue;
				if (kv[0] == "type")
				{
					bType = (string.Compare(kv[1], type, true) == 0);
				}
			}

			xml = xml.Substring(idx);
			return EncryptResult.Good;
		}
		private static void EncryptData(byte[] key, byte[] dataIn, int inIdx, out byte[] dataOut)
		{
			dataOut = new byte[dataIn.Length - inIdx];
			int keyIdx = 0;
			for(int i = 0; i < dataIn.Length; i++)
			{
				dataOut[i] = (byte)(dataIn[inIdx + i] ^ key[keyIdx]);
				if (++keyIdx >= key.Length)
					keyIdx = 0;
			}
		}
	}
}
