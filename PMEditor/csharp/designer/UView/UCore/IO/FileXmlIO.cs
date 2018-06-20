using System;
using System.IO;

namespace UCore.IO
{
    /// <summary>
    /// working for read write file
    /// TODO. Support encryption in furture
    /// </summary>
    public class FileXmlIO
    {
        public static bool Save2File(string fileName, string xml)
        {
            try
            {
                using (StreamWriter sw = new StreamWriter(fileName))
                {
                    sw.Write(xml);
                    sw.Close();
                }
            }
            catch (Exception) { return false; }
            return true;
        }

        public static bool ReadFile(string fileName, ref string xml)
        {
            try
            {
                using (StreamReader sr = new StreamReader(fileName))
                {
                    xml = sr.ReadToEnd();
                    sr.Close();
                }
            }catch (Exception) { return false; }
            return true;
        }
    }
}
