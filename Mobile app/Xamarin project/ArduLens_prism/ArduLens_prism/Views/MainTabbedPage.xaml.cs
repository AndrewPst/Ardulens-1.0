using Xamarin.Forms;

namespace ArduLens_prism.Views
{
    public partial class MainTabbedPage : TabbedPage
    {
        public MainTabbedPage()
        {
            InitializeComponent();
            BarBackgroundColor = Color.Transparent;
        }

        protected override bool OnBackButtonPressed()
        {
            return true;
        }
    }
}
