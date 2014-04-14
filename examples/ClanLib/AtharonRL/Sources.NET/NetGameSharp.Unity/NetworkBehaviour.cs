using NetGameSharp;
using UnityEngine;
using System.Collections;

public class NetworkBehaviour : MonoBehaviour {

	private NetGameClient client;

	// Use this for initialization
	void Start () {
		client = new NetGameClient();

		client.EventConnected += client_EventConnected;
		client.EventDisconnected += client_EventDisconnected;
		client.EventReceived += client_EventReceived;
		
		client.Connect("localhost", "4556");
	}

	private void client_EventReceived(NetGameEvent e)
	{
//		UnityEditor.EditorUtility.DisplayDialog("client_EventReceived", "Body", "Yes", "No");
	}

	private void client_EventDisconnected()
	{
//		UnityEditor.EditorUtility.DisplayDialog("client_EventDisconnected", "Body", "Yes", "No");
	}

	private void client_EventConnected()
	{
//		UnityEditor.EditorUtility.DisplayDialog("client_EventConnected", "Body", "Yes", "No");
	}

	// Update is called once per frame
	void Update () {
		client.ProcessEvents();	
	}
}
