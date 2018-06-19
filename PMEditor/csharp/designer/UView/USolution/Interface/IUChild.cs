using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace USolution.Interface
{
    public interface IUChild
    {
        string Name { get; }

        Guid ID { get; }

        IUGroup Parent { get; set; }
    }
}
