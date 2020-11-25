class CF_ModStorage_Converter
{
	private static bool m_Constructed = false;
	private static autoptr array<typename> m_Types = new array<typename>();
	private static autoptr map<typename, typename> m_TypeMap = new map<typename, typename>();

	private static autoptr map<typename, ref Param2<typename, typename>> m_ArrayTypeMap = new map<typename, ref Param2<typename, typename>>();

	static void Construct()
	{
		if (m_Constructed)
			return;
		
		m_Constructed = true;
		
		//TODO: test why this still worked with all 'types' set to bool
		AddType(bool, CF_ModStorage_Data_Bool);
		AddType(int, CF_ModStorage_Data_Int);
		AddType(float, CF_ModStorage_Data_Float);
		AddType(vector, CF_ModStorage_Data_Vector);
		AddType(string, CF_ModStorage_Data_String);
		AddType(Class, CF_ModStorage_Data_Class);
		
		AddType(TBoolArray, CF_ModStorage_Data_Array_Bool);
		AddType(TIntArray, CF_ModStorage_Data_Array_Int);
		AddType(TFloatArray, CF_ModStorage_Data_Array_Float);
		AddType(TVectorArray, CF_ModStorage_Data_Array_Vector);
		AddType(TStringArray, CF_ModStorage_Data_Array_String);
		AddType(TClassArray, CF_ModStorage_Data_Array_Class);

		SortTypes();

		AddArrayType(TBoolArray, bool, CF_ModStorage_Data_ArrayProperty_Bool);
		AddArrayType(TIntArray, int, CF_ModStorage_Data_ArrayProperty_Int);
		AddArrayType(TFloatArray, float, CF_ModStorage_Data_ArrayProperty_Float);
		AddArrayType(TVectorArray, vector, CF_ModStorage_Data_ArrayProperty_Vector);
		AddArrayType(TStringArray, string, CF_ModStorage_Data_ArrayProperty_String);
	}
	
	private static void AddArrayType(typename arrType, typename type, typename modStorage)
	{
		m_ArrayTypeMap.Insert(arrType, new Param2<typename, typename>(type, modStorage));
	}

	static CF_ModStorage_Data_ArrayProperty GetArrayType(typename type, out string typeStr)
	{
		if (m_ArrayTypeMap.Contains(type))
		{
			typeStr = m_ArrayTypeMap.Get(type).param1.ToString();
			return CF_ModStorage_Data_ArrayProperty.Cast(m_ArrayTypeMap.Get(type).param2.Spawn());
		}

		typeStr = "Class";
		return new CF_ModStorage_Data_ArrayProperty_Class();
	}
	
	private static typename FindType(typename type)
	{		
		for (int i = m_Types.Count() - 1; i >= 0; i--)
		{
			if (type.IsInherited(m_Types[i]) || m_Types[i] == type)
			{
				return m_Types[i];
			}
		}
		
		return Math3D;
	}
	
	private static void SortTypes()
	{
		array<typename> copy = new array<typename>();
		
		for (int i = 0; i < m_Types.Count(); i++)
		{
			int insertIdx = 0;
			for (int j = 0; j < copy.Count(); j++)
			{
				if (m_Types[i].IsInherited(copy[j]))
				{
					insertIdx = j + 1;
				}
			}
			
			if (insertIdx < copy.Count())
			{
				copy.InsertAt(m_Types[i], insertIdx);
			} else
			{
				copy.Insert(m_Types[i]);
			}
		}
		
		m_Types.Clear();
		m_Types.Copy(copy);
	}
	
	private static void AddType(typename type, typename modStorage)
	{
		m_TypeMap.Insert(type, modStorage);
		m_Types.Insert(type);
	}

	static ref CF_ModStorage_Data Create(typename type)
	{
		typename modStorageType = m_TypeMap.Get(FindType(type));
		ref CF_ModStorage_Data data = CF_ModStorage_Data.Cast(modStorageType.Spawn());
		return data;
	}

	static ref CF_ModStorage_Data Read(ParamsReadContext ctx)
	{
		Construct();
		
		string type;
		ctx.Read(type);

		ref CF_ModStorage_Data data = Create(type.ToType());
		
		if (data)
			data.Read(ctx);

		return data;
	}
};