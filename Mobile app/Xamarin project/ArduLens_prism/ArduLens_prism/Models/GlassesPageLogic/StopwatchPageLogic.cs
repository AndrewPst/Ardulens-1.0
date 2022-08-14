using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.Models.GlassesPageLogic
{
    public class StopwatchPageLogic : GlassesPageLogicBase
    {
        public override string Name => "Stopwatch page";

        public override string Key => "swatch_p";

        public override IList<string> OnShow()
        {
            return null;
        }
    }
}
