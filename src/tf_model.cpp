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

#include "tensorflow/cc/client/client_session.h"

#include "tf_model.hpp"

namespace  terraclear
{
    tf::Status tf_model::LoadSavedModel(const std::string& path_to_graphDef)
    {
        tf::GraphDef graph_def;
        tf::SessionOptions options;

        _f_session.reset(tf::NewSession(options));
        TF_CHECK_OK(tf::ReadBinaryProto(tf::Env::Default(), path_to_graphDef, &graph_def));
        tf::Status load_graph_status = _f_session->Create(graph_def);
        if (!load_graph_status.ok())
            std::cerr << "Error loading graph "  << std::endl;
        return load_graph_status;
    }

    tf::Status tf_model::matToTensor(cv::Mat &mat, tf::Tensor &outTensor) 
    {
        auto root = tensorflow::Scope::NewRootScope();
        using namespace ::tensorflow::ops;

        // Trick from https://github.com/tensorflow/tensorflow/issues/8033
        float *p = outTensor.flat<float>().data();

        cv::Mat fakeMat(mat.rows, mat.cols, CV_32FC3, p);

        mat.convertTo(fakeMat, CV_32FC3);

        auto input_tensor = Placeholder(root.WithOpName("input"), tf::DT_FLOAT);
        std::vector<std::pair<std::string, tf::Tensor>> inputs = {{"input", outTensor}};
        auto uint8Caster = Cast(root.WithOpName("uint8_Cast"), outTensor, tf::DT_UINT8);

        // This runs the GraphDef network definition that we've just constructed, and
        // returns the results in the output outTensor.
        tf::GraphDef graph;
        TF_RETURN_IF_ERROR(root.ToGraphDef(&graph));

        std::vector<tf::Tensor> outTensors;
        std::unique_ptr<tf::Session> session(tf::NewSession(tf::SessionOptions()));

        TF_RETURN_IF_ERROR(session->Create(graph));
        TF_RETURN_IF_ERROR(session->Run({inputs}, {"uint8_Cast"}, {}, &outTensors));

        outTensor = outTensors.at(0);
    return tf::Status::OK();
}

    std::vector<tc::bounding_box> tf_model::detect(cv::Mat& img)
    {
        tf::Tensor img_tensor = tf::Tensor(tf::DT_FLOAT, {1,img.rows,img.cols, 3});
        tf::Status readTensorStatus = matToTensor(img, img_tensor);
        tf::Status runStatus = _f_session->Run({ {_inputLayer, img_tensor} }, _outputLayer , {}, &_outputs);  

        // Extract results from the outputs vector
        tf::TTypes<float, 3>::Tensor boxes = _outputs[0].flat_outer_dims<float,3>();
        tf::TTypes<float>::Flat scores = _outputs[1].flat<float>();
        tf::TTypes<float>::Flat classes = _outputs[2].flat<float>();
        tf::TTypes<float>::Flat numDetections = _outputs[3].flat<float>();

        std::vector<tc::bounding_box> tcboxes = {};

        for (int i = 0; i != numDetections.size(); i++)
        {
            if (scores(i) >= _confidence)
            {
                tc::bounding_box bbox;
                bbox.x = boxes(0,i,0);
                bbox.x = boxes(0,i,1);
                bbox.width = boxes(0,i,2);
                bbox.height = boxes(0,i,3);
                tcboxes.push_back(bbox);
            }
        }
      

        return tcboxes;
    }

    
}