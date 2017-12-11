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

using libdarknet.yolosharp;
using System.Drawing;
using libdarknet_yolo;

namespace libdarknet.preview
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Capture _capture;
        private Detector _detector;

        public MainWindow()
        {
            InitializeComponent();
        }
        
        private void Capture_ImageGrabbed(object sender, EventArgs e)
        {
            Mat currentFrame = new Mat();
            _capture.Retrieve(currentFrame);


            Detection[] detections = new Detection[0];
            if (currentFrame != null && _detector != null)
            {
                using (System.Drawing.Bitmap bitmap = currentFrame.Clone().Bitmap)
                {
                    detections = _detector.DetectOnImage(bitmap);
                }
            }

            Dispatcher.Invoke(() =>
            {
                var image = currentFrame.ToImage<Emgu.CV.Structure.Bgr, byte>();
                using (image)
                {
                    if (detections.Count() > 0)
                    {
                        foreach (var detection in detections)
                        {
                            image.Draw(String.Format("{0}, {1}%", detection.obj_name, (int)(detection.prob * 100)), new System.Drawing.Point(detection.x, detection.y),
                                FontFace.HersheySimplex, 0.9, new Emgu.CV.Structure.Bgr(System.Drawing.Color.Blue), 2);
                            image.Draw(new System.Drawing.Rectangle((int)detection.x, (int)detection.y - 20, (int)detection.w, (int)detection.h + 20), new Emgu.CV.Structure.Bgr(System.Drawing.Color.Blue), 2);
                        }

                    }
                    _image.Source = ToBitmapSource(image);
                }
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
            _capture = new Capture(0);
            _capture.ImageGrabbed += Capture_ImageGrabbed;
            _capture.Start();

        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            _detector = new Detector();
            _detector.Load(@"data/yolo-voc.cfg", @"data/yolo-voc.weights", @"data/voc.names");
        }
    }
}
