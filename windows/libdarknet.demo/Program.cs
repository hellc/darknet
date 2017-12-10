using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using libdarknet.yolosharp;
using libdarknet_yolo;

namespace libdarknet.demo
{
    class Program
    {
        static void Main(string[] args)
        {
            Detector detector = new Detector();
            detector.Load(@"data/yolo-voc.cfg", @"data/yolo-voc.weights");
            Console.Clear();
            Console.WriteLine("filename://");
            string fileName = Console.ReadLine();
            Detection[] detections = detector.DetectFromFile(fileName);
            foreach (var detection in detections)
            {
                Console.WriteLine("id:{0}, x:{1}, y:{2}, h:{3}, w:{4}, prob:{5}", detection.obj_id, detection.x, detection.y, detection.h, detection.w, detection.prob);
            }
            Console.ReadKey();
        }
    }
}
