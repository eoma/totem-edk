using System.Text;
using System.Xml;

namespace NetGameSharp
{
    class NetGameNetworkData
    {
        public static NetGameEvent ReceiveData(byte[] message)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(Encoding.UTF8.GetString(message));

            NetGameEvent e = new NetGameEvent(doc.DocumentElement.Name);
            XmlNodeList children = doc.DocumentElement.ChildNodes;
            foreach (XmlNode node in children)
            {
                if (node.NodeType == XmlNodeType.Element)
                {
                    XmlElement element = (XmlElement)node;

                    e.AddArgument(element.InnerText);
                }
            }

            return e;
        }

        public static byte[] SendData(NetGameEvent gameEvent)
        {
            XmlDocument doc = new XmlDocument();

            XmlElement eventElement = doc.CreateElement(gameEvent.Name);
            for(int i = 0; i < gameEvent.ArgumentCount; ++i)
            {
                XmlElement valueElement = doc.CreateElement("string");
                XmlText value = doc.CreateTextNode(gameEvent.GetArgument(i).ToString());

             	valueElement.AppendChild(value);
                eventElement.AppendChild(valueElement);
            }

            doc.AppendChild(eventElement);

            return Encoding.UTF8.GetBytes(doc.OuterXml);
        }
    }
}
