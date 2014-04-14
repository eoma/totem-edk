using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Threading;

namespace NetGameSharp
{
    class TcpConnection : IDisposable
    {
		private readonly Socket _Socket;
		private readonly EventWaitHandle _DataAvailableEvent = new EventWaitHandle(false, EventResetMode.ManualReset);
		private bool _CanReadMore = true;
		private readonly List<byte> _SocketReadData = new List<byte>();
		
		public TcpConnection(string server, int port)
        {
            _Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            Timeout = 15000;
            _Socket.Connect(server, port);
            _Socket.NoDelay = true;
            StartReceiveData();
        }

        public int Timeout
        {
            get { return _Socket.ReceiveTimeout; }
            set { _Socket.ReceiveTimeout = value; _Socket.SendTimeout = value; }
        }

        public EventWaitHandle DataAvailableEvent
        {
            get
            {
                return _DataAvailableEvent;
            }
        }

        public bool IsEndOfStream
        {
            get
            {
                lock (this)
                {
                    return _SocketReadData.Count == 0 && !CanReadMore;
                }
            }
        }

        public int BytesAvailable
        {
            get { lock (this) return _SocketReadData.Count; }
        }

        public ushort ReadUInt16()
        {
            byte[] lengthBinary = ReadBytes(2);
            if (!BitConverter.IsLittleEndian)
                lengthBinary = lengthBinary.Reverse().ToArray();
            return BitConverter.ToUInt16(lengthBinary, 0);
        }

        public byte[] ReadBytes(int length)
        {
            while (CanReadMore && BytesAvailable < length)
            {
                _DataAvailableEvent.Reset();
                if (!_DataAvailableEvent.WaitOne(_Socket.ReceiveTimeout))
                    throw new Exception("ReadBytes timed out");
            }

            lock (this)
            {
                length = Math.Min(length, _SocketReadData.Count);
                byte[] data = _SocketReadData.GetRange(0, length).ToArray();
                _SocketReadData.RemoveRange(0, length);
                if (_SocketReadData.Count == 0 && _CanReadMore)
                    _DataAvailableEvent.Reset();
                return data;
            }
        }

        public void WriteBytes(byte[] bytes)
        {
            _Socket.Send(bytes);
        }

        public void Dispose()
        {
            if (_Socket != null)
                _Socket.Close();
        }

        void StartReceiveData()
        {
            SocketReadResult state = new SocketReadResult();
            _Socket.BeginReceive(state.Buffer, 0, SocketReadResult.BufferSize, 0, ReceiveCallback, state);
        }

        void ReceiveCallback(IAsyncResult ar)
        {
            try
            {
                SocketReadResult state = (SocketReadResult)ar.AsyncState;
                int bytesRead = _Socket.EndReceive(ar);
                lock (this)
                {
                    if (bytesRead == 0)
                    {
                        _CanReadMore = false;
                    }
                    else
                    {
                        for (int i = 0; i < bytesRead; ++i)
                            _SocketReadData.Add(state.Buffer[i]);
                        StartReceiveData();
                    }
                    _DataAvailableEvent.Set();
                }
            }
            catch
            {
            }
        }

        bool CanReadMore
        {
            get { lock (this) return _CanReadMore; }
            set { lock (this) _CanReadMore = value; }
        }
    }
}

class SocketReadResult
{
    public const int BufferSize = 65536;
    public byte[] Buffer = new byte[BufferSize];
}
