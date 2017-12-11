using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using libdarknet.yolosharp;
using libdarknet_yolo;
using System.Drawing.Imaging;
using System.Drawing;
using System.Runtime.InteropServices;

namespace libdarknet.demo
{
    class Program
    {
        static void Main(string[] args)
        {
            Detector detector = new Detector();
            detector.Load(@"data/yolo-voc.cfg", @"data/yolo-voc.weights");
            
            //Console.Clear();
            //Console.WriteLine("filename://");
            //string fileName = Console.ReadLine();
            //Detection[] detections = detector.DetectFromFile(fileName);
            //foreach (var detection in detections)
            //{
            //    Console.WriteLine("id:{0}, x:{1}, y:{2}, h:{3}, w:{4}, prob:{5}\n", detection.obj_id, detection.x, detection.y, detection.h, detection.w, detection.prob);
            //}
            //Console.ReadKey();

            Console.Clear();
            Console.WriteLine("opencv test\n filename://");
            string fileName2 = "person.jpg";//Console.ReadLine();
            
            System.Drawing.Image image = System.Drawing.Image.FromFile(fileName2);
            System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(image);

            Console.WriteLine("w:{0}, h:{1}, c:{2}\n", bitmap.Width, bitmap.Height, bitmap.GetHbitmap());
            Console.ReadKey();

            Detection[] detections = detector.DetectOnImage(bitmap);

            foreach (var detection in detections)
            {
                Console.WriteLine("id:{0}, x:{1}, y:{2}, h:{3}, w:{4}, prob:{5}\n", detection.obj_id, detection.x, detection.y, detection.h, detection.w, detection.prob);
            }
            Console.ReadKey();
        }
    }
}
