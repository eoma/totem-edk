using System;
using System.Windows.Forms;
using NetGameSharp;

namespace NetGameTest
{
	public partial class Form1 : Form
	{
		private readonly NetGameClient client;

		public Form1()
		{
			InitializeComponent();

			client = new NetGameClient();

			client.EventConnected += client_EventConnected;
			client.EventDisconnected += client_EventDisconnected;
			client.EventReceived += client_EventReceived;

			SetDisconnectedState();
		}

		void client_EventReceived(NetGameEvent gameEvent)
		{
            if (gameEvent.ArgumentCount > 0)
            {
                listBox.Items.Add(String.Format("Event {0} received with {1} arguments: ", gameEvent.Name, gameEvent.ArgumentCount));
                for (int i = 0; i < gameEvent.ArgumentCount; ++i)
                {
                    listBox.Items.Add(gameEvent.GetArgument(i).ToString());
                }
            }
            else
            {
                listBox.Items.Add(String.Format("Event {0} received", gameEvent.Name, gameEvent.ArgumentCount));
            }
		}

		void client_EventDisconnected()
		{
			SetDisconnectedState();
			listBox.Items.Add("Disconnected");
		}

		void client_EventConnected()
		{
			SetConnectedState();
			listBox.Items.Add("Connected");
		}

		private void buttonConnect_Click(object sender, EventArgs e)
		{
			client.Connect(textBoxHost.Text, textBoxPort.Text);
		}

		private void buttonDisconnect_Click(object sender, EventArgs e)
		{
			client.Disconnect();
		}

		private void buttonSendLoginEvent_Click(object sender, EventArgs e)
		{
			NetGameEvent loginEvent = new NetGameEvent("Login");
			loginEvent.AddArgument(textBoxLoginUsername.Text);
            loginEvent.AddArgument(textBoxLoginPassword.Text);

			client.SendEvent(loginEvent);
		}

        private void buttonSendCharacterListEvent_Click(object sender, EventArgs e)
        {
            NetGameEvent characterGetListEvent = new NetGameEvent("CharacterGetList");
            client.SendEvent(characterGetListEvent);
        }

		private void SetDisconnectedState()
		{
            buttonSendLoginEvent.Enabled = false;
            buttonSendCharacterListEvent.Enabled = false;
            textBoxHost.Enabled = true;
			textBoxPort.Enabled = true;
			buttonConnect.Visible = true;
			buttonDisconnect.Visible = false;
		}

		private void SetConnectedState()
		{
            buttonSendLoginEvent.Enabled = true;
            buttonSendCharacterListEvent.Enabled = true;
            textBoxHost.Enabled = false;
			textBoxPort.Enabled = false;
			buttonConnect.Visible = false;
			buttonDisconnect.Visible = true;
		}

        private void timerProcess_Tick(object sender, EventArgs e)
        {
            client.ProcessEvents();
        }
    }
}
