class CF_SimpleListboxWidget : CF_BaseListboxWidget
{
	protected SimpleListboxWidget _SimpleListboxWidget;

	override void OnWidgetScriptInit(Widget w)
	{
		#ifdef CF_TRACE_ENABLED
		auto trace = CF_Trace_1(this, "OnWidgetScriptInit").Add(w);
		#endif

		super.OnWidgetScriptInit(w);
		Class.CastTo(_SimpleListboxWidget, w);
	}
};