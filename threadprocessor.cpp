#include "threadprocessor.h"
#include <QDebug>


cv::Rect dlibRectToOpenCVRect(dlib::rectangle r)
{
  return cv::Rect(cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1));
}

dlib::rectangle openCVRectToDlibRect(cv::Rect r)
{
  return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}

void convert(const QImage& src, dlib::matrix<dlib::rgb_pixel>& dst) {
    dst.set_size(src.height(), src.width());
    auto  dstIt = dst.begin();
    int   i     = 0;
    QRgb* st    = (QRgb*)src.bits();
    while (dstIt != dst.end() && i < src.width() * src.height()) {
        const auto& str = st[i];
        dstIt->blue     = qBlue(str);
        dstIt->green    = qGreen(str);
        dstIt->red      = qRed(str);
        ++dstIt;
        ++i;
    }
}

void convert(const QPixmap& src, dlib::matrix<dlib::rgb_pixel>& dst) {

    convert(src.toImage(), dst);
}



ThreadProcessor::ThreadProcessor()
{

}

ThreadProcessor::ThreadProcessor(cv::CascadeClassifier cascade, dlib::shape_predictor model, ImageRotation originalRotation, ImageRotation requiredRotation)
{
    _originalRotation = originalRotation;
    _requiredRotation = requiredRotation;
    _face_cascade = cascade;
    _predictor = model;
    _empty = false;
    return;
}

ThreadProcessor::ThreadProcessor(const ThreadProcessor &t)
{
    //qDebug()<<"Copy constructor called "<<endl;
    _originalRotation = t._originalRotation;
    _requiredRotation = t._requiredRotation;
    _face_cascade = t._face_cascade;
    _predictor =t._predictor;
    _empty = false;
    return;
}

// Setting up input frame
void ThreadProcessor::setFrame(QImage inputImage)
{
    _inputImage = inputImage;
    return;
}

cv::CascadeClassifier ThreadProcessor::getCascade()
{
    return _face_cascade;
}

dlib::shape_predictor ThreadProcessor::getShapePredictor()
{
    return _predictor;
}

std::vector<cv::Rect> ThreadProcessor::getFaces()
{
    return _faces;
}

std::vector<std::vector<cv::Point2f>> ThreadProcessor::getLandmarks()
{
    return _landmarks;
}

bool ThreadProcessor::empty()
{
    return _empty;
}

void ThreadProcessor::run()
{

    cv::Mat cvimage(_inputImage.height(),_inputImage.width(),CV_8UC4,(uchar*)_inputImage.bits(),_inputImage.bytesPerLine());

    //going to grayscale and resizing
    Mat cvimagegray;
    int featureresize = 3;
    cvtColor(cvimage,cvimagegray,COLOR_BGRA2GRAY);
    resize(cvimagegray,cvimagegray,Size(int(cvimagegray.cols/featureresize),int(cvimagegray.rows/featureresize)),0,0,INTER_LINEAR);

    //faces for opencv and dlib
    std::vector<Rect> faces;
    std::vector<Rect> realfaces;
    std::vector<dlib::rectangle> dlibfaces;
    std::vector<std::vector<cv::Point2f>> landmarks;
    //face deecting with opencv cascade classifier
    if(_face_cascade.empty()){
        qDebug() << "empty cascade";
        return;}
    _face_cascade.detectMultiScale(cvimagegray,faces,1.1,3);

        for ( unsigned int i = 0; i < faces.size(); i++ )
            {
              //faces in real scale
              Rect realface = Rect(faces[i].x*featureresize,faces[i].y*featureresize,(faces[i].width)*featureresize,int(faces[i].height)*featureresize);
              realfaces.push_back(realface);
              //same rects for dlib
              dlibfaces.push_back(openCVRectToDlibRect(realface))  ;

              //dlib image image
              dlib::matrix<dlib::rgb_pixel> cimg;
              convert(_inputImage,cimg);

              //detect features with dlib
              dlib::full_object_detection shape = _predictor(cimg, dlibfaces[i]);

              std::vector<cv::Point2f> features;
              //transform features to opencv format
              for (unsigned int i = 0; i < 68; ++i)
                  {
                    features.push_back(cv::Point2f(shape.part(i).x(), shape.part(i).y()));
                  }

              landmarks.push_back(features);

            }
        _landmarks = landmarks;
        _faces = realfaces;

      return;
}
