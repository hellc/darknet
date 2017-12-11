using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using libdarknet_yolo;

namespace libdarknet.yolosharp
{
    public class Detector
    {
        private DetectorWrapper _detectorWrapper;

        public Detector()
        {
            _detectorWrapper = new DetectorWrapper();
        }

        public void Load(string cfgFileName, string weightsFileName, string namesFileName = null)
        {
            _detectorWrapper.Load(cfgFileName, weightsFileName, namesFileName);
        }

        public Detection[] DetectFromFile(string fileName)
        {
            return _detectorWrapper.DetectFromFile(fileName);
        }

        public Detection[] DetectOnImage(System.Drawing.Bitmap bitmap)
        {
            return _detectorWrapper.DetectOnImage(bitmap);
        }
        
        public Detection[] DetectOnImage(DImage image)
        {
            return _detectorWrapper.DetectOnImage(image);
        }
    }
}
