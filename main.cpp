#include <iostream>
#include <ctype.h>
#include <string.h>
#include <bits/stdc++.h>
//#include <math.h>

// function to get parameter from character
long double char_to_parameter(char c, long double a_min = -1.2, long double a_max = 1.2)
{
        long double l = a_max - a_min;
        long double d = l / 24.;

        int j = std::tolower(c) - 97;

        return a_min + j * d;
}

// function to grid coordinates from x and y position
std::pair<int, int> value_to_coordinate(long double x, long double y, long double x_min, long double x_max, int N_x)
{
        int i = (int)std::floor((x - x_min) / (x_max - x_min) * N_x);
        int j = (int)std::floor((y - x_min) / (x_max - x_min) * N_x);

        return std::make_pair(i, j);
}

// function to make one step with the quadratic map
std::pair<long double, long double> quadratic_map(long double x, long double y, long double a[6], long double b[6])
{
        long double _x = a[0] + a[1]*x + a[2]*x*x + a[3]*x*y + a[4]*y + a[5]*y*y;
        long double _y = b[0] + b[1]*x + b[2]*x*x + b[3]*x*y + b[4]*y + b[5]*y*y;

        return std::make_pair(_x, _y);
}

int main()
{
        const int N_i = pow(10, 6);
        const int N_t = pow(10,7);
        const int N_x = 5000;

        const long double x_min = -2., x_max = 2.;

        std::ofstream outfile;

        std::bitset<N_x * N_x> grid;

        long double x = 0, y = 0;
        int i, j;

        long double a[6];
        long double b[6];

        char str[13];

        // set placeholder string to check input
        strncpy(str, "zzzzzzzzzzzz", sizeof(str) - 1);

        // get input from user
        std::cout << "enter 12 characters (a-y, A-Y): ";
        std::cin >> str;

        // check input
        if (str[11] == 'z')
        {
                std::cout << "invalid string, using default 'LUFBBFISGJYS'" << std::endl;
                strncpy(str, "LUFBBFISGJYS", sizeof(str) - 1);
                str[sizeof(str) - 1] = 0;
        }

        // setup parameters
        for (int i = 0; str[i] != 0; i++)
        {

                char c = str[i];
                if (i < 6)
                {
                        a[i] = char_to_parameter(c);
                        std::cout << "a_" << i+1 << " = " << str[i] << " = " << a[i] << std::endl;
                }
                else
                {
                        b[i-6] = char_to_parameter(c);
                        std::cout << "b_" << i-5 << " = " << str[i] << " = " << b[i-6] << std::endl;
                }

        }

        // spin up
        for (int t = 0; t < N_i; t++)
        {
                std::tie(x, y) = quadratic_map(x, y, a, b);
        }

        // record
        for (int t = 0; t < N_t; t++)
        {
                std::tie(x, y) = quadratic_map(x, y, a, b);
                std::tie(i, j) = value_to_coordinate(x, y, x_min, x_max, N_x);

                grid[i + j * N_x] = 1;
        }

        // save grid to file and count non-zero pixels
        int pixels = 0;

        FILE * out = fopen("quadratic_map.ppm", "wb");
        fprintf(out, "P6 %d %d 1\n", N_x, N_x);

        for (int j = 0; j < N_x; j++)
        {
                for (int i = 0; i < N_x; i++)
                {
                        putc(grid[i + N_x*j], out);
                        putc(grid[i + N_x*j], out);
                        putc(grid[i + N_x*j], out);

                        pixels += grid[i + N_x*j];
                }
        }

        fclose(out);

        std::cout << "non-zero pixels: " << pixels << std::endl;

        return 0;
}
