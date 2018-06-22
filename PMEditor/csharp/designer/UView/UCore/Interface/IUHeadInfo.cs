using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UCore.Interface
{
    public interface IUHeadInfo
    {
        Guid ID { get; set; }
        string Name { get; set; }
        string Path { get; set; }
        string Comment { get; set; }
        DateTime Time { get; set; }
    }
}
