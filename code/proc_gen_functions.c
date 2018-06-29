/*
 * This file is part of "Projeto de Época Especial de Introdução à Computação
 * 2017/2018 (PICEsp1718)".
 *
 * PICEsp1718 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PICEsp1718 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PICEsp1718. If not, see <http://www.gnu.org/licenses/>.
 * */

/**
 * @file
 * Useful functions for procedural generation
 *
 * @author Nuno Fachada
 * @date 2018
 * @copyright [GNU General Public License version 3 (GPLv3)](http://www.gnu.org/licenses/gpl.html)
 * */

#include <math.h>

/**
 * @brief
 * Linear function
 *
 * @param x Input variable *x*
 * @param m Slope
 * @param b Intercept value at *yy*
 * @return The *y* output variable
 * */
double linear(double x, double m, double b)
{
    return m * x + b;
}

/**
 * @brief
 * Piecewise linear function
 *
 * @param x Input variable *x*
 * @param m Slope
 * @param b Intercept value at *yy*
 * @param l Value of *x* which separates the sloped and constant parts of the
 * function
 * @return The *y* output variable
 * */
double piecewiseLinear(double x, double m, double b, double l)
{
    return x < l ? m * x + b : m * l + b;
}

/**
 * @brief
 * Logistic function
 *
 * @param x Input variable *x*
 * @param L The curve's maximum value
 * @param x0 The x-value of the sigmoid's midpoint
 * @param k The steepness of the curve
 * @return The *y* output variable
 * */
double logistic(double x, double L, double x0, double k)
{
    return L / (1 + exp(-k * (x - x0)));
}

/**
 * @brief
 * Logarithmic function
 *
 * @param x Input variable *x*
 * @param a Scale modifier
 * @param d Base modifier
 * @return The *y* output variable
 * */
double logarithmic(double x, double a, double d)
{
    return a * log(d * x);
}
