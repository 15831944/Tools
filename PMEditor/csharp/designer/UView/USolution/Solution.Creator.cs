using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace USolution
{
    public partial class Solution
    {
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
