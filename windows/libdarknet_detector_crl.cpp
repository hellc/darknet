#include "libdarknet_detector_crl.hpp"

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

void libdarknet_yolo::DetectorWrapper::Load(System::String ^ cfg_filename, System::String ^ weights_filename)
{
	m_detector->Load(convert_system_string(cfg_filename), convert_system_string(weights_filename));
}

array<libdarknet_yolo::Detection>^ libdarknet_yolo::DetectorWrapper::DetectFromFile(System::String ^ filename)
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

array<libdarknet_yolo::Detection>^ libdarknet_yolo::DetectorWrapper::DetectOnImage(DImage image)
{
	image_t imaget;
	imaget.c = image.c;
	imaget.data = image.data;
	imaget.h = image.h;
	imaget.w = image.w;

	std::vector<bbox_t> results = m_detector->detect(imaget);

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
