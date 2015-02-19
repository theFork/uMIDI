#!/usr/bin/perl
# Copyright 2015 Sebastian Neuser
#
# This file is part of the uMidi firmware.
#
# the uMidi firmware is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# the uMidi firmware is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the uMidi firmware.  If not, see <http://www.gnu.org/licenses/>.

# Exponential function lookup table generator for the uMidi firmware.

use strict;
use warnings;
use File::Basename;
use Getopt::Std;


# --------- C O M M A N D   L I N E   A R G U M E N T S --------- #
# Parse options
my %opts;
getopts("x:y:" , \%opts);


# ------------------------- C H E C K S ------------------------- #
# Check command line parameters
if (!exists($opts{"x"}) or !exists($opts{"y"})) {
    print_help_and_exit();
}
if ($#ARGV < 0) {
    print_help_and_exit();
}


# ---------------------- V A R I A B L E S ---------------------- #

my $PRECISION = 1e-6;
my $xmax = 2**$opts{"x"} - 1;
my $ymax = 2**$opts{"y"} - 1;
my $output_file = shift;


# ------------------- M A I N   P R O G R A M ------------------- #

# Find ideal base for exponential table
my ($base, $iterations) = calculate_base();
printf("Found base %f with %d iterations.\n", $base, $iterations);

printf("Generating lookup table header file %s\n", $output_file);
open(OUTFILE, ">", $output_file) or die;
select(OUTFILE);

# Print file comment
print_header();

# Print exponential lookup table
print("\n// Exponential lookup table\n");
print("const uint16_t exp_table[] = {\n");
for (my $i = 0; $i <= $xmax; $i += 1) {
    # Print indent on new lines
    if ($i % 16 == 0) {
        print("   ");
    }

    printf(" %5d,", int($base ** $i));

    # Begin a new line every 16 values
    if ($i > 0 and $i % 16 == 15) {
        print("\n");
    }
}
print("};\n");

# Print sine lookup table
print("\n// Sine lookup table\n");
print("const uint16_t sine_table[] = {\n");
for (my $i = 0; $i <= 127; $i += 1) {
    # Print indent on new lines
    if ($i % 16 == 0) {
        print("   ");
    }

    printf(" %4d,", int($xmax/2 * sin(3.141592653589793*$i/$xmax) + $xmax/2));

    # Begin a new line every 16 values
    if ($i > 0 and $i % 16 == 15) {
        print("\n");
    }
}
print("};\n");

close(OUTFILE);

# -------------------- S U B R O U T I N E S -------------------- #

sub calculate_base
{
    my $iterations = 0;
    my $base = 1.0;
    my $error = 1;

    while (abs($error) > $PRECISION) {
        $iterations += 1;
        $error = calculate_error($base);
        $base += $PRECISION * $error;
    }
    return $base, $iterations;
}

sub calculate_error
{
    my $base = shift;
    my $y = $base ** $xmax;
    my $div = $ymax - $y;
    return $div;
}

sub print_header
{
    printf("/*
 * Copyright 2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * The MIDI volume controller firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The MIDI volume controller firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Lookup tables for the uMIDI firmware.
 * This file was auto-generated by %s.
*/

#include <stdint.h>\n\n", basename($0));
}

sub print_help_and_exit
{
    printf("usage: %s -x <X-resolution> -y <Y-resolution> <output file>\n", basename($0));
    exit;
}
