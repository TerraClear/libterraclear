// Tensorflow
#include <tensorflow/core/platform/env.h>
#include <tensorflow/core/public/session.h>
#include "tensorflow/cc/ops/const_op.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"
#include "tensorflow/core/framework/tensor_shape.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/lib/core/status.h"

//OPENCV 
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

//Libterraclear
#include "libterraclear/src/vision_core.hpp"

#include <iostream>

namespace tf = tensorflow;
namespace tc = terraclear;

namespace terraclear
{
    class tf_model
    {
        private:

            const cv::Size               _image_size; 
            const int                    _image_channels; 
            const int                    _confidence; 

            std::string                  _inputLayer = "image_tensor:0";
            std::vector<std::string>     _outputLayer = {"detection_boxes:0", "detection_scores:0", "detection_classes:0", "num_detections:0"};
            std::vector<tf::Tensor>      _outputs;

            std::unique_ptr<tf::Session> _f_session;

        public:

            tf_model(const cv::Size& size, const int& channels, const float& confidence): _image_size(size), _image_channels(channels), _confidence(confidence) {};
            tf::Status LoadSavedModel(const std::string& file_name);
            tf::Status matToTensor(cv::Mat& img, tf::Tensor& img_tensor);
            std::vector<tc::bounding_box> detect(cv::Mat& frame);

    };
}