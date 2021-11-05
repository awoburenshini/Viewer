#pragma once
#include <common.h>
#include <vector>

void load_obj(const std::string& filename, MatrixXr &V, MatrixXu &F);


void write_obj(const std::string &filename, 
                      const MatrixXr &V,
                      const MatrixXu &F,
                      const MatrixXr &N                = MatrixXr(),
                      const MatrixXr &Nf               = MatrixXr(),
                      const MatrixXr &UV               = MatrixXr(),
                      const MatrixXr &C                = MatrixXr());