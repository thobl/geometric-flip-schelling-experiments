#pragma once

#include <fstream>
#include <string>

class IpeFile {
 public:
  explicit IpeFile(const std::string& filename, double scaling_factor = 1.0);

  ~IpeFile();

  void new_page();

  void label(const std::string& label, double x, double y,
             const std::string& color = "black");

  void line(double x1, double y1, double x2, double y2,
            const std::string& color = "black");

  void point(double x, double y, const std::string& color = "black");

  void disk(double x, double y, double radius,
            const std::string& color = "black");

  void start_group();
  void start_group_with_clipping(double x1, double y1, double x2, double y2);
  void end_group();
  
 private:
  std::ofstream m_file;
  
  double m_scaling;

  void file_start();
  void file_end();

  void page_start();
  void page_end();
};
