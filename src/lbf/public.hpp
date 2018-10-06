// This header file contains public interface of LBF library.

#ifndef LBF_PUBLIC_HPP_
#define LBF_PUBLIC_HPP_

#include <vector>
#include <string>
#include <opencv2/core/core.hpp>

namespace lbf {

class Config {
 public:
  static inline Config& GetInstance() {
    static Config c;
    return c;
  }
 public:
  int stages_n;
  std::string dataset;
  int landmark_n;
  std::string saved_file_name;
  int initShape_n;

 private:
  Config();
  ~Config() {}
  Config(const Config &other);
  Config &operator=(const Config &other);
};

class BBox {
 public:
  BBox();
  BBox(double x, double y, double w, double h);

  cv::Mat Project(const cv::Mat &shape) const;
  cv::Mat ReProject(const cv::Mat &shape) const;

  double x, y;
  double x_center, y_center;
  double x_scale, y_scale;
  double width, height;
};

class LbfCascador {
 public:
  LbfCascador();
  ~LbfCascador();
  void Init(int stages_n);
  void Train(std::vector<cv::Mat> &imgs, std::vector<cv::Mat> &gt_shapes,
          std::vector<cv::Mat> &current_shapes, std::vector<BBox> &bboxes,
          cv::Mat &mean_shape, int start_from = 0);
  void Test(std::vector<cv::Mat> &imgs, std::vector<cv::Mat> &gt_shapes, std::vector<BBox> &bboxes);
  cv::Mat Predict(cv::Mat &img, BBox &bbox);
  void ResumeTrainModel(int start_from = 0);

  void Read(FILE *fd);
  void Write(FILE *fd);
};

cv::Mat getMeanShape(std::vector<cv::Mat> &gt_shapes, std::vector<BBox> &bboxes);
cv::Mat drawShapeInImage(const cv::Mat &img, const cv::Mat &shape, const BBox &bbox);
void logMsg(const char *fmt, ...);

} // namespace lbf

#endif // LBF_PUBLIC_HPP_
