#!/usr/bin/perl
# Firmware update package generator
#
# This script reads a raw binary file and calculates and appends
# a CRC16 checksum. The firmware is padded to half the size of
# the microcontroller's flash memory to obtain the same checksum
# that xboot calculates during the update process.
#
# Author: Sebastian Neuser

use strict;
use warnings;


# This subroutine calculates a CRC16 byte by byte.
# It was more or less copied over from <avr/iox128a4u.h>.
sub crc16_update
{
    my $crc = shift;    # A reference (!) to the current CRC value
    my $byte = shift;   # A byte to process

    $$crc ^= $byte;
    for (my $i = 0; $i < 8; ++$i) {
        if ($$crc & 1) {
            $$crc = ($$crc >> 1) ^ 0xA001;
        }
        else {
            $$crc = ($$crc >> 1);
        }
    }

    return $$crc;
}



# Check command line parameters
if ($#ARGV < 0) {
    print "usage: $0 <binary>\n";
    exit;
}

# Initialize buffers and counters
my $crc = 0x0000;
my $byte = 0;
my $bytes_processed = 0;

# Read binary firmware from file and compute CRC
printf "Reading binary firmware from %s...\n", $ARGV[0];
open BINARY, "+<", $ARGV[0] or die "Error opening file!";
while (read(BINARY, $byte, 1) > 0) {
    crc16_update \$crc, ord $byte;
    ++$bytes_processed
}

# Continue CRC calculations with padding values (0xff) until half of the
# flash memory size is reached
my $padding_length = 2**16 - $bytes_processed;
printf "Padding CRC with %u bytes of 0xff...\n", $padding_length;
while ($padding_length--) {
    crc16_update \$crc, 0xff;
}

# Append CRC checksum to binary file and close it
print  "Appending checksum to firmware file...";
print BINARY pack('s>', $crc);
close BINARY;

# Print some interesting numbers
print  "Done.\n";
printf "Update package size: %u\n", $bytes_processed + 2;
printf "CRC16 checksum: 0x%x\n", $crc;
