using System;

namespace NetGameSharp
{
    public class NetGameClient : IDisposable
    {
        public void Connect(string server, string port)
        {
            Disconnect();
            _Connection = new NetGameConnection(server, port);
        }

        ~NetGameClient()
        {
            Disconnect();
        }

        public void Dispose()
        {
            Disconnect();
        }

        public void Disconnect()
        {
            if (_Connection != null)
                _Connection.Disconnect();
        }

        public void ProcessEvents()
        {
            if (_Connection != null)
            {
                foreach (NetGameEvent e in _Connection.GetAvailableEvents())
                {
                    if (e.Name == NetGameConnection.DisconnectedEvent)
                        EventDisconnected();
                    else if (e.Name == NetGameConnection.ConnectedEvent)
                        EventConnected();
                    else
                        EventReceived(e);
                }
            }
        }

        public void SendEvent(NetGameEvent gameEvent)
        {
            if (_Connection != null)
                _Connection.SendEvent(gameEvent);
        }

    	public Action EventConnected;
    	public Action EventDisconnected;
    	public Action<NetGameEvent> EventReceived;

		NetGameConnection _Connection;
    }
}
