#ifndef THREADPROCESSOR_H
#define THREADPROCESSOR_H

#include <QThread>
#include <QDebug>
#include <QImage>
#include <QPixmap>


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

using namespace std;
using namespace cv;

enum ImageRotation{
// portrait orientation
Portrait = 0,
// 90 landscape orientation
LandscapeInverted = 90,
// 180 portrait inverted orientation
PortraitInterted = 180,
// 270 landscape orientation
Landscape = 270
};

class ThreadProcessor:public QThread
{
public:


    //default constructor
    explicit ThreadProcessor();

    //constructor
    explicit ThreadProcessor(cv::CascadeClassifier cascade, dlib::shape_predictor model, ImageRotation originalRotation, ImageRotation requiredRotation);

    //copy constructor
    ThreadProcessor(const ThreadProcessor &t);

    //Assignment operator
    ThreadProcessor& operator = (const ThreadProcessor &t)
        {
            //qDebug()<<"Assignment operator called "<<endl;
            this->_empty = false;
            this->_face_cascade = t._face_cascade;
            this->_predictor = t._predictor;           
            return *this;
        }

    // Process frame
    void run();

    // Setting up input frame
    void setFrame(QImage inputImage);

    //get parameters
    cv::CascadeClassifier getCascade();
    dlib::shape_predictor getShapePredictor();

    //get outputs
    std::vector<cv::Rect> getFaces();
    std::vector<std::vector<cv::Point2f>> getLandmarks();

    //check if thread processor is empty
    bool empty();

private:
    //rotation of original image
    ImageRotation _originalRotation = ImageRotation::Landscape;
    //rotation that is neede to be handled
    ImageRotation _requiredRotation = ImageRotation::Portrait;


    //if empty variable
    bool _empty = true;

    //set up parameters
    dlib::shape_predictor _predictor;
    cv::CascadeClassifier _face_cascade;

    //input parameters
    QImage _inputImage;

    //output parameters
    std::vector<cv::Rect> _faces;
    std::vector<std::vector<cv::Point2f>> _landmarks;

};

#endif // THREADPROCESSOR_H
