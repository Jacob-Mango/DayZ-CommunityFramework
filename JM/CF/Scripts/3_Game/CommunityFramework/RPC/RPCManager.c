enum SingeplayerExecutionType
{
	Server = 0,
	Both,
	Client
}

enum SingleplayerExecutionType
{
	Server = 0,
	Both,
	Client
}

enum CallType
{
	Server = 0,
	Client
}

class RPCMetaWrapper
{
	protected Class m_Instance;
	protected int m_SPExecType;

	void RPCMetaWrapper( Class instance, int spExecType ) 
	{
		m_Instance = instance;
		
		m_SPExecType = spExecType;
	}
	
	Class GetInstance() 
	{
		return m_Instance;
	}
	
	// Determines if the server or client function is what is called in singleplayer mode
	int GetSPExecutionType() 
	{
		return m_SPExecType;
	}
};

class RPCManager
{
	protected float m_Timer;

	protected int m_AmountSent;
	protected int m_AmountSentInUpdate;
	protected int m_AmountSentInSecond;

	protected int m_AmountRecieved;
	protected int m_AmountRecievedInUpdate;
	protected int m_AmountRecievedInSecond;

	protected int m_AmountSuccessRecieved;
	protected int m_AmountSuccessRecievedInUpdate;
	protected int m_AmountSuccessRecievedInSecond;

	protected int m_AmountPreviousSent;
	protected int m_AmountPreviousSentInUpdate;
	protected int m_AmountPreviousSentInSecond;

	protected int m_AmountPreviousRecieved;
	protected int m_AmountPreviousRecievedInUpdate;
	protected int m_AmountPreviousRecievedInSecond;

	protected int m_AmountPreviousSuccessRecieved;
	protected int m_AmountPreviousSuccessRecievedInUpdate;
	protected int m_AmountPreviousSuccessRecievedInSecond;

	protected string m_UpdateChecker;

	protected const int FRAMEWORK_RPC_ID = 10042;
	protected autoptr map< string, ref map< string, ref RPCMetaWrapper > > m_RPCActions;

	protected bool m_RPCManagerEnabled = false;

	void RPCManager()
	{
		#ifdef CF_TRACE_ENABLED
		auto trace = CF_Trace_0(this, "RPCManager");
		#endif

		m_RPCActions = new map< string, ref map< string, ref RPCMetaWrapper > >;

		m_UpdateChecker = "JM_CF_RPC";

		if ( GetGame().IsClient() )
			m_UpdateChecker = "JM_CF_RPC_Client";
	}

	void ~RPCManager()
	{
		#ifdef CF_TRACE_ENABLED
		auto trace = CF_Trace_0(this, "~RPCManager");
		#endif

		GetDayZGame().Event_OnRPC.Remove( OnRPC );
		delete m_RPCActions;
	}
	
