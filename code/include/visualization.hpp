
#pragma once

#include <vector>

struct GeometricGraph;
class IpeFile;

void draw_rgg(const GeometricGraph& GG, const std::vector<bool> colors,
              IpeFile& ipe);

double canvas_size();
