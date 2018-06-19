using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace USolution.Interface
{
    public interface IUGroup
    {
        string Name { get; }

        Guid ID { get; }

        IUGroup Parent { get; set; }

        List<IUGroup> Groups { get; }

        List<IUChild> Children { get; }

        bool AddChildGroup(IUGroup group);

        void RemoveGroup(IUGroup group);

        bool IsNameExist(string name);
    }
}
