using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UCore.Server;

namespace DispServer
{
    public class ServerCreator
    {
        private const string TypeName = "DisplayServer";

        public static void Create()
        {
            var sevType = ServerType.Create(TypeName, typeof(ServerCreator));
            //sevType.SetImage();
        }
    }
}
