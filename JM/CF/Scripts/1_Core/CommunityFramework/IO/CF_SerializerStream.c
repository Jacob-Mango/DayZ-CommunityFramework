class CF_SerializerStream : CF_Stream
{
	Serializer m_Serializer;

	void CF_SerializerStream(Serializer serializer)
	{
		m_Serializer = serializer;
	}

	void Write()
	{
		Write(m_Serializer);
	}

	void Read()
	{
		Read(m_Serializer);
	}
};

class CF_SerializerReadStream : CF_SerializerStream
{
	void CF_SerializerReadStream(Serializer serializer)
	{
		Read();
	}
};

class CF_SerializerWriteStream : CF_SerializerStream
{
	void ~CF_SerializerWriteStream()
	{
		Write();
	}
};