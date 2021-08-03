class CF_CanvasLine : CF_ModelBase
{
	float x1;
	float y1;
	float x2;
	float y2;
	float width;
	int color;
};

class CF_CanvasWidget : CF_Widget
{
	reference string Lines;

	protected CanvasWidget _CanvasWidget;
	protected ref CF_ObservableCollection _Lines;

	override void GetProperties()
	{
		CF_Trace trace(this, "GetProperties");

		super.GetProperties();
		
		AddProperty(Lines, "Lines");
	}

	override void OnWidgetScriptInit(Widget w)
	{
		CF_Trace trace(this, "OnWidgetScriptInit", "" + w);

		super.OnWidgetScriptInit(w);
		Class.CastTo(_CanvasWidget, w);
	}

	void OnView_Lines(CF_ModelBase sender, CF_EventArgs args)
	{
		CF_Trace trace(this, "OnView_Lines", "" + sender, args.ToStr());
		
		EnScript.SetClassVar(m_Model, Lines, 0, _Lines);
	}

	//! Automatically updated from Collection events so 'NotifyProperty' isn't needed to be called.
	void OnModel_Lines(CF_ModelBase sender, CF_EventArgs args)
	{
		CF_Trace trace(this, "OnModel_Lines", "" + sender, args.ToStr());
		
		EnScript.GetClassVar(m_Model, Lines, 0, _Lines);

		_CanvasWidget.Clear();
		for (int i = 0; i < _Lines.Count(); i++)
		{
			CF_CanvasLine line;
			if (!Class.CastTo(line, _Lines.GetConverter(i).GetClass())) break;
			_CanvasWidget.DrawLine(line.x1, line.y1, line.x2, line.y2, line.width, line.color);
		}
	}
};