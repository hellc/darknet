//CRL WRAPPER

#include "libdarknet_detector.hpp"

using namespace System;

namespace libdarknet_yolo {

	public value struct Detection
	{
		unsigned int x, y, w, h;	// (x,y) - top-left corner, (w, h) - width & height of bounded box
		float prob;					// confidence - probability that the object was found correctly
		unsigned int obj_id;		// class of object - from range [0, classes-1]
		unsigned int track_id;		// tracking id for video (0 - untracked, 1 - inf - tracked object)
	};

	public ref class DetectorWrapper
	{
		Detector* m_detector;
	public:
		DetectorWrapper() { m_detector = new Detector(); }
		~DetectorWrapper() { this->!DetectorWrapper(); }
		!DetectorWrapper() { delete m_detector; }

		void Load(String^ cfg_filename, String^ weights_filename);
		array<Detection>^ DetectFromFile(String^ filename);
	};
}
