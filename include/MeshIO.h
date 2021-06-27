#pragma once
#include <common.h>
#include <vector>

void load_obj(const std::string& filename, MatrixXf &V, MatrixXu &F);


void write_obj(const std::string &filename, 
                      const MatrixXf &V,
                      const MatrixXu &F,
                      const MatrixXf &N                = MatrixXf(),
                      const MatrixXf &Nf               = MatrixXf(),
                      const MatrixXf &UV               = MatrixXf(),
                      const MatrixXf &C                = MatrixXf());