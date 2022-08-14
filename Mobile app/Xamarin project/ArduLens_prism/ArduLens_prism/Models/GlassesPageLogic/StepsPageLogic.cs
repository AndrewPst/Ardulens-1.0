using System;
using System.Collections.Generic;
using System.Text;

namespace ArduLens_prism.Models.GlassesPageLogic
{
    public class StepsPageLogic : GlassesPageLogicBase
    {
        public override string Name => "Steps page";

        public override string Key => "steps_p";

        public override IList<string> OnShow()
        {
            return null;
        }
    }
}
