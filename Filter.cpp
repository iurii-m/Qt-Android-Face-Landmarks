#include "Filter.h"

#include <QDebug>
#include <QPainter>
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQmlContext>
#include <QDateTime>
#include <QTemporaryFile>
#include <QImage>
#include <QPixmap>




Filter::Filter(QObject* parent) :
    QAbstractVideoFilter(parent),
    m_faceid(0), m_Orientation(0)
{

}

QVideoFilterRunnable* Filter::createFilterRunnable()
{
    return new FilterRunnable(this);
}


int Filter::faceid()
{
    qDebug()<<"Try to return Face id : "<<m_faceid;
    return m_faceid;
}

void Filter::setFaceid(int faceid)
{
    m_faceid = faceid;
    emit faceidChanged(m_faceid);
}

void Filter::mouseplusClick()
{
    qDebug() << this << "  Filter::mouseClick()";
    m_faceid++;
    qDebug()<<"Face id : "<<m_faceid;
    emit clicked();
}

void Filter::mouseminusClick()
{
    qDebug() << this << " - Filter::mouseminusClick()";
    m_faceid=m_faceid-1;
    qDebug()<<"Face id : "<<m_faceid;
    emit clicked();
}


void Filter::test()
{
    qDebug() << this << " - Filter::text()";
    emit clicked();
}

extern QImage qt_imageFromVideoFrame(const QVideoFrame& f);

FilterRunnable::FilterRunnable(Filter *parent):
    m_Filter(parent),
    m_Orientation(0),
    m_Flip(0)
{

}



//In that method we analize the input image and the handle of it executing after the next run method.
QVideoFrame FilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{

  //input - pointer to Qimage frame. Chosen Qimage format is ARGB32  4 channels.
    if (!input->isValid())
    {
        qWarning("Invalid input format");
        return *input;
    }


#ifdef Q_OS_ANDROID
    m_Flip = true;
#else
    m_Flip = surfaceFormat.scanLineDirection() == QVideoSurfaceFormat::BottomToTop;
#endif

//functionality fo frame with no handle
    if (input->handleType() == QAbstractVideoBuffer::NoHandle)
    {
        QImage img = qt_imageFromVideoFrame(*input);

       // qDebug() << Q_FUNC_INFO << "NoHandle";
        if (img.format() == QImage::Format_ARGB32)
        {
            return run(input, surfaceFormat, flags, img);
        }

        return run(input, surfaceFormat, flags, img.convertToFormat(QImage::Format_ARGB32));
    }


    if (input->handleType() == QAbstractVideoBuffer::GLTextureHandle)
    {
       //  qDebug() << Q_FUNC_INFO << "OpenGL";
        QImage img(input->width(), input->height(), QImage::Format_ARGB32);

        GLuint textureId = GLuint(input->handle().toInt());
        QOpenGLContext* ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions* f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers(1, &fbo);
        GLuint prevFbo;
        f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &prevFbo);
        f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        f->glReadPixels(0, 0, input->width(), input->height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        f->glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
        m_Flip = false;
        auto pixel = img.bits();
        for (int y = 0; y < img.height(); y++)
        {
            for (int x = 0; x < img.width(); x++)
            {
                auto T = pixel[0];
                pixel[0] = pixel[2];
                pixel[2] = T;
                pixel += 4;
            }
        }

        return run(input, surfaceFormat, flags, img);
    }

    qDebug() << Q_FUNC_INFO << "Unsupported handle type " << input->handleType();
    return *input;
}

//In that method we do all the opencv stuff.
QVideoFrame FilterRunnable::run(QVideoFrame* input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags, QImage image)
{
    Q_UNUSED(surfaceFormat)
    Q_UNUSED(flags)

   // cv::Mat cvimage(image.height(),image.width(),CV_8UC4,(uchar*)image.bits(),image.bytesPerLine());

   // qDebug() << "empty "<<m_processframe.empty();
   // set up frame processor
   if (m_processframe.empty())
   {
       dlib::shape_predictor predictor;
       cv::CascadeClassifier face_cascade;

       QFile xml(":/lbpcascade_frontalface_improved.xml");

       if(xml.exists()){qDebug() << "Cascade file exists";}
       else {qDebug() << "File does not exists";}
       if(xml.open(QFile::ReadOnly | QFile::Text))
       {QTemporaryFile temp;
           if(temp.open())
           {   temp.write(xml.readAll());
               temp.close();
               cv::CascadeClassifier tempcascade;
               if(tempcascade.load(temp.fileName().toStdString()))
               {   face_cascade = tempcascade;
                   qDebug() << "Successfully loaded classifier!";
               }
               else
               {qDebug() << "Could not load classifier."; }
           }
           else
           {qDebug() << "Can't open temp file.";}
       xml.close();
       }
       else
       { qDebug() << "Can't open XML.";  }



       QFile pred(":/face_landmarks_68.dat");
       if(pred.exists()){qDebug() << "Landmark file exists";}
       else {qDebug() << "Landmark file does not exists";}
       if(pred.open(QFile::ReadOnly))
       {QTemporaryFile temp;
           if(temp.open())
           {   temp.write(pred.readAll());
               temp.close();

               dlib::deserialize(temp.fileName().toStdString()) >> predictor;
               qDebug() << "Successfully loaded!";
           }
           else
           {qDebug() << "Can't open temp file.";}
       pred.close();
       }
       else
       { qDebug() << "Can't open landmarks.";  }

    m_processframe = ThreadProcessor(face_cascade,predictor,ImageRotation::Landscape,ImageRotation::Portrait);

    qDebug() << "empty "<<m_processframe.empty();
   }
   else {
      qDebug() << "Parameters are already loaded";
   }


    //extract result if frame calculation is finished
    // qDebug() << "check if finished"<<m_processframe.isFinished();
    if(m_processframe.isFinished())
    {
       m_faces.clear();
       m_landmarks.clear();
       m_faces = m_processframe.getFaces();
       m_landmarks = m_processframe.getLandmarks();
       m_counter = 0;
    }

    // incrementing framecounter
       m_counter++;

    // set new frame if thread is not running
    // qDebug() << "check if Running"<<m_processframe.isRunning();
    if(!m_processframe.isRunning())
    {
       m_processframe.setFrame(image);
       m_processframe.start();
    }

    qDebug() << "faces" <<m_faces.size() <<"counter " <<m_counter<<"landmark " <<m_landmarks.size();

    //Painting rectangles and landmarks
    QPainter qPainter(&image);
    qPainter.setBrush(Qt::BrushStyle::NoBrush);
    qPainter.setPen(Qt::red);

       for ( unsigned int i = 0; i < m_faces.size(); i++ )
         {
           qPainter.drawRect(m_faces.at(i).x,m_faces.at(i).y,m_faces.at(i).width,m_faces.at(i).height);
         }
    qPainter.setPen(Qt::green);
       for (unsigned int i = 0; i < m_landmarks.size(); ++i)
         {
            for (unsigned int j = 0; j < m_landmarks.at(i).size(); ++j)
                {
                qPainter.drawEllipse(m_landmarks.at(i).at(j).x,m_landmarks.at(i).at(j).y,3,3);
                }
         }

     qPainter.end();


   return !image.isNull() ? QVideoFrame(image) : *input;  // returning the handled image
}
