using System;
using System.ComponentModel.Design.Serialization;
using System.Collections;

namespace DesignUI.Service
{
    internal class DesignerSerializationServiceImpl : IDesignerSerializationService
    {
        private string _fileName;
        private IServiceProvider _serviceProvider;

        public DesignerSerializationServiceImpl(IServiceProvider serviceProvider) : this(serviceProvider, "")
        {}

        public DesignerSerializationServiceImpl(IServiceProvider serviceProvider, string fileName)
        {
            this._fileName = fileName;
            this._serviceProvider = serviceProvider;
        }

        public string FileName { get { return _fileName; } set { _fileName = value; } }

        public ICollection Deserialize(object serializationData)
        {
            SerializationStore serializationStore = serializationData as SerializationStore;
            if (serializationStore != null)
            {
                ComponentSerializationService componentSerializationService = _serviceProvider.GetService(typeof(ComponentSerializationService)) as ComponentSerializationService;
                ICollection collection = componentSerializationService.Deserialize(serializationStore);
                return collection;
            }
            return new object[] { };
        }

        public object Serialize(ICollection objects)
        {
            ComponentSerializationService componentSerializationService = _serviceProvider.GetService(typeof(ComponentSerializationService)) as ComponentSerializationService;
            SerializationStore returnObject = null;
            using (SerializationStore serializationStore = componentSerializationService.CreateStore())
            {
                foreach (object obj in objects)
                {
                    componentSerializationService.Serialize(serializationStore, obj);
                }
                returnObject = serializationStore;
            }
            return returnObject;
        }
    }
}
