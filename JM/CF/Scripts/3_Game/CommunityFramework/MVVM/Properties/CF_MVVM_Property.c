class CF_MVVM_Property : CF_TypeConverter
{
	protected CF_ModelBase m_Model;
	protected CF_ViewModel m_ViewModel;

	protected CF_ObservableCollection m_Collection;
	protected bool m_IsCollection;

	protected string m_Name;
	protected string m_VariableName;

	protected string m_TypeConverterType;
	protected ref CF_TypeConverter m_TypeConverter;

	void CF_MVVM_Property(CF_ViewModel handler, string name)
	{
		CF_Trace trace(this, "CF_MVVM_Property", "" + handler, name);

		m_ViewModel = handler;
		m_Name = name;
	}

	string SetVariableName(string variableName)
	{
		CF_Trace trace(this, "SetVariableName", "" + variableName);

		int colonSeperator = variableName.IndexOf(":");
		if (colonSeperator == -1)
		{
			m_VariableName = variableName;
			return m_VariableName;
		}

		m_VariableName = variableName.Substring(0, colonSeperator);
		m_TypeConverterType = variableName.Substring(colonSeperator + 1, variableName.Length() - colonSeperator - 1);

		return m_VariableName;
	}

	string GetVariableName()
	{
		return m_VariableName;
	}

	string GetName()
	{
		return m_Name;
	}

	string ToStr()
	{
		string str = "[" + ClassName() + "] ";
		str += "Name=" + m_Name;
		str += ", ";
		str += "VariableName=" + m_VariableName;
		str += ", ";
		str += "IsCollection=" + m_IsCollection;
		str += ", ";
		str += "Collection=" + m_Collection;
		str += ", ";
		str += "Model=" + m_Model;
		str += ", ";
		str += "ViewModel=" + m_ViewModel;
		str += ", ";
		str += "TypeConverterType=" + m_TypeConverterType;
		return str;
	}

	void Link(CF_ModelBase model, typename variableType, CF_TypeConverter typeConverter)
	{
		CF_Trace trace(this, "Assign", "" + model);

		m_Model = model;

		m_IsCollection = variableType.IsInherited(CF_ObservableCollection);
		if (m_IsCollection)
		{
			OnModel(new CF_EventArgs());

			return;
		}

		m_TypeConverter = typeConverter;

		CF_TypeConverter typeConverterOverride;
		if (m_TypeConverterType != string.Empty && Class.CastTo(typeConverterOverride, m_TypeConverterType.ToType().Spawn()))
		{
			if (typeConverterOverride.IsInherited(m_TypeConverter.Type()))
			{
				m_TypeConverter = typeConverterOverride;
			}
			else
			{
				CF_Log.Warn("Overriding type converter '%1' doesn't inherit from '%2'. Using '%2'.", typeConverterOverride.ToStr(), m_TypeConverter.ToStr());
			}
		}

		EnScript.SetClassVar(m_ViewModel, "_" + m_Name, 0, m_TypeConverter);

		OnModel(new CF_EventArgs());
	}
	
	void OnView(/*notnull*/ CF_EventArgs evt)
	{
		CF_Trace trace(this, "OnView", evt.ToStr());

		Param param = new Param2<CF_ModelBase, CF_EventArgs>(m_Model, evt);
		g_Script.CallFunctionParams(m_ViewModel, "OnView_" + m_Name, null, param);
	}

	void OnModel(/*notnull*/ CF_EventArgs evt)
	{
		CF_Trace trace(this, "OnModel", evt.ToStr());

		Print(ToStr());

		CF_EventArgs eventOverride = evt;

		if (m_IsCollection && !evt.Type().IsInherited(CF_CollectionEventArgs))
		{
			CF_ObservableCollection _collection;
			EnScript.GetClassVar(m_Model, m_VariableName, 0, _collection);
			if (_collection == m_Collection) return;

			// the previous instance may still be alive, if so, make sure it doesn't notify from now on
			if (m_Collection)
			{
				m_Collection.Init(null);
			}

			if (_collection)
			{
				_collection.Init(m_Model, m_VariableName);

				CF_TypeConverter typeConverter = _collection.GetConverter();
				if (!typeConverter)
				{
					CF_Log.Error("Collection '%1.%2' has no assigned type converter!", "" + m_Model, m_VariableName);
					return;
				}

				CF_TypeConverter replacingTypeConverter;
				if (m_TypeConverterType != string.Empty && Class.CastTo(replacingTypeConverter, m_TypeConverterType.ToType().Spawn()))
				{
					if (replacingTypeConverter.IsInherited(typeConverter.Type()))
					{
						_collection.OverrideConverter(replacingTypeConverter);
					}
					else
					{
						CF_Log.Warn("Overriding type converter '%1' doesn't inherit from '%2'. Using '%2'.", replacingTypeConverter.ToStr(), typeConverter.ToStr());
					}
				}
			}

			m_Collection = _collection;

			// The variable is no longer set to an instance, override event to reset UI
			if (!m_Collection) eventOverride = new CF_CollectionClearEventArgs();
		}

		Param param = new Param2<CF_ModelBase, CF_EventArgs>(m_Model, eventOverride);
		g_Script.CallFunctionParams(m_ViewModel, "OnModel_" + m_Name, null, param);
	}
};