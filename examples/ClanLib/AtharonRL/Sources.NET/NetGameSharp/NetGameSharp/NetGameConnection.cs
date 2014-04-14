using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace NetGameSharp
{
    class NetGameConnection
    {
		private readonly string _Server;
		private readonly int _Port;
		private TcpConnection _Connection;
		private readonly EventWaitHandle _StopEvent = new EventWaitHandle(false, EventResetMode.ManualReset);
		private readonly EventWaitHandle _OutboundDataAvailableEvent = new EventWaitHandle(false, EventResetMode.ManualReset);
		private List<NetGameEvent> _AvailableEvents = new List<NetGameEvent>();
		private readonly List<NetGameEvent> _SendQueue = new List<NetGameEvent>();

		public const string ConnectedEvent = "_connected";
        public const string DisconnectedEvent = "_close";

        public NetGameConnection(string server, string port)
        {
            lock (this)
            {
                _Server = server;
                _Port = Convert.ToInt32(port);
            }
            Thread thread = new Thread(WorkerThreadMain);
            thread.IsBackground = true;
            thread.Start();
        }

        public void Disconnect()
        {
            SendEvent(new NetGameEvent(DisconnectedEvent));
        }

        public void SendEvent(NetGameEvent gameEvent)
        {
            lock (this)
            {
                _SendQueue.Add(gameEvent);
                _OutboundDataAvailableEvent.Set();
            }
        }

        public List<NetGameEvent> GetAvailableEvents()
        {
            lock (this)
            {
                List<NetGameEvent> list = _AvailableEvents;
                _AvailableEvents = new List<NetGameEvent>();
                return list;
            }
        }

        void WorkerThreadMain()
        {
            try
            {
                using (_Connection = new TcpConnection(_Server, _Port))
                {
                    PushInboundEvent(new NetGameEvent(ConnectedEvent));
                    WaitHandle[] wakeupHandles = { _StopEvent, _Connection.DataAvailableEvent, _OutboundDataAvailableEvent };
                    while (true)
                    {
                        int wakeupReason = WaitHandle.WaitAny(wakeupHandles);
                        if (wakeupReason == 0)
                        {
                            break;
                        }
                        else if (wakeupReason == 1)
                        {
                            if (!ReadMessage())
                                break;
                        }
                        else if (wakeupReason == 2)
                        {
                            if (!SendMessage())
                                break;
                        }
                    }
                }
            }
            catch
            {
                PushInboundEvent(new NetGameEvent(DisconnectedEvent));
            }
        }

        bool ReadMessage()
        {
            if (_Connection.IsEndOfStream)
            {
                PushInboundEvent(new NetGameEvent(DisconnectedEvent));
                return false; // Remote host closed the connection
            }

            ushort messageLength = _Connection.ReadUInt16();
            byte[] message = _Connection.ReadBytes(messageLength);
            NetGameEvent gameEvent = NetGameNetworkData.ReceiveData(message);
            PushInboundEvent(gameEvent);
            return gameEvent.Name != DisconnectedEvent;
        }

        bool SendMessage()
        {
            NetGameEvent gameEvent = PopOutboundEvent();
            if (gameEvent != null)
            {
                byte[] message = NetGameNetworkData.SendData(gameEvent);
                byte[] messageLength = BitConverter.GetBytes((ushort)message.Length);
                if (!BitConverter.IsLittleEndian)
					messageLength = messageLength.Reverse().ToArray();

                // To do: merge messageLength and message before sending
                _Connection.WriteBytes(messageLength);
                _Connection.WriteBytes(message);

                if (gameEvent.Name == DisconnectedEvent)
                {
                    PushInboundEvent(new NetGameEvent(DisconnectedEvent));
                    return false;
                }
            }
            return true;
        }

        void PushInboundEvent(NetGameEvent gameEvent)
        {
            lock (this)
            {
                _AvailableEvents.Add(gameEvent);
            }
        }

        NetGameEvent PopOutboundEvent()
        {
            lock (this)
            {
                if (_SendQueue.Count == 0)
                {
                    _OutboundDataAvailableEvent.Reset();
                    return null;
                }
                else
                {
                    NetGameEvent gameEvent = _SendQueue[0];
                    _SendQueue.RemoveAt(0);
                    return gameEvent;
                }
            }
        }
    }
}
