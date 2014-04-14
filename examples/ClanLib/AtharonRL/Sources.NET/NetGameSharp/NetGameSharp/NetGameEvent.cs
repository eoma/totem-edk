using System;
using System.Collections.Generic;

namespace NetGameSharp
{
    public class NetGameEvent
    {
    	private readonly string name;
    	private readonly List<object> arguments = new List<object>();

    	public NetGameEvent(string name)
    	{
    		this.name = name;
    	}

    	public String Name
    	{
			get { return name; }
    	}

    	public void AddArgument(object argument)
    	{
			arguments.Add(argument);
    	}

    	public int ArgumentCount
    	{
			get { return arguments.Count; }
    	}

		public object GetArgument(int index)
		{
			return arguments[index];
		}
    }
}
