#ifndef FaceFilter_H
#define FaceFilter_H

#include <QAbstractVideoFilter>


#include <QVideoFilterRunnable>
#include <QVideoFrame>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

#include "threadprocessor.h"

using namespace std;
using namespace cv;


// In this class realised signals and slots for сommunicating with qml
class Filter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(int orientation MEMBER m_Orientation)
    Q_PROPERTY(int faceid READ faceid WRITE setFaceid NOTIFY faceidChanged)

private:
    friend class FilterRunnable;


public:

    Filter(QObject* parent = nullptr);

    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;

    int faceid();
    void setFaceid(int);


    Q_INVOKABLE void mouseplusClick();
    Q_INVOKABLE void mouseminusClick();

signals:
    void faceidChanged(int faceid);
    void clicked();

public slots:
    void test();


protected:
    int m_faceid;
    int m_Orientation;
};


//In this class all opencv functionality is realised
class FilterRunnable : public QVideoFilterRunnable
{

public:

    FilterRunnable(Filter* parent = nullptr);

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) Q_DECL_OVERRIDE;
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags, QImage image);

private:

    ThreadProcessor m_processframe;

    //output parameters
    std::vector<cv::Rect> m_faces;
    std::vector<std::vector<cv::Point2f>> m_landmarks;

    Filter* m_Filter;
    int m_Orientation;
    int m_Flip;
    int m_counter = 0;
};


#endif
