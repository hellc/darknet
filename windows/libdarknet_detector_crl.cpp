#include "libdarknet_detector_crl.hpp"

std::string convert_system_string(String^ input)
{
	std::string result;
	using namespace Runtime::InteropServices;
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(input)).ToPointer();
	result = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));

	return result;
}

void libdarknet_yolo::DetectorWrapper::Load(String ^ cfg_filename, String ^ weights_filename)
{
	m_detector->Load(convert_system_string(cfg_filename), convert_system_string(weights_filename));
}

array<libdarknet_yolo::Detection>^ libdarknet_yolo::DetectorWrapper::DetectFromFile(String ^ filename)
{
	std::vector<bbox_t> results = m_detector->detect(convert_system_string(filename));

	array<Detection>^ detections = gcnew array<Detection>(results.size());
	for (auto i = 0; i != results.size(); i++) {
		detections[i].h = results[i].h;
		detections[i].w = results[i].w;
		detections[i].x = results[i].x;
		detections[i].y = results[i].y;
		detections[i].obj_id = results[i].obj_id;
		detections[i].prob = results[i].prob;
		detections[i].track_id = results[i].track_id;
	}
	return detections;
}
