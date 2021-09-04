class CF_ProgressBarWidget : CF_SimpleProgressBarWidget
{
	protected ProgressBarWidget _ProgressBarWidget;

	override void OnWidgetScriptInit(Widget w)
	{
		#ifdef CF_TRACE_ENABLED
		auto trace = CF_Trace_1(this, "OnWidgetScriptInit").Add(w);
		#endif

		super.OnWidgetScriptInit(w);
		Class.CastTo(_ProgressBarWidget, w);
	}
};
