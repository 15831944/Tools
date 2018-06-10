using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace USolution
{
    public class Solution
    {
        private string _path;
        private string _name;
        private Guid _id;
        private List<Interface.IUServer> _sevList = new List<Interface.IUServer>();  // Server list

        private Solution()
        { }

        public static Solution LoadSolution(string strSlnFile)
        {
            Solution sln;
            return null;
        }

        public static Solution CreateSolution(string strFileName, string strFilePath)
        {
            Solution sln = new Solution();
            sln._name = strFileName;
            sln._path = strFilePath;
            sln._id = Guid.NewGuid();
            return sln;
        }
    }
}
