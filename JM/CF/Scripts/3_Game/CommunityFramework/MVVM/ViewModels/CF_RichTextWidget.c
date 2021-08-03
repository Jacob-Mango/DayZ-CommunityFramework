class CF_RichTextWidget : CF_TextWidget
{
	reference string ContentHeight;
	reference string ContentOffset;
	reference string ContentSnapToLine;
	reference string LineWidths;

	protected RichTextWidget _RichTextWidget;
	protected ref CF_TypeConverter _ContentHeight = CF_GetDefaultTC();
	protected ref CF_TypeConverter _ContentOffset = CF_GetDefaultTC();
	protected ref CF_TypeConverter _ContentSnapToLine = CF_GetDefaultTC();
	protected ref CF_TypeConverter _LineWidths = CF_GetDefaultTC();

	override void GetProperties()
	{
		CF_Trace trace(this, "GetProperties");

		super.GetProperties();
		
		AddProperty(ContentHeight, "ContentHeight");
		AddProperty(ContentOffset, "ContentOffset");
		AddProperty(ContentSnapToLine, "ContentSnapToLine");
		AddProperty(LineWidths, "LineWidths");
	}

	override void OnWidgetScriptInit(Widget w)
	{
		CF_Trace trace(this, "OnWidgetScriptInit", "" + w);

		super.OnWidgetScriptInit(w);
		Class.CastTo(_RichTextWidget, w);
	}

	void OnView_ContentHeight(CF_ModelBase sender, CF_EventArgs args)
	{
		CF_Trace trace(this, "OnView_ContentHeight", "" + sender, args.ToStr());

		EnScript.SetClassVar(m_Model, ContentHeight, 0, _RichTextWidget.GetContentHeight());
	}

	void OnModel_ContentHeight(CF_ModelBase sender, CF_EventArgs args)
	{
		CF_Trace trace(this, "OnModel_ContentHeight", "" + sender, args.ToStr());

		OnView_ContentHeight(m_Model, args);
	}

	void OnView_ContentOffset(CF_ModelBase sender, CF_EventArgs args)
	{
		CF_Trace trace(this, "OnView_ContentOffset", "" + sender, args.ToStr());

		float _value = _RichTextWidget.GetContentOffset();
		EnScript.SetClassVar(m_Model, ContentOffset, 0, _value);
	}

	void OnModel_ContentOffset(CF_ModelBase sender, CF_EventArgs args)
	{
		CF_Trace trace(this, "OnModel_ContentOffset", "" + sender, args.ToStr());

		float _value;
		EnScript.GetClassVar(m_Model, ContentOffset, 0, _value);
		//_RichTextWidget.SetContentOffset(_value, _ContentSnapToLine);
	}

	void OnView_ContentSnapToLine(CF_ModelBase sender, CF_EventArgs args)
	{
		CF_Trace trace(this, "OnView_ContentSnapToLine", "" + sender, args.ToStr());

		EnScript.SetClassVar(m_Model, ContentSnapToLine, 0, _ContentSnapToLine);
	}

	void OnModel_ContentSnapToLine(CF_ModelBase sender, CF_EventArgs args)
	{
		CF_Trace trace(this, "OnModel_ContentSnapToLine", "" + sender, args.ToStr());

		EnScript.GetClassVar(m_Model, ContentSnapToLine, 0, _ContentSnapToLine);
		//_RichTextWidget.SetContentOffset(_RichTextWidget.GetContentOffset(), _ContentSnapToLine);
	}

	void OnView_LineWidths(CF_ModelBase sender, CF_EventArgs args)
	{
		CF_Trace trace(this, "OnView_LineWidths", "" + sender, args.ToStr());

		array<float> _value;
		EnScript.GetClassVar(m_Model, LineWidths, 0, _value);

		_value.Clear();
		for (int i = 0; i < _RichTextWidget.GetNumLines(); i++)
		{
			_value.Insert(_RichTextWidget.GetLineWidth(i));
		}
	}

	void OnModel_LineWidths(CF_ModelBase sender, CF_EventArgs args)
	{
		CF_Trace trace(this, "OnModel_LineWidths", "" + sender, args.ToStr());

		OnView_LineWidths(m_Model, args);
	}
};