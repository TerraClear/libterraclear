#include <tensorflow/core/platform/env.h>
#include <tensorflow/core/public/session.h>

#include <iostream>

namespace tf = tensorflow;
namespace terraclear
{
    class tf_model
    {
        private:
            /*
            tf::Tensor _tensor = tf::Tensor(tf::DT_FLOAT, shape);
            std::vector<std::string> _outputLayer = 
                {"detection_boxes:0", "detection_scores:0", "detection_classes:0", "num_detections:0"};
            std::vector<tf::Tensor> _outputs
            */
            std::unique_ptr<tf::Session> _session;
        public:

            tf_model();
            // Load graph from file
            tf_model(const std::string &path_to_graph);
            
            // Destructor
            ~tf_model();
            /*
            // Get classes
            tf::Status readLabelsMapFile(const std::string &fileName);
            // Convert cv::Mat to tf::Tensor
            tf::Status matToTensor(const cv::Mat &mat, tf::Tensor &outTensor)
            // Run detection on a cv::Mat
            tf::Status detect(cv::Mat);
            
            std::map<int, std::string> _labelsMap;
            */
    };
}