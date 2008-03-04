/*
function [LuminanceScale, ChrominanceScale, zigzagindex] = QuantTable()

% function with no input argument
% - returns the two scaling tables for DCT coefficients
% one for luminance and one for chrominance
%
% indeices to zig-zag scanning a matrix is also returned


zigzagindex = [1 2 9 17 10 3 4 11 18 25 33 26 19 12 5 6 13 20 27 34 41 49 42 35 28 21 14 7 8 15 22 29 36 43 50 57 58 51 44 37 30 23 16 24 31 38 45 52 59 60 53 46 39 32 40 47 54 61 62 55 48 56 63 64];
*/

int quant_lum_table[8][8] = {
	{ 16, 11, 10, 16, 24, 40, 51, 61 },
	{ 12, 12, 14, 19, 26, 58, 60, 55 },
	{ 14, 13, 16, 24, 40, 57, 69, 56 },
	{ 14, 17, 22, 29, 51, 87, 80, 62 },
	{ 18, 22, 37, 56, 68, 109, 103, 77 },
	{ 24, 35, 55, 64, 81, 104, 113, 92 },
	{ 49, 64, 78, 87, 103, 121, 120, 101 },
	{ 72, 92, 95, 98, 112, 100, 103, 99 }
};

int quant_chrom_table[8][8] = {
	{ 17, 18, 24, 47, 99, 99, 99, 99 },
	{ 18, 21, 26, 66, 99, 99, 99, 99 },
	{ 24, 26, 56, 99, 99, 99, 99, 99 },
	{ 47, 66, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99 }
};
