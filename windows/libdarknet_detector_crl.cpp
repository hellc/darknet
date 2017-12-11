#include "libdarknet_detector_crl.hpp"

std::vector<std::string> m_names;

std::string convert_system_string(System::String^ input)
{
	std::string result;
	using namespace Runtime::InteropServices;
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(input)).ToPointer();
	result = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));

	return result;
}

String^ convert_std_string(std::string str)
{
	return gcnew String(str.c_str());;
}

std::vector<std::string> objects_names_from_file(std::string const filename) {
	std::ifstream file(filename);
	std::vector<std::string> file_lines;
	if (!file.is_open()) return file_lines;
	for (std::string line; file >> line;) file_lines.push_back(line);
	std::cout << "object names loaded \n";
	return file_lines;
}


std::string object_name_by_index(int index) {
	if (index >= m_names.size()) return "";
	return m_names[index];
}

array<libdarknet_yolo::Detection>^ detections_from_vector(std::vector<bbox_t> vector)
{
	int vector_size = vector.size();
	array<libdarknet_yolo::Detection>^ detections = gcnew array<libdarknet_yolo::Detection>(vector_size);

	if (vector_size > 0)
	{
		for (auto i = 0; i != vector_size; i++) {
			detections[i].h = vector[i].h;
			detections[i].w = vector[i].w;
			detections[i].x = vector[i].x;
			detections[i].y = vector[i].y;
			detections[i].obj_id = vector[i].obj_id;
			detections[i].prob = vector[i].prob;
			detections[i].track_id = vector[i].track_id;

			int names_count = m_names.size();
			if (names_count > 0 && vector[i].obj_id < names_count && vector[i].obj_id)
			{
				detections[i].obj_name = convert_std_string(object_name_by_index(vector[i].obj_id));
			}
		}
	}
	
	return detections;
}

void libdarknet_yolo::DetectorWrapper::Load(System::String ^ cfg_filename, System::String ^ weights_filename)
{
	m_detector->Load(convert_system_string(cfg_filename), convert_system_string(weights_filename));
}

void libdarknet_yolo::DetectorWrapper::Load(String ^ cfg_filename, String ^ weights_filename, String ^ names_filename)
{
	m_names = objects_names_from_file(convert_system_string(names_filename));
	m_detector->Load(convert_system_string(cfg_filename), convert_system_string(weights_filename));
}

array<libdarknet_yolo::Detection>^ libdarknet_yolo::DetectorWrapper::DetectFromFile(System::String ^ filename)
{
	std::vector<bbox_t> results = m_detector->detect(convert_system_string(filename));

	return detections_from_vector(results);
}

image_t BitmapToImage_t(System::Drawing::Bitmap^ bitmap)
{
	image_t result;
	IplImage* tmp = new IplImage();

	System::Drawing::Imaging::BitmapData^ bmData = bitmap->LockBits(System::Drawing::Rectangle(0, 0, bitmap->Width, bitmap->Height), System::Drawing::Imaging::ImageLockMode::ReadWrite, bitmap->PixelFormat);
	if (bitmap->PixelFormat == System::Drawing::Imaging::PixelFormat::Format8bppIndexed)
	{
		tmp = cvCreateImage(cvSize(bitmap->Width, bitmap->Height), IPL_DEPTH_8U, 1);
		tmp->imageData = (char*)bmData->Scan0.ToPointer();

		result.data = (float*)bmData->Scan0.ToPointer();
		result.c = 1;
	}

	else if (bitmap->PixelFormat == System::Drawing::Imaging::PixelFormat::Format24bppRgb)
	{
		tmp = cvCreateImage(cvSize(bitmap->Width, bitmap->Height), IPL_DEPTH_8U, 3);
		tmp->imageData = (char*)bmData->Scan0.ToPointer();

		result.data = (float*)bmData->Scan0.ToPointer();
		result.c = 3;
	}

	bitmap->UnlockBits(bmData);

	result.h = tmp->height;
	result.w = tmp->width;

	return result;
}

cv::Mat BitmapToMat(System::Drawing::Bitmap^ bitmap)
{
	System::Drawing::Rectangle blank = System::Drawing::Rectangle(0, 0, bitmap->Width, bitmap->Height);
	System::Drawing::Imaging::BitmapData^ bmpdata = bitmap->LockBits(blank, System::Drawing::Imaging::ImageLockMode::ReadWrite, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
	cv::Mat thisimage(cv::Size(bitmap->Width, bitmap->Height), CV_8UC3, bmpdata->Scan0.ToPointer(), cv::Mat::AUTO_STEP);

	return thisimage;

	bitmap->UnlockBits(bmpdata);

	//cv::Mat result;
	//IplImage* tmp = new IplImage();

	//System::Drawing::Imaging::BitmapData^ bmData = bitmap->LockBits(System::Drawing::Rectangle(0, 0, bitmap->Width, bitmap->Height), System::Drawing::Imaging::ImageLockMode::ReadWrite, bitmap->PixelFormat);
	//if (bitmap->PixelFormat == System::Drawing::Imaging::PixelFormat::Format8bppIndexed)
	//{
	//	tmp = cvCreateImage(cvSize(bitmap->Width, bitmap->Height), IPL_DEPTH_8U, 1);
	//	tmp->imageData = (char*)bmData->Scan0.ToPointer();
	//}

	//else if (bitmap->PixelFormat == System::Drawing::Imaging::PixelFormat::Format24bppRgb)
	//{
	//	tmp = cvCreateImage(cvSize(bitmap->Width, bitmap->Height), IPL_DEPTH_8U, 3);
	//	tmp->imageData = (char*)bmData->Scan0.ToPointer();
	//}

	//bitmap->UnlockBits(bmData);

	//result = cv::Mat(cv::Size(tmp->width, tmp->height), 0, tmp->imageData, tmp->widthStep);

	//return result;
}

array<libdarknet_yolo::Detection>^ libdarknet_yolo::DetectorWrapper::DetectOnImage(System::Drawing::Bitmap^ bitmap)
{
	cv::Mat image = BitmapToMat(bitmap);
	std::vector<bbox_t> results = m_detector->detect(image);

	return detections_from_vector(results);
}

array<libdarknet_yolo::Detection>^ libdarknet_yolo::DetectorWrapper::DetectOnImage(DImage image)
{
	image_t imaget;
	imaget.c = image.c;
	imaget.data = image.data;
	imaget.h = image.h;
	imaget.w = image.w;

	std::vector<bbox_t> results = m_detector->detect(imaget);

	return detections_from_vector(results);
}