	void OnUpdate( float timeSlice )
	{
		#ifdef CF_TRACE_ENABLED
		auto trace = CF_Trace_1(this, "OnUpdate").Add(timeSlice);
		#endif

		string str = "";
		bool anyChanged = false;

		if ( m_AmountPreviousSent != m_AmountSent )
		{
			anyChanged = true;
			str = str + "S T: " + m_AmountSent + " | ";
		}
		if ( m_AmountPreviousSentInUpdate != m_AmountSentInUpdate )
		{
			anyChanged = true;
			str = str + "S 1s: " + m_AmountSentInSecond + " | ";
		}
		if ( m_AmountPreviousSentInSecond != m_AmountSentInSecond )
		{
			anyChanged = true;
			str = str + "S " + timeSlice + "s: " + m_AmountSentInUpdate + " | ";
		}

		if ( m_AmountPreviousRecieved != m_AmountRecieved )
		{
			anyChanged = true;
			str = str + "R T: " + m_AmountRecieved + " | ";
		}
		if ( m_AmountPreviousRecievedInUpdate != m_AmountRecievedInUpdate )
		{
			anyChanged = true;
			str = str + "R 1s: " + m_AmountRecievedInSecond + " | ";
		}
		if ( m_AmountPreviousRecievedInSecond != m_AmountRecievedInSecond )
		{
			anyChanged = true;
			str = str + "R " + timeSlice + "s: " + m_AmountRecievedInUpdate + " | ";
		}

		if ( m_AmountPreviousSuccessRecieved != m_AmountSuccessRecieved )
		{
			anyChanged = true;
			str = str + "S R T: " + m_AmountSuccessRecieved + " | ";
		}
		if ( m_AmountPreviousSuccessRecievedInUpdate != m_AmountSuccessRecievedInUpdate )
		{
			anyChanged = true;
			str = str + "S R 1s: " + m_AmountSuccessRecievedInSecond + " | ";
		}
		if ( m_AmountPreviousSuccessRecievedInSecond != m_AmountSuccessRecievedInSecond )
		{
			anyChanged = true;
			str = str + "S R " + timeSlice + "s: " + m_AmountSuccessRecievedInUpdate;
		}

		m_AmountSentInUpdate = 0;
		m_AmountRecievedInUpdate = 0;
		m_AmountSuccessRecievedInUpdate = 0;

		m_Timer += timeSlice;

		if ( m_Timer >= 1.0 )
		{
			m_Timer = 0;

			m_AmountSentInSecond = 0;
			m_AmountRecievedInSecond = 0;
			m_AmountSuccessRecievedInSecond = 0;
		}

		m_AmountPreviousSent = m_AmountSent;
		m_AmountPreviousSentInUpdate = m_AmountSentInUpdate;
		m_AmountPreviousSentInSecond = m_AmountSentInSecond;
		m_AmountPreviousRecieved = m_AmountRecieved;
		m_AmountPreviousRecievedInUpdate = m_AmountRecievedInUpdate;
		m_AmountPreviousRecievedInSecond = m_AmountRecievedInSecond;
		m_AmountPreviousSuccessRecieved = m_AmountSuccessRecieved;
		m_AmountPreviousSuccessRecievedInUpdate = m_AmountSuccessRecievedInUpdate;
		m_AmountPreviousSuccessRecievedInSecond = m_AmountSuccessRecievedInSecond;
	}

	void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		#ifdef CF_TRACE_ENABLED
		auto trace = CF_Trace_4(this, "OnRPC").Add(sender).Add(target).Add(rpc_type).Add(ctx);
		#endif

		if ( rpc_type != FRAMEWORK_RPC_ID )
		{
			return;
		}
		
		//! read the mod and function name
		//! todo: optimize network transfer by joining modName and funcName, this will remove 4 bytes for string length count. with network traffic, every byte counts
		string modName;
		string funcName;
		if ( !ctx.Read( modName ) || !ctx.Read( funcName ) )
			return;

		m_AmountRecieved++;
		m_AmountRecievedInUpdate++;
		m_AmountRecievedInSecond++;

		map< string, ref RPCMetaWrapper > functions;
		RPCMetaWrapper wrapper;
		if ( !m_RPCActions.Find( modName, functions ) || !functions.Find( funcName, wrapper ) || !wrapper.GetInstance() )
		{
			string recievedFrom = "server";

			if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			{
				if ( sender == NULL )
				{
					recievedFrom = "someone";
				} else 
				{
					recievedFrom = sender.GetPlainId();
				}
			}

			Error( recievedFrom + " tried sending <" + modName + ">::" + funcName + " which does not seem to exist!");
			return;
		}
		
		//! If the ID is +1, then we are sending an iterative array from ::SendRPCs
		if ( rpc_type == FRAMEWORK_RPC_ID + 1 )
		{
			int count;
			if (!ctx.Read(count))
				return;

			for ( int index = 0; index < count; index++ )
			{
				if (callServer)
					GetGame().GameScript.CallFunctionParams( wrapper.GetInstance(), funcName, NULL, new Param4< CallType, ref ParamsReadContext, ref PlayerIdentity, ref Object >( CallType.Server, ctx, sender, target ) );

				if (callClient)
					GetGame().GameScript.CallFunctionParams( wrapper.GetInstance(), funcName, NULL, new Param4< CallType, ref ParamsReadContext, ref PlayerIdentity, ref Object >( CallType.Client, ctx, sender, target ) );
			}

			return;
		}

		if (callServer)
			GetGame().GameScript.CallFunctionParams( wrapper.GetInstance(), funcName, NULL, new Param4< CallType, ref ParamsReadContext, ref PlayerIdentity, ref Object >( CallType.Server, ctx, sender, target ) );

