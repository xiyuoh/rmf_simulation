/*
 * Copyright (C) 2022 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <string>
#include <iostream>

#include <ignition/gui/Application.hh>
#include <ignition/gui/GuiEvents.hh>
#include <ignition/gui/MainWindow.hh>
#include <ignition/gui/qt.h>
#include <ignition/gui/Plugin.hh>

#include <ignition/plugin/Register.hh>

#include <ignition/common/Image.hh>
#include <QQuickImageProvider>

#include <ignition/msgs.hh>
#include <ignition/transport.hh>


class ImageProvider : public QQuickImageProvider
{
  public: ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

  public: QImage requestImage(const QString &, QSize *, const QSize &) override
    {
      if (!this->img.isNull())
      {
        // Must return a copy
        QImage copy(this->img);
        return copy;
      }

      // Placeholder in case we have no image yet
      QImage i(400, 400, QImage::Format_RGB888);
      i.fill(QColor(128, 128, 128, 100));
      return i;
    }

  public: void SetImage(const QImage &_image)
  {
    this->img = _image;
  }

  private: QImage img;
};



class multiview_rendering : public ignition::gui::Plugin
{
  Q_OBJECT

private:
  ignition::transport::Node _node;
  ImageProvider *_provider{nullptr};
  ignition::msgs::Image _image_msg;
  std::string _topic;
  std::recursive_mutex _image_mutex;

public:
  multiview_rendering();
  ~multiview_rendering();

  virtual void LoadConfig(const tinyxml2::XMLElement* _pluginElem)
  override;

protected:
  void image_cb(const ignition::msgs::Image& _msg);
  void process_image();

signals:
  void newImage();
};

multiview_rendering::multiview_rendering() {}

multiview_rendering::~multiview_rendering()
{
  ignition::gui::App()->Engine()->removeImageProvider("test_test_123_multiview");
}

void multiview_rendering::LoadConfig(const tinyxml2::XMLElement* _pluginElem)
{
  if (!_pluginElem)
    return;

  if (this->title.empty())
    this->title = "Multiview Rendering";

  if (_pluginElem->FirstChildElement("topic"))
    _topic = _pluginElem->FirstChildElement("topic")->GetText();

  if (!_node.Subscribe(_topic, &multiview_rendering::image_cb, this))
    std::cerr << "Error subscribing to topic [/" << _topic << "]" << std::endl;

  _provider = new ImageProvider();
  ignition::gui::App()->Engine()->addImageProvider("test_test_123_multiview", _provider);
}

void multiview_rendering::image_cb(const ignition::msgs::Image& _msg)
{
  std::lock_guard<std::recursive_mutex> lock(_image_mutex);
  _image_msg = _msg;

  // Signal to the main thread that the image changed
//   QMetaObject::invokeMethod(this, "ProcessImage")  // why must QMetaObject. idgi.
  process_image();
}

void multiview_rendering::process_image()
{
  unsigned int height = _image_msg.height();
  unsigned int width = _image_msg.width();
  QImage::Format qFormat = QImage::Format_RGB888;

  QImage image = QImage(width, height, qFormat);

  ignition::common::Image output;
  switch (_image_msg.pixel_format_type())
  {
    case ignition::msgs::PixelFormatType::RGB_INT8:
      image = QImage(reinterpret_cast<const uchar *>(_image_msg.data().c_str()), width, height, qFormat);
      std::cout << ">> format is RGB_INT8" << std::endl;
      break;
    case ignition::msgs::PixelFormatType::R_FLOAT32:
      std::cout << ">> format is R_FLOAT32" << std::endl;
      break;
    case ignition::msgs::PixelFormatType::L_INT16:
      std::cout << ">> format is L_INT16" << std::endl;
      break;
    case ignition::msgs::PixelFormatType::L_INT8:
      std::cout << ">> format is L_INT8" << std::endl;
      break;
    default:
    {
      std::cerr << ">>> unsupported image type ya" << std::endl;
      return;
    }
  }

  ignition::common::Image::PixelFormatType pixel_format =
      ignition::common::Image::ConvertPixelFormat(
      ignition::msgs::ConvertPixelFormatType(
      _image_msg.pixel_format_type()));
  // if (pixel_format == ignition::common::Image::PixelFormatType::RGB_INT8)
  // {
  //   std::cout << "pixel format is rgb__int8, yay!" << std::endl;
  // } else {
  //   std::cout << "pixel format not rgb__int8 sobs" << std::endl;
  // }

  // if not rgb, copy values from common::Image to QImage
//   if (pixel_format != ignition::common::Image::PixelFormatType::RGB_INT8)
//   {
//     unsigned int output_size = 0;
//     unsigned char *data = nullptr;
//     output.Data(&data, outputSize);

//     for (unsigned int j = 0; j < height; ++j)
//     {
//       for (unsigned int i = 0; i < width; ++i)
//       {
//         unsigned int idx = j*width*3 + i*3;
//         int r = data[idx];
//         int g = data[idx + 1];
//         int b = data[idx + 2];
//         QRgb value = qTgb(r, g, b);
//         image.setPixel(i, j, value);
//       }
//     }
//     delete [] data;
//   }

  _provider->SetImage(image);
  this->newImage();

}

// Register this plugin
IGNITION_ADD_PLUGIN(multiview_rendering,
  ignition::gui::Plugin)


#include "multiview_rendering.moc"