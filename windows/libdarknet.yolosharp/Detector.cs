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

        public void Load(string cfgFileName, string weightsFileName)
        {
            _detectorWrapper.Load(cfgFileName, weightsFileName);
        }

        public Detection[] DetectFromFile(string fileName)
        {
            return _detectorWrapper.DetectFromFile(fileName);
        }
    }
}
