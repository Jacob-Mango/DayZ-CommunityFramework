class CF_KeyEventArgs : CF_PositionEventArgs
{
	int Key;
	CF_KeyState State;

	override string ToStr()
	{
		string str = super.ToStr();
		str += " Key=" + Key;
		str += " State=" + State;
		return str;
	}
};