		if (callClient)
			GetGame().GameScript.CallFunctionParams( wrapper.GetInstance(), funcName, NULL, new Param4< CallType, ref ParamsReadContext, ref PlayerIdentity, ref Object >( CallType.Client, ctx, sender, target ) );
	}

	void SendRPC( string modName, string funcName, ref Param params = NULL, bool guaranteed = false, ref PlayerIdentity sendToIdentity = NULL, ref Object sendToTarget = NULL )
	{
		#ifdef CF_TRACE_ENABLED
		auto trace = CF_Trace_6(this, "SendRPC").Add(modName).Add(funcName).Add(params).Add(guaranteed).Add(sendToIdentity).Add(sendToTarget);
		#endif

		m_AmountSent++;
		m_AmountSentInUpdate++;
		m_AmountSentInSecond++;
		
		string sendTo = "server";

		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			if ( sendToIdentity == NULL )
			{
				sendTo = "everyone";
			} else 
			{
				sendTo = sendToIdentity.GetPlainId();
			}
		}

		auto sendData = new ref array< ref Param >;
		sendData.Insert( new ref Param2< string, string >( modName, funcName ) );
		sendData.Insert( params );
		
		//In case we are in the singleplayer and the data is consumed twice for both client and server, we need to add it twice. Better than making a deep copy with more complicated rules on receiving
		if ( !GetGame().IsMultiplayer() )
		{
			if ( m_RPCActions.Contains( modName ) )
			{
				if ( m_RPCActions[ modName ].Contains( funcName ) )
				{
					ref RPCMetaWrapper wrapper = m_RPCActions[ modName ][ funcName ];
					
					if ( ( wrapper.GetSPExecutionType() == SingleplayerExecutionType.Both ) )
					{
						ctx.Write(params);
					}
				}
			}
		}
		
		ctx.Send(sendToTarget, FRAMEWORK_RPC_ID, guaranteed, sendToIdentity);
	}	

	void SendRPCs( string modName, string funcName, ref array< ref Param > params, bool guaranteed = false, ref PlayerIdentity sendToIdentity = NULL, ref Object sendToTarget = NULL )
	{
		#ifdef CF_TRACE_ENABLED
		auto trace = CF_Trace_6(this, "SendRPCs").Add(modName).Add(funcName).Add(params).Add(guaranteed).Add(sendToIdentity).Add(sendToTarget);
		#endif

		m_AmountSent++;
		m_AmountSentInUpdate++;
		m_AmountSentInSecond++;

		ScriptRPC ctx = new ScriptRPC();
		ctx.Write(modName);
		ctx.Write(funcName);
		ctx.Write(params.Count());

		bool duplicate = false;

		//In case we are in the singleplayer and the data is consumed twice for both client and server, we need to add it twice. Better than making a deep copy with more complicated rules on receiving
		if ( !GetGame().IsMultiplayer() )
		{
			if ( m_RPCActions.Contains( modName ) && m_RPCActions[ modName ].Contains( funcName ) )
			{					
				duplicate = m_RPCActions[ modName ][ funcName ].GetSPExecutionType() == SingleplayerExecutionType.Both;
			}
		}

		params.InsertAt( new ref Param2< string, string >( modName, funcName ), 0 );

		ctx.Send(sendToTarget, FRAMEWORK_RPC_ID + 1, guaranteed, sendToIdentity);
	}

	static ref CF_RPC_Context Prepare(string modName, string funcName, Object target = null, bool guaranteed = false)
	{
		return CF_RPC_Context.Prepare(modName, funcName, target, guaranteed);
	}

	bool AddRPC( string modName, string funcName, Class instance, int singlePlayerExecType = SingleplayerExecutionType.Server )
	{
		#ifdef CF_TRACE_ENABLED
		auto trace = CF_Trace_4(this, "AddRPC").Add(modName).Add(funcName).Add(instance).Add(singlePlayerExecType);
		#endif

		if ( !m_RPCActions.Contains( modName ) )
		{
			m_RPCActions.Set( modName, new ref map< string, ref RPCMetaWrapper > );
		}
		
		auto wrapper = new ref RPCMetaWrapper( instance, singlePlayerExecType );
		
		m_RPCActions[ modName ].Set( funcName, wrapper );

		if ( !m_RPCManagerEnabled )
		{
			GetDayZGame().Event_OnRPC.Insert( OnRPC );

			m_RPCManagerEnabled = true;
		}

		return true;
	}
};

static ref RPCManager g_RPCManager;

static ref RPCManager GetRPCManager()
{
	if ( !g_RPCManager )
	{
		g_RPCManager = new ref RPCManager;
	}

	return g_RPCManager;
}
