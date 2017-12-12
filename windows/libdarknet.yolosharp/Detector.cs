using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using libdarknet_yolo;

namespace libdarknet.yolosharp
{
    public class Detector : IDisposable
    {
        private DetectorWrapper _detectorWrapper;
        public bool IsLoaded { get; set; }

        public Detector()
        {
            IsLoaded = false;
            _detectorWrapper = new DetectorWrapper();
        }

        public void Load(string cfgFileName, string weightsFileName)
        {
            _detectorWrapper.Load(cfgFileName, weightsFileName);
            IsLoaded = true;
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


        public Detection[] Tracking(Detection[] detections)
        {
            return _detectorWrapper.Tracking(detections);
        }

        public void Dispose()
        {
            if (IsLoaded)
            {
                _detectorWrapper.Dispose();
            }
            _detectorWrapper = null;
        }
    }
}
