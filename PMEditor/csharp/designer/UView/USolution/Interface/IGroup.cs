using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace USolution.Interface
{
    public interface IGroup
    {
        string Name { get; }
        Guid ID { get; }
        IGroup Parent { get; set; }

        bool AddChildGroup(IGroup group);

        void RemoveGroup(IGroup group);

        bool IsNameExist(string name);
    }
}
