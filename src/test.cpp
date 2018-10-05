#include <cstdio>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "lbf/public.hpp"

using namespace cv;
using namespace std;

// dirty but works
void parseTxt(string &txt, vector<Mat> &imgs, vector<Mat> &gt_shapes, vector<lbf::BBox> &bboxes);

int test(void) {
    lbf::Config &config = lbf::Config::GetInstance();

    lbf::LbfCascador lbf_cascador;
    FILE *fd = fopen(config.saved_file_name.c_str(), "rb");
    lbf_cascador.Read(fd);
    fclose(fd);

    lbf::LOG("Load test data from %s", config.dataset.c_str());
    string txt = config.dataset + "/test.txt";
    vector<Mat> imgs, gt_shapes;
    vector<lbf::BBox> bboxes;
    parseTxt(txt, imgs, gt_shapes, bboxes);

    int N = imgs.size();
    lbf_cascador.Test(imgs, gt_shapes, bboxes);

    return 0;
}

int run(void) {
    lbf::Config &config = lbf::Config::GetInstance();
    FILE *fd = fopen((config.dataset + "/test.txt").c_str(), "r");
    assert(fd);
    int N;
    int landmark_n = config.landmark_n;
    fscanf(fd, "%d", &N);
    char img_path[256];
    double bbox[4];
    vector<double> x(landmark_n), y(landmark_n);

    lbf::LbfCascador lbf_cascador;
    FILE *model = fopen(config.saved_file_name.c_str(), "rb");
    lbf_cascador.Read(model);
    fclose(model);

    for (int i = 0; i < N; i++) {
        fscanf(fd, "%s", img_path);
        for (int j = 0; j < 4; j++) {
            fscanf(fd, "%lf", &bbox[j]);
        }
        for (int j = 0; j < landmark_n; j++) {
            fscanf(fd, "%lf%lf", &x[j], &y[j]);
        }
        Mat img = imread(img_path);
        // crop img
        double x_min, y_min, x_max, y_max;
        x_min = *min_element(x.begin(), x.end());
        x_max = *max_element(x.begin(), x.end());
        y_min = *min_element(y.begin(), y.end());
        y_max = *max_element(y.begin(), y.end());
        x_min = max(0., x_min - bbox[2] / 2);
        x_max = min(img.cols - 1., x_max + bbox[2] / 2);
        y_min = max(0., y_min - bbox[3] / 2);
        y_max = min(img.rows - 1., y_max + bbox[3] / 2);
        double x_, y_, w_, h_;
        x_ = x_min; y_ = y_min;
        w_ = x_max - x_min; h_ = y_max - y_min;
        lbf::BBox bbox_(bbox[0] - x_, bbox[1] - y_, bbox[2], bbox[3]);
        Rect roi(x_, y_, w_, h_);
        img = img(roi).clone();

        Mat gray;
        cvtColor(img, gray, CV_BGR2GRAY);
        lbf::LOG("Run %s", img_path);
        Mat shape = lbf_cascador.Predict(gray, bbox_);
        img = drawShapeInImage(img, shape, bbox_);
        imshow("landmark", img);
        waitKey(0);
    }
    fclose(fd);
    return 0;
}
