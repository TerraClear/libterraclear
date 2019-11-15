#include "tf_model.hpp"

namespace  terraclear
{

    tf_model::tf_model() { }

    tf_model::tf_model(const std::string &path_to_graph) 
    { 
        // TensorFlow graph reader
        tensorflow::GraphDef graph_def;
        // Read model
        tensorflow::Status load_graph_status = tensorflow::ReadBinaryProto(tensorflow::Env::Default(), path_to_graph, &graph_def); 

        // Make sure graph read properly   
        if (!load_graph_status.ok()) 
        {
            std::cout << load_graph_status.ToString() << "\n";
        }
        
        tensorflow::Status session_create_status = _session->Create(graph_def);

        if (!session_create_status.ok())
            std::cout << "Loaded model" << std::endl;

    }
    
    /*
    tf::Status readLabelsMapFile(const string &fileName)
    {
        // Read file into a string
        ifstream t(fileName);

        if (t.bad())
            return tensorflow::errors::NotFound("Failed to load labels map at '", fileName, "'");
        
        stringstream buffer;
        buffer << t.rdbuf();
        string fileString = buffer.str();

        // Search entry patterns of type 'item { ... }' and parse each of them
        smatch matcherEntry;
        smatch matcherId;
        smatch matcherName;
        const regex reEntry("item \\{([\\S\\s]*?)\\}");
        const regex reId("[0-9]+");
        const regex reName("\'.+\'");
        string entry;

        auto stringBegin = sregex_iterator(fileString.begin(), fileString.end(), reEntry);
        auto stringEnd = sregex_iterator();

        int id;
        string name;
        for (sregex_iterator i = stringBegin; i != stringEnd; i++) {
            matcherEntry = *i;
            entry = matcherEntry.str();
            regex_search(entry, matcherId, reId);
            if (!matcherId.empty())
                id = stoi(matcherId[0].str());
            else
                continue;
            regex_search(entry, matcherName, reName);
            if (!matcherName.empty())
                name = matcherName[0].str().substr(1, matcherName[0].str().length() - 2);
            else
                continue;
            _labelsMap.insert(pair<int, string>(id, name));
        }
        return Status::OK();
    }

    tf::Status readTensorFromMat(const Mat &mat, Tensor &outTensor) 
    {
        auto root = tensorflow::Scope::NewRootScope();
        using namespace ::tensorflow::ops;

        // Trick from https://github.com/tensorflow/tensorflow/issues/8033
        float *p = outTensor.flat<float>().data();
        Mat fakeMat(mat.rows, mat.cols, CV_32FC3, p);
        mat.convertTo(fakeMat, CV_32FC3);

        auto input_tensor = Placeholder(root.WithOpName("input"), tensorflow::DT_FLOAT);
        vector<pair<string, tensorflow::Tensor>> inputs = {{"input", outTensor}};
        auto uint8Caster = Cast(root.WithOpName("uint8_Cast"), outTensor, tensorflow::DT_UINT8);

        // This runs the GraphDef network definition that we've just constructed, and
        // returns the results in the output outTensor.
        tensorflow::GraphDef graph;
        TF_RETURN_IF_ERROR(root.ToGraphDef(&graph));

        vector<Tensor> outTensors;
        unique_ptr<tensorflow::Session> session(tensorflow::NewSession(tensorflow::SessionOptions()));

        TF_RETURN_IF_ERROR(session->Create(graph));
        TF_RETURN_IF_ERROR(session->Run({inputs}, {"uint8_Cast"}, {}, &outTensors));

        outTensor = outTensors.at(0);

        return Status::OK();
    }

    tf::Status detect(cv::Mat& mat)
    {
        // Convert new image to tensor
        tf::Status readTensorFromMat(mat, _tensor);

        tf::Status runStatus = session->Run({{inputLayer, _tensor}}, _outputLayer, {}, &_outputs);
        if (!runStatus.ok()) {
            LOG(ERROR) << "Running model failed: " << runStatus;
            return -1;
        }

        // Extract results from the outputs vector
        tf::TTypes<float>::Flat scores = outputs[1].flat<float>();
        tf::TTypes<float>::Flat classes = outputs[2].flat<float>();
        tf::TTypes<float>::Flat numDetections = outputs[3].flat<float>();
        tf::TTypes<float, 3>::Tensor boxes = outputs[0].flat_outer_dims<float,3>();

    }
    */
}