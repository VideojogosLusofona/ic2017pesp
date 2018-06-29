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
 * Header file of useful functions for procedural generation
 *
 * @author Nuno Fachada
 * @date 2018
 * @copyright [GNU General Public License version 3 (GPLv3)](http://www.gnu.org/licenses/gpl.html)
 * */


/* Linear function */
double linear(double x, double m, double b);

/* Piecewise linear function */
double piecewiseLinear(double x, double m, double b, double l);

/* Logistic function */
double logistic(double x, double L, double x0, double k);

/* Logarithmic function */
double logarithmic(double x, double a, double d);
