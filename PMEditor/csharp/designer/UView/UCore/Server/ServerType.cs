using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UCore.Server
{
    public class ServerType
    {
        public static Dictionary<string, ServerType> ServerTypeMap = new Dictionary<string, ServerType>();
        public static ServerType Create(string name, Type sevType)
        {
            if (ServerTypeMap.ContainsKey(name)) return null;   // already exist
            var st = new ServerType(name, sevType);
            ServerTypeMap.Add(name, st);
            return st;
        }

        private string _name;
        private Type _type;

        internal ServerType(string name, Type t)
        {
            _name = name;
            _type = t;
        }

        public string Name { get { return _name; } }
        public Type SevType { get { return _type; } }
    }
}
