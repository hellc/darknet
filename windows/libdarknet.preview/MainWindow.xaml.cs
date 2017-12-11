using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using Emgu.CV;

using System.Runtime.InteropServices;
using Emgu.CV.CvEnum;

namespace libdarknet.preview
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Capture _capture;

        public MainWindow()
        {
            InitializeComponent();
        }
        
        private void Capture_ImageGrabbed(object sender, EventArgs e)
        {
            Mat currentFrame = new Mat();
            _capture.Retrieve(currentFrame);
            
            Dispatcher.Invoke(() =>
            {
                _image.Source = ToBitmapSource(currentFrame.ToImage<Emgu.CV.Structure.Bgr, byte>());
            });
        }

        [DllImport("gdi32")]
        private static extern int DeleteObject(IntPtr o);
        private static BitmapSource ToBitmapSource(IImage image)
        {
            using (System.Drawing.Bitmap source = image.Bitmap)
            {
                IntPtr ptr = source.GetHbitmap(); //obtain the Hbitmap

                BitmapSource bs = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(
                    ptr,
                    IntPtr.Zero,
                    Int32Rect.Empty,
                    System.Windows.Media.Imaging.BitmapSizeOptions.FromEmptyOptions());

                DeleteObject(ptr); //release the HBitmap
                return bs;
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            _capture = new Capture(@"rtsp://technicom:CAMx4tech_1@147.232.79.94/axis-media/media.amp");
            _capture.ImageGrabbed += Capture_ImageGrabbed;
            _capture.Start();
        }
    }
}
